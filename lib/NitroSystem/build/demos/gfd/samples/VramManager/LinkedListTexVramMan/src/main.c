/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - gfd - samples - VramManager - LinkedListTexVramMan
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.9 $
 *---------------------------------------------------------------------------*/
//
// �����N�h���X�gVRAM�}�l�[�W���̓������W���[���̓�����e�X�g���܂��B
// �������W���[���̓e�N�X�`���}�l�[�W���A�p���b�g�}�l�[�W���ɋ��ʂ̃R�[�h�ł��B
// 
//
#include <nitro.h>

#include <nnsys/gfd.h>
#include "gfd_demolib.h"

//
// �}�l�[�W���������p�����[�^
//
#define SIZE_VRAMMAN        0x80000     // �Ǘ��e�N�X�`���̈�T�C�Y
#define SIZE_VRAMMAN_4X4    0x21000     // �Ǘ��e�N�X�`���̈�T�C�Y(4X4�e�N�X�`���p)
#define NUM_VRAMMAN_MEMBLK  20          // �Ǘ��u���b�N��(�ו������ꂽ�󂫗̈搔�̍ő�l�ƂȂ�܂��B)
#define SIZE_PLTTMAN        0x18000     // �Ǘ��p���b�g�̈�T�C�Y

//
// �m�ۃe�X�g�Ɏg�p����̈�̃T�C�Y
//
#define ALLOCATE_SIZE               0x4000
#define PLTT_ALLOC_SIZE_4           0x0008
#define PLTT_ALLOC_SIZE_16          0x0020
#define PLTT_ALLOC_SIZE_X           0x0023
#define PLTT_ALLOC_SIZE_4PLTTMAX    0x10000


#define VRAM_SLOT_SIZE    0x20000


#define TEX4X4_ENABLE           TRUE
#define TEX4X4_DISABLE          FALSE
#define PLTT4_TRUE              TRUE
#define PLTT4_FALSE             FALSE

/*---------------------------------------------------------------------------*
  Name:         TexAllocateTest_

  Description:  �e�N�X�`���}�l�[�W���̏������e�X�g
                ���܂��܂ȃP�[�X�ŏ������������Ȃ��Ă݂�B
                ���������������s���Ȃ������ꍇ�́A�}�l�[�W�������̃A�T�[�g�Ɏ��s����͂��B
                
                
  Arguments:    �Ȃ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
static void TexManInitTest_()
{
    const u32 szWork = NNS_GfdGetLnkTexVramManagerWorkSize( NUM_VRAMMAN_MEMBLK );
    void* pMgrWork   = GfDDemo_Allock( szWork );
    const u32 myMgrSize = 0x8000;
    u32 szNrm;
    
    NNS_GFD_NULL_ASSERT( pMgrWork );
    
    //
    // �}�l�[�W���̏�����
    //
    // �ʏ�p��1�X���b�g�ȉ�
    {
        szNrm = myMgrSize;
        // 4x4 �p�� 0�X���b�g
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      0,
                                      pMgrWork, szWork, TRUE );
    }
    
    // �ʏ�p��1 - �Q�X���b�g
    {
        szNrm = myMgrSize + VRAM_SLOT_SIZE;
        // 4x4 �p�� 0�X���b�g
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      0,
                                      pMgrWork, szWork, TRUE );
        
        // 4x4 �p�� 1�X���b�g �ȉ�
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      myMgrSize,
                                      pMgrWork, szWork, TRUE );
    }
    
    // �ʏ�p��2 - 3 �X���b�g
    {
        szNrm  = myMgrSize + VRAM_SLOT_SIZE * 2;
        // 4x4 �p�� 0�X���b�g
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      0,
                                      pMgrWork, szWork, TRUE );
        // 4x4 �p�� 1�X���b�g �ȉ�
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      myMgrSize,
                                      pMgrWork, szWork, TRUE );
        // 4x4 �p�� 2 �X���b�g �ȉ�
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      myMgrSize + VRAM_SLOT_SIZE,
                                      pMgrWork, szWork, TRUE );
    }        
    
    // �ʏ�p��3 - 4 �X���b�g
    {
        szNrm  = myMgrSize + VRAM_SLOT_SIZE * 3;
        // 4x4 �p�� 0�X���b�g
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      0,
                                      pMgrWork, szWork, TRUE );
        // 4x4 �p�� 1�X���b�g �ȉ�
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      myMgrSize,
                                      pMgrWork, szWork, TRUE );
        // 4x4 �p�� 2 �X���b�g �ȉ�
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      myMgrSize + VRAM_SLOT_SIZE,
                                      pMgrWork, szWork, TRUE );
    }
    
    // ���̑����`�F�b�N�ŋ��E�l�p�����[�^�𒲍�
    {
        
        NNS_GfdInitLnkTexVramManager( VRAM_SLOT_SIZE * 4,
                                      0,
                                      pMgrWork, szWork, TRUE );
        
        NNS_GfdInitLnkTexVramManager( VRAM_SLOT_SIZE * 4,
                                      VRAM_SLOT_SIZE * 2,
                                      pMgrWork, szWork, TRUE );
        
        NNS_GfdInitLnkTexVramManager( VRAM_SLOT_SIZE * 3,
                                      VRAM_SLOT_SIZE * 2,
                                      pMgrWork, szWork, TRUE );
    }
    
    GfDDemo_Free( pMgrWork );
}

/*---------------------------------------------------------------------------*
  Name:         TexAllocateTest_

  Description:  �e�N�X�`���}�l�[�W���̊m�ۃe�X�g
                
  Arguments:    �Ȃ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
static void TexAllocateTest_()
{
    const u32 szWork = NNS_GfdGetLnkTexVramManagerWorkSize( NUM_VRAMMAN_MEMBLK );
    void* pMgrWork   = GfDDemo_Allock( szWork );
    
    NNS_GFD_NULL_ASSERT( pMgrWork );
    
    //
    // �}�l�[�W���̏�����
    //
    NNS_GfdInitLnkTexVramManager( SIZE_VRAMMAN,
                                  SIZE_VRAMMAN_4X4,
                                  pMgrWork,
                                  szWork,
                                  TRUE );
                                  
    
    
    {
        NNSGfdTexKey texKey1, texKey2, texKey3;
    
        // �X���b�g�T�C�Y���傫���̈�̊m�ۂ���������B
        texKey1 = NNS_GfdAllocTexVram( VRAM_SLOT_SIZE + 0x100, TEX4X4_DISABLE, 0 );
        NNS_GFD_ASSERT( texKey1 != NNS_GFD_ALLOC_ERROR_TEXKEY );
        NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey1 ) == 0 );
        
        
        // �A���P�[�g����
        texKey1 = NNS_GfdAllocTexVram( ALLOCATE_SIZE, TEX4X4_ENABLE, 0 );
        texKey2 = NNS_GfdAllocTexVram( ALLOCATE_SIZE, TEX4X4_ENABLE, 0 );
        texKey3 = NNS_GfdAllocTexVram( ALLOCATE_SIZE, TEX4X4_ENABLE, 0 );
        
        
        //OS_Printf("addr = %x, size = %x\n", NNS_GfdGetTexKeyAddr(texKey1), NNS_GfdGetTexKeySize(texKey1) );
        //OS_Printf("addr = %x, size = %x\n", NNS_GfdGetTexKeyAddr(texKey2), NNS_GfdGetTexKeySize(texKey2) );
        //OS_Printf("addr = %x, size = %x\n", NNS_GfdGetTexKeyAddr(texKey3), NNS_GfdGetTexKeySize(texKey3) );
        
        
        NNS_GFD_ASSERT( NNS_GfdGetTexKeyAddr(texKey1) == ALLOCATE_SIZE * 0 &&  
                     NNS_GfdGetTexKeySize(texKey1) == ALLOCATE_SIZE );
        
        NNS_GFD_ASSERT( NNS_GfdGetTexKeyAddr(texKey2) == ALLOCATE_SIZE * 1 &&  
                     NNS_GfdGetTexKeySize(texKey2) == ALLOCATE_SIZE );
                     
        NNS_GFD_ASSERT( NNS_GfdGetTexKeyAddr(texKey3) == ALLOCATE_SIZE * 2 &&  
                     NNS_GfdGetTexKeySize(texKey3) == ALLOCATE_SIZE );
        
        // Free ����������
        NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey2 ) == 0 );
        
        // �ēx�����T�C�Y�ŃA���P�[�g
        texKey2 = NNS_GfdAllocTexVram( ALLOCATE_SIZE, TEX4X4_ENABLE, 0 );
        // ��قǃt���[�����̈�Ɋm�ۂ����
        NNS_GFD_ASSERT( NNS_GfdGetTexKeyAddr(texKey2) == ALLOCATE_SIZE * 1 &&  
                     NNS_GfdGetTexKeySize(texKey2) == ALLOCATE_SIZE );
    
        // Free ����������
        NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey1 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey2 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey3 ) == 0 );
        
        
        texKey1 = NNS_GfdAllocTexVram( ALLOCATE_SIZE * 2 , TEX4X4_ENABLE, 0 );
        NNS_GFD_ASSERT( NNS_GfdGetTexKeyAddr(texKey1) == ALLOCATE_SIZE * 0 &&  
                     NNS_GfdGetTexKeySize(texKey1) == ALLOCATE_SIZE * 2  );
        NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey1 ) == 0 );
        
        
        // ���ׂĂ�VRAM���g���؂��āA�m�ۂɎ��s����
        // NNS_GFD_MAX_NUM_TEX_VRAM_SLOT       4
        // NNS_GFD_MAX_NUM_TEX4x4_VRAM_SLOT    2
        {
            // 4X4
            texKey1 = NNS_GfdAllocTexVram( 0x20000, TEX4X4_ENABLE, 0 );
            NNS_GFD_ASSERT( texKey1 != NNS_GFD_ALLOC_ERROR_TEXKEY );
            texKey2 = NNS_GfdAllocTexVram( 0x01000, TEX4X4_ENABLE, 0 );
            NNS_GFD_ASSERT( texKey2 != NNS_GFD_ALLOC_ERROR_TEXKEY );
            
            //
            // �������m�ۓ���̎��s���m�F�������ꍇ�̓R�����g���������s���Ă݂Ă�������
            //
            // ���ׂĎg���������B���s����͂�
            //texKey3 = NNS_GfdAllocTexVram( 0x1, TEX4X4_ENABLE, 0 );
            //NNS_GFD_ASSERT( texKey3 == NNS_GFD_ALLOC_ERROR_TEXKEY );
            
            // Normal
            texKey3 = NNS_GfdAllocTexVram( 0x20000, TEX4X4_DISABLE, 0 );
            NNS_GFD_ASSERT( texKey3 != NNS_GFD_ALLOC_ERROR_TEXKEY );
            
            
            // ������Ԃ��f�o�b�N�o�͂��Ă݂�
            NNS_GfdDumpLnkTexVramManager();
        
            
            // �J������
            NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey1 ) == 0 );
            NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey2 ) == 0 );
            NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey3 ) == 0 );
            
        
        }
        
        // �����ȃT�C�Y�̐؂�グ�������m�F���܂�
        {
            NNSGfdTexKey    key;
            key = NNS_GfdAllocTexVram( 0, TEX4X4_ENABLE, 0 );
            NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(key) == NNS_GFD_TEXSIZE_MIN  );
            NNS_GFD_ASSERT( NNS_GfdFreeTexVram( key ) == 0 );
            
            key = NNS_GfdAllocTexVram( 1, TEX4X4_ENABLE, 0 );
            NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(key) == NNS_GFD_TEXSIZE_MIN  );
            NNS_GFD_ASSERT( NNS_GfdFreeTexVram( key ) == 0 );
            
            key = NNS_GfdAllocTexVram( NNS_GFD_TEXSIZE_MIN, TEX4X4_ENABLE, 0 );
            NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(key) == NNS_GFD_TEXSIZE_MIN  );
            NNS_GFD_ASSERT( NNS_GfdFreeTexVram( key ) == 0 );
            
            key = NNS_GfdAllocTexVram( NNS_GFD_TEXSIZE_MIN + 1, TEX4X4_ENABLE, 0 );
            NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(key) == NNS_GFD_TEXSIZE_MIN * 2 );
            NNS_GFD_ASSERT( NNS_GfdFreeTexVram( key ) == 0 );
        }
    }
    GfDDemo_Free( pMgrWork );
    
}

/*---------------------------------------------------------------------------*
  Name:         IsValidKey_

  Description:  �p���b�g�L�[���Ӑ}����������L�[�����肵�܂��B
                
  Arguments:    addr            �Ӑ}����A�h���X
                size            �Ӑ}����T�C�Y
                
  Returns:      �p���b�g�L�[���Ӑ}����������L�[�Ȃ�TRUE
  
 *---------------------------------------------------------------------------*/
static BOOL IsValidKey_( NNSGfdPlttKey key, u32 addr, u32 size )
{
    const u32 kaddr = NNS_GfdGetPlttKeyAddr( key );
    const u32 ksize = NNS_GfdGetPlttKeySize( key );
    
    return (BOOL)( kaddr == addr  &&  ksize == size );
}

/*---------------------------------------------------------------------------*
  Name:         PlttAllocateTest_

  Description:  �p���b�g�}�l�[�W���̊m�ۃe�X�g
                
  Arguments:    �Ȃ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
static void PlttAllocateTest_()
{
    NNSGfdPlttKey     key1, key2, key3;
    
    const u32 szWork 
        = NNS_GfdGetLnkPlttVramManagerWorkSize( NUM_VRAMMAN_MEMBLK );
    void* pMgrWork   = GfDDemo_Allock( szWork );
    
    //
    // �}�l�[�W���̏�����
    //
    NNS_GfdInitLnkPlttVramManager( SIZE_PLTTMAN, 
                                   pMgrWork,
                                   szWork,
                                   TRUE );
    // �m�ہA�J��
    {
        key1 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_16, PLTT4_FALSE, TRUE );
        NNS_GFD_ASSERT( IsValidKey_( key1, 0, PLTT_ALLOC_SIZE_16 ) );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key1 ) == 0 );
    }
    
    
    // �m�� �������J�� �ēx�m�� ( ������ => ������ => ������ )
    {
        key1 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_16, PLTT4_FALSE, TRUE );
        key2 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_16, PLTT4_FALSE, TRUE );
        key3 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_16, PLTT4_FALSE, TRUE );
        
        NNS_GFD_ASSERT( IsValidKey_( key1,                  0    , PLTT_ALLOC_SIZE_16 ) );
        NNS_GFD_ASSERT( IsValidKey_( key2, PLTT_ALLOC_SIZE_16 * 1, PLTT_ALLOC_SIZE_16 ) );
        NNS_GFD_ASSERT( IsValidKey_( key3, PLTT_ALLOC_SIZE_16 * 2, PLTT_ALLOC_SIZE_16 ) );
        
        
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key2 ) == 0 );
        key2 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_16, PLTT4_FALSE, TRUE );
        NNS_GFD_ASSERT( IsValidKey_( key2, PLTT_ALLOC_SIZE_16 * 1, PLTT_ALLOC_SIZE_16 ) );
        
        
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key1 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key2 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key3 ) == 0 );
    }
    
    // 4�F�p���b�g�� 0x10000 �ȏ�̗̈���m�ۂ��悤�Ƃ��Ď��s���Ă݂�
    // �ő���Ɋm�ۂ��Ċm�ۂɎ��s���Ă݂�
    {
        // 4�F�p���b�g���Q�Ɖ\�ȍő���̑傫���Ŋm�ۂ���
        key1 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_4PLTTMAX, PLTT4_TRUE, TRUE );
        NNS_GFD_ASSERT( IsValidKey_( key1, 0, PLTT_ALLOC_SIZE_4PLTTMAX ) );
        
        //
        // �������m�ۓ���̎��s���m�F�������ꍇ�̓R�����g���������s���Ă݂Ă�������
        //
        // 4�F�p���b�g�͂���ȏ�A���Ȃ��͂�
        // key2 = NNS_GfdAllocPlttVram( 0x7000, PLTT4_TRUE, TRUE );
        // NNS_GFD_ASSERT( key2 == NNS_GFD_ALLOC_ERROR_PLTTKEY );
        
        // �������A�ʏ�̃p���b�g�͂܂�����͂�
        key2 = NNS_GfdAllocPlttVram( 0x7000, PLTT4_FALSE, TRUE );
        NNS_GFD_ASSERT( IsValidKey_( key2, PLTT_ALLOC_SIZE_4PLTTMAX, 0x7000 ) );
        NNS_GFD_ASSERT( key2 != NNS_GFD_ALLOC_ERROR_PLTTKEY );
        
        //
        // �������m�ۓ���̎��s���m�F�������ꍇ�̓R�����g���������s���Ă݂Ă�������
        //
        // ���x�́A���ʂɗe�ʕs���Ŏ��s����
        // key3 = NNS_GfdAllocPlttVram( 0x7000, PLTT4_FALSE, TRUE );
        // NNS_GFD_ASSERT( key3 == NNS_GFD_ALLOC_ERROR_PLTTKEY );
        
        // ������Ԃ��f�o�b�N�o�͂��Ă݂�
        NNS_GfdDumpLnkPlttVramManager();
            
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key1 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key2 ) == 0 );
    }
    
    
    
    // 4�F�p���b�g�A���C�������g���������s���Ă��邩�H
    //
    {
        // 4�F�p���b�g1�����m��
        // => 16�F�p���b�g���m�� (�A���C�������g�̂���8�o�C�g�t���[�u���b�N������)
        // => 4�F�p���b�g���m�� (8�o�C�g�t���[�u���b�N���g�p�����͂�)
        key1 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_4, PLTT4_TRUE, TRUE );
        key2 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_16, PLTT4_FALSE, TRUE );
        key3 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_4, PLTT4_TRUE, TRUE );
        
        NNS_GFD_ASSERT( IsValidKey_( key1, 0, PLTT_ALLOC_SIZE_4 ) );
        // �p�f�B���O����� addr == PLTT_ALLOC_SIZE_4 �Ƃ͂Ȃ�Ȃ��͂�
        NNS_GFD_ASSERT( IsValidKey_( key2, 0x10, PLTT_ALLOC_SIZE_16 ) );
        // �p�f�B���O�̍ۂɐ������ꂽ�t���[�u���b�N���g�p�����͂�
        NNS_GFD_ASSERT( IsValidKey_( key3, PLTT_ALLOC_SIZE_4, PLTT_ALLOC_SIZE_4 ) );
        
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key1 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key2 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key3 ) == 0 );
        
        
        // 4�F�p���b�g2�����m��
        // => 16�F�p���b�g���m�� (�A���C�������g�̂��߂̃p�f�B���O������ł���I)
        // => 4�F�p���b�g���m�� (���������Ċm�ۂ����A�h���X�͈قȂ�I)
        key1 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_4 * 2, PLTT4_TRUE, TRUE );
        key2 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_16, PLTT4_FALSE, TRUE );
        
        NNS_GFD_ASSERT( IsValidKey_( key1, 0, PLTT_ALLOC_SIZE_4 * 2) );
        // �p�f�B���O�͍s���Ȃ��͂�
        NNS_GFD_ASSERT( IsValidKey_( key2, PLTT_ALLOC_SIZE_4 * 2, PLTT_ALLOC_SIZE_16 ) );
        
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key1 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key2 ) == 0 );
    }
    
    // �����ȃT�C�Y�̐؂�グ�������m�F���܂�
    {
        NNSGfdPlttKey    key;
        key = NNS_GfdAllocPlttVram( 0, PLTT4_TRUE, TRUE );
        NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(key) == NNS_GFD_PLTTSIZE_MIN  );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key ) == 0 );
        
        key = NNS_GfdAllocPlttVram( 1, PLTT4_TRUE, TRUE );
        NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(key) == NNS_GFD_PLTTSIZE_MIN  );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key ) == 0 );
        
        key = NNS_GfdAllocPlttVram( NNS_GFD_PLTTSIZE_MIN, PLTT4_TRUE, TRUE );
        NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(key) == NNS_GFD_PLTTSIZE_MIN  );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key ) == 0 );
        
        key = NNS_GfdAllocPlttVram( NNS_GFD_PLTTSIZE_MIN+1, PLTT4_TRUE, TRUE );
        NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(key) == NNS_GFD_PLTTSIZE_MIN * 2 );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key ) == 0 );
    }
    
    GfDDemo_Free( pMgrWork );
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  �T���v���̃��C���֐�
                
  Arguments:    �Ȃ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    GFDDemo_CommonInit();
    
    //
    // ��{�I�Ȋ֐��Q�̃e�X�g
    //
    {
        OS_Printf("tests-begin----------\n");
            TexManInitTest_();
            TexAllocateTest_();
            PlttAllocateTest_();
            
        OS_Printf("tests-end------------\n");
    }
    
    //
    // �������[�v
    //
    while(TRUE){}
}

