/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - gfd - src - VramManager
  File:     gfd_TexVramMan.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

#include <nnsys/gfd/VramManager/gfd_VramMan.h>
#include <nnsys/gfd/VramManager/gfd_TexVramMan_Types.h>

static NNSGfdTexKey	AllocTexVram_( u32 szByte, BOOL is4x4comp, u32 opt );
static int			FreeTexVram_( NNSGfdTexKey memKey );


NNSGfdFuncAllocTexVram  NNS_GfdDefaultFuncAllocTexVram = AllocTexVram_;
NNSGfdFuncFreeTexVram   NNS_GfdDefaultFuncFreeTexVram  = FreeTexVram_;

/*---------------------------------------------------------------------------*
  Name:         AllocTexVram_

  Description:  �������m�ۂ̃_�~�[�֐��B�f�t�H���g�̃������m�ۊ֐����o�^����
  				�Ă��Ȃ����A���̊֐����Ă΂�܂��B
                 
  Arguments:   szByte           :  �T�C�Y
               is4x4comp        :  4x4���k�e�N�X�`�����H
               opt              :  �����ˑ��p�����[�^
                            
  Returns:     �e�N�X�`���L�[
 *---------------------------------------------------------------------------*/
static NNSGfdTexKey
AllocTexVram_( u32 /* szByte */, BOOL /* is4x4comp */, u32 /* opt */ )
{
    NNS_GFD_WARNING("no default AllocTexVram function.");

    // �G���[�F�G���[��\������TexKey��������
    return NNS_GFD_ALLOC_ERROR_TEXKEY;
}

/*---------------------------------------------------------------------------*
  Name:         FreeFrmTexVram_

  Description:  �������J���̃_�~�[�֐��B�f�t�H���g�̃���������֐����o�^����
  				�Ă��Ȃ����A���̊֐����Ă΂�܂��B
                 
  Arguments:   memKey          :   �e�N�X�`���L�[
                            
  Returns:     ���ہi0 �Ȃ�� ����)
 *---------------------------------------------------------------------------*/
static int
FreeTexVram_( NNSGfdTexKey /* memKey */ )
{
    NNS_GFD_WARNING("no default FreeTexVram function.");

    return -1;
}
