/*
  Project:  NitroSDK - wireless_shared - demos - wh - default
  File:     wh_config.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wh_config.h,v $
  Revision 1.6  2007/10/03 01:05:11  okubata_ryoma
  Copyright fix

  Revision 1.5  2007/10/02 09:05:32  yosizaki
  small fix about WM_SIZE_DS_PARENT_HEADER

  Revision 1.4  2006/04/11 00:01:31  okubata_ryoma
  Copyright fix

  Revision 1.3  2006/04/10 13:18:57  yosizaki
  support WH_MP_FREQUENCY.

  Revision 1.2  2005/12/21 02:36:44  adachi_hiroaki
  define���鎯�ʎq��ύX

  Revision 1.1  2005/12/21 02:20:17  adachi_hiroaki
  wh_config.h�̏ꏊ��ύX

  Revision 1.2  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.1  2004/12/20 07:17:48  takano_makoto
  �A�v���P�[�V�������̖����ʐM�p�����[�^��wh_config.h�Ƃ��ĕ���

  $NoKeywords$
 */

#ifndef _DEFAULT_WH_CONFIG_H__
#define _DEFAULT_WH_CONFIG_H__


// �����Ŏg�p����DMA�ԍ�
#define WH_DMA_NO                 2

// �q�@�ő吔�i�e�@���܂܂Ȃ����j
#define WH_CHILD_MAX              15

// �V�F�A�o����f�[�^�̍ő�T�C�Y
#define WH_DS_DATA_SIZE           12

// 1��̒ʐM�ő����f�[�^�̍ő�T�C�Y
// �f�[�^�V�F�A�����O�ɉ����Ēʏ�̒ʐM������ꍇ�́A���̕�����
// �����̒l�𑝂₵�Ă��������B���̍ۂ́A�����p�P�b�g���M�ɂ��ǉ���
// �w�b�_�t�b�^�������Z����K�v������܂��B
// �ڂ����� docs/TechnicalNotes/WirelessManager.doc ���Q�Ƃ��Ă��������B
// GUIDELINE : �K�C�h���C�������|�C���g(6.3.2)
// ���t�@�����X�̃��C�����X�}�l�[�W��(WM)���}�\�E��񁨖����ʐM���Ԍv�Z�V�[�g
// �Ōv�Z���� MP �ʐM1�񕪂̏��v���Ԃ� 5600 �ʕb�ȉ��ƂȂ邱�Ƃ𐄏����Ă��܂��B
#define WH_PARENT_MAX_SIZE      (WH_DS_DATA_SIZE * (1 + WH_CHILD_MAX) + WM_SIZE_DS_PARENT_HEADER)
#define WH_CHILD_MAX_SIZE       (WH_DS_DATA_SIZE)

// 1�s�N�`���[�t���[���������MP�ʐM�񐔏��
// �f�[�^�V�F�A�����O�ƃu���b�N�]���ȂǕ����̃v���g�R������s����ꍇ��
// ���̒l��1���傫��(�܂��͖�����������0��)�ݒ肷��K�v������܂��B
// �����łȂ��ꍇ�A�����Ƃ��D��x�̍���1�̃v���g�R���ȊO��
// MP�ʐM����؎��s�ł��Ȃ��Ȃ��Ă��܂��܂��B
#define WH_MP_FREQUENCY           1

// �ʏ�� MP �ʐM�Ŏg�p����|�[�g
#define WH_DATA_PORT              14

// �ʏ�� MP �ʐM�Ŏg�p����D��x
#define WH_DATA_PRIO              WM_PRIORITY_NORMAL

// �f�[�^�V�F�A�����O�Ŏg�p����|�[�g
#define WH_DS_PORT                13



#endif // _DEFAULT_WH_CONFIG_H__
