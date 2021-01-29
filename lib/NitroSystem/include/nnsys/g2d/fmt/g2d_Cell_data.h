/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d - fmt
  File:     g2d_Cell_data.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.26 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_G2D_CELL_DATA_H_
#define NNS_G2D_CELL_DATA_H_

#include <nitro/types.h>
#include <nnsys/g2d/fmt/g2d_Character_data.h>// �Z�� ��`�̓L�����N�^�}�b�s���O���Ɉˑ�����
#include <nnsys/g2d/fmt/g2d_Common_data.h>
#include <nnsys/g2d/g2d_config.h>

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// �o�C�i���t�@�C�� ���ʎq �֘A
#define NNS_G2D_BINFILE_SIG_CELL          (u32)'NCER'
#define NNS_G2D_BLKSIG_CELLBANK           (u32)'CEBK'

#define NNS_G2D_USEREXBLK_CELLATTR        (u32)'UCAT'


//------------------------------------------------------------------------------
// �o�C�i���t�@�C�� �g���q
#define NNS_G2D_BINFILE_EXT_CELL          "NCER"

//------------------------------------------------------------------------------



// NNSG2dCellData.bUseFlipHV �����Q�Ƃ��邽�߂� �V�t�g��
#define NNS_G2D_CELL_FLIP_H_SHIFT           (0 + 8) 
#define NNS_G2D_CELL_FLIP_V_SHIFT           (1 + 8)
#define NNS_G2D_CELL_FLIP_HV_SHIFT          (2 + 8)

#define NNS_G2D_CELL_HAS_BR_SHIFT           (3 + 8)

#define NNS_G2D_CELL_BS_R_MASK              0x3F        // 6 bit 
#define NNS_G2D_CELL_BS_R_OFFSET            (2)
#define NNS_G2D_CELL_BS_R_SHIFT             (0)

#ifndef NNS_G2D_LIMIT_CELL_X_128
    #define NNS_G2D_CELL_MAX_X                  255
#else // NNS_G2D_LIMIT_CELL_X_128
    #define NNS_G2D_CELL_MAX_X                  127
#endif // NNS_G2D_LIMIT_CELL_X_128

#define NNS_G2D_CELL_MAX_Y                  127

//------------------------------------------------------------------------------
// NNSG2dCellDataBank.cellBankAttr ���̃r�b�g����`
#define NNS_G2D_CELLBK_ATTR_CELLWITHBR       0x1 // NNSG2dCellDataBank.cellBankAttr�� & ���Ƃ�
                                                 // �Z���f�[�^�� 
                                                 // NNSG2dCellDataWithBR �Ȃ� 1 
                                                 // NNSG2dCellData �Ȃ� 0

//
// �o�[�W�������
// Ver         �ύX�_
// -------------------------------------
// 1.0         Vram�]���A�j���@�\�̊g��
//
#define NNS_G2D_NCER_MAJOR_VER              1
#define NNS_G2D_NCER_MINOR_VER              0       



//------------------------------------------------------------------------------
// �֐������F�݊����̂��߁A�Â����̂�ʖ��Ƃ��Ē�`���܂�
#define NNS_G2dGetSizeReqiredVramTransferCellDataBank   NNS_G2dGetSizeRequiredVramTransferCellDataBank   

/*---------------------------------------------------------------------------*
  Name:         NNSG2dCellVramTransferData

  Description:  1�̃Z����\������̂ɕK�v��Vram�]���Ɋւ�����
                
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dCellVramTransferData
{
    u32        srcDataOffset;
    u32        szByte;
    
}NNSG2dCellVramTransferData;

/*---------------------------------------------------------------------------*
  Name:         NNSG2dVramTransferData

  Description:  �Z����\������̂ɕK�v��Vram�]���Ɋւ�����
                
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dVramTransferData
{
    u32                                 szByteMax;             // ���ׂĂ�VRAM�]�����̍ő�o�C�g��
    NNSG2dCellVramTransferData*         pCellTransferDataArray;// �ێ������ NNSG2dCellDataBank 
                                                               // �� numCells ���̒����ł�
    
}NNSG2dVramTransferData;




/*---------------------------------------------------------------------------*
  Name:         NNSG2dCellOAMAttrData

  Description:  �Z�����`���� OAM�A�g���r���[�g
                
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dCellOAMAttrData
{
    u16         attr0;
    u16         attr1;
    u16         attr2;

}NNSG2dCellOAMAttrData;

/*---------------------------------------------------------------------------*
  Name:         NNSG2dCellData

  Description:  �Z���̋��E���(��`)���`����
                
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dCellBoundingRectS16
{
    s16                       maxX;
    s16                       maxY;
    s16                       minX;
    s16                       minY;
    
}NNSG2dCellBoundingRectS16; 

/*---------------------------------------------------------------------------*
  Name:         NNSG2dCellData

  Description:  �Z�����`���� �T�O
                
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dCellData
{
    u16                       numOAMAttrs;      // OAM �A�g���r���[�g��
    u16                       cellAttr;         // �Z���̎�ނɊւ�����
                                                // �t���b�v �Ɋւ��� ��� (���8�r�b�g)
                                                // ���E���Ɋւ�����
                                                // 
                                                // �`��œK���Ɋւ��� ��� (����8�r�b�g)
                                                //      (6Bit => ���E���̔��a >> 2 )
                                                
    
    NNSG2dCellOAMAttrData*    pOamAttrArray;    // NNSG2dCellOAMAttrData �z��ւ̃|�C���^

}NNSG2dCellData;

/*---------------------------------------------------------------------------*
  Name:         NNSG2dCellDataWithBR
  
  Description:  �Z�����`���� �T�O(���E���Ƃ���NNSG2dCellBoundingRect������)
                
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dCellDataWithBR
{
    NNSG2dCellData              cellData;
    NNSG2dCellBoundingRectS16   boundingRect;
    
}NNSG2dCellDataWithBR;

/*---------------------------------------------------------------------------*
  Name:         NNSG2dCellDataBank

  Description:  ���� �Z���f�[�^���܂Ƃ߂� �T�O
                �ʏ�� NNSG2dCellDataBank == �o�C�i���t�@�C��( NCER ) �ł�
                
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dCellDataBank
{
    u16                              numCells;               // �Z������
    u16                              cellBankAttr;           // ����
    NNSG2dCellData*                  pCellDataArrayHead;     // offset addres 
    NNSG2dCharacterDataMapingType    mappingMode;            // �Q�ƃL�����N�^�̃}�b�s���O���[�h
    
    NNSG2dVramTransferData*          pVramTransferData;      // Vram �]�� �A�j�� �Ɋւ�����
    
    void*                            pStringBank;            // �����o���N
    void*                            pExtendedData;          // offset addr (if it exists.)
    
    
}NNSG2dCellDataBank;

/*---------------------------------------------------------------------------*
  Name:         NNSG2dCellDataBankBlock 

  Description:  NNSG2dCellDataBank���i�[����o�C�i���u���b�N
  
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dCellDataBankBlock
{
    NNSG2dBinaryBlockHeader     blockHeader;
    NNSG2dCellDataBank          cellDataBank;
    
}NNSG2dCellDataBankBlock;


//------------------------------------------------------------------------------
typedef struct NNSG2dUserExCellAttr
{
    u32*           pAttr;
    
}NNSG2dUserExCellAttr;

typedef struct NNSG2dUserExCellAttrBank
{
    u16                      numCells; // 
    u16                      numAttribute; // �A�g���r���[�g���F���݂�1�Œ�
    NNSG2dUserExCellAttr*    pCellAttrArray;   
    
}NNSG2dUserExCellAttrBank;



//------------------------------------------------------------------------------
// �C�����C���֐�
//------------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dSetCellBankHasBR

  Description:  �Z�������E��`�����Z���o���N���ǂ������擾���܂�
                << �R���o�[�^���g�p����֐��ł� >>
                
  Arguments:    pCellBank:           �Z���o���N���
                
                
  Returns:      �Z���o���N�����E��`�������ǂ���
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNSi_G2dSetCellBankHasBR(  NNSG2dCellDataBank* pCellBank )
{
    NNS_G2D_NULL_ASSERT( pCellBank );
    pCellBank->cellBankAttr |= NNS_G2D_CELLBK_ATTR_CELLWITHBR;
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dCellBankHasBR

  Description:  �Z�������E��`�����Z���o���N���ǂ������擾���܂�
                
  Arguments:    pCellBank:           �Z���o���N���
                
                
  Returns:      �Z���o���N�����E��`�������ǂ���
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE BOOL NNSi_G2dCellBankHasBR(  const NNSG2dCellDataBank* pCellBank )
{
    return (BOOL)( pCellBank->cellBankAttr & NNS_G2D_CELLBK_ATTR_CELLWITHBR );
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dSetCellAttrFlipFlag

  Description:  �t���b�v���p�ݒ�����i�[���� u16 �ϐ� ��ݒ肵�Ԃ��܂� 
                
  Arguments:    bH:         H�t���b�v
                bV:         V�t���b�v
                bHV:        HV�t���b�v
                
  Returns:      �����i�[���� u16 �ϐ� 
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE u16 NNSi_G2dSetCellAttrFlipFlag( BOOL bH, BOOL bV, BOOL bHV )
{
    return (u16)((bH  << NNS_G2D_CELL_FLIP_H_SHIFT) |
                 (bV  << NNS_G2D_CELL_FLIP_V_SHIFT) |
                 (bHV << NNS_G2D_CELL_FLIP_HV_SHIFT)  );
}
/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dSetCellAttrHasBR

  Description:  �Z�������E��`�������ǂ�����ݒ肵�� u16 �ϐ� ��ݒ肵�Ԃ��܂� 
                << �R���o�[�^���g�p����֐��ł� >>
                
  Arguments:    bHasBR:         �Z�������E��`�������ǂ���
                
                
  Returns:      �����i�[���� u16 �ϐ� 
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE u16 NNSi_G2dSetCellAttrHasBR( BOOL bHasBR )
{
    return (u16)( bHasBR << NNS_G2D_CELL_HAS_BR_SHIFT );
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dIsCellFlipH

  Description:  H�t���b�v ���g�p���邩�������܂�
                
  Arguments:    pCell:           �Z���f�[�^ 
                
  Returns:      �g�p���邩�ǂ���
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE BOOL NNSi_G2dIsCellFlipH( const NNSG2dCellData* pCell )
{
    
    return (BOOL)( ( pCell->cellAttr >> NNS_G2D_CELL_FLIP_H_SHIFT) & 0x1);
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dIsCellFlipV

  Description:  V�t���b�v ���g�p���邩�������܂�
                
  Arguments:    pCell:           �Z���f�[�^ 
                
  Returns:      �g�p���邩�ǂ���
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE BOOL NNSi_G2dIsCellFlipV( const NNSG2dCellData* pCell )
{
    return (BOOL)( (pCell->cellAttr >> NNS_G2D_CELL_FLIP_V_SHIFT) & 0x1);
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dIsCellFlipHV

  Description:  HV�t���b�v ���g�p���邩�������܂�
                
  Arguments:    pCell:           �Z���f�[�^ 
                
  Returns:      �g�p���邩�ǂ���
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE BOOL NNSi_G2dIsCellFlipHV( const NNSG2dCellData* pCell )
{
    return (BOOL)( (pCell->cellAttr >> NNS_G2D_CELL_FLIP_HV_SHIFT) & 0x1);
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dCellHasBR

  Description:  �Z�������E��`�������ǂ������擾���܂�
                
  Arguments:    pCell:           �Z���f�[�^
                
                
  Returns:      �Z�������E��`�������ǂ���
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE BOOL NNS_G2dCellHasBR(  const NNSG2dCellData* pCell )
{
    return (BOOL)( (pCell->cellAttr >> NNS_G2D_CELL_HAS_BR_SHIFT) & 0x1);
}

NNS_G2D_INLINE BOOL NNSi_G2dCellHasBR(  const NNSG2dCellData* pCell )
{
    return NNS_G2dCellHasBR( pCell );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dCellBoundingRect

  Description:  �Z���̋��E��`���擾���܂��B
                �����ł̓Z�������E��`���������肵�܂���B
                ����̓��[�U�R�[�h�ōs���Ă��������B
                
  Arguments:    pCell:           �Z���f�[�^
                
                
  Returns:      �Z�������E��`
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE const NNSG2dCellBoundingRectS16* 
NNS_G2dGetCellBoundingRect( const NNSG2dCellData* pCell )
{
    // NNS_G2D_ASSERT( NNSi_G2dCellHasBR( pCell ) )
    const NNSG2dCellDataWithBR* pBR = ( const NNSG2dCellDataWithBR* )pCell;
    return &pBR->boundingRect;
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dSetCellBoundingSphereR

  Description:  �Z���̋��E������ݒ肵�܂��B(�{�֐��̓R���o�[�^�ɂ��g�p�����֐��ł�)
                8Bit���E�����a����2�r�b�g�E�V�t�g���āApCell->cellAttr�Ɋi�[���܂��B

                ���a���̉���2�r�b�g�͐؎̂đO�ɁA�؂�グ�����������Ȃ��܂��B
                
                << �R���o�[�^���g�p����֐��ł� >>
                
                
  Arguments:    pCell:           �Z���f�[�^
                R    :           ���E�����a
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNSi_G2dSetCellBoundingSphereR( NNSG2dCellData* pCell, u8 R )
{
    //
    // �؂�グ����
    //
    if( (R & 0x3) != 0 )
    {
        R += 0x1 << NNS_G2D_CELL_BS_R_OFFSET;
        R &= ~0x3;
    }

    R = (u8)(R >> NNS_G2D_CELL_BS_R_OFFSET);
    pCell->cellAttr |= (( R & NNS_G2D_CELL_BS_R_MASK ) << NNS_G2D_CELL_BS_R_SHIFT);
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetCellBoundingSphereR    
                NNSi_G2dGetCellBoundingSphereR

  Description:  �Z���̋��E�������擾���܂��B
                
  Arguments:    pCell:           �Z���f�[�^
                
                
  Returns:      ���E�����a
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE u8 NNS_G2dGetCellBoundingSphereR( const NNSG2dCellData* pCell )
{
    u8 R =  (u8)( (pCell->cellAttr >> NNS_G2D_CELL_BS_R_SHIFT) & NNS_G2D_CELL_BS_R_MASK );
    return (u8)(R << NNS_G2D_CELL_BS_R_OFFSET);
}

NNS_G2D_INLINE u8 NNSi_G2dGetCellBoundingSphereR( const NNSG2dCellData* pCell )
{
    // TODO:�x����\������
    return NNS_G2dGetCellBoundingSphereR( pCell );
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dCellDataBankHasVramTransferData

  Description:  �Z���o���N��VRAM�]�����������Ă��邩�������܂��B
                
  Arguments:    pCellBank:           �Z���f�[�^�o���N
                
                
  Returns:      �����Ă����TRUE
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE BOOL NNS_G2dCellDataBankHasVramTransferData( const NNSG2dCellDataBank* pCellBank )
{
    // NNS_G2D_NULL_ASSERT( pCellBank );
    return (BOOL)( pCellBank->pVramTransferData != NULL );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetSizeRequiredVramTransferCellDataBank

  Description:  VRAM�]���Z���o���N���K�v�Ƃ���VRAM�̈�̑傫���i�o�C�g�j���擾���܂��B
                VRAM�]�����������Ă��Ȃ��ꍇ�̓[����Ԃ��܂��B
                
  Arguments:    pCellBank:           �Z���f�[�^�o���N
                
                
  Returns:      �K�v�Ƃ���VRAM�̈�̑傫���i�o�C�g�j
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE u32 NNS_G2dGetSizeRequiredVramTransferCellDataBank( const NNSG2dCellDataBank* pCellBank )
{
    // NNS_G2D_NULL_ASSERT( pCellBank );
    if( NNS_G2dCellDataBankHasVramTransferData( pCellBank ) )
    {
        const NNSG2dVramTransferData* pData = pCellBank->pVramTransferData;
        
        return pData->szByteMax;
    }
    
    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetCellVramTransferData

  Description:  �w�肳�ꂽ�Z���ԍ���VRAM�]���Z���f�[�^���擾���܂�
                
                �Z�����ȏ�̃Z���ԍ����w�肳�ꂽ�ꍇ�A�A�T�[�g�Ɏ��s���܂�
                VRAM�]�����������Ȃ��Z���f�[�^�o���N���w�肳�ꂽ�ꍇ�A�A�T�[�g�Ɏ��s���܂�
                
  Arguments:    pCellBank   :           �Z���f�[�^�o���N
                cellIdx     :           �Z���ԍ�
                
  Returns:      �K�v�Ƃ���VRAM�̈�̑傫���i�o�C�g�j
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE const NNSG2dCellVramTransferData* 
NNSi_G2dGetCellVramTransferData( const NNSG2dCellDataBank* pCellBank, u16 cellIdx )
{
    //( pCellBank );
    //( cellIdx < pCellBank->numCells );
    // ( NNS_G2dCellDataBankHasVramTransferData( pCellBank )
    const NNSG2dVramTransferData*       pTransferData = pCellBank->pVramTransferData;
        
    return &pTransferData->pCellTransferDataArray[cellIdx];
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetUserExCellAttrBankFromCellBank

  Description:  �Z���o���N����Z���g���A�g���r���[�g�o���N���擾���܂��B
                
                
  Arguments:    pCellBank   :           �Z���f�[�^�o���N
                
                
  Returns:      �Z���g���A�g���r���[�g�o���N
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE const NNSG2dUserExCellAttrBank* 
NNS_G2dGetUserExCellAttrBankFromCellBank( const NNSG2dCellDataBank* pCellBank )
{
    // �u���b�N���擾���܂�
    const NNSG2dUserExDataBlock* pBlk 
        = NNSi_G2dGetUserExDataBlkByID( pCellBank->pExtendedData,
                                        NNS_G2D_USEREXBLK_CELLATTR );
    // �u���b�N�̎擾�ɐ���������...
    if( pBlk != NULL )
    {
        return (const NNSG2dUserExCellAttrBank*)(pBlk + 1);
    }else{
        return NULL;                                
    }
}
/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetUserExCellAttr

  Description:  �Z���g���A�g���r���[�g�o���N����Z���g���A�g���r���[�g
                ���Z���ԍ����w�肵�Ď擾���܂��B
                
                
  Arguments:    pCellAttrBank   :           �Z���g���A�g���r���[�g�o���N
                idx             :           �Z���ԍ�                
                
  Returns:      �Z���g���A�g���r���[�g
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE const NNSG2dUserExCellAttr* 
NNS_G2dGetUserExCellAttr
( 
    const NNSG2dUserExCellAttrBank* pCellAttrBank, 
    u16                             idx
)
{
    if( idx < pCellAttrBank->numCells )
    {
        return &pCellAttrBank->pCellAttrArray[idx];
    }else{
        return NULL;    
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetUserExCellAttrValue

  Description:  �Z���g���A�g���r���[�g����A�g���r���[�g�l���擾���܂�
                
                
  Arguments:    pCellAttrBank   :           �Z���g���A�g���r���[�g
                
                
  Returns:      �A�g���r���[�g�l
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE const u32
NNS_G2dGetUserExCellAttrValue
( 
    const NNSG2dUserExCellAttr* pCellAttr 
)
{
    return pCellAttr->pAttr[0];
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dCopyCellAsOamAttr

  Description:  �Z���f�[�^����Oam�A�g���r���[�g����GXOamAttr�ɃR�s�[���܂��B
                
  Arguments:    pCell       :       �Z���f�[�^
                idx         :       OAM�A�g���r���[�g�ԍ�
                pDst        :       �R�s�[�� GXOamAttr
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dCopyCellAsOamAttr( const NNSG2dCellData* pCell, u16 idx, GXOamAttr* pDst )
{
    //NNS_G2D_NULL_ASSERT( pCell );
    //NNS_G2D_NULL_ASSERT( pDst );
    //NNS_G2D_MINMAX_ASSERT( idx, 0, pCell->numOAMAttrs );
    {
        const NNSG2dCellOAMAttrData* pSrc = pCell->pOamAttrArray + idx;
        
        pDst->attr0 = pSrc->attr0;
        pDst->attr1 = pSrc->attr1;
        pDst->attr2 = pSrc->attr2;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dRepeatXinCellSpace

  Description:  �Z������OBJ�̃��s�[�g�i���W��荞�݁j�������s���܂�
                ���� s16 �l�� -128 - 127 �ւƕϊ�����܂��B
                
  Arguments:    srcX       :       ���̒l
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE  s16 NNS_G2dRepeatXinCellSpace( s16 srcX )
{
    if( srcX > NNS_G2D_CELL_MAX_X )
    { 
        srcX |= 0xFF00; 
    }
    return srcX;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dRepeatYinCellSpace

  Description:  �Z������OBJ�̃��s�[�g�i���W��荞�݁j�������s���܂�
                ���� s16 �l�� -128 - 127 �ւƕϊ�����܂��B
                
  Arguments:    srcY       :       ���̒l
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE  s16 NNS_G2dRepeatYinCellSpace( s16 srcY )
{   
    if( srcY > NNS_G2D_CELL_MAX_Y )
    { 
        srcY |= 0xFF00; 
    }
    return srcY;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_G2D_CELL_DATA_H_

