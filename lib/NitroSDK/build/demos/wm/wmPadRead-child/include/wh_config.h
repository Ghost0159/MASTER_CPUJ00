/*
  Project:  NitroSDK - demo
  File:     wh_config.h

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wh_config.h,v $
  Revision 1.1  2007/10/27 14:55:13  seiki_masashi
  inital upload

  $NoKeywords$
 */

#ifndef WH_CONFIG_H__
#define WH_CONFIG_H__

// �����Ŏg�p����DMA�ԍ�
#define WH_DMA_NO                 2

// �q�@�ő吔�i�e�@���܂܂Ȃ����j
#define WH_CHILD_MAX              15

// �V�F�A�o����f�[�^�̍ő�T�C�Y
#define WH_DS_DATA_SIZE           8

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
#define WH_DS_PORT                12



#endif // WH_CONFIG_H__
