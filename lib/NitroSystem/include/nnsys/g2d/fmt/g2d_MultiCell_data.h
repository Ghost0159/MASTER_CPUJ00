/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d - fmt
  File:     g2d_MultiCell_data.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.14 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_G2D_MULTICELL_DATA_H_
#define NNS_G2D_MULTICELL_DATA_H_

#include <nitro/types.h>
#include <nnsys/g2d/fmt/g2d_Common_data.h>
#include <nnsys/g2d/fmt/g2d_Anim_data.h>
#include <nnsys/g2d/fmt/g2d_Cell_data.h>
#include <nnsys/g2d/g2d_config.h>


#ifdef __cplusplus
extern "C" {
#endif

#define NNS_G2D_BINFILE_EXT_MULTICELL     "NMCR"

#define NNS_G2D_BINFILE_SIG_MULTICELL          (u32)'NMCR'
#define NNS_G2D_BLKSIG_MULTICELLBANK           (u32)'MCBK'

//
// �o�[�W�������
// Ver         �ύX�_
// -------------------------------------
// 1.0         ����
//
#define NNS_G2D_NMCR_MAJOR_VER             (u8)1
#define NNS_G2D_NMCR_MINOR_VER             (u8)0



#define NNS_G2D_MCNODE_PLAYMODE_MASK        0x0F
#define NNS_G2D_MCNODE_PLAYMODE_SHIFT       0
#define NNS_G2D_MCNODE_VISIBILITY_SHIFT     5
#define NNS_G2D_MCNODE_CELLANIMIDX_SHIFT    8
#define NNS_G2D_MCNODE_CELLANIMIDX_MASK     0xFF00

typedef enum NNSG2dMCAnimationPlayMode
{
    NNS_G2D_MCANIM_PLAYMODE_RESET = 0,
    NNS_G2D_MCANIM_PLAYMODE_CONTINUE  = 1,
    NNS_G2D_MCANIM_PLAYMODE_PAUSE = 2,
    NNS_G2D_MCANIM_PLAYMODE_MAX
    
}NNSG2dMCAnimationPlayMode;

//------------------------------------------------------------------------------
typedef struct NNSG2dMultiCellHierarchyData
{
    u16         animSequenceIdx;        // NNSG2dCellAnimation �� �Đ�����V�[�P���X�ԍ�
    s16         posX;                   // MultiCell ���[�J���n�ł� NNSG2dCellAnimation �� �ʒu
    s16         posY;                   // MultiCell ���[�J���n�ł� NNSG2dCellAnimation �� �ʒu
                                        
                                        
    u16         nodeAttr;               // Node���� ( NNSG2dMCAnimationPlayMode �Ȃ� 
                                        // 16 ............ 8 ... 5 .....4 .........................0
                                        //   �Z���A�j���ԍ�   �\��    ��   NNSG2dMCAnimationPlayMode
                                        //
    
    
}NNSG2dMultiCellHierarchyData;


//------------------------------------------------------------------------------
// NNSG2dMultiCellData ��`�f�[�^
// NNSG2dMultiCellAnimation ��`�t�@�C���Ɋi�[�����f�[�^
// NNSG2dMultiCellInstance �ɂ���ĎQ�Ƃ����
//
typedef struct NNSG2dMultiCellData
{
    u16                             numNodes;
//  u16                             numTotalOams;     // MultiCell�̕`��ɕK�v�ȑ�Oam��(�������F�g�p�s��)
    u16                             numCellAnim;      // �K�v�ɂȂ�Z���A�j�����̂̐�
    NNSG2dMultiCellHierarchyData*   pHierDataArray;   // �Đ��V�[�P���X�A�ʒu�A�Ȃ�(numNodes �� )

}NNSG2dMultiCellData;


//------------------------------------------------------------------------------
typedef struct NNSG2dMultiCellDataBank
{
    u16                             numMultiCellData;
    u16                             pad16;
    NNSG2dMultiCellData*            pMultiCellDataArray;
    NNSG2dMultiCellHierarchyData*   pHierarchyDataArray;
    void*                           pStringBank;
    void*                           pExtendedData;        // offset addr (if it exists.)
    // MultiCell���\������ NNSG2dCellAnimation����`����Ă��� NNSG2dAnimBankData
    // const NNSG2dAnimBankData*      pAnimDataBank;// �֘A����AnimBank (�����^�C���ɂ�Unpack���ɐݒ肳��܂�)
    //
    // �ύX�F���̏���NNSG2dMultiCellInstance(���̑�)�ŕێ����邱�ƂƂ���
    // 
    // NNSG2dMultiCellInstance(�����^�C������)�ł̎g�p�P�[�X���l����ƁA 
    // pAnimDataBank��� �� NNSG2dMultiCellData �ŕێ���������񂾂��ANNSG2dMultiCellData�ŕێ������
    //  �E�f�[�^�ʑ������傫��( �� NNSG2dMultiCellData �� ���ʂ��ƂĂ������ꍇ���l������ )
    //  �EpAnimDataBank �f�[�^�͏d������ꍇ������
    // 
    // ����Ȃ�΁ANNSG2dMultiCellData �� ���� NNSG2dMultiCellInstance �� �f�[�^�������ƂƂ���B
    // ��L�̌��_���܂���������ł��Ȃ����A�����^�C�����Ŋ֘A�t���镪�����_��͍����B
    // (���� ������ NNSG2dMultiCellInstance �� NNSG2dMultiCellData �����L����P�[�X�Ŏ��R�x�������B)
    // 
    
    
}NNSG2dMultiCellDataBank;

//------------------------------------------------------------------------------
typedef struct NNSG2dMultiCellDataBankBlock
{
    NNSG2dBinaryBlockHeader     blockHeader;
    NNSG2dMultiCellDataBank     multiCellDataBank;
    
}NNSG2dMultiCellDataBankBlock;


//------------------------------------------------------------------------------
typedef struct NNSG2dUserExMultiCellAttr
{
    u32*           pAttr;
    
}NNSG2dUserExMultiCellAttr;

typedef struct NNSG2dUserExMultiCellAttrBank
{
    u16                          numMultiCells; // 
    u16                          numAttribute;  // �A�g���r���[�g���F���݂�1�Œ�
    NNSG2dUserExMultiCellAttr*   pMCAttrArray;   
    
}NNSG2dUserExMultiCellAttrBank;



//------------------------------------------------------------------------------
// inline functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// �}���`�Z���m�[�h�A�g���r���[�g��ݒ肵�܂�
// �R���o�[�^���g�p����֐��ł��B
// 
NNS_G2D_INLINE void 
NNSi_G2dSetMultiCellNodeAttribute
( 
    NNSG2dMCAnimationPlayMode    mode, 
    int                          bVisibility,
    u16*                         pDstAttr
)
{
    *pDstAttr = (u16)( ( ( mode & NNS_G2D_MCNODE_PLAYMODE_MASK ) << NNS_G2D_MCNODE_PLAYMODE_SHIFT ) | 
                       ( ( bVisibility & 0x1 ) << NNS_G2D_MCNODE_VISIBILITY_SHIFT ) );
}

//------------------------------------------------------------------------------
// Node�̉���Ԃ��擾���܂�
NNS_G2D_INLINE BOOL 
NNSi_G2dIsMultiCellNodeVisible
( 
    const NNSG2dMultiCellHierarchyData* pNode 
)
{
    return (BOOL)(( pNode->nodeAttr >> NNS_G2D_MCNODE_VISIBILITY_SHIFT ) & 0x1);
}

//------------------------------------------------------------------------------
// Node�Ƀo�C���h�����Z���A�j���[�V�����̍Đ��������擾���܂�
NNS_G2D_INLINE NNSG2dMCAnimationPlayMode 
NNSi_G2dGetMultiCellNodePlayMode
( 
    const NNSG2dMultiCellHierarchyData* pNode 
)
{
    const NNSG2dMCAnimationPlayMode mode 
        = (NNSG2dMCAnimationPlayMode) ( ( pNode->nodeAttr >> NNS_G2D_MCNODE_PLAYMODE_SHIFT ) & 
                                          NNS_G2D_MCNODE_PLAYMODE_MASK );
    // TODO:ASSERT
                                                
    return mode;
}
//------------------------------------------------------------------------------
// Node���Q�Ƃ���Z���A�j���̔ԍ���ݒ肵�܂�
// �R���o�[�^���痘�p�����֐��ł��B
NNS_G2D_INLINE void NNSi_G2dSetMC2NodeCellAinmIdx
( 
    NNSG2dMultiCellHierarchyData*  pNodeData, 
    u8                             idx 
)
{
    pNodeData->nodeAttr &= ~NNS_G2D_MCNODE_CELLANIMIDX_MASK;
    pNodeData->nodeAttr |= idx << NNS_G2D_MCNODE_CELLANIMIDX_SHIFT;
}

//------------------------------------------------------------------------------
// Node���Q�Ƃ���Z���A�j���̔ԍ����擾���܂�
NNS_G2D_INLINE u16 
NNSi_G2dGetMC2NodeCellAinmIdx
( 
    const NNSG2dMultiCellHierarchyData*  pNodeData
)
{
    return (u16)((NNS_G2D_MCNODE_CELLANIMIDX_MASK & pNodeData->nodeAttr ) >> NNS_G2D_MCNODE_CELLANIMIDX_SHIFT);
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetUserExCellAttrBankFromMCBank

  Description:  �}���`�Z���o���N����Z���g���A�g���r���[�g�o���N���擾���܂��B
                
                
  Arguments:    pMCBank   :           �}���`�Z���o���N
                
                
  Returns:      �Z���g���A�g���r���[�g�o���N
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE const NNSG2dUserExCellAttrBank* 
NNS_G2dGetUserExCellAttrBankFromMCBank( const NNSG2dMultiCellDataBank* pMCBank )
{
    // �u���b�N���擾���܂�
    const NNSG2dUserExDataBlock* pBlk 
        = NNSi_G2dGetUserExDataBlkByID( pMCBank->pExtendedData,
                                        NNS_G2D_USEREXBLK_CELLATTR );
    // �u���b�N�̎擾�ɐ���������...
    if( pBlk != NULL )
    {
        return (const NNSG2dUserExCellAttrBank*)(pBlk + 1);
    }else{
        return NULL;                                
    }
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_G2D_MULTICELL_DATA_H_

