/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WVR - include
  File:     wvr.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wvr.h,v $
  Revision 1.3  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.2  2005/02/09 09:13:16  terui
  StartUp�֐��Ƀp�����[�^��ǉ��B
  VRAM���w��\�ɂȂ������Ƃɂ��A�֐��������C���B

  Revision 1.1  2004/12/20 00:44:36  terui
  Initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITRO_WVR_ARM9_WVR_H_
#define NITRO_WVR_ARM9_WVR_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*===========================================================================*/

#include    <nitro/gx/gx_vramcnt.h>

// �񓯊� API �̃R�[���o�b�N�֐��^
typedef void (*WVRCallbackFunc) (void *arg, WVRResult result);


/*---------------------------------------------------------------------------*
  Name:         WVR_StartUpAsync

  Description:  �������C�u�������쓮�J�n����B
                ������~����܂ŁA�w�肵��VRAM( C or D )�ւ̃A�N�Z�X�͋֎~�ɂȂ�B

  Arguments:    vram        -   ARM7�Ɋ��蓖�Ă�VRAM�o���N���w��B
                callback    -   �����������̃R�[���o�b�N�֐����w��B
                arg         -   �R�[���o�b�N�֐��ɓn���������w��B

  Returns:      �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WVRResult WVR_StartUpAsync(GXVRamARM7 vram, WVRCallbackFunc callback, void *arg);

/*---------------------------------------------------------------------------*
  Name:         WVR_TerminateAsync

  Description:  �������C�u������������~����B
                �񓯊�����������AVRAM( C or D )�ւ̃A�N�Z�X�͋������B

  Arguments:    callback    -   �����������̃R�[���o�b�N�֐����w��B
                arg         -   �R�[���o�b�N�֐��ɓn���������w��B

  Returns:      �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WVRResult WVR_TerminateAsync(WVRCallbackFunc callback, void *arg);


/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* NITRO_WVR_ARM9_WVR_H_ */

/*---------------------------------------------------------------------------*
    End of file
 *---------------------------------------------------------------------------*/
