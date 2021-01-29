/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d
  File:     g2d_Entity.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.11 $
 *---------------------------------------------------------------------------*/
 
#ifndef NNS_G2D_ENTITY2_H_
#define NNS_G2D_ENTITY2_H_

#include <nitro.h>
#include <nnsys/g2d/g2d_config.h>
#include <nnsys/g2d/g2d_Animation.h>
#include <nnsys/g2d/g2d_PaletteTable.h>


#ifdef __cplusplus
extern "C" {
#endif

//
// �������s�����֐��̕ʖ�
// �݊����ێ��̂��ߕʖ��Ƃ��ĈȑO�̊֐���錾���܂��B
// 
#define NNS_G2dInitializeEntity        NNS_G2dInitEntity
#define NNS_G2dSetCurrentAnimation     NNS_G2dSetEntityCurrentAnimation



#define NNS_G2D_ASSERT_ENTITY_VALID( entity )           \
           NNS_G2D_ASSERTMSG( NNS_G2dIsEntityValid( ( entity ) ),"A Invalid Entity instance was detected." );


/*---------------------------------------------------------------------------*
  Name:         NNSG2dEntity

  Description:  �`��p�f�[�^�ƃA�j���[�V�����f�[�^���O���[�v������T�O
                �Q�[���L�����N�^��{�\��
                
 *---------------------------------------------------------------------------*/ 
typedef struct NNSG2dEntity
{
    void*                         pDrawStuff;                 // �`��f�[�^
    const NNSG2dEntityData*       pEntityData;                // �ÓI�f�[�^�ւ̎Q��
    const NNSG2dAnimBankData*     pAnimDataBank;              // �֘A�t�����Ă��� Bank
    u16                           currentSequenceIdx;         // ���ݍĐ����̃V�[�P���X�ԍ�
    u16                           pad16_;                     // �p�f�B���O
    //
    // NULL�ł���� bPaletteChangeEnable = FALSE �Ƃ݂Ȃ�
    // �e�[�u���ނ́A�T�C�Y���傫�����A�����L�����N�^�ɋ��L����邱�Ƃ������ƍl������
    // ����ă|�C���^��ێ�����d�l�Ƃ����B
    // 
    NNSG2dPaletteSwapTable*         pPaletteTbl;
    
}NNSG2dEntity;



//------------------------------------------------------------------------------
void    NNS_G2dInitEntity      
( 
    NNSG2dEntity*               pEntity, 
    void*                       pDrawStuff, 
    const NNSG2dEntityData*     pEntityData, 
    const NNSG2dAnimBankData*   pAnimDataBank 
);
void    NNS_G2dSetEntityCurrentAnimation  ( NNSG2dEntity* pEntity, u16 idx );



//------------------------------------------------------------------------------
void NNS_G2dSetEntityPaletteTable   ( NNSG2dEntity* pEntity, NNSG2dPaletteSwapTable* pPlttTbl );
void NNS_G2dResetEntityPaletteTable ( NNSG2dEntity* pEntity );
BOOL NNS_G2dIsEntityPaletteTblEnable ( const NNSG2dEntity* pEntity );



//------------------------------------------------------------------------------
void NNS_G2dTickEntity( NNSG2dEntity*    pEntity, fx32 dt );
void NNS_G2dSetEntityCurrentFrame
( 
    NNSG2dEntity*    pEntity,
    u16              frameIndex  
);

//------------------------------------------------------------------------------
void NNS_G2dSetEntitySpeed
(
    NNSG2dEntity*     pEntity,
    fx32              speed 
);

//------------------------------------------------------------------------------
BOOL            NNS_G2dIsEntityValid( NNSG2dEntity*    pEntity );

//------------------------------------------------------------------------------
NNSG2dAnimController* NNS_G2dGetEntityAnimCtrl( NNSG2dEntity*    pEntity );


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_G2D_ENTITY_H_