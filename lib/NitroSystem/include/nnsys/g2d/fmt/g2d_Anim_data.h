/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d - fmt
  File:     g2d_Anim_data.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.17 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_G2D_ANIM_DATA_H_
#define NNS_G2D_ANIM_DATA_H_





#include <nitro/types.h>
#include <nitro/fx/fx.h>


#include <nnsys/g2d/g2d_config.h>
#include <nnsys/g2d/fmt/g2d_Common_data.h>

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// �o�C�i���t�@�C�� ���ʎq �֘A
#define NNS_G2D_BINFILE_SIG_CELLANIM          (u32)'NANR'
#define NNS_G2D_BINFILE_SIG_MULTICELLANIM     (u32)'NMAR'

#define NNS_G2D_BLKSIG_ANIMBANK               (u32)'ABNK'
#define NNS_G2D_USEREXBLK_ANMATTR             (u32)'UAAT'

//------------------------------------------------------------------------------
// �o�C�i���t�@�C�� �g���q
#define NNS_G2D_BINFILE_EXT_CELLANIM          "NANR"
#define NNS_G2D_BINFILE_EXT_MULTICELLANIM     "NMAR"

//
// �o�[�W�������
// Ver         �ύX�_
// -------------------------------------
// 1.0         ����
//
#define NNS_G2D_NANR_MAJOR_VER   1
#define NNS_G2D_NANR_MINOR_VER   0


// NNSG2dAnimSequenceData �����o�A�N�Z�X�ɗ��p
#define NNS_G2D_ANIMTYPE_SHIFT      16
#define NNS_G2D_ANIMTYPE_MASK       0xFF00
#define NNS_G2D_ANIMELEM_MASK       0x00FF


/*---------------------------------------------------------------------------*
  Name:         NNSG2dAnimationType

  Description:  Animation �� ���
                NNSG2dAnimSequence ���ێ����܂�
                �i�܂�A�ЂƂ�Sequence���ł͓���NNSG2dAnimationType���ۏႳ��܂��j
                
 *---------------------------------------------------------------------------*/
typedef enum NNSG2dAnimationType
{
    NNS_G2D_ANIMATIONTYPE_INVALID           = 0x0, // �s���Ȏ��
    NNS_G2D_ANIMATIONTYPE_CELL                   , // �Z��
    NNS_G2D_ANIMATIONTYPE_MULTICELLLOCATION      , // �}���`�Z��
    NNS_G2D_ANIMATIONTYPE_MAX
}
NNSG2dAnimationType;

/*---------------------------------------------------------------------------*
  Name:         NNSG2dAnimationElement

  Description:  Animation �v�f �� ���
                NNSG2dAnimSequence ���ێ����܂�
                �i�܂�A�ЂƂ�Sequence���ł͓���NNSG2dAnimationElement���ۏႳ��܂��j
                
 *---------------------------------------------------------------------------*/
typedef enum NNSG2dAnimationElement
{
    NNS_G2D_ANIMELEMENT_INDEX           = 0x0, // Index �̂�
    NNS_G2D_ANIMELEMENT_INDEX_SRT            , // Index + SRT 
    NNS_G2D_ANIMELEMENT_INDEX_T              , // Index + T 
    NNS_G2D_ANIMELEMENT_MAX
}
NNSG2dAnimationElement;

/*---------------------------------------------------------------------------*
  Name:         NNSG2dAnimationPlayMode

  Description:  �A�j���[�V�����V�[�P���X�̍Đ����@
                NNSG2dAnimSequence ���ێ����܂�
                
                
 *---------------------------------------------------------------------------*/
typedef enum NNSG2dAnimationPlayMode
{
    NNS_G2D_ANIMATIONPLAYMODE_INVALID = 0x0,    // ����
    NNS_G2D_ANIMATIONPLAYMODE_FORWARD,          // �����^�C���Đ�(������)
    NNS_G2D_ANIMATIONPLAYMODE_FORWARD_LOOP,     // ���s�[�g�Đ�(���������[�v)
    NNS_G2D_ANIMATIONPLAYMODE_REVERSE,          // �����Đ�(���o�[�X�i���{�t�����j
    NNS_G2D_ANIMATIONPLAYMODE_REVERSE_LOOP,     // �����Đ����s�[�g�i���o�[�X�i���{�t�������j ���[�v�j
    NNS_G2D_ANIMATIONPLAYMODE_MAX               
}
NNSG2dAnimationPlayMode;


/*---------------------------------------------------------------------------*
  Name:         NNSG2dAnimDataSRT

  Description:  �A�j���[�V��������
                NNSG2dAnimFrameData.pContent �̐悪���̃f�[�^�ɂȂ��Ă��܂��B
                �����I�ɂ́A�A�j���[�V�������� NNSG2dAnimDataSRT �ȊO�� 
                �����̃t�H�[�}�b�g�̃T�|�[�g���\�肳��Ă��܂��B
                                
 *---------------------------------------------------------------------------*/
typedef       u16       NNSG2dAnimData; // index �̂�

typedef struct NNSG2dAnimDataSRT        // index + SRT ���
{
    u16         index;      // �C���f�b�N�X
    
    u16         rotZ;       // ��]
    
    fx32        sx;         // �X�P�[��X
    fx32        sy;         // �X�P�[��Y
    
    s16         px;         // �ʒuX
    s16         py;         // �ʒuY
    
}NNSG2dAnimDataSRT;

typedef struct NNSG2dAnimDataT        // index + T ���
{
    u16         index;      // �C���f�b�N�X    
    u16         pad_;       // ��]

    s16         px;         // �ʒuX
    s16         py;         // �ʒuY
    
}NNSG2dAnimDataT;



/*---------------------------------------------------------------------------*
  Name:         NNSG2dAnimFrameData

  Description:  �A�j���[�V�����t���[��
                �A�j���[�V�������\������P�ʗv�f
                �A�j���[�V�����t���[���������A�Ȃ��� �A�j���[�V�����V�[�P���X��
                �`�����܂��B
                
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dAnimFrameData
{
    void*           pContent;   // �A�j���[�V�������ʂւ̃|�C���^
    u16             frames;     // �A�j���t���[����������( �P�� : �r�f�I�t���[�� )
    u16             pad16;      // pading 
    
}NNSG2dAnimFrameData;





/*---------------------------------------------------------------------------*
  Name:         NNSG2dAnimFrameData

  Description:  �A�j���[�V�����V�[�P���X
                ��A�̃A�j���[�V�����t���[���̏W��
                ������A�j���[�V�����ɑΉ�����T�O�ł��B
                
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dAnimSequenceData
{
    u16                     numFrames;         // �V�[�P���X���\������A�j���[�V�����t���[����
    u16                     loopStartFrameIdx; // ���[�v�J�n�A�j���[�V�����t�t���[���ԍ�
    
    u32                     animType;          // �A�j���[�V�����̎��(���16 bit)|
                                               //  �A�j���[�V�����̗v�f(����16 bit)
                                               // �iNNSG2dAnimFrameData.pContent�̐悪�ω����܂��j
                                                      
    NNSG2dAnimationPlayMode playMode;          // �A�j���[�V�����V�[�P���X�̍Đ����@
    NNSG2dAnimFrameData*    pAnmFrameArray;    // offset form the head of pFrameArray.
    
}NNSG2dAnimSequenceData;





/*---------------------------------------------------------------------------*
  Name:         NNSG2dAnimBankData

  Description:  �A�j���[�V�����o���N
                �����̃A�j���[�V�����V�[�P���X���܂Ƃ߂�T�O�ł��B
                �ʏ� �A�j���[�V�����o���N == �A�j���[�V�����t�@�C���ł��B
                
                
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dAnimBankData
{
    
    u16                       numSequences;         // �A�j���[�V�����V�[�P���X��
    u16                       numTotalFrames;       // ���A�j���[�V�����t���[����
    NNSG2dAnimSequenceData*   pSequenceArrayHead;   // �A�j���[�V�����V�[�P���X�z��ւ̃|�C���^
    NNSG2dAnimFrameData*      pFrameArrayHead;      // �A�j���[�V�����t���[���z��ւ̃|�C���^
    void*                     pAnimContents;        // �A�j���[�V�������ʔz��ւ̃|�C���^
    void*                     pStringBank;          // �X�g�����O�o���N�ւ̃|�C���^�i���s���ɐݒ�j
    void*                     pExtendedData;        // ���C�u�����g���̈�ւ̃|�C���^(���g�p)

}NNSG2dAnimBankData;






/*---------------------------------------------------------------------------*
  Name:         NNSG2dAnimBankDataBlock

  Description:  �A�j���[�V�����o���N�u���b�N
                �A�j���[�V�����o���N���܂ރo�C�i���u���b�N�ł��B
                
                
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dAnimBankDataBlock
{
    NNSG2dBinaryBlockHeader     blockHeader;    // �o�C�i���w�b�_
    NNSG2dAnimBankData          animBankData;   // �A�j���[�V�����o���N
    
}NNSG2dAnimBankDataBlock;

//------------------------------------------------------------------------------
typedef struct NNSG2dUserExAnimFrameAttr
{
    u32*           pAttr;
    
}NNSG2dUserExAnimFrameAttr;

typedef struct NNSG2dUserExAnimSequenceAttr
{
    u16                          numFrames;
    u16                          pad16;
    u32*                         pAttr;    
    NNSG2dUserExAnimFrameAttr*   pAnmFrmAttrArray;
        
}NNSG2dUserExAnimSequenceAttr;

typedef struct NNSG2dUserExAnimAttrBank
{
    u16                           numSequences; // �A�j���[�V�����V�[�P���X��
    u16                           numAttribute; // �A�g���r���[�g���F���݂�1�Œ�
    NNSG2dUserExAnimSequenceAttr* pAnmSeqAttrArray;   
    
}NNSG2dUserExAnimAttrBank;





//------------------------------------------------------------------------------
// inline functions.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// �A�j���[�V������ގ擾�֘A
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
NNS_G2D_INLINE u32 NNSi_G2dMakeAnimType( NNSG2dAnimationType animType, NNSG2dAnimationElement elmType )
{
    return (u32)animType << NNS_G2D_ANIMTYPE_SHIFT | ((u32)elmType & NNS_G2D_ANIMELEM_MASK );
}
//------------------------------------------------------------------------------
NNS_G2D_INLINE NNSG2dAnimationType 
NNSi_G2dGetAnimSequenceAnimType( u32 animType )
{
    return  (NNSG2dAnimationType)( animType >> NNS_G2D_ANIMTYPE_SHIFT );
}
//------------------------------------------------------------------------------
NNS_G2D_INLINE NNSG2dAnimationElement
NNSi_G2dGetAnimSequenceElementType( u32 animType )
{
    return  (NNSG2dAnimationElement)( animType & NNS_G2D_ANIMELEM_MASK );
}

//------------------------------------------------------------------------------
NNS_G2D_INLINE NNSG2dAnimationType 
NNS_G2dGetAnimSequenceAnimType( const NNSG2dAnimSequenceData* pAnimSeq )
{
//    NNS_G2D_NULL_ASSERT( pAnimSeq );
    
    return  NNSi_G2dGetAnimSequenceAnimType( pAnimSeq->animType );
}

//------------------------------------------------------------------------------
NNS_G2D_INLINE NNSG2dAnimationElement
NNS_G2dGetAnimSequenceElementType( const NNSG2dAnimSequenceData* pAnimSeq )
{
//    NNS_G2D_NULL_ASSERT( pAnimSeq );
    
    return  NNSi_G2dGetAnimSequenceElementType( pAnimSeq->animType );
}

//------------------------------------------------------------------------------
// �V�[�P���X�̑��r�f�I�t���[�������v�Z���܂��B
NNS_G2D_INLINE u32 NNS_G2dCalcAnimSequenceTotalVideoFrames
( 
     const NNSG2dAnimSequenceData* pAnimSeq 
)
{
    u32    i;
    u32    total = 0;
    
    for( i = 0; i < pAnimSeq->numFrames; i++ )
    {
        total += pAnimSeq->pAnmFrameArray[i].frames;
    }
    return total;
}
//------------------------------------------------------------------------------
// �A�j���[�V�����o���N���̃A�j���[�V�����V�[�P���X�ԍ����擾���܂�
// �A�j���[�V�����V�[�P���X���A�j���[�V�����o���N���Ɋ܂܂����̂ł��邱�Ƃ����҂��Ă��܂��B
// 
#ifndef NNS_FROM_TOOL
NNS_G2D_INLINE u16 NNS_G2dGetAnimSequenceIndex
( 
    const NNSG2dAnimBankData*     pAnimBank,
    const NNSG2dAnimSequenceData* pAnimSeq 
)
{
    return (u16)(
        ((u32)pAnimSeq - (u32)pAnimBank->pSequenceArrayHead) 
        / (u32)sizeof( NNSG2dAnimSequenceData ) );
}
#else
// NNS_G2dGetAnimSequenceIndex() �� VC���ł͌x������������R�[�h�Ȃ̂ŁA��`���܂���B
#endif

//------------------------------------------------------------------------------
// NNSG2dAnimBankData ���� NNSG2dUserExAnimAttrBank ���擾���܂�
// �擾�Ɏ��s�����ꍇ�́ANULL���Ԃ�܂��B
// ���[�U�g���A�g���r���[�g���܂ރf�[�^���o�͂��邽�߂ɂ́Ag2dcvtr.exe 2.8 �ȍ~��
// ���p���A-oua �I�v�V�������w�肵�ăf�[�^���o�͂���K�v������܂��B
// 
NNS_G2D_INLINE const NNSG2dUserExAnimAttrBank* 
NNS_G2dGetUserExAnimAttrBank( const NNSG2dAnimBankData* pAnimBank )
{
    // �u���b�N���擾���܂�
    const NNSG2dUserExDataBlock* pBlk 
        = NNSi_G2dGetUserExDataBlkByID( pAnimBank->pExtendedData,
                                        NNS_G2D_USEREXBLK_ANMATTR );
    // �u���b�N�̎擾�ɐ���������...
    if( pBlk != NULL )
    {
        return (const NNSG2dUserExAnimAttrBank*)(pBlk + 1);
    }else{
        return NULL;                                
    }
}

//------------------------------------------------------------------------------
// �ԍ����w�肵�ăA�j���[�V�����V�[�P���X�̊g���A�g���r���[�g�����擾���܂�
// �s���Ȕԍ����w�肳�ꂽ�ꍇ�́ANULL��Ԃ��܂��B
//
NNS_G2D_INLINE const NNSG2dUserExAnimSequenceAttr* 
NNS_G2dGetUserExAnimSequenceAttr
( 
    const NNSG2dUserExAnimAttrBank* pAnimAttrBank,
    u16                             idx 
)
{
    NNS_G2D_NULL_ASSERT( pAnimAttrBank );
    
    if( idx < pAnimAttrBank->numSequences )
    {
        return &pAnimAttrBank->pAnmSeqAttrArray[idx];
    }else{
        return NULL;
    }
}

//------------------------------------------------------------------------------
// �ԍ����w�肵�ăA�j���[�V�����V�[�P���X�̊g���A�g���r���[�g�����擾���܂�
// �s���Ȕԍ����w�肳�ꂽ�ꍇ�́ANULL��Ԃ��܂��B
//
NNS_G2D_INLINE const NNSG2dUserExAnimFrameAttr* 
NNS_G2dGetUserExAnimFrameAttr
( 
    const NNSG2dUserExAnimSequenceAttr* pAnimSeqAttr,
    u16                                 idx 
)
{
    NNS_G2D_NULL_ASSERT( pAnimSeqAttr );
    
    if( pAnimSeqAttr->numFrames )
    {
        return &pAnimSeqAttr->pAnmFrmAttrArray[idx];            
    }else{
        return NULL;
    }   
}
//------------------------------------------------------------------------------
// �A�j���[�V�����V�[�P���X��
// �A�g���r���[�g�l���擾���܂�
NNS_G2D_INLINE u32
NNS_G2dGetUserExAnimSeqAttrValue
( 
    const NNSG2dUserExAnimSequenceAttr* pAnimSeqAttr
)
{
    NNS_G2D_NULL_ASSERT( pAnimSeqAttr );
    return pAnimSeqAttr->pAttr[0];
}

//------------------------------------------------------------------------------
// �A�j���[�V�����t���[����
// �A�g���r���[�g�l���擾���܂�
NNS_G2D_INLINE u32
NNS_G2dGetUserExAnimFrmAttrValue
( 
    const NNSG2dUserExAnimFrameAttr* pFrmAttr
)
{
    NNS_G2D_NULL_ASSERT( pFrmAttr );
    return pFrmAttr->pAttr[0];
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_G2D_ANIM_DATA_H_

