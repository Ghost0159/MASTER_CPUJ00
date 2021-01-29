/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - shared - demos
  File:     ap_info.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ap_info.c,v $
  Revision 1.4  2006/03/15 10:07:24  kitase_hirotake
  INDENT-OFF �̒ǉ�

  Revision 1.3  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.2  2005/11/02 04:13:42  terui
  Fix comment.

  Revision 1.1  2005/07/21 08:38:16  terui
  �T���v���ŋ��L�\�ȃ\�[�X�t�@�C���Ƃ��ĐV�K�ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include "ap_info.h"

/*---------------------------------------------------------------------------*
    < AP ��� > ���̃t�@�C���̒�`���A�ڑ����� AP �ɍ��킹�ĕҏW���ĉ������B

                �Ȃ��A�ҏW���Ȃ��ꍇ�� �C�ӂ� BSS-ID�A�C�ӂ� ESS-ID�A�I�[�v��
                �V�X�e���F�؁AWEP �Í����͂Ȃ��A�Ƃ����ݒ�ɂȂ��Ă��܂��B
 *---------------------------------------------------------------------------*/
#define LOCAL_WCM_WEP_MODE  WCM_WEPMODE_NONE

/* *INDENT-OFF* */
SDK_WEAK_SYMBOL const WcmControlApInfo  constApInfo =
{
    { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff } ,        // BSS-ID

    {
        LOCAL_WCM_WEP_MODE ,                        // WEP �Í������[�h
        0 ,                                         // WEP �L�[�ԍ�

        /* WEP �L�[�f�[�^�� */
#if     ( LOCAL_WCM_WEP_MODE == WCM_WEPMODE_40 )
        {
            0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        }
#elif   ( LOCAL_WCM_WEP_MODE == WCM_WEPMODE_104 )
        {
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        }
#elif   ( LOCAL_WCM_WEP_MODE == WCM_WEPMODE_128 )
        {
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00
        }
#else /* ( LOCAL_WCM_WEP_MODE == WCM_WEPMODE_NONE ) */
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        }
#endif
    } ,

    { "" } ,                                        // ESS-ID

    WCM_OPTION_AUTH_OPENSYSTEM                      // 802.11 �F�؃��[�h
};
/* *INDENT-ON* */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/