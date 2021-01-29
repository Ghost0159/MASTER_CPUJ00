/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless-all
  File:     wh_config.h

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wh_config.h,v $
  Revision 1.1  2006/04/10 13:07:32  yosizaki
  initial upload.

  $NoKeywords$
 *---------------------------------------------------------------------------*/

#ifndef WM_DEMO_WIRELESS_ALL_WH_CONFIG_H_
#define WM_DEMO_WIRELESS_ALL_WH_CONFIG_H_


/* ���̃f���Ŏg�p���� GGID */
#define WH_GGID                   SDK_MAKEGGID_SYSTEM(0x15)

// �����Ŏg�p����DMA�ԍ�
#define WH_DMA_NO                 2

#define WH_PLAYER_MAX             4

// �q�@�ő吔�i�e�@���܂܂Ȃ����j
#define WH_CHILD_MAX              (WH_PLAYER_MAX - 1)

// �V�F�A�o����f�[�^�̍ő�T�C�Y
#define WH_DS_DATA_SIZE           32

#define WH_PARENT_MAX_SIZE        512
#define WH_CHILD_MAX_SIZE       (WH_DS_DATA_SIZE)

// �ʏ�� MP �ʐM�Ŏg�p����|�[�g
#define WH_DATA_PORT              14

// �ʏ�� MP �ʐM�Ŏg�p����D��x
#define WH_DATA_PRIO              WM_PRIORITY_NORMAL

// WFS �ƃf�[�^�V�F�A�����O�𕹗p���邽�ߘA���]�����[�h�� */
#define WH_MP_FREQUENCY           0

// �f�[�^�V�F�A�����O�Ŏg�p����|�[�g
#define WH_DS_PORT                13

// WFS�Ŏg�p����|�[�g
#define PORT_WFS                  4

#endif /* WM_DEMO_WIRELESS_ALL_WH_CONFIG_H_ */
