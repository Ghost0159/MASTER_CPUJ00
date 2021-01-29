/*---------------------------------------------------------------------------*
  Project:  NitroDWC -  - 
  File:     dwc_nd.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_nd.h,v $
  Revision 1.8  2006/09/05 05:37:09  takayama
  DWC_NdCleanupAsync�֐��̕Ԃ�l�� void ���� BOOL �ɕύX�����B

  Revision 1.7  2006/05/15 01:18:19  nakata
  5/12�ł�DWC_Nd���C�u�����ɑΉ�

  Revision 1.6  2006/05/12 11:25:10  nakata
  ��؂���̎w�E�ɑΉ�

  Revision 1.5  2006/05/10 11:59:21  nakata
  Nd���C�u�����̎d�l�ύX�ɔ����ď������֐���ύX

  Revision 1.4  2006/05/02 00:27:15  nakata
  DWC_Nd��5��2���łɍX�V

  Revision 1.3  2006/04/28 12:26:58  nakata
  DWC_ND_PARAMERR�̃R���p�C����warning�}��

  Revision 1.2  2006/04/28 10:09:02  nakata
  DWC_NdInitAsync��password�ǉ�
  DWC_NdGetFileAsync�̎d�l���ύX�ɔ����֐��ύX
  �e��萔���K�C�h���C���ɉ��������̂ɕύX����w�b�_��ǉ�

  Revision 1.1  2006/04/25 11:46:56  nakata
  DWC_Nd���b�p��ǉ�



  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_ND_H_
#define DWC_ND_H_

#include <nd/DWCi_Nd.h>

#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------------------------------------------------------
        define
   ------------------------------------------------------------------------- */

//


/* -------------------------------------------------------------------------
        enum
   ------------------------------------------------------------------------- */

//


/* -------------------------------------------------------------------------
        typedef - function
   ------------------------------------------------------------------------- */

//


/* -------------------------------------------------------------------------
        struct
   ------------------------------------------------------------------------- */

//


/* -------------------------------------------------------------------------
        function - external
   ------------------------------------------------------------------------- */

/*---------------------------------------------------------------------------*
  Name:			DWC_NdInitAsync

  Description:	Nd���C�u�����������֐�

  Arguments:	callback - Nd���C�u�������Ăяo���ʒm�R�[���o�b�N�֐��ւ̃|�C���^
                gamecd   - �_�E�����[�h�T�[�o�ւ̃A�N�Z�X�ɕK�v�ȃQ�[���R�[�h
                passwd   - �_�E�����[�h�T�[�o�ւ̃A�N�Z�X�ɕK�v�ȃp�X���[�h
  Returns:		TRUE  - ����������
				FALSE - ���������s
 *---------------------------------------------------------------------------*/
BOOL DWC_NdInitAsync(DWCNdCallback callback, char *gamecd, char *passwd);

/*---------------------------------------------------------------------------*
  Name:			DWC_NdCleanupAsync

  Description:	Nd���C�u��������֐�

  Arguments:	callback - Nd���C�u�������Ăяo����������ʒm�R�[���o�b�N�֐��ւ̃|�C���^
  Returns:		�Ȃ�
 *---------------------------------------------------------------------------*/
BOOL DWC_NdCleanupAsync(DWCNdCleanupCallback callback);

/*---------------------------------------------------------------------------*
  Name:			DWC_NdSetAttr

  Description:	�_�E�����[�h�������ݒ�֐�

  Arguments:	attr1 - ����������1
				attr2 - ����������2
				attr3 - ����������3
  Returns:		TRUE  - �����ݒ萬��
				FALSE - �����ݒ莸�s
 *---------------------------------------------------------------------------*/
BOOL DWC_NdSetAttr(const char* attr1, const char* attr2, const char* attr3);

/*---------------------------------------------------------------------------*
  Name:			DWC_NdGetFileListNumAsync

  Description:	�t�@�C�����X�g���G���g�����擾�����J�n�֐�

  Arguments:	entrynum - �ʐM�������Ƀt�@�C�����X�g���G���g�������i�[�����̈�ւ̃|�C���^
  Returns:		TRUE  - �ʐM���J�n���ꏈ�������҂����
				FALSE - �������s
 *---------------------------------------------------------------------------*/
BOOL DWC_NdGetFileListNumAsync(int* entrynum);

/*---------------------------------------------------------------------------*
  Name:			DWC_NdFileListAsync

  Description:	�t�@�C�����X�g�擾�����J�n�֐�

  Arguments:	filelist - �擾���ꂽ�t�@�C�����X�g���i�[�����z��ւ̃|�C���^
				offset   - �擾����t�@�C�����X�g�̊J�n�ʒu�̃I�t�Z�b�g
				num      - �擾����t�@�C�����X�g�̍ő咷
  Returns:		TRUE  - �ʐM���J�n���ꏈ�������҂����
				FALSE - �������s
 *---------------------------------------------------------------------------*/
BOOL DWC_NdGetFileListAsync(DWCNdFileInfo* filelist, unsigned int offset, unsigned int num);

/*---------------------------------------------------------------------------*
  Name:			DWC_NdGetFileAsync

  Description:	�t�@�C���擾�����J�n�֐�

  Arguments:	filename - �擾����t�@�C���̖��O
				buf      - �擾���ꂽ�t�@�C���̃f�[�^���i�[�����̈�ւ̃|�C���^
				size     - �擾���ꂽ�t�@�C���̃f�[�^���i�[�����̈�̃T�C�Y
  Returns:		TRUE  - �ʐM���J�n���ꏈ�������҂����
				FALSE - �������s
 *---------------------------------------------------------------------------*/
BOOL DWC_NdGetFileAsync(DWCNdFileInfo *fileinfo, char* buf, unsigned int size);

/*---------------------------------------------------------------------------*
  Name:			DWC_NdCancelAsync

  Description:	���s�������̒��f�ʒm�֐�

  Arguments:	�Ȃ�
  Returns:		TRUE  - �������f�ɐ������A���������҂����
				FALSE - �������s
 *---------------------------------------------------------------------------*/
BOOL DWC_NdCancelAsync(void);

/*---------------------------------------------------------------------------*
  Name:			DWC_NdGetProgress

  Description:	���s�������̐i�s�x�擾�֐�

  Arguments:	received   - ��M�ς݃f�[�^�̃o�C�g�����i�[����̈�ւ̃|�C���^
				contentlen - ��M���̃f�[�^��Content-Length���i�[����̈�ւ̃|�C���^
  Returns:		TRUE  - �i�s�x�擾����
				FALSE - �i�s�x�擾���s�A���邢�͏��������s���Ă��Ȃ�
 *---------------------------------------------------------------------------*/
BOOL DWC_NdGetProgress(u32* received, u32* contentlen);

#ifdef __cplusplus
}
#endif

#endif // DWC_ND_H_
