/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - simple-1
  File:     wc.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wc.c,v $
  Revision 1.32  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.31  2004/10/22 11:06:01  terui
  wc���W���[���͖������L�̈�ɐ؂�o����܂����B

  Revision 1.30  2004/10/18 04:17:46  terui
  �r�[�R���Ԋu�A�X�L�����Ԋu���΂������d�l��ǉ��B

  Revision 1.29  2004/10/05 09:45:29  terui
  MAC�A�h���X��u8�^�̔z��ɓ���B

  Revision 1.28  2004/10/04 13:40:25  terui
  �Q�[���O���[�vID��u32�^�ɓ���B

  Revision 1.27  2004/09/30 08:45:31  seiki_masashi
  GGID �Ɋ���U��ꂽ�����Ȓl��ݒ�

  Revision 1.26  2004/09/28 09:08:04  terui
  WMBssDesc��length��r�����[�h�P�ʂł��邱�Ƃ�z�肵�Ă��Ȃ������o�O���C���B

  Revision 1.25  2004/09/27 13:32:48  terui
  Scan���ɒ��O�̃X�L������GameInfo���c���Ă���ꍇ�ւ̑Ώ�

  Revision 1.24  2004/09/15 12:21:09  terui
  �f�o�b�O�o�͂Ɋւ���֐��y�ђ�`��ύX�B

  Revision 1.23  2004/09/10 12:08:10  seiki_masashi
  �L�[�V�F�A�����O�̑��M�o�C�g�������C�u�������ŉB������悤�ɕύX

  Revision 1.22  2004/09/10 11:49:25  terui
  �������A�I�����̏������R�i�K�ɕ�����d�l�ւ̑Ή��B

  Revision 1.21  2004/09/03 04:42:13  seiki_masashi
  ���M���s�� errcode �� WM_ERRCODE_SEND_FAILED �ɕύX

  Revision 1.20  2004/09/01 05:51:05  seiki_masashi
  MP.ind �ł� WM_ERRCODE_INVALID_POLLBITMAP �ւ̑Ή�

  Revision 1.19  2004/09/01 05:17:27  seiki_masashi
  WM_ERRCODE_INVALID_POLLBITMAP �ւ̑Ή�

  Revision 1.18  2004/08/30 05:37:39  seiki_masashi
  ���M�\������O�̑��M�̏I���݂̂ɕύX

  Revision 1.17  2004/08/30 02:41:19  seiki_masashi
  small fix.

  Revision 1.16  2004/08/30 02:10:35  seiki_masashi
  �萔��`�ʒu�̕ύX

  Revision 1.15  2004/08/30 01:56:53  seiki_masashi
  Port ��M�R�[���o�b�N���g�p����悤�ɕύX

  Revision 1.14  2004/08/27 07:55:04  terui
  �r�[�R�����X�g���̏�����ύX�A�ڑ��������̏�����ύX�B

  Revision 1.13  2004/08/23 04:40:54  ooe
  WM_StartConnect()�̎d�l�ύX�ɑΉ��B

  Revision 1.12  2004/08/20 04:31:55  terui
  �ő�ڑ��q�@�䐔�𒴂����ꍇ�̃f�o�b�O�o�͂�ǉ��B

  Revision 1.11  2004/08/20 00:23:09  terui
  WMParentParam�̍\���ύX�ɑΏ��B

  Revision 1.9  2004/08/19 02:55:17  terui
  �G���g���[�ɂ��ڑ�����d�l�̓����ɔ��������B

  Revision 1.8  2004/08/18 06:25:19  terui
  �f�t�H���g�ڑ��`�����l����ύX

  Revision 1.7  2004/08/18 00:27:11  seiki_masashi
  small fix

  Revision 1.6  2004/08/17 12:40:45  seiki_masashi
  WM_StartMP �̈����ɑ��M�o�b�t�@���w�肷��悤�ɕύX

  Revision 1.5  2004/08/09 00:48:31  terui
  WM�̎d�l�ύX�ɑΉ�

  Revision 1.4  2004/07/28 02:48:14  terui
  �m�ۂ��郁�����ʂ̌v�Z��ύX

  Revision 1.3  2004/07/27 08:09:30  yasu
  delete initializing of reserved area

  Revision 1.2  2004/07/24 03:21:15  yasu
  Fix at masking OS_Printf

  Revision 1.1  2004/07/23 15:28:14  terui
  �������ĐV�K�ǉ�

  Revision 1.5  2004/07/23 14:46:05  terui
  fix comment. turn off print debug switch.

  Revision 1.4  2004/07/23 01:03:55  terui
  �R�[�h�𐮗��B

  Revision 1.2  2004/07/15 13:54:29  seiki_masashi
  ���݂� WM �̎d�l�ɏC��

  Revision 1.1  2004/07/15 12:48:42  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
