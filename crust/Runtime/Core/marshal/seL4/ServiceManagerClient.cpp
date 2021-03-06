
#include "Elastos.Droid.Content.h"
#include <binder/Parcel.h>
#include <binder/IServiceManager.h>
#include <binder/Binder.h>
#include <utils/Log.h>
#include "elastos/droid/os/CServiceManager.h"
#include "elastos/droid/os/Process.h"
#include <elastos/core/AutoLock.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Utility::Logging::Slogger;

#define ELASTOS_SERVICEMGR_NAME "elastos.servicemanager"

namespace Elastos {
namespace Droid {
namespace Os {

enum {
    ADD_SERVICE = android::IBinder::FIRST_CALL_TRANSACTION,
    GET_SERVICE,
    REMOVE_SERVICE,
    CHECK_SERVICE,
    LIST_SERVICES,
};

// come from marshal.h
typedef struct InterfacePack
{
    EMuid               m_clsid;             // clsid of object
    UInt32              m_uIndex;            // interface index in class
    android::sp<android::IBinder> m_pBinder; // dbus unique connection name of stub
} InterfacePack;

CAR_INTERFACE_IMPL(CServiceManager, Object, IServiceManager)

CAR_SINGLETON_IMPL(CServiceManager)

CServiceManager::CServiceManager() :
    mServiceCache(11)
{}

CServiceManager::~CServiceManager()
{
    mServiceCache.Clear();
}

android::sp<android::IBinder> get_service_manager()
{
    static android::sp<android::IBinder> binder;
    if (binder.get() != NULL) {
        return binder;
    }
    ALOGD("elastos servicemanager try getting...\n");
    android::sp<android::IServiceManager> sm = android::defaultServiceManager();
    do {
        binder = sm->getService(android::String16(ELASTOS_SERVICEMGR_NAME));
        if (binder != 0) {
            break;
        }
        usleep(500000);
    } while (true);
    ALOGD("elastos servicemanager getted.\n");
    return binder;
}

ECode CServiceManager::AddService(
    /* [in] */ const String& name,
    /* [in] */ IInterface * service)
{
    return AddService(name, service, FALSE);
}

ECode CServiceManager::AddService(
    /* [in] */ const String& name,
    /* [in] */ IInterface * service,
    /* [in] */ Boolean allowIsolated)
{
    if (name.IsNullOrEmpty() || !service) {
        return NOERROR;
    }

    ECode ec = NOERROR;
    android::Parcel data, reply;
    void * buf = NULL;
    Int32 size;

    ec = _CObject_MarshalInterface(service, MarshalType_IPC, &buf, &size);
    if (FAILED(ec)) {
        if (buf != NULL) free(buf);
        Slogger::E("CServiceManager", "Failed to add service %s, ec=%08x", name.string(), ec);
        return ec;
    }

    data.writeCString(name.string());
    data.writeInt32((int32_t)size);
    data.writeStrongBinder(((InterfacePack*)buf)->m_pBinder);
    ((InterfacePack*)buf)->m_pBinder = NULL;
    data.write((const void *)buf, (size_t)size);
    free(buf);
    data.writeInt32(allowIsolated ? 1 : 0);
    get_service_manager()->transact(ADD_SERVICE, data, &reply);
    return (ECode)reply.readInt32();
}

ECode CServiceManager::GetService(
    /* [in] */ const String& name,
    /* [out] */ IInterface** service)
{
    VALIDATE_NOT_NULL(service);
    *service = NULL;

    if (name.IsNullOrEmpty()) {
        return NOERROR;
    }

    AutoLock lock(mServiceCacheLock);
    HashMap<String, AutoPtr<IInterface> >::Iterator it =
            mServiceCache.Find(name);
    if (it != mServiceCache.End()) {
        *service = it->mSecond;
        REFCOUNT_ADD(*service);
        return NOERROR;
    }

    ECode ec = NOERROR;
    android::Parcel data, reply;
    void * buf = NULL;
    android::sp<android::IBinder> binder;
    Int32 size;

    data.writeCString(name.string());
    android::status_t ret = get_service_manager()->transact(GET_SERVICE, data, &reply);
    if (ret != android::NO_ERROR) {
        Slogger::E("CServiceManager", "Failed to transact GET_SERVICE %s", name.string());
        return E_REMOTE_EXCEPTION;
    }

    ec = (ECode)reply.readInt32();
    if (FAILED(ec)) {
        Slogger::E("CServiceManager", "Failed to get service %s, ec=%08x", name.string(), ec);
        return ec;
    }

    size = reply.readInt32();
    binder = reply.readStrongBinder();
    buf = malloc(size);
    if (!buf) {
        Slogger::E("CServiceManager", "Failed to get service %s, no enough memory, needs %d",
            name.string(), size);
        return E_OUT_OF_MEMORY;
    }

    reply.read(buf, (size_t)size);
    ((InterfacePack*)buf)->m_pBinder = binder;
    ec = _CObject_UnmarshalInterface(
        buf, MarshalType_IPC, UnmarshalFlag_Noncoexisting, service, &size);
    free(buf);
    if (SUCCEEDED(ec)) {
        mServiceCache[name] = *service;
        return NOERROR;
    }

    Slogger::E("CServiceManager", "Failed to get service %s, ec=%08x", name.string(), ec);
    return ec;
}

/**
 * This is only intended to be called when the process is first being brought
 * up and bound by the activity manager. There is only one thread in the process
 * at that time, so no locking is done.
 *
 * @param cache the cache of service references
 * @hide
 */
ECode CServiceManager::InitServiceCache(
    /* [in] */ IMap* services)
{
    if (!mServiceCache.IsEmpty() && Process::SupportsProcesses()) {
//        throw new IllegalStateException("setServiceCache may only be called once");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    sCache.putAll(cache);
    assert(0);
    return NOERROR;
}

ECode CServiceManager::CheckService(
    /* [in] */ const String& name,
    /* [out] */ IInterface** service)
{
    assert(0);
    return NOERROR;
}

static String String16ToString(const android::String16& str16)
{
    Int32 size = str16.size();
    const char16_t* p = str16.string();
    AutoPtr<ArrayOf<Char32> > buf = ArrayOf<Char32>::Alloc(size);
    for (Int32 i = 0; i < size; ++i) {
        buf->Set(i, (Char32)(*(p + i)));
    }
    String str(*buf);
    return str;
}

ECode CServiceManager::ListServices(
    /* [out, callee] */ ArrayOf<String>** services)
{
    VALIDATE_NOT_NULL(services)

    AutoPtr< ArrayOf<String> > array;

    android::Parcel data, reply;
    android::status_t ret = get_service_manager()->transact(LIST_SERVICES, data, &reply);
    if (ret != android::NO_ERROR) {
        array = ArrayOf<String>::Alloc(0);
        *services = array;
        REFCOUNT_ADD(*services)
        return E_REMOTE_EXCEPTION;
    }

    Int32 size = reply.readInt32();
    array = ArrayOf<String>::Alloc(size);
    for (Int32 i = 0; i < size; ++i) {
        android::String16 str16 = reply.readString16();
        array->Set(i, String16ToString(str16));
    }

    *services = array;
    REFCOUNT_ADD(*services)
    return NOERROR;
}

ECode CServiceManager::SetPermissionController(
    /* [in] */ IPermissionController* controller)
{
    assert(0);
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
