/*---------------------------------------------------------------------------*
  Project:  NitroDWC - base - init
  File:     dwc_init.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_init.h,v $
  Revision 1.7  2007/02/16 08:24:00  takayama
  DWC_GetAuthenticatedUserId()��ǉ��B

  Revision 1.6  2005/12/15 05:35:29  arakit
  �f�o�b�O�p��NVRAM�N���A�֐�DWC_Debug_ClearConsoleWiFiInfo()���폜���A
  DWC_Init()�̑S�Ă̖߂�l��������悤��NVRAM��j�󂷂�f�o�b�O�֐�
  DWC_Debug_DWCInitError()��ǉ������B

  Revision 1.5  2005/11/01 10:37:57  arakit
  enum�̒Ԃ���C�������B

  Revision 1.4  2005/09/21 06:54:59  sasakit
  �f�o�b�O�p�̖{��Wi-Fi���[�U�����폜����֐���ǉ��B

  Revision 1.3  2005/09/03 00:49:19  sasakit
  Fix typo cplusplu -> cplusplus

  Revision 1.2  2005/08/31 02:35:41  sasakit
  DWC_Init()���ĂԂƂ��́A�������֘A�֐����Z�b�g���Ȃ��Ă��ǂ��悤�ɂ����B

  Revision 1.1  2005/08/19 12:14:44  sasakit
  �����[�X�Ɍ����ďC��

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_INIT_H_
#define DWC_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    DWC_INIT_RESULT_NOERROR,
    DWC_INIT_RESULT_CREATE_USERID,
    DWC_INIT_RESULT_DESTROY_USERID,
    DWC_INIT_RESULT_DESTROY_OTHER_SETTING,
    DWC_INIT_RESULT_LAST,

    DWC_INIT_RESULT_DESTORY_USERID = DWC_INIT_RESULT_DESTROY_USERID,
    DWC_INIT_RESULT_DESTORY_OTHER_SETTING = DWC_INIT_RESULT_DESTROY_OTHER_SETTING
};

extern int DWC_Init( void* work );

extern u64 DWC_GetAuthenticatedUserId( void );

extern void DWC_Debug_DWCInitError( void* work, int dwc_init_error );

#ifdef __cplusplus
}
#endif

#endif // DWC_INIT_H_
