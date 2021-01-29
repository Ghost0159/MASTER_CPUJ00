/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - CellAnimation
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.17 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      �Z���A�j���[�V������`��A�X�V����T���v��
//
//  ������@
//      �\���L�[�㉺    �Đ����x�ύX
//      �\���L�[���E    �Đ��V�[�P���X�ύX
//      A               �Đ�<=>��~�؂�ւ�
//      B               ������<=>�t�����Đ��؂�ւ�
//      X               �R�[���o�b�N�^�C�v�؂�ւ�
//      Y               �Đ��̍ăX�^�[�g
//  ����
//      �Z���A�j���[�V�������Đ����܂��B�܂��A���܂��܂ȍĐ�����̋@�\�A
//      �R�[���o�b�N�֐��Ăяo���@�\�������܂��B
// ============================================================================


#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"


//------------------------------------------------------------------------------
// �e��萔
#define NUM_OF_OAM              48                      // OAM�}�l�[�W���Ǘ��Ώۂ�OAM��

#define ANIM_SPEED_UNIT         (FX32_ONE >> 3)         // �A�j���[�V�������x�̕ω�����

#define CALLBACK_PARAM          0x12345678              // �R�[���o�b�N�֐��̃p�����[�^
#define CALL_CALLBACK_FRAME     3                       // NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM �̎���
                                                        // �R�[���o�b�N�֐����Ăяo���t���[���ԍ�
                                                        
#define TEX_BASE                0x0                     // �e�N�X�`���x�[�X�A�h���X
#define TEX_PLTT_BASE           0x0                     // �e�N�X�`���p���b�g�x�[�X�A�h���X

//------------------------------------------------------------------------------
// �O���[�o�������o
static NNSG2dOamManagerInstance objOamManager;          // OBJ �o�͗p OAM �}�l�[�W��

static NNSG2dImageProxy         sImageProxy;            // Cell �p�L�����N�^/�e�N�X�`���v���L�V
static NNSG2dImagePaletteProxy  sPaletteProxy;          // Cell �p�p���b�g�v���L�V

// �R�[���o�b�N�^�C�v��
static const char* CALL_BACK_TYPE_NAME[] = {
    "NONE    ",     // NNS_G2D_ANMCALLBACKTYPE_NONE
    "LAST_FRM",     // NNS_G2D_ANMCALLBACKTYPE_LAST_FRM
    "SPEC_FRM",     // NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM
    "EVER_FRM",     // NNS_G2D_ANMCALLBACKTYPE_EVER_FRM
};

// Cell �\���ʒu
static const NNSG2dFVec2        CELL_POSITION = {120 << FX32_SHIFT, 120 << FX32_SHIFT};

// �A�j���[�V�����R�[���o�b�N���Ăяo���ꂽ��
static int                      callCount = 0;

// �A�j���[�V�����R�[���o�b�N�̎��
static NNSG2dAnmCallbackType    callBackType = NNS_G2D_ANMCALLBACKTYPE_NONE;


/*---------------------------------------------------------------------------*
  Name:         CallBackFunction

  Description:  �A�j���[�V�����R���g���[������R�[���o�b�N�����֐��ł��B

  Arguments:    param:          �R�[���o�b�N�֐��o�^���Ɏw�肵��
                                ���[�U�p�����[�^�ł��B
                currentFrame:   �R�[���o�b�N�^�C�v��
                                NNS_G2D_ANMCALLBACKTYPE_LAST_FRM �̏ꍇ��
                                �A�j���[�V�����R���g���[���� currentTime ��
                                fx32 �^�œn����܂��B
                                ����ȊO�ł� u16 �^�̌��݂̃t���[���ԍ���
                                �n����܂��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void CallBackFunction( u32 param, fx32 currentFrame )
{
#pragma unused( param )
#pragma unused( currentFrame )

    // �Ă΂ꂽ�񐔂��X�V
    callCount++;
    
    OS_Printf("CallBackFunction is called: param=0x%p, frame=0x%p\n", param, currentFrame);
}

/*---------------------------------------------------------------------------*
  Name:         InitOamManager

  Description:  OAM �}�l�[�W���V�X�e�������������܂��B

  Arguments:    pObjManager:    OBJ �o�͗p�ɏ���������OAM�}�l�[�W���ւ�
                                �|�C���^�B
  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitOamManager( NNSG2dOamManagerInstance* pObjManager )
{
    BOOL    result;

    // Oam Manager �V�X�e���̏�����
    NNS_G2dInitOamManagerModule();

    
    result = NNS_G2dGetNewOamManagerInstanceAsFastTransferMode( pObjManager, 
                                                                0, 
                                                                NUM_OF_OAM, 
                                                                NNS_G2D_OAMTYPE_MAIN );    
    SDK_ASSERT( result );
}

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  �ȉ��̏��������s���܂��B
                  �Z���A�j���[�V�����f�[�^�̓ǂݍ���
                  �Z���A�j���[�V�����̏�����
                  �L�����N�^�f�[�^�A�p���b�g�f�[�^�̓ǂݍ���

  Arguments:    ppAnimBank: �Z���A�j���[�V�����f�[�^�ւ̃|�C���^��
                            �󂯎��|�C���^�B
                ppCellAnim: �Z���A�j���[�V�����ւ̃|�C���^���󂯎��|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadResources(NNSG2dAnimBankData** ppAnimBank, NNSG2dCellAnimation** ppCellAnim)
{
    void* pBuf;

    NNS_G2dInitImageProxy( &sImageProxy );
    NNS_G2dInitImagePaletteProxy( &sPaletteProxy );

    //
    // Cell �֘A������
    //
    {
        NNSG2dCellDataBank*          pCellBank = NULL;

        // �Z���f�[�^����уZ���A�j���[�V�����f�[�^�����[�h�B
        // �Z���f�[�^����уZ���A�j���[�V�����f�[�^�͍Ō�܂�
        // ���C����������Ŏg�p����̂ł����� pBuf �͉�����܂���B
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/CellAnimation.NCER" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNANR( ppAnimBank, "data/CellAnimation.NANR" );
        SDK_NULL_ASSERT( pBuf );

        //
        // �Z���A�j���[�V�����̎��̂����������܂�
        //
        {
            *ppCellAnim = G2DDemo_GetNewCellAnimation(1);
            SDK_NULL_ASSERT( *ppCellAnim );

            SDK_NULL_ASSERT( NNS_G2dGetAnimSequenceByIdx(*ppAnimBank, 0) );

            NNS_G2dInitCellAnimation(
                *ppCellAnim,
                NNS_G2dGetAnimSequenceByIdx(*ppAnimBank, 0),
                pCellBank );
        }
    }

    //
    // VRAM �֘A�̏�����
    //
    {
        //------------------------------------------------------------------------------
        // load character data for 2D
        {
            NNSG2dCharacterData*        pCharData;

            pBuf = G2DDemo_LoadNCGR( &pCharData, "data/CellAnimation.NCGR" );
            SDK_NULL_ASSERT( pBuf );

            // Loading For 2D Graphics Engine.
            DC_FlushRange( pCharData->pRawData, pCharData->szByte );
            GX_LoadOBJ( (void*)pCharData->pRawData, TEX_BASE, pCharData->szByte );
            
            // �L�����N�^�f�[�^�� VRAM �ɃR�s�[�����̂�
            // ���� pBuf �͊J�����܂��B�ȉ������B
            G2DDemo_Free( pBuf );
        }

        //------------------------------------------------------------------------------
        // load palette data
        {
            NNSG2dPaletteData*        pPlttData;

            pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/CellAnimation.NCLR" );
            SDK_NULL_ASSERT( pBuf );

            DC_FlushRange( pPlttData->pRawData, pPlttData->szByte );
            GX_LoadOBJPltt( (void*)pPlttData->pRawData, TEX_PLTT_BASE, pPlttData->szByte );
            
            G2DDemo_Free( pBuf );

        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         ChangeCallBackType

  Description:  �R�[���o�b�N�^�C�v�̕ύX���s���܂��B

  Arguments:    pAnimCtrl:      �R�[���o�b�N�^�C�v��ύX����A�j���[�V����
                                �R���g���[���ւ̃|�C���^�B
                callBackType:   �V�����R�[���o�b�N�^�C�v�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void ChangeCallBackType
(
    NNSG2dAnimController*   pAnimCtrl,
    NNSG2dAnmCallbackType   callBackType
)
{
    //
    // ������ = ���Z�b�g
    //
    NNS_G2dInitAnimCtrlCallBackFunctor( pAnimCtrl );


    switch( callBackType )
    {
    case NNS_G2D_ANMCALLBACKTYPE_NONE:
        break;

    case NNS_G2D_ANMCALLBACKTYPE_LAST_FRM:
    case NNS_G2D_ANMCALLBACKTYPE_EVER_FRM:
        NNS_G2dSetAnimCtrlCallBackFunctor(
            pAnimCtrl,
            callBackType,
            CALLBACK_PARAM,
            CallBackFunction );
        break;

    case NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM:
        NNS_G2dSetAnimCtrlCallBackFunctorAtAnimFrame(
            pAnimCtrl,
            CALLBACK_PARAM,
            CallBackFunction,
            CALL_CALLBACK_FRAME );
        break;

    default:
        SDK_ASSERT(FALSE);
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         ProcessInput

  Description:  �p�b�h���͂�ǂݍ��݁A���͂ɉ����ăA�j���[�V����
                �p�����[�^��ύX���܂��B

  Arguments:    pIndex:         �V�[�P���X�C���f�b�N�X�ւ̃|�C���^�B
                                ���E�L�[�ɂ��V�[�P���X��؂�ւ��܂��B
                pAnimSpeed:     �A�j���[�V�����X�s�[�h�ւ̃|�C���^�B
                                �㉺�L�[�ɂ��X�s�[�h��ύX���܂��B
                pAnimCtrl:      �A�j���[�V�����R���g���[���ւ̃|�C���^�B
                                A / B �{�^���ɂ�� ��~ / ���] �𑀍삵�܂��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void ProcessInput(u16* pIndex, fx32* pAnimSpeed, NNSG2dAnimController* pAnimCtrl )
{
    G2DDemo_ReadGamePad();

    // change animation sequence.
    {
        if(G2DDEMO_IS_TRIGGER(PAD_KEY_RIGHT))
        {
            (*pIndex)++;
        }

        if(G2DDEMO_IS_TRIGGER(PAD_KEY_LEFT))
        {
            (*pIndex)--;
        }
    }

    // change animation speed.
    {
        if (G2DDEMO_IS_TRIGGER(PAD_KEY_UP))
        {
            *pAnimSpeed +=ANIM_SPEED_UNIT;
        }

        if (G2DDEMO_IS_TRIGGER(PAD_KEY_DOWN))
        {
            *pAnimSpeed -= ANIM_SPEED_UNIT;

            if( *pAnimSpeed < 0 )
            {
                *pAnimSpeed = 0;
            }
        }
    }

    // �Đ�����
    {
        if (G2DDEMO_IS_TRIGGER(PAD_BUTTON_B ))
        {
            pAnimCtrl->bReverse = ! pAnimCtrl->bReverse;
        }

        if (G2DDEMO_IS_TRIGGER(PAD_BUTTON_A ))
        {
            pAnimCtrl->bActive = ! pAnimCtrl->bActive;
        }
    }

    // �R�[���o�b�N��ނ̕ύX
    if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_X) )
    {
        callBackType++;
        if( callBackType >= AnmCallbackType_MAX )
        {
            callBackType = NNS_G2D_ANMCALLBACKTYPE_NONE;
        }
        ChangeCallBackType(pAnimCtrl, callBackType);
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
    NNSG2dAnimBankData*         pAnimBank = NULL;   // Cell�A�j���[�V�����f�[�^
    NNSG2dCellAnimation*        pCellAnim = NULL;   // CellAnimation ����
    

    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_PrintInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        InitOamManager( &objOamManager );
        LoadResources( &pAnimBank, &pCellAnim );
        
        // start display
        SVC_WaitVBlankIntr();
        GX_DispOn();
        GXS_DispOn();
    }

    //----------------------------------------------------
    // Main loop
    while( TRUE )
    {
        static u16      currentSeqIdx = 0;       // �`��V�[�P���X�ԍ�
        static fx32     animSpeed     = FX32_ONE;// �A�j���[�V�����X�s�[�h

        //-------------------------------------------------
        // �p�b�h����
        //
        {
            const u16 oldSeqIndex = currentSeqIdx;
            const u16 numSequence = NNS_G2dGetNumAnimSequence( pAnimBank );

            ProcessInput(
                &currentSeqIdx,
                &animSpeed,
                NNS_G2dGetCellAnimationAnimCtrl(pCellAnim) );

            if( (s16)currentSeqIdx >= numSequence )
            {
                currentSeqIdx= 0;
            }

            if( (s16)currentSeqIdx < 0 )
            {
                currentSeqIdx = (u16)(numSequence - 1);
            }
             
            //
            // �ύX������ΓK�p���܂�
            //
            if( oldSeqIndex != currentSeqIdx )
            {
                const NNSG2dAnimSequence* pSeq = NULL;

                pSeq = NNS_G2dGetAnimSequenceByIdx( pAnimBank, currentSeqIdx );
                SDK_NULL_ASSERT( pSeq );
                NNS_G2dSetCellAnimationSequence( pCellAnim, pSeq );
                NNS_G2dStartAnimCtrl( NNS_G2dGetCellAnimationAnimCtrl( pCellAnim ) );
            }
            
            // �Đ��̍ăX�^�[�g
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_Y) )
            {
                NNS_G2dRestartCellAnimation( pCellAnim );
            }
        }
        
        
        //-------------------------------------------------
        // �`��
        //
        {
            u16                     numOamDrawn = 0;  // �`�悳�ꂽ OAM ��
            static GXOamAttr        temp[NUM_OF_OAM]; // �e���|���� OAM �o�b�t�@
            const NNSG2dCellData*   pCell = NULL;     // �\������ Cell

            pCell = NNS_G2dGetCellAnimationCurrentCell( pCellAnim );
            SDK_NULL_ASSERT( pCell );

            //
            // Cell �Ɠ����� OBJ���X�g���e���|�����ɏ����o���܂�
            //
            numOamDrawn = NNS_G2dMakeCellToOams(
                                temp,           // �o�͐�Oam�o�b�t�@
                                NUM_OF_OAM,     // �o�͐�o�b�t�@��
                                pCell,          // �o�͑Ώ� Cell
                                NULL,           // Affine �ϊ�
                                &CELL_POSITION, // �I�t�Z�b�g�ʒu
                                NULL,           // Affine Index
                                FALSE );        // �{�pAffine���H

            SDK_ASSERT( numOamDrawn < NUM_OF_OAM);

            // OAM �}�l�[�W����OBJ���X�g��o�^���܂�
            (void)NNS_G2dEntryOamManagerOam( &objOamManager, temp, numOamDrawn );
        }

        //-------------------------------------------------
        // �f�o�b�N������`��
        //
        {
            const NNSG2dAnimController* pAnimCtrl = NNS_G2dGetCellAnimationAnimCtrl(pCellAnim);
            
            G2DDemo_PrintOutf(0, 0, "sequence: %3d / %3d", currentSeqIdx, NNS_G2dGetNumAnimSequence( pAnimBank ));
            G2DDemo_PrintOutf(0, 1, "speed:    %7.3f", (float)animSpeed / FX32_ONE );
            G2DDemo_PrintOutf(0, 2, "frame:    %3d / %3d",
                pAnimCtrl->pCurrent - pAnimCtrl->pAnimSequence->pAnmFrameArray,
                pAnimCtrl->pAnimSequence->numFrames );
            G2DDemo_PrintOutf(0, 3, "time:     %3d / %3d",
                pAnimCtrl->currentTime >> FX32_SHIFT,
                pAnimCtrl->pCurrent->frames);
            G2DDemo_PrintOut(22, 0, "Active:");
            G2DDemo_PrintOut(29, 1, NNS_G2dIsAnimCtrlActive(pAnimCtrl) ? "yes": " no");
            G2DDemo_PrintOut(22, 2, "Reverse:");
            G2DDemo_PrintOut(29, 3, pAnimCtrl->bReverse ? "yes": " no");
            
            G2DDemo_PrintOutf(0, 21, "callback type:  %s",
                CALL_BACK_TYPE_NAME[callBackType]);
            G2DDemo_PrintOutf(0, 22, "callback count: %4d", 0);
            
            G2DDemo_PrintOutf(16, 22, "%5d", callCount);
        }
        
        //-------------------------------------------------
        // V Blank �҂�
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();
            
        //-------------------------------------------------
        // �}�l�[�W���̓��e��HW�ɏ����o���܂�
        //            
        {
            NNS_G2dApplyOamManagerToHW( &objOamManager );
            NNS_G2dResetOamManagerBuffer( &objOamManager );
        }

        //-------------------------------------------------
        // �f�o�b�N��񕶎����HW�ɏ����o���܂�   
        //
        G2DDemo_PrintApplyToHW();   
        
        //-------------------------------------------------
        // �A�j���[�V�������X�V���܂��B
        // 
        {
            NNS_G2dTickCellAnimation( pCellAnim, animSpeed );
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

