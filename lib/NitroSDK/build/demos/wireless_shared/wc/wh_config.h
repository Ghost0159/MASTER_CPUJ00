/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless_shared - wc
  File:     wh_config.h

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wh_config.h,v $
  Revision 1.1  2007/10/03 05:38:57  yosizaki
  initial upload.

  $NoKeywords$
 *---------------------------------------------------------------------------*/

#ifndef WM_DEMO_WIRELESS_SHARED_WC_WH_CONFIG_H_
#define WM_DEMO_WIRELESS_SHARED_WC_WH_CONFIG_H_

#include "wc.h"

#define WH_DMA_NO                 WC_DMA_NO
#define WH_DATA_PORT              WC_DEFAULT_PORT
#define WH_DATA_PRIO              WC_DEFAULT_PORT_PRIO
#define WH_DS_PORT                WC_DS_PORT_NUMBER

// WC�ł͈ȉ��̏���WMParemtParam�̕ϐ��Ƃ��ĊǗ����Ă��邽�߁A
// �Ƃ肠�����ő�l���w�肵��WH�ɏ[���ȃo�b�t�@�T�C�Y���`�������̂�
// wh.c ���Ń}�N�����Ē�`���ĕϐ����Q�Ƃ���悤�؂�ւ���B
#define WH_PLAYER_MAX             16
#define WH_CHILD_MAX              (WH_PLAYER_MAX - 1)
#define WH_DS_DATA_SIZE           32
#define WH_PARENT_MAX_SIZE        512
#define WH_CHILD_MAX_SIZE         512

#define WH_MP_FREQUENCY           0


#endif /* WM_DEMO_WIRELESS_SHARED_WC_WH_CONFIG_H_ */
