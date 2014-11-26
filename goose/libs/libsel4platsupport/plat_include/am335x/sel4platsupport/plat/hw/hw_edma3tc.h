/*
 * Copyright 2014, NICTA
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(NICTA_BSD)
 */

/**
 * \file  hw_edma3tc.h
 *
 * \brief EDMA3TC register definitions
 */

/*
* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
*/
/*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


#ifndef _HW_EDMA3TC_H_
#define _HW_EDMA3TC_H_

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************\
* Register Offsets
\**************************************************************************/
#define EDMA3TC_REVID		(0x0)
#define EDMA3TC_TCCFG		(0x4)
#define EDMA3TC_TCSTAT		(0x100)
#define EDMA3TC_ERRSTAT		(0x120)
#define EDMA3TC_ERREN		(0x124)
#define EDMA3TC_ERRCLR		(0x128)
#define EDMA3TC_ERRDET		(0x12C)
#define EDMA3TC_ERRCMD		(0x130)
#define EDMA3TC_RDRATE		(0x140)
#define EDMA3TC_SAOPT		(0x240)
#define EDMA3TC_SASRC		(0x244)
#define EDMA3TC_SACNT		(0x248)
#define EDMA3TC_SADST		(0x24C)
#define EDMA3TC_SABIDX		(0x250)
#define EDMA3TC_SAMPPRXY	(0x254)
#define EDMA3TC_SACNTRLD	(0x258)
#define EDMA3TC_SASRCBREF	(0x25C)
#define EDMA3TC_SADSTBREF	(0x260)
#define EDMA3TC_DFCNTRLD	(0x280)
#define EDMA3TC_DFSRCBREF	(0x284)
#define EDMA3TC_DFDSTBREF	(0x288)
#define EDMA3TC_DFOPT(n)	(0x300 + (0x40 * 0))
#define EDMA3TC_DFSRC(n)	(0x304 + (0x40 * 0))
#define EDMA3TC_DFCNT		(0x308 + (0x40 * 0))
#define EDMA3TC_DFDST		(0x30C + (0x40 * 0))
#define EDMA3TC_DFBIDX		(0x310 + (0x40 * 0))
#define EDMA3TC_DFMPPRXY	(0x314 + (0x40 * 0))

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* DFOPT */
#define EDMA3TC_DFOPT_TCCHEN (0x00400000u)
#define EDMA3TC_DFOPT_TCCHEN_SHIFT (0x00000016u)
#define EDMA3TC_DFOPT_TCINTEN (0x00100000u)
#define EDMA3TC_DFOPT_TCINTEN_SHIFT (0x00000014u)
#define EDMA3TC_DFOPT_TCC (0x0003F000u)
#define EDMA3TC_DFOPT_TCC_SHIFT (0x0000000Cu)
#define EDMA3TC_DFOPT_FWID (0x00000700u)
#define EDMA3TC_DFOPT_FWID_SHIFT (0x00000008u)
/*----FWID Tokens----*/
#define EDMA3TC_DFOPT_FWID_8BIT (0x00000000u)
#define EDMA3TC_DFOPT_FWID_16BIT (0x00000001u)
#define EDMA3TC_DFOPT_FWID_32BIT (0x00000002u)
#define EDMA3TC_DFOPT_FWID_64BIT (0x00000003u)
#define EDMA3TC_DFOPT_FWID_128BIT (0x00000004u)
#define EDMA3TC_DFOPT_FWID_256BIT (0x00000005u)
#define EDMA3TC_DFOPT_PRI (0x00000070u)
#define EDMA3TC_DFOPT_PRI_SHIFT (0x00000004u)
#define EDMA3TC_DFOPT_PRI_PRI0 (0x00000000u)
#define EDMA3TC_DFOPT_PRI_PRI1 (0x00000001u)
#define EDMA3TC_DFOPT_PRI_PRI2 (0x00000002u)
#define EDMA3TC_DFOPT_PRI_PRI3 (0x00000003u)
#define EDMA3TC_DFOPT_PRI_PRI4 (0x00000004u)
#define EDMA3TC_DFOPT_PRI_PRI5 (0x00000005u)
#define EDMA3TC_DFOPT_PRI_PRI6 (0x00000006u)
#define EDMA3TC_DFOPT_PRI_PRI7 (0x00000007u)
#define EDMA3TC_DFOPT_DAM (0x00000002u)
#define EDMA3TC_DFOPT_DAM_SHIFT (0x00000001u)
#define EDMA3TC_DFOPT_SAM (0x00000001u)
#define EDMA3TC_DFOPT_SAM_SHIFT (0x00000000u)

/* DFSRC */

#define EDMA3TC_DFSRC_SADDR (0xFFFFFFFFu)
#define EDMA3TC_DFSRC_SADDR_SHIFT (0x00000000u)


/* DFCNT */

#define EDMA3TC_DFCNT_BCNT (0xFFFF0000u)
#define EDMA3TC_DFCNT_BCNT_SHIFT (0x00000010u)

#define EDMA3TC_DFCNT_ACNT (0x0000FFFFu)
#define EDMA3TC_DFCNT_ACNT_SHIFT (0x00000000u)


/* DFDST */

#define EDMA3TC_DFDST_DADDR (0xFFFFFFFFu)
#define EDMA3TC_DFDST_DADDR_SHIFT (0x00000000u)


/* DFBIDX */

#define EDMA3TC_DFBIDX_DSTBIDX (0xFFFF0000u)
#define EDMA3TC_DFBIDX_DSTBIDX_SHIFT (0x00000010u)

#define EDMA3TC_DFBIDX_SRCBIDX (0x0000FFFFu)
#define EDMA3TC_DFBIDX_SRCBIDX_SHIFT (0x00000000u)


/* DFMPPRXY */


#define EDMA3TC_DFMPPRXY_PRIV (0x00000100u)
#define EDMA3TC_DFMPPRXY_PRIV_SHIFT (0x00000008u)
#define EDMA3TC_DFMPPRXY_PRIVID (0x0000000Fu)
#define EDMA3TC_DFMPPRXY_PRIVID_SHIFT (0x00000000u)
/*----PRIVID Tokens----*/
#define EDMATC_DFMPPRXY_PRIVID_ARM   (0x00000000u)
#define EDMATC_DFMPPRXY_PRIVID_DSP   (0x00000001u)
#define EDMATC_DFMPPRXY_PRIVID_DMAX  (0x00000002u)
#define EDMATC_DFMPPRXY_PRIVID_HPI   (0x00000003u)
#define EDMATC_DFMPPRXY_PRIVID_EMAC  (0x00000004u)
#define EDMATC_DFMPPRXY_PRIVID_USB1  (0x00000005u)
#define EDMATC_DFMPPRXY_PRIVID_USB0  (0x00000006u)
#define EDMATC_DFMPPRXY_PRIVID_LCDC  (0x00000007u)


/* PID */

#define EDMA3TC_PID_PID (0xFFFFFFFFu)
#define EDMA3TC_PID_PID_SHIFT (0x00000000u)


/* TCCFG */


#define EDMA3TC_TCCFG_DREGDEPTH (0x00000300u)
#define EDMA3TC_TCCFG_DREGDEPTH_SHIFT (0x00000008u)
/*----DREGDEPTH Tokens----*/
#define EDMA3TC_TCCFG_DREGDEPTH_1ENTRY (0x00000000u)
#define EDMA3TC_TCCFG_DREGDEPTH_2ENTRY (0x00000001u)
#define EDMA3TC_TCCFG_DREGDEPTH_4ENTRY (0x00000002u)


#define EDMA3TC_TCCFG_BUSWIDTH (0x00000030u)
#define EDMA3TC_TCCFG_BUSWIDTH_SHIFT (0x00000004u)
/*----BUSWIDTH Tokens----*/
#define EDMA3TC_TCCFG_BUSWIDTH_32BIT (0x00000000u)
#define EDMA3TC_TCCFG_BUSWIDTH_64BIT (0x00000001u)
#define EDMA3TC_TCCFG_BUSWIDTH_128BIT (0x00000002u)


#define EDMA3TC_TCCFG_FIFOSIZE (0x00000007u)
#define EDMA3TC_TCCFG_FIFOSIZE_SHIFT (0x00000000u)
/*----FIFOSIZE Tokens----*/
#define EDMA3TC_TCCFG_FIFOSIZE_32BYTE (0x00000000u)
#define EDMA3TC_TCCFG_FIFOSIZE_64BYTE (0x00000001u)
#define EDMA3TC_TCCFG_FIFOSIZE_128BYTE (0x00000002u)
#define EDMA3TC_TCCFG_FIFOSIZE_256BYTE (0x00000003u)
#define EDMA3TC_TCCFG_FIFOSIZE_512BYTE (0x00000004u)


/* TCSTAT */


#define EDMA3TC_TCSTAT_DFSTRTPTR (0x00001800u)
#define EDMA3TC_TCSTAT_DFSTRTPTR_SHIFT (0x0000000Bu)
#define EDMA3TC_TCSTAT_DSTACTV (0x00000070u)
#define EDMA3TC_TCSTAT_DSTACTV_SHIFT (0x00000004u)
#define EDMA3TC_TCSTAT_DSTACTV_EMPTY (0x00000000u)
#define EDMA3TC_TCSTAT_DSTACTV_1TR (0x00000001u)
#define EDMA3TC_TCSTAT_DSTACTV_2TR (0x00000002u)
#define EDMA3TC_TCSTAT_DSTACTV_3TR (0x00000003u)
#define EDMA3TC_TCSTAT_DSTACTV_4TR (0x00000004u)


#define EDMA3TC_TCSTAT_WSACTV (0x00000004u)
#define EDMA3TC_TCSTAT_WSACTV_SHIFT (0x00000002u)
#define EDMA3TC_TCSTAT_SRCACTV (0x00000002u)
#define EDMA3TC_TCSTAT_SRCACTV_SHIFT (0x00000001u)
#define EDMA3TC_TCSTAT_PROGBUSY (0x00000001u)
#define EDMA3TC_TCSTAT_PROGBUSY_SHIFT (0x00000000u)

/* ERRSTAT */


#define EDMA3TC_ERRSTAT_MMRAERR (0x00000008u)
#define EDMA3TC_ERRSTAT_MMRAERR_SHIFT (0x00000003u)
#define EDMA3TC_ERRSTAT_TRERR (0x00000004u)
#define EDMA3TC_ERRSTAT_TRERR_SHIFT (0x00000002u)
#define EDMA3TC_ERRSTAT_BUSERR (0x00000001u)
#define EDMA3TC_ERRSTAT_BUSERR_SHIFT (0x00000000u)

/* ERREN */


#define EDMA3TC_ERREN_MMRAERR (0x00000008u)
#define EDMA3TC_ERREN_MMRAERR_SHIFT (0x00000003u)
#define EDMA3TC_ERREN_TRERR (0x00000004u)
#define EDMA3TC_ERREN_TRERR_SHIFT (0x00000002u)
#define EDMA3TC_ERREN_BUSERR (0x00000001u)
#define EDMA3TC_ERREN_BUSERR_SHIFT (0x00000000u)


/* ERRCLR */


#define EDMA3TC_ERRCLR_MMRAERR (0x00000008u)
#define EDMA3TC_ERRCLR_MMRAERR_SHIFT (0x00000003u)
#define EDMA3TC_ERRCLR_TRERR (0x00000004u)
#define EDMA3TC_ERRCLR_TRERR_SHIFT (0x00000002u)
#define EDMA3TC_ERRCLR_BUSERR (0x00000001u)
#define EDMA3TC_ERRCLR_BUSERR_SHIFT (0x00000000u)

/* ERRDET */


#define EDMA3TC_ERRDET_TCCHEN (0x00020000u)
#define EDMA3TC_ERRDET_TCCHEN_SHIFT (0x00000011u)

#define EDMA3TC_ERRDET_TCINTEN (0x00010000u)
#define EDMA3TC_ERRDET_TCINTEN_SHIFT (0x00000010u)


#define EDMA3TC_ERRDET_TCC (0x00003F00u)
#define EDMA3TC_ERRDET_TCC_SHIFT (0x00000008u)


#define EDMA3TC_ERRDET_STAT (0x0000000Fu)
#define EDMA3TC_ERRDET_STAT_SHIFT (0x00000000u)
/*----STAT Tokens----*/
#define EDMA3TC_ERRDET_STAT_NONE (0x00000000u)
#define EDMA3TC_ERRDET_STAT_READ_ADDRESS (0x00000001u)
#define EDMA3TC_ERRDET_STAT_READ_PRIVILEGE (0x00000002u)
#define EDMA3TC_ERRDET_STAT_READ_TIMEOUT (0x00000003u)
#define EDMA3TC_ERRDET_STAT_READ_DATA (0x00000004u)
#define EDMA3TC_ERRDET_STAT_READ_EXCLUSIVE (0x00000007u)
#define EDMA3TC_ERRDET_STAT_WRITE_ADDRESS (0x00000009u)
#define EDMA3TC_ERRDET_STAT_WRITE_PRIVILEGE (0x0000000Au)
#define EDMA3TC_ERRDET_STAT_WRITE_TIMEOUT (0x0000000Bu)
#define EDMA3TC_ERRDET_STAT_WRITE_DATA (0x0000000Cu)
#define EDMA3TC_ERRDET_STAT_WRITE_EXCLUSIVE (0x0000000Fu)


/* ERRCMD */


#define EDMA3TC_ERRCMD_EVAL (0x00000001u)
#define EDMA3TC_ERRCMD_EVAL_SHIFT (0x00000000u)

/* RDRATE */


#define EDMA3TC_RDRATE_RDRATE (0x00000007u)
#define EDMA3TC_RDRATE_RDRATE_SHIFT (0x00000000u)
/*----RDRATE Tokens----*/
#define EDMA3TC_RDRATE_RDRATE_AFAP (0x00000000u)
#define EDMA3TC_RDRATE_RDRATE_4CYCLE (0x00000001u)
#define EDMA3TC_RDRATE_RDRATE_8CYCLE (0x00000002u)
#define EDMA3TC_RDRATE_RDRATE_16CYCLE (0x00000003u)
#define EDMA3TC_RDRATE_RDRATE_32CYCLE (0x00000004u)


/* SAOPT */


#define EDMA3TC_SAOPT_TCCHEN (0x00400000u)
#define EDMA3TC_SAOPT_TCCHEN_SHIFT (0x00000016u)
#define EDMA3TC_SAOPT_TCINTEN (0x00100000u)
#define EDMA3TC_SAOPT_TCINTEN_SHIFT (0x00000014u)
#define EDMA3TC_SAOPT_TCC (0x0003F000u)
#define EDMA3TC_SAOPT_TCC_SHIFT (0x0000000Cu)
#define EDMA3TC_SAOPT_FWID (0x00000700u)
#define EDMA3TC_SAOPT_FWID_SHIFT (0x00000008u)
/*----FWID Tokens----*/
#define EDMA3TC_SAOPT_FWID_8BIT (0x00000000u)
#define EDMA3TC_SAOPT_FWID_16BIT (0x00000001u)
#define EDMA3TC_SAOPT_FWID_32BIT (0x00000002u)
#define EDMA3TC_SAOPT_FWID_64BIT (0x00000003u)
#define EDMA3TC_SAOPT_FWID_128BIT (0x00000004u)
#define EDMA3TC_SAOPT_FWID_256BIT (0x00000005u)


#define EDMA3TC_SAOPT_PRI (0x00000070u)
#define EDMA3TC_SAOPT_PRI_SHIFT (0x00000004u)
#define EDMA3TC_SAOPT_PRI_PRI0 (0x00000000u)
#define EDMA3TC_SAOPT_PRI_PRI1 (0x00000001u)
#define EDMA3TC_SAOPT_PRI_PRI2 (0x00000002u)
#define EDMA3TC_SAOPT_PRI_PRI3 (0x00000003u)
#define EDMA3TC_SAOPT_PRI_PRI4 (0x00000004u)
#define EDMA3TC_SAOPT_PRI_PRI5 (0x00000005u)
#define EDMA3TC_SAOPT_PRI_PRI6 (0x00000006u)
#define EDMA3TC_SAOPT_PRI_PRI7 (0x00000007u)


#define EDMA3TC_SAOPT_DAM (0x00000002u)
#define EDMA3TC_SAOPT_DAM_SHIFT (0x00000001u)
#define EDMA3TC_SAOPT_SAM (0x00000001u)
#define EDMA3TC_SAOPT_SAM_SHIFT (0x00000000u)

/* SASRC */

#define EDMA3TC_SASRC_SADDR (0xFFFFFFFFu)
#define EDMA3TC_SASRC_SADDR_SHIFT (0x00000000u)


/* SACNT */

#define EDMA3TC_SACNT_BCNT (0xFFFF0000u)
#define EDMA3TC_SACNT_BCNT_SHIFT (0x00000010u)

#define EDMA3TC_SACNT_ACNT (0x0000FFFFu)
#define EDMA3TC_SACNT_ACNT_SHIFT (0x00000000u)


/* SADST */

#define EDMA3TC_SADST_DADDR (0xFFFFFFFFu)
#define EDMA3TC_SADST_DADDR_SHIFT (0x00000000u)


/* SABIDX */

#define EDMA3TC_SABIDX_DSTBIDX (0xFFFF0000u)
#define EDMA3TC_SABIDX_DSTBIDX_SHIFT (0x00000010u)

#define EDMA3TC_SABIDX_SRCBIDX (0x0000FFFFu)
#define EDMA3TC_SABIDX_SRCBIDX_SHIFT (0x00000000u)


/* SAMPPRXY */


#define EDMA3TC_SAMPPRXY_PRIV (0x00000100u)
#define EDMA3TC_SAMPPRXY_PRIV_SHIFT (0x00000008u)
#define EDMA3TC_SAMPPRXY_PRIVID (0x0000000Fu)
#define EDMA3TC_SAMPPRXY_PRIVID_SHIFT (0x00000000u)
/*----PRIVID Tokens----*/
#define EDMATC_SAMPPRXY_PRIVID_ARM   (0x00000000u)
#define EDMATC_SAMPPRXY_PRIVID_DSP   (0x00000001u)
#define EDMATC_SAMPPRXY_PRIVID_DMAX  (0x00000002u)
#define EDMATC_SAMPPRXY_PRIVID_HPI   (0x00000003u)
#define EDMATC_SAMPPRXY_PRIVID_EMAC  (0x00000004u)
#define EDMATC_SAMPPRXY_PRIVID_USB1  (0x00000005u)
#define EDMATC_SAMPPRXY_PRIVID_USB0  (0x00000006u)
#define EDMATC_SAMPPRXY_PRIVID_LCDC  (0x00000007u)


/* SACNTRLD */


#define EDMA3TC_SACNTRLD_ACNTRLD (0x0000FFFFu)
#define EDMA3TC_SACNTRLD_ACNTRLD_SHIFT (0x00000000u)


/* SASRCBREF */

#define EDMA3TC_SASRCBREF_SADDRBREF (0xFFFFFFFFu)
#define EDMA3TC_SASRCBREF_SADDRBREF_SHIFT (0x00000000u)


/* SADSTBREF */

#define EDMA3TC_SADSTBREF_DADDRBREF (0xFFFFFFFFu)
#define EDMA3TC_SADSTBREF_DADDRBREF_SHIFT (0x00000000u)


/* DFCNTRLD */


#define EDMA3TC_DFCNTRLD_ACNTRLD (0x0000FFFFu)
#define EDMA3TC_DFCNTRLD_ACNTRLD_SHIFT (0x00000000u)


/* DFSRCBREF */

#define EDMA3TC_DFSRCBREF_SADDRBREF (0xFFFFFFFFu)
#define EDMA3TC_DFSRCBREF_SADDRBREF_SHIFT (0x00000000u)


/* DFDSTBREF */

#define EDMA3TC_DFDSTBREF_DADDRBREF (0xFFFFFFFFu)
#define EDMA3TC_DFDSTBREF_DADDRBREF_SHIFT (0x00000000u)


#ifdef __cplusplus
}
#endif

#endif

