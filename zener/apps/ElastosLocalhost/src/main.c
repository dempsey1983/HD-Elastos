/*
 * Copyright 2015, Tongji Operating System Group & elastos.org
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 */

/*
 * HD-Elastos No. 1 process, the core process, the first process, the initial process
 */


/* Include Kconfig variables. */
#include <autoconf.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>

#include <allocman/bootstrap.h>
#include <allocman/vka.h>

#include <platsupport/timer.h>

#include <sel4platsupport/platsupport.h>
#include <sel4platsupport/plat/timer.h>
#include <sel4utils/vspace.h>
#include <sel4utils/stack.h>
#include <sel4utils/process.h>

#include <simple/simple.h>

#include <utils/util.h>

#include <vka/object.h>
#include <vka/capops.h>

#include <vspace/vspace.h>

#include "compute_env.h"

/* Contains information about the test environment.
 * Define struct env in your application. */
struct env {
    /* An initialised vka that may be used by the test. */
    vka_t vka;
    /* virtual memory management interface */
    vspace_t vspace;
    /* abtracts over kernel version and boot environment */
    simple_t simple;
    /* path for the default timer irq handler */
    cspacepath_t irq_path;
#ifdef CONFIG_ARCH_ARM
    /* frame for the default timer */
    cspacepath_t frame_path;
#elif CONFIG_ARCH_IA32
    /* io port for the default timer */
    seL4_CPtr io_port_cap;
#endif
    /* init data frame vaddr */
    compute_env_data_t *init;
    /* extra cap to the init data frame for mapping into the remote vspace */
    seL4_CPtr init_frame_cap_copy;
};
typedef struct env *env_t;

#define HI_APP "sel4test-hi"
#define SUCCESS 0
#define FAILURE (-1)
#define HI_CONFIG    "[CONFIG] Hi~"
#define HI_SETUP     "[SETUP] Hi~"
#define HI_ARGV      "[ARGV] Hi~"
#define HI_SPAWN     "[SPAWN] Hi~"
#define HI_SENDDATA  "[SENDDATA] Hi~"
#define HI_WAIT      "[WAIT] Hi~"
#define HI_UNMAP     "[UNMAP] Hi~"
#define HI_RESET     "[RESET] Hi~"
#define HI_DESTORY   "[DESTORY] Hi~"

/* ammount of untyped memory to reserve for the driver (32mb) */
#define DRIVER_UNTYPED_MEMORY (1 << 25)
/* Number of untypeds to try and use to allocate the driver memory.
 * if we cannot get 32mb with 16 untypeds then something is probably wrong */
#define DRIVER_NUM_UNTYPEDS 16

/* dimensions of virtual memory for the allocator to use */
#define ALLOCATOR_VIRTUAL_POOL_SIZE ((1 << seL4_PageBits) * 100)

/* static memory for the allocator to bootstrap with */
#define ALLOCATOR_STATIC_POOL_SIZE ((1 << seL4_PageBits) * 10)
static char allocator_mem_pool[ALLOCATOR_STATIC_POOL_SIZE];

/* static memory for virtual memory bootstrapping */
static sel4utils_alloc_data_t data;

/* environment encapsulating allocation interfaces etc */
static struct env env;
/* the number of untyped objects we have to give out to processes */
static int num_untypeds;
/* list of untypeds to give out to test processes */
static vka_object_t untypeds[CONFIG_MAX_NUM_BOOTINFO_UNTYPED_CAPS];
/* list of sizes (in bits) corresponding to untyped */
static uint8_t untyped_size_bits_list[CONFIG_MAX_NUM_BOOTINFO_UNTYPED_CAPS];

/* initialise our runtime environment */
static void init_env(env_t env)
{
    allocman_t *allocman;
    UNUSED reservation_t virtual_reservation;
    UNUSED int error;

    /* create an allocator */
    allocman = bootstrap_use_current_simple(&env->simple, ALLOCATOR_STATIC_POOL_SIZE, allocator_mem_pool);
    assert(allocman);

    /* create a vka (interface for interacting with the underlying allocator) */
    allocman_make_vka(&env->vka, allocman);

    /* create a vspace (virtual memory management interface). We pass
     * boot info not because it will use capabilities from it, but so
     * it knows the address and will add it as a reserved region */
    error = sel4utils_bootstrap_vspace_with_bootinfo_leaky(&env->vspace,
                                                           &data, simple_get_pd(&env->simple), &env->vka, seL4_GetBootInfo());

    /* fill the allocator with virtual memory */
    void *vaddr;
    virtual_reservation = vspace_reserve_range(&env->vspace,
                                               ALLOCATOR_VIRTUAL_POOL_SIZE, seL4_AllRights, 1, &vaddr);
    assert(virtual_reservation.res);
    bootstrap_configure_virtual_pool(allocman, vaddr,
                                     ALLOCATOR_VIRTUAL_POOL_SIZE, simple_get_pd(&env->simple));
}

/* copy a cap to a process, returning the cptr in the process' cspace */
static seL4_CPtr copy_cap_to_process(sel4utils_process_t *process, seL4_CPtr cap)
{
    seL4_CPtr copied_cap;
    cspacepath_t path;

    vka_cspace_make_path(&env.vka, cap, &path);
    copied_cap = sel4utils_copy_cap_to_process(process, path);
    assert(copied_cap != 0);

    return copied_cap;
}

/* Free a list of objects */
static void free_objects(vka_object_t *objects, unsigned int num)
{
    for (unsigned int i = 0; i < num; i++) {
        vka_free_object(&env.vka, &objects[i]);
    }
}

/* Allocate untypeds till either a certain number of bytes is allocated
 * or a certain number of untyped objects */
static unsigned int allocate_untypeds(vka_object_t *untypeds, size_t bytes, unsigned int max_untypeds)
{
    unsigned int num_untypeds = 0;
    size_t allocated = 0;

    /* try to allocate as many of each possible untyped size as possible */
    for (uint8_t size_bits = seL4_WordBits - 1; size_bits > PAGE_BITS_4K; size_bits--) {
        /* keep allocating until we run out, or if allocating would
         * cause us to allocate too much memory*/
        while (num_untypeds < max_untypeds &&
               allocated + BIT(size_bits) <= bytes &&
               vka_alloc_untyped(&env.vka, size_bits, &untypeds[num_untypeds]) == 0) {
            allocated += BIT(size_bits);
            num_untypeds++;
        }
    }
    return num_untypeds;
}

/* extract a large number of untypeds from the allocator */
static unsigned int populate_untypeds(vka_object_t *untypeds)
{
    /* First reserve some memory for the driver */
    vka_object_t reserve[DRIVER_NUM_UNTYPEDS];
    unsigned int reserve_num = allocate_untypeds(reserve, DRIVER_UNTYPED_MEMORY, DRIVER_NUM_UNTYPEDS);

    /* Now allocate everything else for the tests */
    unsigned int num_untypeds = allocate_untypeds(untypeds, UINT_MAX, ARRAY_SIZE(untyped_size_bits_list));
    /* Fill out the size_bits list */
    for (unsigned int i = 0; i < num_untypeds; i++) {
        untyped_size_bits_list[i] = untypeds[i].size_bits;
    }

    /* Return reserve memory */
    free_objects(reserve, reserve_num);

    /* Return number of untypeds for tests */
    assert(num_untypeds > 0);
    return num_untypeds;
}

/* copy untyped caps into a processes cspace, return the cap range they can be found in */
static seL4_SlotRegion copy_untypeds_to_process(sel4utils_process_t *process, vka_object_t *untypeds, int num_untypeds)
{
    seL4_SlotRegion range = {0};

    for (int i = 0; i < num_untypeds; i++) {
        int slot = copy_cap_to_process(process, untypeds[i].cptr);

        /* set up the cap range */
        if (i == 0) {
            range.start = slot;
        }
        range.end = slot;
    }
    assert((range.end - range.start) + 1 == num_untypeds);
    return range;
}

/* map the init data into the process, and send the address via ipc */
static void *send_init_data(env_t env, seL4_CPtr endpoint, sel4utils_process_t *process)
{
    /* map the cap into remote vspace */
    void *remote_vaddr = vspace_map_pages(&process->vspace, &env->init_frame_cap_copy, NULL, seL4_AllRights, 1, PAGE_BITS_4K, 1);
    assert(remote_vaddr != 0);

    /* now send a message telling the process what address the data is at */
    seL4_MessageInfo_t info = seL4_MessageInfo_new(seL4_NoFault, 0, 0, 1);
    seL4_SetMR(0, (seL4_Word) remote_vaddr);
    seL4_Send(endpoint, info);

    return remote_vaddr;
}

/* copy the caps required to set up the sel4platsupport default timer */
static void copy_timer_caps(compute_env_data_t *init, env_t env, sel4utils_process_t *test_process)
{
#ifdef CONFIG_ARCH_ARM
    /* Timer frame cap (only for arm). Here we assume the sel4platsupport
     * default timer only requires one frame. */
    init->timer_frame = copy_cap_to_process(test_process, env->frame_path.capPtr);
    assert(init->timer_frame != 0);
#endif

    /* irq cap for the timer irq */
    init->timer_irq = copy_cap_to_process(test_process, env->irq_path.capPtr);
    assert(init->timer_irq != 0);

#ifdef CONFIG_ARCH_IA32
    /* io port cap (since the default timer on ia32 is the PIT) */
    init->io_port = copy_cap_to_process(test_process, env->io_port_cap);
    assert(init->io_port != 0);
#endif
}

static void init_timer_caps(env_t env)
{
    /* get the timer irq cap */
    seL4_CPtr cap;
    UNUSED int error = vka_cspace_alloc(&env->vka, &cap);
    assert(error == 0);

    vka_cspace_make_path(&env->vka, cap, &env->irq_path);
    error = simple_get_IRQ_control(&env->simple, DEFAULT_TIMER_INTERRUPT, env->irq_path);
    assert(error == 0);

#ifdef CONFIG_ARCH_ARM
    /* get the timer frame cap */
    error = vka_cspace_alloc(&env->vka, &cap);
    assert(error == 0);

    vka_cspace_make_path(&env->vka, cap, &env->frame_path);
    error = simple_get_frame_cap(&env->simple, (void *) DEFAULT_TIMER_PADDR, PAGE_BITS_4K, &env->frame_path);
    assert(error == 0);
#elif CONFIG_ARCH_IA32
    env->io_port_cap = simple_get_IOPort_cap(&env->simple, PIT_IO_PORT_MIN, PIT_IO_PORT_MAX);
    assert(env->io_port_cap != 0);
#else
#error "Unknown architecture"
#endif
}

/* Call application sel4test-hi.
 * This is just a simple demo. */
int run_hi(void)
{
    UNUSED int error;
    sel4utils_process_t hi_process;

    /* Test intro banner. */
    printf("  Test: Hi~\n");

    error = sel4utils_configure_process(&hi_process, &env.vka, &env.vspace,
                                        env.init->priority, HI_APP);
    assert(error == 0);
    printf("================== %s =================\n", HI_CONFIG);

    /* set up caps about the process */
    env.init->page_directory = copy_cap_to_process(&hi_process, hi_process.pd.cptr);
    env.init->root_cnode = SEL4UTILS_CNODE_SLOT;
    env.init->tcb = copy_cap_to_process(&hi_process, hi_process.thread.tcb.cptr);
    env.init->domain = copy_cap_to_process(&hi_process, simple_get_init_cap(&env.simple, seL4_CapDomain));
#ifndef CONFIG_KERNEL_STABLE
    env.init->asid_pool = copy_cap_to_process(&hi_process, simple_get_init_cap(&env.simple, seL4_CapInitThreadASIDPool));
#endif /* CONFIG_KERNEL_STABLE */
#ifdef CONFIG_IOMMU
    env.init->io_space = copy_cap_to_process(&hi_process, simple_get_init_cap(&env.simple, seL4_CapIOSpace));
#endif /* CONFIG_IOMMU */
    /* setup data about untypeds */
    env.init->untypeds = copy_untypeds_to_process(&hi_process, untypeds, num_untypeds);
    copy_timer_caps(env.init, &env, &hi_process);
    /* copy the fault endpoint - we wait on the endpoint for a message
     * or a fault to see when the test finishes */
    seL4_CPtr endpoint = copy_cap_to_process(&hi_process, hi_process.fault_endpoint.cptr);

    printf("================== %s =================\n", HI_SETUP);

    /* WARNING: DO NOT COPY MORE CAPS TO THE PROCESS BEYOND THIS POINT,
     * AS THE SLOTS WILL BE CONSIDERED FREE AND OVERRIDDEN BY THE TEST PROCESS. */
    /* set up free slot range */
    env.init->cspace_size_bits = CONFIG_SEL4UTILS_CSPACE_SIZE_BITS;
    env.init->free_slots.start = endpoint + 1;
    env.init->free_slots.end = (1u << CONFIG_SEL4UTILS_CSPACE_SIZE_BITS);
    assert(env.init->free_slots.start < env.init->free_slots.end);
    /* copy test name */
    strncpy(env.init->name, "process_hi", 11);
#ifdef SEL4_DEBUG_KERNEL
    seL4_DebugNameThread(hi_process.thread.tcb.cptr, env.init->name);
#endif

    /* set up args for the test process */
    char endpoint_string[10];
    char sel4test_name[] = { HI_APP };
    // char zero_string[] = {"0"};
    // char *argv[] = {sel4test_name, zero_string, endpoint_string};
    char *argv[] = {sel4test_name};
    argv[0] = endpoint_string;
    snprintf(endpoint_string, 10, "%d", endpoint);

    printf("================== %s =================\n", HI_ARGV);

    /* spawn the process */
    error = sel4utils_spawn_process_v(&hi_process, &env.vka, &env.vspace,
                            ARRAY_SIZE(argv), argv, 1);
    assert(error == 0);
    printf("================== %s =================\n", HI_SPAWN);

    /* send env.init_data to the new process */
    void *remote_vaddr = send_init_data(&env, hi_process.fault_endpoint.cptr, &hi_process);

    printf("================== %s =================\n", HI_SENDDATA);

    /* wait on it to finish or fault, report result */
    seL4_Word badge;
    seL4_MessageInfo_t info = seL4_Wait(hi_process.fault_endpoint.cptr, &badge);

    printf("================== %s =================\n", HI_WAIT);

    int result = seL4_GetMR(0);
    if (seL4_MessageInfo_get_label(info) != seL4_NoFault) {
        sel4utils_print_fault_message(info, "test_hi");
        result = FAILURE;
    }

    /* unmap the env.init data frame */
    vspace_unmap_pages(&hi_process.vspace, remote_vaddr, 1, PAGE_BITS_4K, NULL);

    printf("================== %s =================\n", HI_UNMAP);

    /* reset all the untypeds for the next test */
    for (int i = 0; i < num_untypeds; i++) {
        cspacepath_t path;
        vka_cspace_make_path(&env.vka, untypeds[i].cptr, &path);
        vka_cnode_revoke(&path);
    }
    printf("================== %s =================\n", HI_RESET);

    /* destroy the process */
    sel4utils_destroy_process(&hi_process, &env.vka);
    printf("================== %s =================\n", HI_DESTORY);

    assert(result == SUCCESS);
    return result;
}

void *main_continued(void *arg UNUSED)
{
    /* allocate lots of untyped memory for tests to use */
    num_untypeds = populate_untypeds(untypeds);

    /* create a frame that will act as the init data, we can then map that
     * in to target processes */
    env.init = (compute_env_data_t *)vspace_new_pages(&env.vspace, seL4_AllRights, 1, PAGE_BITS_4K);
    assert(env.init != NULL);

    env.init->ComputeEnvType = LocalhostCOMPUTE_ENV;

    /* copy the cap to map into the remote process */
    cspacepath_t src, dest;
    vka_cspace_make_path(&env.vka, vspace_get_cap(&env.vspace, env.init), &src);

    UNUSED int error = vka_cspace_alloc(&env.vka, &env.init_frame_cap_copy);
    assert(error == 0);
    vka_cspace_make_path(&env.vka, env.init_frame_cap_copy, &dest);
    error = vka_cnode_copy(&dest, &src, seL4_AllRights);
    assert(error == 0);

    /* copy the untyped size bits list across to the init frame */
    memcpy(env.init->untyped_size_bits_list, untyped_size_bits_list, sizeof(uint8_t) * num_untypeds);

    /* parse elf region data about the test image to pass to the tests app */
    // num_elf_regions = sel4utils_elf_num_regions(HI_APP);
    // assert(num_elf_regions < MAX_REGIONS);
    // sel4utils_elf_reserve(NULL, HI_APP, elf_regions);

    /* copy the region list for the process to clone itself */
    // memcpy(env.init->elf_regions, elf_regions, sizeof(sel4utils_elf_region_t) * num_elf_regions);
    // env.init->num_elf_regions = num_elf_regions;

    /* get the caps we need to send to tests to set up a timer */
    init_timer_caps(&env);

    /* setup init data that won't change test-to-test */
    env.init->priority = seL4_MaxPrio - 1;

    /* ================== My Execution ================= */

    char test_str[10];
    /* Hello, sel4test */
    printf("\nHello, sel4test!\n");

    /* Test for scanf */
    printf("INPUT A STRING: \n");
    // scanf("%s", test_str);
    snprintf(test_str, 10, "%x", 0xf0000000);
    printf("OUTPUT = %s\n", test_str);

    run_hi();

    return 0;
}

int main(void)
{
    seL4_BootInfo *info = seL4_GetBootInfo();
#ifdef SEL4_DEBUG_KERNEL
    seL4_DebugNameThread(seL4_CapInitThreadTCB, "Elastos.Localhost");
#endif

    compile_time_assert(init_data_fits_in_ipc_buffer, sizeof(compute_env_data_t) < PAGE_SIZE_4K);
    /* initialise libsel4simple, which abstracts away which kernel version
     * we are running on */

/*
#ifdef CONFIG_KERNEL_STABLE
    simple_stable_init_bootinfo(&env.simple, info);
#else
    simple_default_init_bootinfo(&env.simple, info);
#endif
*/
    simple_init_bootinfo(&env.simple, info);

    /* initialise the test environment - allocator, cspace manager, vspace manager, timer */
    init_env(&env);

    /* enable serial driver */
    platsupport_serial_setup_simple(NULL, &env.simple, &env.vka);

    /* switch to a bigger, safer stack with a guard page
     * before starting the tests */
    printf("Switching to a safer, bigger stack... ");
    fflush(stdout);
    int res = (int)sel4utils_run_on_stack(&env.vspace, main_continued, NULL);
    test_assert_fatal(res == 0);

    return 0;
}

