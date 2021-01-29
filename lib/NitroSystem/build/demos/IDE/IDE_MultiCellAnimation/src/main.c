/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - IDE - IDE_MultiCellAnimation
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.4 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      �}���`�Z���A�j���[�V������`��A�X�V����T���v��
//      �قȂ�����Ŏ��̂����������A�p�t�H�[�}���X���r���邱�Ƃ��ł��܂��B
//
//  ������@
//      �\���L�[�㉺    �Đ����x�ύX
//      A               �Đ�<=>��~�؂�ւ�
//      B               ������<=>�t�����Đ��؂�ւ�
//      X               �}���`�Z���A�j���[�V�����̎��̂̎�ނ�ύX����
//      Y               �}���`�Z���A�j���[�V�����̃��Z�b�g
// ============================================================================

// �v���R���p�C���w�b�_���g�p����B
// #include <nitro.h>
// #include <nnsys/g2d.h>

#include "g2d_demolib.h"


#define NUM_OF_OAM              128                  // OAM�}�l�[�W���Ǘ��Ώۂ�OAM��
#define NUM_OF_AFFINE           (NUM_OF_OAM/4)      // OAM�}�l�[�W���Ǘ��Ώۂ�Affine�p�����[�^��

#define ANIM_SPEED_UNIT         (FX32_ONE >> 3)     // �A�j���[�V�������x�̕ω�����

//
// �}���`�Z�����̂̎��
// pMCAnim_ �z��ɃA�N�Z�X����ۂɎg�p����܂�
//
typedef enum 
{
    MCANIM_SHARE_CELLANIM = 0x0,
    MCANIM_DONOT_SHARE_CELLANIM = 0x1
    
}MyMCAnimType;

static const char* mcAnimTypeStr [] =
{
    "     ShareCellAnim",
    "DoNotShareCellAnim"
};

static const int CvtTblToNNSG2dMCType [] = 
{
    NNS_G2D_MCTYPE_SHARE_CELLANIM,
    NNS_G2D_MCTYPE_DONOT_SHARE_CELLANIM
};


// Cell �\���ʒu
static const NNSG2dFVec2           CELL_POSITION = {120 << FX32_SHIFT, 150 << FX32_SHIFT};
//
// MultiCellAnimation ����
// �قȂ�2�̕����Ŏ��̂����������܂��B
// 
static NNSG2dMultiCellAnimation   mcAnim[2];


// OAM�A�g���r���[�g�ꎞ�o�b�t�@
static GXOamAttr                tempOamBuffer[NUM_OF_OAM];

//
// �}���`�Z���o���N
//
static NNSG2dMultiCellDataBank*        pMCBank = NULL;     // �}���`�Z���f�[�^


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
        NNSG2dCellDataBank*             pCellBank = NULL;   // �Z���f�[�^
        NNSG2dCellAnimBankData*         pAnimBank = NULL;   // �Z���A�j���[�V����
        NNSG2dMultiCellAnimBankData*    pMCABank = NULL;    // �}���`�Z���A�j���[�V����

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
        // �}���`�Z���A�j���[�V�����̎��̂����������܂�
        // �قȂ������2�̎��̂����������܂��B
        //
        InitMultiCellInstance_( &mcAnim[MCANIM_SHARE_CELLANIM],
                                pAnimBank,
                                pCellBank,
                                pMCBank,
                                pMCABank,
                                NNS_G2D_MCTYPE_SHARE_CELLANIM  );

        
        InitMultiCellInstance_( &mcAnim[MCANIM_DONOT_SHARE_CELLANIM],
                                pAnimBank,
                                pCellBank,
                                pMCBank,
                                pMCABank,
                                NNS_G2D_MCTYPE_DONOT_SHARE_CELLANIM  );
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

/*---------------------------------------------------------------------------*
  Name:         ProcessInput

  Description:  �p�b�h���͂�ǂݍ��݁A���͂ɉ����ăA�j���[�V����
                �p�����[�^��ύX���܂��B

  Arguments:    pAnimSpeed: �A�j���[�V�����X�s�[�h�ւ̃|�C���^�B
                            �㉺�L�[�ɂ��X�s�[�h��ύX���܂��B
                pAnimCtrl:  �A�j���[�V�����R���g���[���ւ̃|�C���^�B
                            A / B �{�^���ɂ�� ��~ / ���] �𑀍삵�܂��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void ProcessInput(fx32* pAnimSpeed, NNSG2dAnimController* pAnimCtrl)
{
    G2DDemo_ReadGamePad();

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
        u16                             numOamDrawn = 0;                  // �c�� OAM ��
        
        const NNSG2dMultiCellInstance*  pMultiCell = &(pMcAnim->multiCellInstance); // �\������ MultiCell
        SDK_NULL_ASSERT( pMultiCell );

        //
        // MultiCell �Ɠ����� OBJ���X�g���e���|�����ɏ����o���܂�
        //
        numOamDrawn = NNS_G2dMakeSimpleMultiCellToOams(
                            tempOamBuffer,  // �o�͐�Oam�o�b�t�@
                            NUM_OF_OAM,     // �o�͐�o�b�t�@��
                            pMultiCell,     // �o�͑Ώ� MultiCell
                            NULL,           // Affine �ϊ�
                            &CELL_POSITION, // �I�t�Z�b�g�ʒu
                            NULL,           // Affine Index
                            FALSE );        // �{�pAffine���H

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
    MyMCAnimType                mcAnimType = MCANIM_SHARE_CELLANIM;
        
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
        static OSTick                  time         = 0;
        NNSG2dMultiCellAnimation*      pMcAnim      = &mcAnim[mcAnimType];
        
        //
        //  ------ Pad handling.
        //
        {
            ProcessInput(&animSpeed, NNS_G2dGetMCAnimAnimCtrl(pMcAnim));
            
            // ---- �}���`�Z���A�j���[�V�������̂̎�ނ�؂�ւ��܂��B
            if (G2DDEMO_IS_TRIGGER(PAD_BUTTON_X ))
            {
                if( mcAnimType == MCANIM_SHARE_CELLANIM )
                {
                    mcAnimType = MCANIM_DONOT_SHARE_CELLANIM;
                }else{
                    mcAnimType = MCANIM_SHARE_CELLANIM;
                }
            }
            
            // ---- ���Z�b�g
            if (G2DDEMO_IS_PRESS(PAD_BUTTON_Y ))
            {
                NNS_G2dRestartMCAnimation( pMcAnim );
            }
        }

        //  ------ �`��
        //
        // ���ʃ��x���̕`��API�𗘗p���� MultiCell ��`�悵�܂��B
        // ��ʃ��W���[���� G2dRenderer �� ���p����`����@������܂��B
        //
        //
        AppDraw( pMcAnim, &oamManager );
        
        
        //  ------ �A�j���[�V�����̍X�V
        // �`�悳��Ă�����̍X�V�ɂ����������Ԃ��v�����܂�
        {
            u32 i;
            for( i = 0; i < 2; i++ )
            {
                if( i == mcAnimType ) 
                {
                    time = OS_GetTick();
                        NNS_G2dTickMCAnimation( &mcAnim[i], animSpeed );
                    time = OS_GetTick()- time;
                }else{
                        NNS_G2dTickMCAnimation( &mcAnim[i], animSpeed );
                }
            }
        }
        
        
        // ------ �f�o�b�N�������`�悵�܂�
        {
            const NNSG2dAnimController* pAnimCtrl = NNS_G2dGetMCAnimAnimCtrl(pMcAnim);

            G2DDemo_PrintOutf(0, 0, "speed: %7.3f", (float)animSpeed / FX32_ONE );
            G2DDemo_PrintOutf(0, 1, "frame: %3d / %3d",
                pAnimCtrl->pCurrent - pAnimCtrl->pAnimSequence->pAnmFrameArray,
                pAnimCtrl->pAnimSequence->numFrames );
            
            G2DDemo_PrintOutf( 0, 2, "TIMEForUpdate     :%06ld usec", OS_TicksToMicroSeconds(time) );
            
            // ���݂̎��̂̏������ɕK�v�ȃ��[�N�������̃T�C�Y
            G2DDemo_PrintOutf( 0, 3, "szWorkMem:%06ld byte", 
                               NNS_G2dGetMCWorkAreaSize( pMCBank, 
                               (NNSG2dMCType)CvtTblToNNSG2dMCType[ mcAnimType ] ) );
                               
            // ���݂̎��̂̎��
            G2DDemo_PrintOutf( 0, 4, "Type     :%s", mcAnimTypeStr[mcAnimType] );
            
            G2DDemo_PrintOut(19, 0, "Active:");
            G2DDemo_PrintOut(28, 0, NNS_G2dIsAnimCtrlActive(pAnimCtrl) ? "yes": " no");
            G2DDemo_PrintOut(19, 1, "Reverse:");
            G2DDemo_PrintOut(28, 1, pAnimCtrl->bReverse ? "yes": " no");
        }

        // ------ V Blank �҂�
        SVC_WaitVBlankIntr();
        G2DDemo_PrintApplyToHW();   // �A�j���[�V��������HW�ɏ����o���܂�

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

