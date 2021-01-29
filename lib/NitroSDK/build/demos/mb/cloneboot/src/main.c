/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - cloneboot
  File:     main.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.4  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.3  2004/11/24 11:07:05  yosizaki
  change ParentIdentifier to ParentMain.

  Revision 1.2  2004/11/24 10:17:32  yasu
  Untabify

  Revision 1.1  2004/11/15 06:21:18  yosizaki
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include "common.h"

/*
 * �N���[���u�[�g����������A�v���P�[�V�����̃T���v��.
 *
 * MB���C�u�����̃T���v���̓}���`�u�[�g�@�\���g�����߁A
 * �����ʐM��(�������邢�͗L��)�ǂ����̊J���@�ނ𕡐��K�v�Ƃ��܂��B
 * $NitroSDK/bin/ARM9-TS/Release/ �f�B���N�g���ɂ���
 * mb_child.bin �v���O�������ŏI���@�ɂ�����}���`�u�[�g�q�@�Ɠ�����
 * �@�\��񋟂���T���v���ɂȂ��Ă��܂��̂ŁA
 * ���̃o�C�i�����T���v���v���O�����Ɠ������@�ő��̋@�ނɓǂݍ��݁A
 * �ꏏ�Ɏ��s���Ă��������B
 *
 */

/******************************************************************************/



//============================================================================
//   �֐���`
//============================================================================

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C�����[�`��

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    /*
     * ���̃T���v���ł�, multiboot-Model �T���v���f�������̂܂܎g�p��
     * ���g��DS�_�E�����[�h�v���C�q�@�v���O�������ǂ����𔻒f����
     * �P���ɏ����𕪂��Ă��܂�.
     *
     * �N���[���u�[�g�Őe�@�Ǝq�@�Ƃ̊Ԃɂ����Ȋ�����_�Ƃ��Ă�
     *  1. �q�@�� CARD �A�N�Z�X���ł��Ȃ�
     *  2. 8kB �ȓ��̐e�@��p�R�[�h���܂܂Ȃ���΂Ȃ�Ȃ�
     *  3. ���C�����X�ʐM�̎菇���e�@/�q�@�ňقȂ�
     * ���������܂�.
     *
     * �����̓_�ɒ��ӂ��ăA�v���P�[�V�����݌v�ɍ������b�p�[�����őΉ������
     * �قƂ�ǂ̓��e��e�q�ŋ��ʉ�����l�p�Ƒΐ�v���C���T�|�[�g��
     * CARD-ROM �e�ʂ��ߖ񂳂ꂽ�����I�ȃv���O�������쐬���邱�Ƃ��o���܂�.
     * �t��, DS�_�E�����[�h�v���C�̐e�@�Ǝq�@�̊ԂɑS�����ʓ_���������
     * ��ɋ������悤�ȗ��_�͓����Ȃ��ł��傤.
     */
    if (!MB_IsMultiBootChild())
    {
        ParentMain();
    }
    else
    {
        ChildMain();
    }

    /* ����͂����֋y�т܂��� */
}
