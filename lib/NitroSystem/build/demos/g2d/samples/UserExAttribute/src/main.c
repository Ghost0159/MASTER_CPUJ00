/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - UserExAttribute
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      ���[�U�g���A�g���r���[�g���̗��p�̎d���������T���v���ł��B
//      �R���o�[�^�̐V�@�\�ł���A-oua �I�v�V�����𗘗p����
//      ���[�U�g���A�g���r���[�g�����o�͂��܂��B
//      
//      ���[�U�g���A�g���r���[�g���𗘗p����ɂ́A�Z���A�A�j���[�V�����t���[����
//      �΂��Ċg���R�����g��t������K�v������܂��B
//      ���̂��߁A�g���R�����g��t���ł���V�����o�[�W������NITRO=CHARACTER��
//      �f�[�^���쐬����K�v������܂��B
//
//      �{�T���v���ł́A�}���`�Z�����̃Z���A�j���[�V�������Ƃ�
//      �����̎�ނ̃A�g���r���[�g�l���擾���A���̌��ʂ��f�o�b�N������ŕ\�����܂��B
//      
//      ���ۂ̃Q�[���v���W�F�N�g�ł́A
//          ��T�E���h�Đ��̃g���K
//          ��Փ˔���T�C�Y�̊i�[
//      �ȂǐF�X�ȉ��p���l�����܂��B
//
//  ������@
//      �\���L�[�㉺    �Đ����x�ύX
//      Y               ���[�U�g���A�g���r���[�g�̎擾���@�̕ύX
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"


#define NUM_OF_OAM              128                  // OAM�}�l�[�W���Ǘ��Ώۂ�OAM��
#define NUM_OF_AFFINE           (NUM_OF_OAM/4)      // OAM�}�l�[�W���Ǘ��Ώۂ�Affine�p�����[�^��

#define ANIM_SPEED_UNIT         (FX32_ONE >> 3)     // �A�j���[�V�������x�̕ω�����

#define NUM_CELL_ANIMATION      10

//
// �A�j���[�V�����R���g���[���̃R�[���o�b�N�p�����[�^��
// �f�[�^��n���ۂɎg�p����A�f�[�^�\����
// �R���g���[���ɂ͖{�\���̂̃|�C���^��n���B
//
typedef struct MyAnimCtrlData
{
    NNSG2dAnimController*       pAnmCtrl;       // �A�j���[�V�����R���g���[��
    u16                         idx;            // �Z���A�j���[�V�����ԍ�
    u16                         pad16;          // �p�f�B���O
    BOOL                        bAttributeIsNotZero;// �[���l�łȂ��A�g���r���[�g���������ꂽ?
    
}MyAnimCtrlData;
static MyAnimCtrlData              animDataArray[NUM_CELL_ANIMATION];


// Cell �\���ʒu
static const NNSG2dFVec2           MULTICELL_POSITION = {120 << FX32_SHIFT, 150 << FX32_SHIFT};
//
// MultiCellAnimation ����
// �قȂ�2�̕����Ŏ��̂����������܂��B
// 
static NNSG2dMultiCellAnimation   mcAnim;


// OAM�A�g���r���[�g�ꎞ�o�b�t�@
static GXOamAttr                tempOamBuffer[NUM_OF_OAM];

//
// �}���`�Z���o���N
//
static NNSG2dMultiCellDataBank*        pMCBank = NULL;     // �}���`�Z���f�[�^
static NNSG2dCellDataBank*             pCellBank = NULL;   // �Z���f�[�^
static NNSG2dCellAnimBankData*         pAnimBank = NULL;   // �Z���A�j���[�V����
static NNSG2dMultiCellAnimBankData*    pMCABank = NULL;    // �}���`�Z���A�j���[�V����

//
// ���[�U�g���A�g���r���[�g�֘A
//
static const NNSG2dUserExCellAttrBank*  pCellAttrBank = NULL;
static const NNSG2dUserExAnimAttrBank*  pAnimAttrBank = NULL;

//
// �g���A�g���r���[�g�̎擾���@
//
typedef enum MY_GET_ATTRIBUTE_MODE
{
    MY_GET_ATTRIBUTE_MODE_CELL,    // �Z���g���A�g���r���[�g���Q�Ƃ��܂�
    MY_GET_ATTRIBUTE_MODE_FRAME,   // �A�j���[�V�����t���[��
                                   // �g���A�g���r���[�g���Q�Ƃ��܂�
    MY_GET_ATTRIBUTE_MODE_MAX

}MY_GET_ATTRIBUTE_MODE;
   
static MY_GET_ATTRIBUTE_MODE    attributeMode = MY_GET_ATTRIBUTE_MODE_CELL;


//------------------------------------------------------------------------------
// �v���g�^�C�v�錾

static void InitOamManager(NNSG2dOamManagerInstance* pOamManager);
static void LoadResources();
static void ProcessInput(fx32* pAnimSpeed, NNSG2dAnimController* pAnimCtrl);
void VBlankIntr(void);



/*---------------------------------------------------------------------------*
  Name:         InitOamManager

  Description:  OAM�}�l�[�W���V�X�e����OAM�}�l�[�W���̏��������s���܂��B

  Arguments:    pOamManager:    ����������OAM�}�l�[�W���ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitOamManager(NNSG2dOamManagerInstance* pOamManager)
{
    BOOL    result;

    // Oam Manager �V�X�e���̏�����
    NNS_G2dInitOamManagerModule();

    result = NNS_G2dGetNewManagerInstance(
                pOamManager,
                0,
                NUM_OF_OAM - 1,
                NNS_G2D_OAMTYPE_MAIN );
    SDK_ASSERT( result );

    result = NNS_G2dInitManagerInstanceAffine( pOamManager, 0, NUM_OF_AFFINE - 1 );
    SDK_ASSERT( result );
}



/*---------------------------------------------------------------------------*
  Name:         InitMultiCellInstance_

  Description:  �}���`�Z�����̂����������܂��B
  
  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitMultiCellInstance_
(
    NNSG2dMultiCellAnimation*       pMCAnim,
    NNSG2dCellAnimBankData*         pAnimBank,
    NNSG2dCellDataBank*             pCellBank,
    NNSG2dMultiCellDataBank*        pMCBank,
    NNSG2dMultiCellAnimBankData*    pMCABank,
    NNSG2dMCType                    mcType
)
{
    // �������ɕK�v�ȃ��[�N�������̃T�C�Y���擾���܂�
    const u32 szWork = NNS_G2dGetMCWorkAreaSize( pMCBank, mcType );
    void* pWorkMem   = G2DDemo_Alloc( szWork );
    
    // ������
    NNS_G2dInitMCAnimationInstance( pMCAnim, 
                                    pWorkMem, 
                                    pAnimBank, 
                                    pCellBank, 
                                    pMCBank, 
                                    mcType );
    
    SDK_NULL_ASSERT( pMCAnim );
    
    // �}���`�Z���A�j���[�V�����ɍĐ�����V�[�P���X���Z�b�g
    {
        const NNSG2dMultiCellAnimSequence* pSequence = NULL;    
        // �Đ�����V�[�P���X���擾
        pSequence = NNS_G2dGetAnimSequenceByIdx( pMCABank, 0 );
        SDK_ASSERT( pSequence );

        NNS_G2dSetAnimSequenceToMCAnimation( pMCAnim, pSequence);
    }
}

//------------------------------------------------------------------------------
// �}���`�Z�����̃Z���A�j���[�V�����ɐݒ肳���R�[���o�b�N�֐�
// �A�v���P�[�V�����̓��샂�[�h�ɂ���������
// ���[�U�g���A�g���r���[�g���擾���A
// �A�g���r���[�g�̒l���K��l�ȊO���ǂ������f�[�^�Ƃ��ĕۑ����܂��B
//
static void CellAnimCallBackFunction( u32 param, fx32 currentFrame )
{
#pragma unused(currentFrame)
    
    MyAnimCtrlData*             pAnmCtrlData    = (MyAnimCtrlData*)param;
    const NNSG2dAnimController* pAnmCtrl        = pAnmCtrlData->pAnmCtrl;
     
    u32 attrVal;
    
    //
    // �A�g���r���[�g�擾���@�ɂ����...
    //
    switch( attributeMode )
    {
    // ----- �Z���g���A�g���r���[�g���Q��
    case MY_GET_ATTRIBUTE_MODE_CELL:
        {
            // ���݂̃Z���ԍ����擾
            const u16 cellIdx = NNS_G2dGetAnimCtrlCurrentElemIdxVal( pAnmCtrl );
            // �Z���ԍ��ɑΉ�����A�g���r���[�g���擾
            const NNSG2dUserExCellAttr* pAttr 
                = NNS_G2dGetUserExCellAttr( pCellAttrBank, cellIdx );
            attrVal = NNS_G2dGetUserExCellAttrValue( pAttr );
            
            break;
        }
    // ----- �A�j���[�V�����t���[���g���A�g���r���[�g���Q��
    case MY_GET_ATTRIBUTE_MODE_FRAME:
        {
            // ���݂̃V�[�P���X�ԍ����擾
            const NNSG2dAnimSequence* pSeq 
                = NNS_G2dGetAnimCtrlCurrentAnimSequence( pAnmCtrl );
            const u16 seqIdx 
                = NNS_G2dGetAnimSequenceIndex( pAnimBank, pSeq );
            // �A�j���[�V�����V�[�P���X�ɑΉ������V�[�P���X�g���A�g���r���[�g���擾
            // �i�{�T���v���ł͗��p���܂��񂪁A�V�[�P���X�g���A�g���r���[�g�����
            // �A�g���r���[�g�l���擾�\�ł��B�j
            const NNSG2dUserExAnimSequenceAttr* pSeqAttr 
                = NNS_G2dGetUserExAnimSequenceAttr( pAnimAttrBank, seqIdx );
                
            // �Đ����̃A�j���[�V�����t���[���ԍ����擾
            const u16 currenFrmIdx = NNS_G2dGetAnimCtrlCurrentFrame( pAnmCtrl );
            // �V�[�P���X�g���A�g���r���[�g�ƃt���[���ԍ�����
            // �t���[���g���A�g���r���[�g���擾
            const NNSG2dUserExAnimFrameAttr*  pFrmAttr 
                = NNS_G2dGetUserExAnimFrameAttr( pSeqAttr, currenFrmIdx );                                                        
            attrVal = NNS_G2dGetUserExAnimFrmAttrValue( pFrmAttr );
            
            break;
        }
    }
        
    //
    // �擾�����A�g���r���[�g�l���[���ł͂Ȃ��Ȃ�
    // ���̂��Ƃ��L�^���Ă���
    //
    if( attrVal != 0 )
    { 
        pAnmCtrlData->bAttributeIsNotZero = TRUE;
    }else{
        pAnmCtrlData->bAttributeIsNotZero = FALSE;
    }
}

//------------------------------------------------------------------------------
// 
/*---------------------------------------------------------------------------*
  Name:         SetupMCCellAnimCallBackFunc

  Description:  NNS_G2dTraverseMCCellAnims()(�}���`�Z������֐�)
                �Ƀp�����[�^�Ƃ��ēn�����A�R�[���o�b�N�֐�
                NNS_G2dTraverseMCCellAnims()���Ŋe�Z���A�j���[�V�����ɂ���
                �Ăяo����܂��B
                
                �{�֐����ŁA�Z���A�j���[�V�����̎��A�j���[�V�����R���g���[����
                �R�[���o�b�N�֐���ݒ肵�Ă��܂��B
                 

  Arguments:    userParamaters: ���[�U�p�����[�^
                                �i�{�T���v���ł́AanimDataArray�ւ̃|�C���^���������Ă��܂��j
                pCellAnim:      �}���`�Z���A�j���[�V�������̃Z���A�j���[�V����
                idx�F           �Z���A�j���[�V�����ԍ�

  Returns:      �R�[���o�b�N�Ăяo���𒆒f���邩�H
                �R�[���o�b�N�Ăяo���𒆒f�������ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL SetupMCCellAnimCallBackFunc
(
    u32                  userParamaters,
    NNSG2dCellAnimation* pCellAnim, 
    u16                  idx 
)
{
#pragma unused( userParamaters )     
    
    //
    // �{�T���v���f���ł́AuserParamaters�Ƃ��āA
    // animDataArray�ւ̃|�C���^���������Ă��܂��B
    //
    MyAnimCtrlData* pAnmDataArray = (MyAnimCtrlData*)userParamaters;
    
    // �Z���A�j���[�V��������A�j���[�V�����R���g���[����
    // �擾���܂�
    NNSG2dAnimController* pAnmCtrl 
        = NNS_G2dGetCellAnimationAnimCtrl( pCellAnim );

    // �A�j���[�V�����R���g���[���̃t�@���N�^�����������܂�    
    NNS_G2dInitAnimCtrlCallBackFunctor( pAnmCtrl );
    
    // �R�[���o�b�N�t�@���N�^�̃p�����[�^
    // �Ƃ��ė��p����郆�[�U��`�̍\���̂Ƀf�[�^���i�[���܂��B
    pAnmDataArray[idx].pAnmCtrl = pAnmCtrl;
    pAnmDataArray[idx].idx      = idx;
    
    // �A�j���[�V�����R���g���[���̃t�@���N�^�Ƀp�����[�^
    // �ƃR�[���o�b�N�֐���ݒ肵�܂�
    NNS_G2dSetAnimCtrlCallBackFunctor
    ( 
        pAnmCtrl,
        NNS_G2D_ANMCALLBACKTYPE_EVER_FRM,
        (u32)&pAnmDataArray[idx],
        CellAnimCallBackFunction 
    );
    
    return TRUE;    
} 

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  �}���`�Z���A�j���[�V�����̍\�z�ƃL�����N�^�f�[�^�A
                �p���b�g�f�[�^�̓ǂݍ��݂��s���܂��B

  Arguments:    ppMCAnim:   �\�z�����}���`�Z���A�j���[�V�����ւ̃|�C���^��
                            �󂯎��|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadResources()
{
    void* pBuf;

    //
    // MultiCell �֘A������
    //
    {
        
        // �Z���f�[�^�A�Z���A�j���[�V�����A�}���`�Z���f�[�^�A
        // �}���`�Z���A�j���[�V���������[�h�B
        // �����͍Ō�܂Ń��C����������Ŏg�p����̂� pBuf ��������܂���B
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/MultiCellAnimation.NCER" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNANR( &pAnimBank, "data/MultiCellAnimation.NANR" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNMCR( &pMCBank, "data/MultiCellAnimation.NMCR" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNMAR( &pMCABank, "data/MultiCellAnimation.NMAR" );
        SDK_NULL_ASSERT( pBuf );

        
        //
        // �g���A�g���r���[�g�o���N���擾���܂�
        // �R���o�[�^�I�v�V���� -oua ���w�肵�ăf�[�^���R���o�[�g����
        // �K�v������܂��B
        //
        // �����ł́A�Z���o���N�A�A�j���o���N����擾���s���Ă��܂����A
        // ���ɁA�}���`�Z���o���N�A�}���`�Z���A�j���o���N�A��������l
        // �̕��@�Ńf�[�^���擾�\�ł��B
        //
        // �Z���g���A�g���r���[�g�o���N���擾���܂�
        pCellAttrBank = NNS_G2dGetUserExCellAttrBankFromCellBank( pCellBank ); 
        // �A�j���g���A�g���r���[�g�o���N���擾���܂�
        pAnimAttrBank = NNS_G2dGetUserExAnimAttrBank( pAnimBank );

        //
        // �}���`�Z���A�j���[�V�����̎��̂����������܂�
        // �قȂ������2�̎��̂����������܂��B
        //
        InitMultiCellInstance_( &mcAnim,
                                pAnimBank,
                                pCellBank,
                                pMCBank,
                                pMCABank,
                                NNS_G2D_MCTYPE_SHARE_CELLANIM  );
        
        //
        // �}���`�Z�����\������Z���A�j���[�V����������
        // �e�Z���A�j���[�V�����ɑ΂���
        // ���[�U�ŗL�̏��������s���R�[���o�b�N�֐����Ăяo���܂��B
        //
        // ���[�U�p�����[�^�Ƃ��āAanimDataArray�ւ̃|�C���^��n���Ă��܂��B
        // ���̃|�C���^�𗘗p���āA�R�[���o�b�N������animDataArray�̃f�[�^�����������Ă��܂��B
        NNS_G2dTraverseMCCellAnims( &mcAnim.multiCellInstance,
                                    SetupMCCellAnimCallBackFunc,
                                    (u32)(&animDataArray) );
    }

    //
    // VRAM �֘A�̏�����
    //
    {
        // Loading NCG (Character Data)
        {
            NNSG2dCharacterData*        pCharData = NULL;

            pBuf = G2DDemo_LoadNCGR( &pCharData, "data/MultiCellAnimation.NCGR" );
            SDK_NULL_ASSERT( pBuf );

            // Loading For 2D Graphics Engine.
            DC_FlushRange( pCharData->pRawData, pCharData->szByte );
            GX_LoadOBJ( (void*)pCharData->pRawData, 0, pCharData->szByte );

            // �L�����N�^�f�[�^�� VRAM �ɃR�s�[�����̂�
            // ���� pBuf �͊J�����܂��B�ȉ������B
            G2DDemo_Free( pBuf );
        }

        // Loading NCL (Palette Data)
        {
            NNSG2dPaletteData*        pPltData = NULL;

            pBuf = G2DDemo_LoadNCLR( &pPltData, "data/MultiCellAnimation.NCLR" );
            SDK_NULL_ASSERT( pBuf );

            // Loading For 2D Graphics Engine.
            DC_FlushRange( pPltData->pRawData, pPltData->szByte );
            GX_LoadOBJPltt( (void*)pPltData->pRawData, 0, pPltData->szByte );

            G2DDemo_Free( pBuf );
        }
    }
}


//--------------------------------------------------------------------------------------------------------
//�}���`�Z���̕`��
static void AppDraw
( 
    const NNSG2dMultiCellAnimation* pMcAnim, 
    NNSG2dOamManagerInstance*       pOamMgr 
)
{
    // �`��
    //
    // �჌�x���ȕ`����@�� MultiCell ��`�悵�܂��B
    // ��ʃ��W���[���� G2dRenderer �� ���p����`����@������܂��B
    //
    //
    {
        u16                             numOamDrawn = 0;                  
        
        // �\������ MultiCell
        const NNSG2dMultiCellInstance*  pMultiCell = &(pMcAnim->multiCellInstance); 
        SDK_NULL_ASSERT( pMultiCell );

        //
        // MultiCell �Ɠ����� OBJ���X�g���e���|�����ɏ����o���܂�
        //
        numOamDrawn = NNS_G2dMakeSimpleMultiCellToOams(
                            tempOamBuffer,         // �o�͐�Oam�o�b�t�@
                            NUM_OF_OAM,            // �o�͐�o�b�t�@��
                            pMultiCell,            // �o�͑Ώ� MultiCell
                            NULL,                  // Affine �ϊ�
                            &MULTICELL_POSITION,   // �I�t�Z�b�g�ʒu
                            NULL,                  // Affine Index
                            FALSE );               // �{�pAffine���H

        SDK_ASSERT( numOamDrawn < NUM_OF_OAM );

        // OAM�}�l�[�W����OBJ���X�g��o�^���܂�
        (void)NNS_G2dEntryOamManagerOam( pOamMgr, tempOamBuffer, numOamDrawn );
    }
}



   

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C���֐��ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    NNSG2dOamManagerInstance    oamManager;     // Oam �}�l�[�W������
    
        
    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_PrintInit();
        InitOamManager( &oamManager );
        LoadResources();
        
        // start display
        {
            SVC_WaitVBlankIntr();
            GX_DispOn();
            GXS_DispOn();
        }
    }

    //----------------------------------------------------
    // Main loop
    while( TRUE )
    {
        static fx32                    animSpeed    = FX32_ONE; // �A�j���[�V�����X�s�[�h
        NNSG2dMultiCellAnimation*      pMcAnim      = &mcAnim;
        
        //
        //  ------ Pad handling.
        //
        {
            G2DDemo_ReadGamePad();

            // change animation speed.
            {
                if (G2DDEMO_IS_TRIGGER(PAD_KEY_UP))
                {
                    animSpeed +=ANIM_SPEED_UNIT;
                }

                if (G2DDEMO_IS_TRIGGER(PAD_KEY_DOWN))
                {
                    animSpeed -= ANIM_SPEED_UNIT;

                    if( animSpeed < 0 )
                    {
                        animSpeed = 0;
                    }
                }
            }
            
            // ---- �g���A�g���r���[�g�̎擾���@��ύX���܂�
            if (G2DDEMO_IS_TRIGGER(PAD_BUTTON_Y ))
            {
                attributeMode 
                    = (MY_GET_ATTRIBUTE_MODE)((++attributeMode) % MY_GET_ATTRIBUTE_MODE_MAX);
            }
        }

        //  ------ �`��
        AppDraw( pMcAnim, &oamManager );
        //  ------ �A�j���[�V�����̍X�V
        NNS_G2dTickMCAnimation( &mcAnim, animSpeed );
        
        
        
        // ------ �f�o�b�N�������`�悵�܂�
        {
            const NNSG2dAnimController* pAnimCtrl = NNS_G2dGetMCAnimAnimCtrl(pMcAnim);

            G2DDemo_PrintOutf(0, 0, "speed: %7.3f", (float)animSpeed / FX32_ONE );
            G2DDemo_PrintOutf(0, 1, "frame: %3d / %3d",
                pAnimCtrl->pCurrent - pAnimCtrl->pAnimSequence->pAnmFrameArray,
                pAnimCtrl->pAnimSequence->numFrames );
            
            //
            // �Z���A�j���[�V������
            // ��������K��l�[���ȊO�̊g���A�g���r���[�g��F�����Ă���ꍇ��
            // o �� �\�����A����ȊO�̏ꍇ�� x ��\�����܂��B
            //
            {
                int i;
                G2DDemo_PrintOutf( 0, 3,      "-----------------" );
                G2DDemo_PrintOutf( 0, 4,      "attribute " );
                G2DDemo_PrintOutf( 0, 5,      "-----------------" );
                
                for( i = 0; i < NUM_CELL_ANIMATION; i++ )
                {
                    if( animDataArray[i].bAttributeIsNotZero == TRUE )
                    {
                        G2DDemo_PrintOutf( 0, 6+i, "%02d:o", i );
                    }else{
                        G2DDemo_PrintOutf( 0, 6+i, "%02d:x", i );
                    }
                }
                G2DDemo_PrintOutf( 0, 6 + NUM_CELL_ANIMATION, "----- " );
            }
        }

        // ------ V Blank �҂�
        SVC_WaitVBlankIntr();
        G2DDemo_PrintApplyToHW();   // �f�o�b�N���������HW�ɏ����o���܂�

        // ------ �}�l�[�W���̓��e��HW�ɏ����o���܂�
        {
            NNS_G2dApplyOamManagerToHW          ( &oamManager );
            NNS_G2dResetOamManagerBuffer        ( &oamManager );
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V�u�����N���荞�݂��������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );                   // checking VBlank interrupt
}

