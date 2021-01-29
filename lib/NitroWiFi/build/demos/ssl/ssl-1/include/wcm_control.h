/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SSL - demos - ssl-1
  File:     wcm_control.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wcm_control.h,v $
  Revision 1.3  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.2  2005/11/02 02:41:33  terui
  Fix comment.

  Revision 1.1  2005/07/21 11:47:03  seiki_masashi
  ssl/ssl-1 �Ɉړ�

  Revision 1.1  2005/07/18 08:10:26  seiki_masashi
  ssl-1 �f���̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef WCM_CONTROL_H_
#define WCM_CONTROL_H_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         InitWcmControl

  Description:  �����ϐ������������AWCM �R���g���[���X���b�h���N������B
                ���ɃX���b�h���N�����Ă���ꍇ�͉����s��Ȃ��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    InitWcmControl(void);

/*---------------------------------------------------------------------------*
  Name:         TerminateWcmControl

  Description:  WCM �R���g���[���X���b�h���I������B
                �X���b�h���N������Ă��Ȃ��ꍇ�͉����s��Ȃ��B
    NOTICE:     ���֐��̓X���b�h���I������ׂ̃g���K�ƂȂ邾���Ȃ̂ŁA�X���b�h
                ���̂��I������܂ő҂����ɖ߂�_�ɒ��ӁB
                �X���b�h���̂̏I����҂������ꍇ�́AOS_SendMessage �Ăяo����
                ������ OS_JoinThread ���Ăяo���K�v������B
                �Ȃ��AWCM_GetPhase �֐��� WCM_PHASE_NULL ��Ԃ��悤�ɂȂ��
                �X���b�h�͏I�����Ă���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    TerminateWcmControl(void);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif

#endif /* WCM_CONTROL_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
