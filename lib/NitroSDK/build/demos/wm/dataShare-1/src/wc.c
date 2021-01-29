/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - dataShare-1
  File:     wc.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wc.c,v $
  Revision 1.30  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.29  2004/10/22 11:05:54  terui
  wc���W���[���͖������L�̈�ɐ؂�o����܂����B

  Revision 1.28  2004/10/18 04:17:46  terui
  �r�[�R���Ԋu�A�X�L�����Ԋu���΂������d�l��ǉ��B

  Revision 1.27  2004/10/05 09:45:29  terui
  MAC�A�h���X��u8�^�̔z��ɓ���B

  Revision 1.26  2004/10/04 13:40:25  terui
  �Q�[���O���[�vID��u32�^�ɓ���B

  Revision 1.25  2004/09/30 08:45:09  seiki_masashi
  GGID �Ɋ���U��ꂽ�����Ȓl��ݒ�

  Revision 1.24  2004/09/28 09:08:04  terui
  WMBssDesc��length��r�����[�h�P�ʂł��邱�Ƃ�z�肵�Ă��Ȃ������o�O���C���B

  Revision 1.23  2004/09/27 13:32:48  terui
  Scan���ɒ��O�̃X�L������GameInfo���c���Ă���ꍇ�ւ̑Ώ�

  Revision 1.22  2004/09/15 12:21:17  terui
  �f�o�b�O�o�͂Ɋւ���֐��y�ђ�`��ύX�B

  Revision 1.21  2004/09/10 12:08:10  seiki_masashi
  �L�[�V�F�A�����O�̑��M�o�C�g�������C�u�������ŉB������悤�ɕύX

  Revision 1.20  2004/09/10 11:53:07  terui
  �������A�I�����̏������R�i�K�ɕ�����d�l�ւ̑Ή��B

  Revision 1.19  2004/09/10 04:36:35  seiki_masashi
  ����M�o�b�t�@��K�v�Œ���̃T�C�Y�ɏk��

  Revision 1.18  2004/09/10 02:48:25  seiki_masashi
  small fix.

  Revision 1.17  2004/09/10 01:19:50  seiki_masashi
  �ő�ڑ��q�@���� WC_MAX_NUM_CHILD �Őݒ肷��悤�ɕύX

  Revision 1.16  2004/09/09 12:41:32  seiki_masashi
  �q�@�ő呗�M�f�[�^�T�C�Y��K���T�C�Y�ɏC��

  Revision 1.15  2004/09/03 04:42:13  seiki_masashi
  ���M���s�� errcode �� WM_ERRCODE_SEND_FAILED �ɕύX

  Revision 1.14  2004/09/01 05:51:05  seiki_masashi
  MP.ind �ł� WM_ERRCODE_INVALID_POLLBITMAP �ւ̑Ή�

  Revision 1.13  2004/09/01 05:17:11  seiki_masashi
  WM_ERRCODE_INVALID_POLLBITMAP �ւ̑Ή�

  Revision 1.12  2004/08/30 01:57:30  seiki_masashi
  WM_STATECODE_CHILD_CONNECTED �� WM_STATECODE_CONNECTED �ɕύX

  Revision 1.11  2004/08/27 08:03:16  terui
  �r�[�R�����X�g���̏�����ύX�A�ڑ��������̏�����ύX�B

  Revision 1.10  2004/08/23 04:41:01  ooe
  WM_StartConnect()�̎d�l�ύX�ɑΉ��B

  Revision 1.9  2004/08/20 04:31:55  terui
  �ő�ڑ��q�@�䐔�𒴂����ꍇ�̃f�o�b�O�o�͂�ǉ��B

  Revision 1.8  2004/08/19 15:34:23  miya
  modified WMGameInfo structure.

  Revision 1.7  2004/08/19 02:55:17  terui
  �G���g���[�ɂ��ڑ�����d�l�̓����ɔ��������B

  Revision 1.6  2004/08/18 00:28:35  seiki_masashi
  small fix

  Revision 1.5  2004/08/17 12:26:37  seiki_masashi
  WM_StartMP �̈����ɑ��M�o�b�t�@���w�肷��悤�ɕύX
  WM_ERRCODE_FRAME_GAP �̔p�~�ɔ����C��

  Revision 1.4  2004/08/12 14:57:11  seiki_masashi
  WM_StepDataSharing �̕Ԃ�l�� WM_ERRCODE_FRAME_GAP �ɑΉ�

  Revision 1.3  2004/08/11 08:01:29  terui
  WcInit�Ɉ�����ǉ����AWM_StartDataSharing���ɗ��p����悤�����B

  Revision 1.2  2004/08/10 06:38:24  terui
  WcGetAid�ǉ��B�R�����g�C���A�s�v�֐��폜�Ȃǃ\�[�X�𐮗��B

  Revision 1.1  2004/08/10 00:41:54  terui
  Initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
