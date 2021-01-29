/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - VRamAnimation
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.16 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      VRAM �]���}�l�[�W����p���ăZ���A�j���[�V������`��A�X�V����T���v��
//      �VRAM�]���p�̃o�C�i���f�[�^���쐬���邽�߂ɃR���o�[�^��-vta �I�v�V������
//       �w�肵�ăf�[�^�o�͂��s���Ă��܂��B(MakeFile���m�F���Ă��������B )
//
//  ������@
//      START �{�^��    Cell �̏o�͕��@�� OBJ <=> SoftwareSprite ��
//                      �؂�ւ��܂��B
//      A �{�^��        �\������ Cell ��؂�ւ��܂�(�؂�ւ���Z�������݂���Ȃ��)�B
//      X �{�^��        Cell �̊g��k���ړ���]��Ԃ����Z�b�g���܂��B
//      �\���L�[        Cell ���ړ����܂��B
//      B �{�^�� + �\���L�[
//                      Cell ����]���܂��B
//      Y �{�^�� + �\���L�[
//                      Cell ���g��k�����܂��B
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"


#define CHARNAME_TO_OFFSET(charname)    (charname * 32)     // �L�����N�^�l�[������ VRAM ��̃I�t�Z�b�g�ɕϊ�

#define SCREEN_WIDTH                256                     // ��ʕ�
#define SCREEN_HEIGHT               192                     // ��ʍ���

#define AFFINE_IDX                  3                       // Cell �p Affine Index
#define CELL_ROTATE_UNIT            0xFF                    // Cell �̒P�ʉ�]��
#define CELL_SCALE_UNIT             ((FX32_ONE >> 7) * 3 )  // Cell �̒P�ʊg��k����
                                                            // scale �l�� 0 �t�߂̒l�ɂȂ�Ȃ��悤�ɂ��Ă���܂�
#define CELL_INIT_POS_X             (120 << FX32_SHIFT)     // Cell �̏����ʒu X
#define CELL_INIT_POS_Y             (120 << FX32_SHIFT)     // Cell �̏����ʒu Y

#define INIT_OUTPUT_TYPE            NNS_G2D_SURFACETYPE_MAIN2D   // �o�͕��@�̏����l

#define NUM_OF_OAM                  128                     // OAM �}�l�[�W���Ɋ��蓖�Ă� OAM �̐�
#define NUM_OF_CELLANM              1                       // �\������Z���A�j���[�V�����̐�
#define NUM_OF_VRAMSTATE            NUM_OF_CELLANM          // �Z���]���}�l�[�W����������Ԑ�
#define NUM_OF_VRAMTASK             NUM_OF_CELLANM          // VRAM �]���}�l�[�W���������]���^�X�N�̐�

#define PLTT_BASE                   0x0                     // �p���b�g�x�[�X�A�h���X

#define CELL0_CHARNAME_BEGIN_MAIN   6                       // ���p���郁�C����� OAM �̊J�n�ԍ�
#define CELL0_CHARNAME_BEGIN_SUB    0                       // ���p����T�u��� OAM �̊J�n�ԍ�
#define CHAR_MAIN_OFFSET            CHARNAME_TO_OFFSET(CELL0_CHARNAME_BEGIN_MAIN)
#define CHAR_SUB_OFFSET             CHARNAME_TO_OFFSET(CELL0_CHARNAME_BEGIN_SUB)

#define TEX_OFFSET                  128                     // ���p����e�N�X�`�� VRAM �̃I�t�Z�b�g



//------------------------------------------------------------------------------
// �\���̒�`

typedef struct CellInfo
{
    u16          cellIdx;   // �`�� Cell �ԍ�
    u16          rotate;    // ��]�p
    NNSG2dFVec2  scale;     // �g�嗦
    NNSG2dFVec2  pos;       // Cell �`��ʒu
} CellInfo;


//------------------------------------------------------------------------------
// �O���[�o���ϐ�

static NNSG2dImageProxy             sImageProxy;    // Cell �p�L�����N�^/�e�N�X�`���v���L�V
static NNSG2dImagePaletteProxy      sPaletteProxy;  // Cell �p�p���b�g�v���L�V

static NNSG2dOamManagerInstance*    spCurrentOam;   // �R�[���o�b�N�����Ώ�OAM�}�l�[�W��

static NNSG2dAnimBankData*          spAnimBank;     // �A�j���[�V�����o���N
static NNSG2dCellAnimation*         spCellAnim;     // �Z���A�j���[�V����

// VRAM �]���}�l�[�W���p�̃��[�N�̈�
static NNSGfdVramTransferTask       sTransferTaskArray[NUM_OF_VRAMTASK];

// �Z���]���}�l�[�W���p�̃��[�N�̈�
static NNSG2dCellTransferState      sVramStateArray[NUM_OF_VRAMSTATE];

static u32                          sCellTransManHandle;    // �Z���]����ԊǗ��I�u�W�F�N�g�̃n���h��


//------------------------------------------------------------------------------
// �v���g�^�C�v�錾

void VBlankIntr(void);
static NNSG2dCellDataBank* LoadResources(void);
static void ProcessInput(CellInfo* pCellInfo, NNSG2dSurfaceType* pOutputType, u16 numCells);
static void MakeAffineMatrix(MtxFx22* pMtx, MtxFx22* pMtx2D, const CellInfo* pCellInfo);
static BOOL CallBackAddOam( const GXOamAttr* pOam, u16 affineIndex, BOOL bDoubleAffine );
static u16 CallBackAddAffine( const MtxFx22* mtx );
static void InitOamManager(NNSG2dOamManagerInstance*   pObjOamManager);
static void InitRenderer(
                NNSG2dRendererInstance* pRenderer,
                NNSG2dRenderSurface*    pSurface);


/*---------------------------------------------------------------------------*
  Name:         CallbackRegistTransferTask

  Description:  NNS_G2dDraw* �֐������ŃL�����N�^�f�[�^�̓]���^�X�N��
                �o�^���邽�߂ɌĂ΂��֐��ł��B

  Arguments:    type:       �]���� VRAM �̈���
                dstAddr:    �]����A�h���X�I�t�Z�b�g
                pSrc:       �]�����f�[�^�ւ̃|�C���^�B
                szByte:     �]�����ׂ��f�[�^�T�C�Y�B

  Returns:      ���������ꍇ�� TRUE�A�����łȂ���� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static BOOL CallbackRegistTransferTask(
                NNS_GFD_DST_TYPE type,
                u32 dstAddr,
                void* pSrc,
                u32 szByte )
{
    // VRAM �]���}�l�[�W���ɂ��̂܂ܓn��
    return NNS_GfdRegisterNewVramTransferTask(
                type,
                dstAddr,
                pSrc,
                szByte );
}

/*---------------------------------------------------------------------------*
  Name:         CallBackAddOam

  Description:  NNS_G2dDraw* �֐������� OAM ��ǉ����邽�߂ɌĂ΂��֐��ł��B

  Arguments:    pOam:           �ǉ����ׂ� OAM �ւ̃|�C���^�B
                affineIndex:    ���� OAM ���g�p���� Affine �C���f�b�N�X�B
                                32 �ȏ�̒l�ł���\��������܂��B
                                Affine �𖢎g�p�̏ꍇ��
                                NNS_G2D_OAM_AFFINE_IDX_NONE ���w�肵�܂��B
                bDoubleAffine:  �{�pAffine���[�h���ǂ����B

  Returns:      ���������ꍇ�� TRUE�A�����łȂ���� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static BOOL CallBackAddOam( const GXOamAttr* pOam, u16 affineIndex, BOOL bDoubleAffine )
{
#pragma unused( bDoubleAffine )
    SDK_NULL_ASSERT( spCurrentOam );
    SDK_NULL_ASSERT( pOam );

    return NNS_G2dEntryOamManagerOamWithAffineIdx( spCurrentOam, pOam, affineIndex );
}

/*---------------------------------------------------------------------------*
  Name:         CallBackAddAffine

  Description:  NNS_G2dDraw* �֐������� Affine �p�����[�^ ��ǉ����邽�߂�
                �Ă΂��֐��ł��B

  Arguments:    mtx:        �ǉ����ׂ� Affine �ϊ��s��ւ̃|�C���^

  Returns:      �ǉ����� Affine �p�����[�^���Q�Ƃ��邽�߂� AffineIndex ��
                �Ԃ��܂��B�B
 *---------------------------------------------------------------------------*/
static u16 CallBackAddAffine( const MtxFx22* mtx )
{
    SDK_NULL_ASSERT( spCurrentOam );
    SDK_NULL_ASSERT( mtx );
    return NNS_G2dEntryOamManagerAffine( spCurrentOam, mtx );
}

/*---------------------------------------------------------------------------*
  Name:         InitOamManager

  Description:  OAM �}�l�[�W���V�X�e���̏������ƁA1��OAM�}�l�[�W��
                ���̂̏��������s���܂��B

  Arguments:    pOamManager:    ���������� OAM �}�l�[�W���ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitOamManager(NNSG2dOamManagerInstance* pOamManager)
{
    BOOL result;
    SDK_NULL_ASSERT( pOamManager );

    NNS_G2dInitOamManagerModule();

    result = NNS_G2dGetNewOamManagerInstanceAsFastTransferMode(
                pOamManager, 0, NUM_OF_OAM, NNS_G2D_OAMTYPE_MAIN );
    SDK_ASSERT( result );
}

/*---------------------------------------------------------------------------*
  Name:         InitRenderer

  Description:  Renderer �� Surface �����������܂��B

  Arguments:    pRenderer:  ���������� Renderer �ւ̃|�C���^�B
                pSurface:   ���������� Surface �ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitRenderer(
                NNSG2dRendererInstance* pRenderer,
                NNSG2dRenderSurface*    pSurface)
{
    NNSG2dViewRect* pRect = &(pSurface->viewRect);
    SDK_NULL_ASSERT( pRenderer );
    SDK_NULL_ASSERT( pSurface );

    NNS_G2dInitRenderer( pRenderer );
    NNS_G2dInitRenderSurface( pSurface );

    // �\����`
    pRect->posTopLeft.x = FX32_ONE * 0;
    pRect->posTopLeft.y = FX32_ONE * 0;
    pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
    pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;

    // �R�[���o�b�N�֐�
    pSurface->pFuncOamRegister       = CallBackAddOam;
    pSurface->pFuncOamAffineRegister = CallBackAddAffine;

    // Surface �o�^
    NNS_G2dAddRendererTargetSurface( pRenderer, pSurface );

    // �v���L�V�o�^
    NNS_G2dSetRendererImageProxy( pRenderer, &sImageProxy, &sPaletteProxy );

        // Z �I�t�Z�b�g�������w��
    NNS_G2dSetRendererSpriteZoffset( pRenderer, - FX32_ONE );
}

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  �K�v�ȃf�[�^���t�@�C������ǂݍ��݂܂��B
                �܂��Z���A�j���[�V�����̍\�z���s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �ǂݍ��񂾃Z���o���N�ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
static NNSG2dCellDataBank* LoadResources(void)
{
    NNSG2dCharacterData* pCharData2D;
    NNSG2dCharacterData* pCharData3D;
    NNSG2dCellDataBank* pCellBank;
    void* pBuf;

    // Proxy ������
    NNS_G2dInitImageProxy( &sImageProxy );
    NNS_G2dInitImagePaletteProxy( &sPaletteProxy );

    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/VRamAnimation.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            PLTT_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sPaletteProxy);

        // Loading For 3D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            PLTT_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &sPaletteProxy);

        // �p���b�g�f�[�^�� VRAM �ɃR�s�[�����̂�
        // ���� pBuf �͊J�����܂��B
        G2DDemo_Free( pBuf );
    }

    //------------------------------------------------------------------------------
    // load character data for 2D
    {
        pBuf = G2DDemo_LoadNCGR( &pCharData2D, "data/VRamAnimation.NCGR" );
        SDK_NULL_ASSERT( pBuf );
        // ���̃f�[�^�͍Ō�܂Ń��C����������Ŏg�p����̂�
        // ���� pBuf �͊J�����܂���B�ȉ������B

        // ���� NNS_G2dLoad* �֐��ƈقȂ� NNS_G2dLoadImageVramTransfer ��
        // �L�����N�^�f�[�^��VRAM�ɓǂݍ��݂܂���B
        // �L�����N�^�f�[�^�̑����� ImageProxy �� HW �ɐݒ肷�邾���ł��B
        // ���̂��߃L�����N�^�f�[�^�����C���������ɕێ�����K�v������܂��B
        NNS_G2dLoadImageVramTransfer(
            pCharData2D,
            CHAR_MAIN_OFFSET,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sImageProxy );
    }

    // load character data for 3D (software sprite)
    {
        pBuf = G2DDemo_LoadNCBR( &pCharData3D, "data/VRamAnimation.NCBR" );
        SDK_NULL_ASSERT( pBuf );

        NNS_G2dLoadImageVramTransfer(
            pCharData3D,
            TEX_OFFSET,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &sImageProxy );
    }

    //------------------------------------------------------------------------------
    // load cell data
    {
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/VRamAnimation.NCER" );
        SDK_NULL_ASSERT( pBuf );
    }


    //------------------------------------------------------------------------------
    // load animation data
    {
        pBuf = G2DDemo_LoadNANR( &spAnimBank, "data/VRamAnimation.NANR" );
        SDK_NULL_ASSERT( pBuf );
    }

    //------------------------------------------------------------------------------
    // �Z���A�j���[�V�������\�z���܂�
    {
        // VRAM �]���̂��߂̏��������s���܂�
        {
            NNS_GfdInitVramTransferManager(
                sTransferTaskArray,
                NUM_OF_VRAMTASK
            );

            NNS_G2dInitCellTransferStateManager(
                sVramStateArray,
                NUM_OF_VRAMSTATE,
                CallbackRegistTransferTask          // <- (1)
            );

            // �Z���]����ԊǗ��I�u�W�F�N�g��V�����擾���܂��B
            // �I�u�W�F�N�g���s�K�v�ɂȂ�����
            // NNS_G2dFreeCellTransferStateHandle( handle ) �ŉ�����܂��B
            sCellTransManHandle = NNS_G2dGetNewCellTransferStateHandle();
            SDK_ASSERT( sCellTransManHandle != NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE );
        }

        // �Z���A�j���[�V�����̎��̂����������܂�
        {
            spCellAnim = G2DDemo_GetNewCellAnimation(1);
            SDK_NULL_ASSERT( spCellAnim );

            SDK_NULL_ASSERT( NNS_G2dGetAnimSequenceByIdx(spAnimBank, 0) );
            SDK_ASSERT( NNS_G2dCellDataBankHasVramTransferData(pCellBank) );

            // NNS_G2dInitializeVramTransferedCellAnimation �̑� 8,9 ���������� NULL �ȊO�ɂ���ꍇ�́A
            // ��{�I�ɂ�2�̃f�[�^�T�C�Y���������Ȃ���΂Ȃ�܂���
            SDK_ASSERT( pCharData2D->szByte == pCharData3D->szByte );

            NNS_G2dInitCellAnimationVramTransfered(
                spCellAnim,
                NNS_G2dGetAnimSequenceByIdx(spAnimBank, 0),
                pCellBank,

                sCellTransManHandle,    // ���̃Z���A�j���[�V������VRAM�]�����Ǘ�����
                                        // �Z���]����ԊǗ��I�u�W�F�N�g�̃n���h��
                                        // �ȉ���3�� (1) ��Callback�֐��� dstAddr �Ƃ��ēn����܂�
                                        // �ʏ�� ImageProxy �Ɏw�肵�����̂Ɠ����l���w�肵�܂�
                TEX_OFFSET,             // �e�N�X�`���]����I�t�Z�b�g
                CHAR_MAIN_OFFSET,       // ���C����ʃL�����N�^�]����I�t�Z�b�g
                CHAR_SUB_OFFSET,        // �T�u��ʃL�����N�^�]����I�t�Z�b�g
                                        // �ȉ���2�͂ǂ��炩�� NULL �ł��\���܂���
                pCharData2D->pRawData,  // �L�����N�^�f�[�^
                pCharData3D->pRawData,  // �e�N�X�`���f�[�^
                pCharData2D->szByte     // �e�N�X�`���f�[�^ or �L�����N�^�f�[�^�̃T�C�Y
            );
        }
    }

    return pCellBank;
}


/*---------------------------------------------------------------------------*
  Name:         ProcessInput

  Description:  �L�[���͂ɂ��������āACell �̕\����Ԃ�ύX���܂��B
                �\�� Cell �̐؂�ւ��A��ԃ��Z�b�g���s���܂��B

  Arguments:    pCellInfo:      �L�[���͂𔽉f������ CellInfo �ւ̃|�C���^
                pOutputType:    �o�͕��@���i�[�����o�b�t�@�ւ̃|�C���^
                numCells:       �I���\�� Cell �̐�

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void ProcessInput(CellInfo* pCellInfo, NNSG2dSurfaceType* pOutputType, u16 numCells)
{
    SDK_NULL_ASSERT( pCellInfo );
    SDK_NULL_ASSERT( pOutputType );

    G2DDemo_ReadGamePad();

    // Change current Cell
    if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_A) )
    {
       pCellInfo->cellIdx++;
        if( pCellInfo->cellIdx >= numCells )
        {
            pCellInfo->cellIdx = 0;
        }
    }

    // Rotate Cell
    if( G2DDEMO_IS_PRESS(PAD_BUTTON_B) )
    {
        if ( G2DDEMO_IS_PRESS(PAD_KEY_LEFT) )
        {
            pCellInfo->rotate -= CELL_ROTATE_UNIT;      // �����v���
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
        {
            pCellInfo->rotate += CELL_ROTATE_UNIT;      // ���v���
        }
    }

    // Zoom Cell
    if( G2DDEMO_IS_PRESS(PAD_BUTTON_Y) )
    {
        if ( G2DDEMO_IS_PRESS(PAD_KEY_UP) )
        {
            pCellInfo->scale.y += CELL_SCALE_UNIT;   // Y�����g��
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_DOWN) )
        {
            pCellInfo->scale.y -= CELL_SCALE_UNIT;   // Y�����k��
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_LEFT) )
        {
            pCellInfo->scale.x += CELL_SCALE_UNIT;   // X�����g��
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
        {
            pCellInfo->scale.x -= CELL_SCALE_UNIT;   // X�����k��
        }
    }

    // Move Cell
    if( ! G2DDEMO_IS_PRESS(PAD_BUTTON_B) && ! G2DDEMO_IS_PRESS(PAD_BUTTON_Y) )
    {
        if ( G2DDEMO_IS_PRESS(PAD_KEY_UP) )
        {
            pCellInfo->pos.y -= FX32_ONE;
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_DOWN) )
        {
            pCellInfo->pos.y += FX32_ONE;
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_LEFT) )
        {
            pCellInfo->pos.x -= FX32_ONE;
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
        {
            pCellInfo->pos.x += FX32_ONE;
        }
    }

    // Reset
    if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_X) )
    {
        pCellInfo->rotate      = 0;
        pCellInfo->scale       = (NNSG2dFVec2){FX32_ONE, FX32_ONE};
        pCellInfo->pos         = (NNSG2dFVec2){CELL_INIT_POS_X, CELL_INIT_POS_Y};
    }

    // Toggle Output Type
    if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_START) )
    {
        *pOutputType = (*pOutputType == NNS_G2D_SURFACETYPE_MAIN2D) ?
                            NNS_G2D_SURFACETYPE_MAIN3D:
                            NNS_G2D_SURFACETYPE_MAIN2D;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C���֐��ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    NNSG2dRendererInstance      render;         // �`��p Renderer
    NNSG2dRenderSurface         surface;        // ���C����� Surface
    NNSG2dCellDataBank*         pCellBank;      // Cell �f�[�^
    NNSG2dOamManagerInstance    oamManager;     // 2D�`�掞��OAM�}�l�[�W��
    NNSG2dSurfaceType           outputType = INIT_OUTPUT_TYPE;


    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();
        InitOamManager( &oamManager );
        InitRenderer( &render, &surface );
        pCellBank = LoadResources();
    }

    // start display
    {
        SVC_WaitVBlankIntr();
        GX_DispOn();
        GXS_DispOn();
    }

    // Main loop
    while( TRUE )
    {
        static CellInfo cellInfo = {
            0,                                  // �`�� Cell �ԍ�
            0,                                  // ��]�p
            {FX32_ONE, FX32_ONE},               // �g�嗦
            {CELL_INIT_POS_X, CELL_INIT_POS_Y}  // Cell �`��ʒu
        };


        // ���͂�ǂݎ�� Cell �̕\�����Əo�͕��@���X�V���܂�
        ProcessInput(&cellInfo, &outputType, pCellBank->numCells);

        surface.type = outputType;

        // �`��
        //
        // Renderer ���W���[����p���� Cell ��`�悵�܂��B
        //
        {
            spCurrentOam = &oamManager;

            NNS_G2dBeginRendering( &render );
                NNS_G2dPushMtx();

                    NNS_G2dTranslate( cellInfo.pos.x, cellInfo.pos.y, 0 );
                    NNS_G2dRotZ( FX_SinIdx( cellInfo.rotate ), FX_CosIdx( cellInfo.rotate ) );
                    NNS_G2dScale( cellInfo.scale.x, cellInfo.scale.y, FX32_ONE );

                    NNS_G2dDrawCellAnimation( spCellAnim );

                NNS_G2dPopMtx();
            NNS_G2dEndRendering();

            // �Z���]���}�l�[�W���ւ̓]���v���� �R�[���o�b�N�֐��o�R��
            // VRAM �]���}�l�[�W���ɑ���܂�
            NNS_G2dUpdateCellTransferStateManager();
        }

        // �\�����̏o��
        {
            G2DDemo_PrintOutf(0, 0, "pos:   x=%3d     y=%3d",
                cellInfo.pos.x >> FX32_SHIFT, cellInfo.pos.y >> FX32_SHIFT);
            G2DDemo_PrintOutf(0, 1, "scale: x=%7.3f y=%7.3f",
                (double)cellInfo.scale.x / FX32_ONE, (double)cellInfo.scale.y / FX32_ONE);
            G2DDemo_PrintOutf(0, 2, "rot:   %5d (%6.2f)",
                cellInfo.rotate, 360.0 * cellInfo.rotate / 0x10000);
            G2DDemo_PrintOut(0, 23, (outputType == NNS_G2D_SURFACETYPE_MAIN2D) ?
                   "OBJ           ": "SoftwareSprite");
        }

        // V Blank �҂�
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();


        //
        // �o�b�t�@�̓��e��HW�ɏ����o���܂�
        //
        {
            // VRAM �]�����s���܂�
            NNS_GfdDoVramTransfer();

            // �\�����������o���܂�
            G2DDemo_PrintApplyToHW();

            // OAM �������o���܂�
            NNS_G2dApplyOamManagerToHW( &oamManager );
            NNS_G2dResetOamManagerBuffer( &oamManager );
        }

        NNS_G2dTickCellAnimation( spCellAnim, FX32_ONE );
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


