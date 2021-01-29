/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-1
  File:     wc.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wc.c,v $
  Revision 1.15  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.14  2004/10/28 13:08:07  yosizaki
  remove all the sources. (moved to wireless_shared!)

  Revision 1.13  2004/10/28 11:04:21  yosizaki
  add callback for block-transfer.
  merge with ../wireless_shared.

  Revision 1.12  2004/10/19 10:27:02  miya
  Port��M�R�[���o�b�N�̎g�p�ƃv�����g����NITRO��ʏ�ɕ\������悤�ɕύX

  Revision 1.11  2004/10/18 04:17:46  terui
  �r�[�R���Ԋu�A�X�L�����Ԋu���΂������d�l��ǉ��B

  Revision 1.10  2004/10/05 09:45:29  terui
  MAC�A�h���X��u8�^�̔z��ɓ���B

  Revision 1.9  2004/10/04 13:44:06  terui
  �Q�[���O���[�vID��u32�^�ɓ���B

  Revision 1.8  2004/10/04 05:51:26  miya
  change transfer size according to the programing guideline

  Revision 1.7  2004/09/30 08:28:43  yosizaki
  add SDK_MAKEGGID_SYSTEM macro.

  Revision 1.6  2004/09/13 08:32:26  miya
  WBT_InitParent�̈����̈Ӗ��ύX

  Revision 1.5  2004/09/10 12:28:10  seiki_masashi
  �L�[�V�F�A�����O�̑��M�o�C�g�������C�u�������ŉB������悤�ɕύX

  Revision 1.4  2004/09/09 06:25:17  miya
  small fix.

  Revision 1.3  2004/09/03 04:40:18  seiki_masashi
  ���M���s�� errcode �� WM_ERRCODE_SEND_FAILED �ɕύX
  WM_ERRCODE_INVALID_POLLBITMAP �ւ̑Ή�

  Revision 1.2  2004/08/26 23:54:25  miya
  ���O�ύX�Ȃ�

  Revision 1.1  2004/08/23 09:47:33  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
