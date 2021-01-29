/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - gfd
  File:     gfdi_LinkedListVramMan_Common.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.9 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_GFDI_LINKEDLISTVRAMMAN_COMMON_H_
#define NNS_GFDI_LINKEDLISTVRAMMAN_COMMON_H_


#include <nnsys/gfd.h>

//------------------------------------------------------------------------------
typedef struct NNSiGfdLnkVramBlock NNSiGfdLnkVramBlock;

/*---------------------------------------------------------------------------*
  Name:         NNSiGfdLnkVramBlock

  Description:  �������̈�Ǘ��u���b�N
  
 *---------------------------------------------------------------------------*/
struct NNSiGfdLnkVramBlock
{
    u32                         addr;       // �̈�J�n�A�h���X
    u32                         szByte;     // �̈�T�C�Y(�[���͕s���ł�)
    
    NNSiGfdLnkVramBlock*        pBlkPrev;   // �O�̗̈�(�A�h���X�ʒu�I�Ȋ֘A�͂Ȃ�)
    NNSiGfdLnkVramBlock*        pBlkNext;   // ���̗̈�(�A�h���X�ʒu�I�Ȋ֘A�͂Ȃ�)
    
};

/*---------------------------------------------------------------------------*
  Name:         NNSiGfdLnkMemRegion

  Description:  ���������
                end > start �𖞂������̂Ƃ��܂�
  
 *---------------------------------------------------------------------------*/
typedef struct NNSiGfdLnkMemRegion
{
    u32       start;
    u32       end;
    
}NNSiGfdLnkMemRegion;

/*---------------------------------------------------------------------------*
  Name:         NNSiGfdLnkVramMan

  Description:  �}�l�[�W��
                �ʏ�̃q�[�v�ȂǂƂ͈قȂ�A�g�p�ςݗ̈�̊Ǘ����̃��X�g�������܂���
                �Ǘ����̈�i���C����������j�� �Ǘ��̈�A�h���X�iVRAM��j�Ɋ֘A������ 
                TextureKey�i�A�h���X + �T�C�Y�̏������j����̎g�p�ςݗ̈�̊Ǘ����
                �ւ̃��b�N�A�b�v��������߂ł��B
                �t���[�̍ۂɂ� �A�h���X + �T�C�Y ���� �g�p�̈�̃t���[�̈�ւ̕ԋp���s��
                �Ă��܂��B
                
 *---------------------------------------------------------------------------*/
typedef struct NNSiGfdLnkVramMan
{
    NNSiGfdLnkVramBlock*         pFreeList;         // ���g�p�̈�u���b�N���X�g
      
}NNSiGfdLnkVramMan;




//------------------------------------------------------------------------------
// �֐��錾
//------------------------------------------------------------------------------
void NNSi_GfdDumpLnkVramManFreeListInfo
( 
    const NNSiGfdLnkVramBlock*      pFreeListHead,
    u32                             szReserved 
);

void NNSi_GfdDumpLnkVramManFreeListInfoEx( 
    const NNSiGfdLnkVramBlock*      pFreeListHead, 
    NNSGfdLnkDumpCallBack           pFunc, 
    void*                           pUserData );
    

void 
NNSi_GfdInitLnkVramMan( NNSiGfdLnkVramMan* pMgr );


NNSiGfdLnkVramBlock* 
NNSi_GfdInitLnkVramBlockPool
( 
    NNSiGfdLnkVramBlock*    pArrayHead, 
    u32                     lengthOfArray 
); 

BOOL
NNSi_GfdAddNewFreeBlock
(
    NNSiGfdLnkVramMan*      pMan, 
    NNSiGfdLnkVramBlock**   ppBlockPoolList,
    u32                     baseAddr,
    u32                     szByte
);


BOOL
NNSi_GfdAllocLnkVram
( 
    NNSiGfdLnkVramMan*      pMan, 
    NNSiGfdLnkVramBlock**   ppBlockPoolList,
    u32*                    pRetAddr,
    u32                     szByte
); 

BOOL
NNSi_GfdAllocLnkVramAligned
( 
    NNSiGfdLnkVramMan*      pMan, 
    NNSiGfdLnkVramBlock**   ppBlockPoolList,
    u32*                    pRetAddr,
    u32                     szByte,
    u32                     alignment
);

void NNSi_GfdMergeAllFreeBlocks( 
    NNSiGfdLnkVramMan*      pMan, 
    NNSiGfdLnkVramBlock**   ppBlockPoolList
);

BOOL NNSi_GfdFreeLnkVram
( 
    NNSiGfdLnkVramMan*      pMan, 
    NNSiGfdLnkVramBlock**   ppBlockPoolList,
    u32                     addr,
    u32                     szByte
);





#endif // NNS_GFDI_LINKEDLISTVRAMMAN_COMMON_H_

