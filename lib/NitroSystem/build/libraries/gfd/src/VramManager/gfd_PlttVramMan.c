/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - gfd - src - VramManager
  File:     gfd_PlttVramMan.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

#include <nnsys/gfd/VramManager/gfd_VramMan.h>
#include <nnsys/gfd/VramManager/gfd_PlttVramMan_Types.h>

static NNSGfdPlttKey	AllocPlttVram_( u32 szByte, BOOL b4Pltt, u32 bAllocFromLo );
static int				FreePlttVram_( NNSGfdPlttKey key );


NNSGfdFuncAllocPlttVram NNS_GfdDefaultFuncAllocPlttVram = AllocPlttVram_;
NNSGfdFuncFreePlttVram  NNS_GfdDefaultFuncFreePlttVram  = FreePlttVram_;

/*---------------------------------------------------------------------------*
  Name:         AllocPlttVram_

  Description:  �������m�ۂ̃_�~�[�֐��B�f�t�H���g�̃������m�ۊ֐����o�^����
  				�Ă��Ȃ����A���̊֐����Ă΂�܂��B
                 
  Arguments:   szByte           :  �T�C�Y
               bPltt4           :  4�F�e�N�X�`�����H
               bAllocFromHead   :  �̈�擪�i���ʁj����m�ۂ��邩�H
                            
  Returns:     �e�N�X�`���p���b�g�L�[
 *---------------------------------------------------------------------------*/
static NNSGfdPlttKey
AllocPlttVram_( u32 /* szByte */, BOOL /* b4Pltt */, u32 /* bAllocFromLo */ )
{
    NNS_GFD_WARNING("no default AllocPlttVram function.");

    // �G���[�F�G���[��\������PlttKey��������
    return NNS_GFD_ALLOC_ERROR_PLTTKEY;
}

/*---------------------------------------------------------------------------*
  Name:         FreePlttTexVram_

  Description:  �������J���̃_�~�[�֐��B�f�t�H���g�̃���������֐����o�^����
  				�Ă��Ȃ����A���̊֐����Ă΂�܂��B
                 
  Arguments:   plttKey          :   �e�N�X�`���p���b�g�L�[
                            
  Returns:     ���ہi0 �Ȃ�� ����)
 *---------------------------------------------------------------------------*/
static int
FreePlttVram_( NNSGfdPlttKey /* plttKey */ )
{
    NNS_GFD_WARNING("no default FreePlttVram function.");

    return -1;
}
