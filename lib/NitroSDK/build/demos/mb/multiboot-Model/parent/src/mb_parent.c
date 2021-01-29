/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-Model
  File:     mb_parent.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_parent.c,v $
  Revision 1.30  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.29  2004/11/12 09:01:50  yosizaki
  this file was moved to /demos/wireless_shared/mbp.

  Revision 1.28  2004/11/12 05:21:33  takano_makoto
  IsSameMacAddr�֐���WM_IsBssidEqual�֐��ɒu������

  Revision 1.27  2004/11/12 04:38:32  takano_makoto
  �s�K�v��include�w�b�_���폜

  Revision 1.26  2004/11/10 01:12:53  takano_makoto
  WM_CopyBssid���g�p����悤�ɕύX

  Revision 1.25  2004/11/02 18:08:42  takano_makoto
  fix comments.

  Revision 1.24  2004/11/02 09:15:23  takano_makoto
  MAC�A�h���X�̕ۑ���MI_CpuCopy���g�p���Ȃ��悤�ɕύX

  Revision 1.23  2004/10/28 01:31:09  takano_makoto
  wmhigh��wireless_shared��wh�ֈڍs

  Revision 1.22  2004/10/27 02:07:41  takano_makoto
  �s���Ȏq�@CONNECT���ɂ����ɂ�Disconnect�����Ƀ��N�G�X�g�̂��������_��Kick����悤�ɏC��

  Revision 1.21  2004/10/26 09:18:32  takano_makoto
  MB_DisconnectChild��ǉ�

  Revision 1.19  2004/10/21 00:42:42  yosizaki
  add SHARED_WH switch.

  Revision 1.18  2004/10/18 11:44:06  yosizaki
  change to use MB_StartParentEx/MB_EndEx (switch)

  Revision 1.17  2004/10/08 01:26:28  takano_makoto
  Init���̃��[�U���擾�ŁA�R�s�[����src, dest���t�ɂ��Ă����s����C��

  Revision 1.16  2004/10/05 09:45:29  terui
  MAC�A�h���X��u8�^�̔z��ɓ���B

  Revision 1.15  2004/10/05 07:49:36  takano_makoto
  �q�@��Accept��mb_parent.c�̃R�[���o�b�N���ŏ�������悤�ɏC��

  Revision 1.14  2004/10/05 02:15:05  takano_makoto
  �G���g���[��Ԃő҂��āA���ׂĂ̎q�@�ň�ĂɃ_�E�����[�h���J�n����悤�ɕύX�B

  Revision 1.13  2004/10/04 13:40:25  terui
  �Q�[���O���[�vID��u32�^�ɓ���B

  Revision 1.12  2004/10/04 10:09:08  takano_makoto
  MB_COMM_PSTATE_WAIT_TO_SEND�̏�Ԃ�ǉ�

  Revision 1.11  2004/09/21 10:52:25  takano_makoto
  fix ASSERT in MBP_GetChildBmp().

  Revision 1.10  2004/09/21 09:12:49  yasu
  fix typo in MBP_GetChildBmp()

  Revision 1.9  2004/09/21 06:57:03  takano_makoto
  MB_COMM_PSTATE_ERROR�̃G���[�R�[�h�ɑ΂��鏈����ǉ�

  Revision 1.8  2004/09/17 14:10:39  takano_makoto
  fix bugs in MBP_GetPlayerNo.

  Revision 1.7  2004/09/17 11:35:24  takano_makoto
  �֐���`�̋L�q������ύX


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
