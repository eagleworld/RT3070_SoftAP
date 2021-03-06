/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2009, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************
 
 	Module Name:
	rtusb_io.h

	Abstract:

	Revision History:
	Who			When	    What
	--------	----------  ----------------------------------------------
	Name		Date	    Modification logs
*/

#ifndef __RTUSB_IO_H__
#define __RTUSB_IO_H__

#include "wpa_cmm.h"
#include "rtmp_type.h"

/* First RTUSB IO command number */
#define CMDTHREAD_FIRST_CMD_ID						0x0D730101

#define CMDTHREAD_RESET_BULK_OUT						0x0D730101
#define CMDTHREAD_RESET_BULK_IN						0x0D730102
#define CMDTHREAD_CHECK_GPIO							0x0D730103	
#define CMDTHREAD_SET_ASIC_WCID						0x0D730104
#define CMDTHREAD_SET_CLIENT_MAC_ENTRY				0x0D730105	


#ifdef CONFIG_AP_SUPPORT
#define CMDTHREAD_AP_UPDATE_CAPABILITY_AND_ERPIE	0x0D73010A
#define CMDTHREAD_AP_ENABLE_TX_BURST				0x0D73010B
#define CMDTHREAD_AP_DISABLE_TX_BURST				0x0D73010C
#define CMDTHREAD_AP_ADJUST_EXP_ACK_TIME			0x0D73010D
#define CMDTHREAD_AP_RECOVER_EXP_ACK_TIME			0x0D73010E
#define CMDTHREAD_CHAN_RESCAN						0x0D73010F
#endif // CONFIG_AP_SUPPORT //

#ifdef WSC_INCLUDED
#ifdef WSC_LED_SUPPORT
#define CMDTHREAD_SET_WPS_LED_STATUS				0x0D730110 // Set WPS LED status (LED_WPS_XXX).
#define CMDTHREAD_LED_WPS_MODE10					0x0D730111
#endif // WSC_LED_SUPPORT //
#endif // WSC_INCLUDED //	

/* Security related */
#define CMDTHREAD_SET_WCID_SEC_INFO					0x0D730112
#define CMDTHREAD_SET_ASIC_WCID_IVEIV				0x0D730113
#define CMDTHREAD_SET_ASIC_WCID_ATTR				0x0D730114
#define CMDTHREAD_SET_ASIC_SHARED_KEY				0x0D730115
#define CMDTHREAD_SET_ASIC_PAIRWISE_KEY				0x0D730116
#define CMDTHREAD_REMOVE_PAIRWISE_KEY				0x0D730117

#ifdef CONFIG_AP_SUPPORT
#define CMDTHREAD_802_11_COUNTER_MEASURE			0x0D730119
#endif // CONFIG_AP_SUPPORT //

// add by johnli, fix "in_interrupt" error when call "MacTableDeleteEntry" in Rx tasklet
#define CMDTHREAD_UPDATE_PROTECT					0x0D73011A
// end johnli

#ifdef LINUX
#ifdef RT_CFG80211_SUPPORT
#define CMDTHREAD_REG_HINT							0x0D73011B
#define CMDTHREAD_REG_HINT_11D						0x0D73011C
#define CMDTHREAD_SCAN_END							0x0D73011D
#endif // RT_CFG80211_SUPPORT //
#endif // LINUX //

#ifdef BT_COEXISTENCE_SUPPORT
#define RT_CMD_BT_COEXISTENCE_DETECTION				0x0D73011E
#endif // BT_COEXISTENCE_SUPPORT //



typedef	struct	_CMDHandler_TLV	{
	USHORT		Offset;
	USHORT		Length;
	UCHAR		DataFirst;
}	CMDHandler_TLV, *PCMDHandler_TLV;



typedef struct   _RT_SET_ASIC_WCID {
	ULONG WCID;          // mechanism for rekeying: 0:disable, 1: time-based, 2: packet-based
	ULONG SetTid;        // time-based: seconds, packet-based: kilo-packets
	ULONG DeleteTid;        // time-based: seconds, packet-based: kilo-packets
	UCHAR Addr[MAC_ADDR_LEN];	// avoid in interrupt when write key
} RT_SET_ASIC_WCID,*PRT_SET_ASIC_WCID;

typedef struct   _RT_ASIC_WCID_SEC_INFO {
	UCHAR		BssIdx;
	UCHAR		KeyIdx;
	UCHAR		CipherAlg;
	UINT8		Wcid;
	UINT8		KeyTabFlag;
} RT_ASIC_WCID_SEC_INFO,*PRT_ASIC_WCID_SEC_INFO;

typedef struct   _RT_ASIC_WCID_IVEIV_ENTRY {
	UINT8		Wcid;
	UINT32		Iv;
	UINT32		Eiv;
} RT_ASIC_WCID_IVEIV_ENTRY,*PRT_ASIC_WCID_IVEIV_ENTRY;

typedef struct   _RT_ASIC_WCID_ATTR_ENTRY {
	UCHAR		BssIdx;
	UCHAR		KeyIdx;
	UCHAR		CipherAlg;
	UINT8		Wcid;
	UINT8		KeyTabFlag;
} RT_ASIC_WCID_ATTR_ENTRY,*PRT_ASIC_WCID_ATTR_ENTRY;

typedef struct  _RT_ASIC_PAIRWISE_KEY {
	UINT8		WCID;
	CIPHER_KEY	CipherKey;
} RT_ASIC_PAIRWISE_KEY, *PRT_ASIC_PAIRWISE_KEY;

typedef struct  _RT_ASIC_SHARED_KEY {
	UCHAR		BssIndex;
	UCHAR		KeyIdx;
	CIPHER_KEY 	CipherKey;
} RT_ASIC_SHARED_KEY, *PRT_ASIC_SHARED_KEY;

/******************************************************************************

  	USB Cmd to ASIC Related MACRO
  	
******************************************************************************/
// reset MAC of a station entry to 0xFFFFFFFFFFFF
#define RTMP_STA_ENTRY_MAC_RESET(pAd, Wcid)					\
	{	RT_SET_ASIC_WCID	SetAsicWcid;						\
		SetAsicWcid.WCID = Wcid;								\
		SetAsicWcid.SetTid = 0xffffffff;						\
		SetAsicWcid.DeleteTid = 0xffffffff;						\
		RTEnqueueInternalCmd(pAd, CMDTHREAD_SET_ASIC_WCID, 	\
				&SetAsicWcid, sizeof(RT_SET_ASIC_WCID));	}

// add by johnli, fix "in_interrupt" error when call "MacTableDeleteEntry" in Rx tasklet
// Set MAC register value according operation mode
#define RTMP_UPDATE_PROTECT(pAd)	\
 	RTEnqueueInternalCmd(pAd, CMDTHREAD_UPDATE_PROTECT, NULL, 0);
#ifdef CONFIG_AP_SUPPORT
#define RTMP_AP_UPDATE_CAPABILITY_AND_ERPIE(pAd)	\
 	RTEnqueueInternalCmd(pAd, CMDTHREAD_AP_UPDATE_CAPABILITY_AND_ERPIE, NULL, 0);
#endif // CONFIG_AP_SUPPORT //
// end johnli

// Insert the BA bitmap to ASIC for the Wcid entry
#define RTMP_ADD_BA_SESSION_TO_ASIC(_pAd, _Aid, _TID)					\
		do{																\
			RT_SET_ASIC_WCID	SetAsicWcid;							\
			SetAsicWcid.WCID = (_Aid);									\
			SetAsicWcid.SetTid = (0x10000<<(_TID));						\
			SetAsicWcid.DeleteTid = 0xffffffff;							\
			RTEnqueueInternalCmd((_pAd), CMDTHREAD_SET_ASIC_WCID, &SetAsicWcid, sizeof(RT_SET_ASIC_WCID));	\
		}while(0)

// Remove the BA bitmap from ASIC for the Wcid entry
#define RTMP_DEL_BA_SESSION_FROM_ASIC(_pAd, _Wcid, _TID)				\
		do{																\
			RT_SET_ASIC_WCID	SetAsicWcid;							\
			SetAsicWcid.WCID = (_Wcid);									\
			SetAsicWcid.SetTid = (0xffffffff);							\
			SetAsicWcid.DeleteTid = (0x10000<<(_TID) );					\
			RTEnqueueInternalCmd((_pAd), CMDTHREAD_SET_ASIC_WCID, &SetAsicWcid, sizeof(RT_SET_ASIC_WCID));	\
		}while(0)


#endif // __RTUSB_IO_H__ //
