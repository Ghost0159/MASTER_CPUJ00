/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d
  File:     g2d_MultiCellAnimation.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.22 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_G2D_MULTICELLANIMATION_H_
#define NNS_G2D_MULTICELLANIMATION_H_

#include <nitro.h>
#include <nnsys/g2d/fmt/g2d_MultiCell_data.h>

#include <nnsys/g2d/g2d_Node.h>
#include <nnsys/g2d/g2d_Animation.h>
#include <nnsys/g2d/g2d_CellAnimation.h>

#ifdef __cplusplus
extern "C" {
#endif


//------------------------------------------------------------------------------
// �܂����������t�H�[�}�b�g�ł����A�킩��₷���̂��߂ɕʖ����`���܂��B
typedef NNSG2dAnimSequence            NNSG2dMultiCellAnimSequence;
typedef NNSG2dAnimBankData            NNSG2dMultiCellAnimBankData;


//
// �}���`�Z�����̂��ׂẴZ���A�j���[�V�����𑖍����Ȃ���Ăяo�����֐�
// �R�[���o�b�N�Ăяo���𒆒f����ꍇ��FALSE��Ԃ��܂��B
//
typedef BOOL (*NNSG2dMCTraverseCellAnimCallBack)
( 
    u32                   userParamater,
    NNSG2dCellAnimation*  pCellAnim, 
    u16                   cellAnimIdx 
);    
//
// �}���`�Z�����̂��ׂẴZ���A�j���[�V�����𑖍����Ȃ���Ăяo�����֐�
// �R�[���o�b�N�Ăяo���𒆒f����ꍇ��FALSE��Ԃ��܂��B
//
typedef BOOL (*NNSG2dMCTraverseNodeCallBack)
( 
    u32                                 userParamater,
    const NNSG2dMultiCellHierarchyData* pNodeData,
    NNSG2dCellAnimation*                pCellAnim, 
    u16                                 nodeIdx 
);    


//
// �������s�����֐��̕ʖ�
// �݊����ێ��̂��ߕʖ��Ƃ��ĈȑO�̊֐���錾���܂��B
// 
#define NNS_G2dInitializeMCAnimation       NNS_G2dInitMCAnimation
#define NNS_G2dInitializeMCInstance        NNS_G2dInitMCInstance
#define NNS_G2dSetMCAnimSpeed              NNS_G2dSetMCAnimationSpeed

typedef enum 
{
    NNS_G2D_MCTYPE_DONOT_SHARE_CELLANIM,
    NNS_G2D_MCTYPE_SHARE_CELLANIM
    
}NNSG2dMCType;

typedef struct NNSG2dMCNodeArray
{
    NNSG2dNode*                         pNodeArray;         // Node �z��
    u16                                 numNode;            // Node ��
    u16                                 pad16_;             // �p�f�B���O
    
}NNSG2dMCNodeArray;

typedef struct NNSG2dMCCellAnimation
{
    NNSG2dCellAnimation     cellAnim;
    BOOL                    bInited;
    
}NNSG2dMCCellAnimation;


typedef struct NNSG2dMCNodeCellAnimArray
{
    NNSG2dMCCellAnimation*       cellAnimArray;
    
}NNSG2dMCNodeCellAnimArray;

//------------------------------------------------------------------------------
// ���ӁFpCurrentMultiCell �� 
//       ( numNode > pCurrentMultiCell->numNodes ) �𖞂����K�v������܂��B
//       �����A�قƂ�ǂ̏ꍇ�� numNode == pCurrentMultiCell->numNodes �ł��傤
//
//       �ȑO �� �o�[�W������ ComposedObj �̑�֊T�O�ł�
typedef struct NNSG2dMultiCellInstance 
{
    const NNSG2dMultiCellData*          pCurrentMultiCell;  // �}���`�Z���f�[�^
    
    const NNSG2dCellAnimBankData*       pAnimDataBank;      // MultiCell���\������ 
                                                            // NNSG2dCellAnimation����`����Ă��� 
                                                            // NNSG2dAnimBankData
    
    //
    // ��������������ɂ���ē����̃f�[�^���̂̍\�������ƂȂ�
    // NNSG2dMCNodeArray �� �������� NNSG2dMCNodeCellAnimArray�ƂȂ�
    //
    //
    NNSG2dMCType                        mcType;
    void*                               pCellAnimInstasnces;
    
    /*
    NNSG2dNode*                         pNodeArray;         // Node �z��
    u16                                 numNode;            // Node ��
    u16                                 pad16_;             // �p�f�B���O
    */
    
}NNSG2dMultiCellInstance;



//------------------------------------------------------------------------------
// animCtrl �� ���ʂ� NNSG2dMultiCellData�ւ�Index���������܂��B
// ���C�u�����̓f�[�^�x�[�X���炻��Index���L�[�Ƃ��āANNSG2dMultiCellData�ւ̃|�C���^���擾���܂��B
// ���̌�A�擾���� NNSG2dMultiCellData �� NNSG2dMultiCellInstance �ɐݒ肵�܂��B
typedef struct NNSG2dMultiCellAnimation
{
    NNSG2dAnimController              animCtrl;             // �A�j���[�V�����R���g���[��
    
    u16                               totalVideoFrame;      // ���r�f�I�t���[��
    u16                               pad16;                // �p�f�B���O
    
    NNSG2dMultiCellInstance           multiCellInstance;    // �}���`�Z������
    
    const NNSG2dMultiCellDataBank*    pMultiCellDataBank;   // �}���`�Z���f�[�^�o���N
    
    NNSG2dSRTControl                  srtCtrl;              // SRT �A�j���[�V��������
    
    
    // TODO:Bounding volume
}NNSG2dMultiCellAnimation;




//------------------------------------------------------------------------------
// �������֘A
//------------------------------------------------------------------------------


//------------------ NEW ------------------------------
void NNS_G2dInitMCAnimationInstance
( 
    NNSG2dMultiCellAnimation*          pMultiCellAnim,  
    void*                              pWork, 
    const NNSG2dCellAnimBankData*      pAnimBank,  
    const NNSG2dCellDataBank*          pCellDataBank,
    const NNSG2dMultiCellDataBank*     pMultiCellDataBank,
    NNSG2dMCType                       mcType
);




//------------------------------------------------------------------------------
void NNS_G2dSetAnimSequenceToMCAnimation
( 
    NNSG2dMultiCellAnimation*             pMultiCellAnim, 
    const NNSG2dMultiCellAnimSequence*    pAnimSeq 
);
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
u16 NNS_G2dGetMCNumNodesRequired
( 
    const NNSG2dMultiCellAnimSequence*    pMultiCellSeq, 
    const NNSG2dMultiCellDataBank*        pMultiCellDataBank 
);
u16 NNS_G2dGetMCBankNumNodesRequired
( 
    const NNSG2dMultiCellDataBank*       pMultiCellDataBank 
);
u32 NNS_G2dGetMCWorkAreaSize
(
    const NNSG2dMultiCellDataBank*       pMultiCellDataBank,
    NNSG2dMCType                         mcType
);

//------------------------------------------------------------------------------
// �A�j���[�V�����֘A
//------------------------------------------------------------------------------
void NNS_G2dTickMCInstance( NNSG2dMultiCellInstance* pMultiCellAnim, fx32 frames );
void NNS_G2dTickMCAnimation( NNSG2dMultiCellAnimation* pMultiCellAnim, fx32 frames );
//------------------------------------------------------------------------------
void NNS_G2dSetMCAnimationCurrentFrame
( 
    NNSG2dMultiCellAnimation*   pMultiCellAnim, 
    u16                         frameIndex 
);

//------------------------------------------------------------------------------
// �}���`�Z�����̃Z���A�j���[�V�����̃A�j���[�V�����t���[����ݒ肷��
void NNS_G2dSetMCAnimationCellAnimFrame
( 
    NNSG2dMultiCellAnimation*   pMultiCellAnim, 
    u16                         caFrameIndex     
);

void NNS_G2dStartMCCellAnimationAll
(
    NNSG2dMultiCellInstance*    pMCInst 
);

void NNS_G2dRestartMCAnimation
(
    NNSG2dMultiCellAnimation*   pMultiCellAnim
);

//------------------------------------------------------------------------------
void NNS_G2dSetMCAnimationSpeed
(
    NNSG2dMultiCellAnimation*   pMultiCellAnim, 
    fx32                        speed  
);

void NNS_G2dResetMCCellAnimationAll
(
    NNSG2dMultiCellInstance*    pMCInst 
);

//------------------------------------------------------------------------------
// OAM ��� �� ����
u16 NNS_G2dMakeSimpleMultiCellToOams
( 
    GXOamAttr*                      pDstOams, 
    u16                             numDstOams,
    const NNSG2dMultiCellInstance*  pMCellInst, 
    const MtxFx22*                  pMtxSR, 
    const NNSG2dFVec2*              pBaseTrans,
    u16                             affineIndex,
    BOOL                            bDoubleAffine 
);

//------------------------------------------------------------------------------
void NNS_G2dTraverseMCCellAnims
( 
    NNSG2dMultiCellInstance*         pMCellInst,
    NNSG2dMCTraverseCellAnimCallBack pCBFunc,
    u32                              userParamater
);
//------------------------------------------------------------------------------
void NNS_G2dTraverseMCNodes
( 
    NNSG2dMultiCellInstance*        pMCellInst,
    NNSG2dMCTraverseNodeCallBack    pCBFunc,
    u32                             userParamater
);


//------------------ OLD ------------------------------
// �Â�API
// �ߋ��̃o�[�W�����Ƃ̌݊����ێ��̂��ߎc����Ă��܂��B
void NNS_G2dInitMCAnimation( 
    NNSG2dMultiCellAnimation*          pMultiCellAnim, 
    NNSG2dNode*                        pNodeArray, 
    NNSG2dCellAnimation*               pCellAnim, 
    u16                                numNode, 
    const NNSG2dCellAnimBankData*      pAnimBank,  
    const NNSG2dCellDataBank*          pCellDataBank,
    const NNSG2dMultiCellDataBank*     pMultiCellDataBank 
);
void NNS_G2dInitMCInstance
( 
    NNSG2dMultiCellInstance*      pMultiCell, 
    NNSG2dNode*                   pNodeArray, 
    NNSG2dCellAnimation*          pCellAnim, 
    u16                           numNode, 
    const NNSG2dCellAnimBankData* pAnimBank,  
    const NNSG2dCellDataBank*     pCellDataBank 
);
BOOL NNS_G2dSetMCDataToMCInstance( 
    NNSG2dMultiCellInstance*       pMCInst, 
    const NNSG2dMultiCellData*     pMcData
);
//------------------ OLD ------------------------------


//------------------------------------------------------------------------------
// inline funcs.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
NNS_G2D_INLINE NNSG2dAnimController* NNS_G2dGetMCAnimAnimCtrl
( 
    NNSG2dMultiCellAnimation* pMultiCellAnim 
)
{
    NNS_G2D_NULL_ASSERT( pMultiCellAnim );
    return &pMultiCellAnim->animCtrl;
}

//
// �������[�N�������ւ̃|�C���^���擾���܂�
//
NNS_G2D_INLINE void* NNSi_G2dGetMCInstanceWorkMemory
(
    NNSG2dMultiCellInstance*      pMultiCell
)
{
    return pMultiCell->pCellAnimInstasnces;
}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_G2D_MULTICELLANIMATION_H_

