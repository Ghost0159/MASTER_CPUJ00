/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - Renderer_2LCD
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
//      Renderer ���W���[����p����2���(���ʂƂ������)�\�����s���T���v���B
//
//      �e��ʊO�̃Z���̕`���������邽�߁A���J�����O�֐��������_���ɐݒ肵�Ă��܂��B
//
//  ������@
//      START �{�^��    Cell �̏o�͕��@�� OBJ <=> SoftwareSprite ��
//                      �؂�ւ��܂��B
//      X �{�^��        Cell �y�� Surface �̈ʒu�����Z�b�g���܂��B
//      �\���L�[        Cell ���ړ����܂��B
//      Y + �\���L�[    ���C�� Surface ���ړ����܂��B
//      B + �\���L�[    �T�u Surface ���ړ����܂��B
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

//------------------------------------------------------------------------------
// �e��萔��`
#define SCREEN_WIDTH                256                     // ��ʕ�
#define SCREEN_HEIGHT               192                     // ��ʍ���


#define INIT_OUTPUT_TYPE            NNS_G2D_SURFACETYPE_MAIN2D   // �o�͕��@�̏����l

#define NUM_OF_OAM                  128                 // OAM �}�l�[�W���Ɋ��蓖�Ă� OAM �̐�
#define NUM_OF_AFFINE               (NUM_OF_OAM / 4)    // OAM �}�l�[�W���Ɋ��蓖�Ă� Affine�p�����[�^ �̐�

#define CHARA_BASE                  0x0                 // �L�����N�^�C���[�W�x�[�X�A�h���X
#define PLTT_BASE                   0x0                 // �p���b�g�x�[�X�A�h���X

#define SURFACE_MAIN_INIT_POS ((NNSG2dFVec2){0 << FX32_SHIFT, 0 << FX32_SHIFT})
#define SURFACE_SUB_INIT_POS ((NNSG2dFVec2){0 << FX32_SHIFT, SCREEN_HEIGHT << FX32_SHIFT})

#define CELL_A_INIT_POS ((NNSG2dFVec2){150 << FX32_SHIFT, 120 << FX32_SHIFT})


static const NNSG2dFVec2 CELL_B_POS = {100 << FX32_SHIFT, 120 << FX32_SHIFT};
static const NNSG2dFVec2 CELL_C_POS = {100 << FX32_SHIFT, 300 << FX32_SHIFT};


//------------------------------------------------------------------------------
// �\���̒�`


//------------------------------------------------------------------------------
// �O���[�o���ϐ�

static NNSG2dImageProxy             sImageProxy;    // Cell �p�L�����N�^/�e�N�X�`���v���L�V
static NNSG2dImagePaletteProxy      sPaletteProxy;  // Cell �p�p���b�g�v���L�V

static NNSG2dOamManagerInstance     sMainOamMan;    // ���C����ʗp OAM �}�l�[�W��
static NNSG2dOamManagerInstance     sSubOamMan;     // �T�u��ʗp OAM �}�l�[�W��

static NNSG2dAnimBankData*          spAnimBank;     // �A�j���[�V�����o���N
static NNSG2dCellAnimation*         spCellAnim;     // �Z���A�j���[�V����

//------------------------------------------------------------------------------
// �v���g�^�C�v�錾



/*---------------------------------------------------------------------------*
  Name:         CallBackAddOam*

  Description:  NNS_G2dDraw* �֐������� OAM ��ǉ����邽�߂ɌĂ΂��֐��ł��B
                OAM �}�l�[�W���� OAM ��n���܂��B

  Arguments:    pOam:           �ǉ����ׂ� OAM �ւ̃|�C���^�B
                affineIndex:    ���� OAM ���g�p���� Affine �C���f�b�N�X�B
                                32 �ȏ�̒l�ł���\��������܂��B
                                Affine �𖢎g�p�̏ꍇ��
                                NNS_G2D_OAM_AFFINE_IDX_NONE ���w�肵�܂��B
                bDoubleAffine:  �{�pAffine���[�h���ǂ����B

  Returns:      ���������ꍇ�� TRUE�A�����łȂ���� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
// ���C����ʗp
static BOOL CallBackAddOamMain( const GXOamAttr* pOam, u16 affineIndex, BOOL bDoubleAffine )
{
#pragma unused( bDoubleAffine )
    SDK_NULL_ASSERT( pOam );

    return NNS_G2dEntryOamManagerOamWithAffineIdx( &sMainOamMan, pOam, affineIndex );
}

// �T�u��ʗp
static BOOL CallBackAddOamSub( const GXOamAttr* pOam, u16 affineIndex, BOOL bDoubleAffine )
{
#pragma unused( bDoubleAffine )
    SDK_NULL_ASSERT( pOam );

    return NNS_G2dEntryOamManagerOamWithAffineIdx( &sSubOamMan, pOam, affineIndex );
}

/*---------------------------------------------------------------------------*
  Name:         CallBackAddAffine*

  Description:  NNS_G2dDraw* �֐������� Affine �p�����[�^ ��ǉ����邽�߂�
                �Ă΂��֐��ł��B
                OAM �}�l�[�W���� Affine �p�����[�^��n���܂��B

  Arguments:    mtx:        �ǉ����ׂ� Affine �ϊ��s��ւ̃|�C���^

  Returns:      �ǉ����� Affine �p�����[�^���Q�Ƃ��邽�߂� AffineIndex ��
                �Ԃ��܂��B�B
 *---------------------------------------------------------------------------*/
// ���C����ʗp
static u16 CallBackAddAffineMain( const MtxFx22* mtx )
{
    SDK_NULL_ASSERT( mtx );
    return NNS_G2dEntryOamManagerAffine( &sMainOamMan, mtx );
}

// �T�u��ʗp
static u16 CallBackAddAffineSub( const MtxFx22* mtx )
{
    SDK_NULL_ASSERT( mtx );
    return NNS_G2dEntryOamManagerAffine( &sSubOamMan, mtx );
}

/*---------------------------------------------------------------------------*
  Name:         CallBackCulling

  Description:  NNS_G2dDraw* �֐������ŃZ���̕\��/��\���𔻒肷�邽�߂�
                �Ă΂��֐��ł��B
                �R���o�[�^�ɂ���ăZ���f�[�^�ɕt�����ꂽ�Z���̋��E�����a��p��
                �ΏۃZ���� Surface �����ɂ��邩�ǂ����𔻒肵�܂��B
                
                ���ӁF
                ���E���Z�o�̃A���S���Y������A�T���v���Ŏg�p���Ă���Z���̋��E���̔��a��32���x�i���a��64���x�j��
                ���ۂ̕\���T�C�Y�������Ȃ�傫�ȃT�C�Y�ƂȂ�܂��B�i���ۂɂ�36�ƂȂ��Ă���悤�ł��j�B
                ���̂��߁A���J�����O�����ŉ��Ɣ��肳��A��荞�݂̂��߁A��ʔ��Α��ɕ\������Ă��܂��܂��B

                �΍�͂��܂��܂ȕ��@���l�����܂��B

                A�D�f�ތŗL�̏������l����������ȃJ�����O�֐�����������B
                B�D�Z�����E��`���𗘗p���ăJ�����O�������Ȃ��i�ڍׂ̓R���o�[�^�}�j���A�����Q�Ƃ��������B�j�B
                C�DOBJ�P�ʃJ�����O���s���B

                ����̃T���v���ł́A
                A�D�f�ތŗL�̏������l����������ȃJ�����O�֐�����������B
                �̕��@���Ƃ�A
                
                ���ׂẴZ�����Z�����_����Z��Y����������OBJ��z�u���Ă���Ɖ��肵
                �{��
                const fx32  minY = py - R;
                const fx32  maxY = py + R;
                �Ƃ��ׂ�������
                ��
                const fx32  minY = py - R;
                const fx32  maxY = py;
                �Ƃ��Ă��܂��B


  Arguments:    pCell:      ����Ώۂ̃Z���f�[�^�ւ̃|�C���^�B
                pMtx:       ����Ώۂ̃Z���ɓK�p�������W�ϊ��s��B
                pViewRect:  �o�͐� Surface �̕\����`�B

  Returns:      �ΏۃZ����\������̂ł���� TRUE�A�����łȂ���� FALSE�B
 *---------------------------------------------------------------------------*/
static BOOL CallBackCulling(
                const NNSG2dCellData*   pCell,
                const MtxFx32*          pMtx,
                const NNSG2dViewRect*   pViewRect)
{
    // �R���o�[�^�ɂ��v�Z���ꂽ���E�����a���擾����
    const fx32  R = NNSi_G2dGetCellBoundingSphereR( pCell ) * FX32_ONE;

    // �Z���̕\���ʒu���_�����߂�
    const fx32  px = pMtx->_20 - pViewRect->posTopLeft.x;
    const fx32  py = pMtx->_21 - pViewRect->posTopLeft.y;

    // �Z���̋��E���������`�����߂�
    // ���ӁF���ׂẴZ�����Z�����_����Z��Y����������OBJ��z�u���Ă���Ɖ��肵�Ă���B
#if 1
    const fx32  minY = py - R;
    const fx32  maxY = py;
#else
    const fx32  minY = py - R;
    const fx32  maxY = py + R;              
#endif 

    const fx32  minX = px - R;
    const fx32  maxX = px + R;


    if( (maxY > 0) && (minY < pViewRect->sizeView.y) )
    {
        if( (maxX > 0) && (minX < pViewRect->sizeView.x) )
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         InitOamManager

  Description:  OAM �}�l�[�W�����̂̏��������s���܂��B

  Arguments:    pOamManager:    ���������� OAM �}�l�[�W���ւ̃|�C���^�B
                type:           �ǂ̃O���t�B�b�N�X�G���W�������ɏ��������邩��
                                �w�肵�܂��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitOamManager(NNSG2dOamManagerInstance* pOamManager, NNSG2dOamType type)
{
    BOOL result;
    SDK_NULL_ASSERT( pOamManager );

    result = NNS_G2dGetNewManagerInstance( pOamManager, 0, NUM_OF_OAM - 1, type );
    SDK_ASSERT( result );
    result = NNS_G2dInitManagerInstanceAffine( pOamManager, 0, NUM_OF_AFFINE - 1 );
    SDK_ASSERT( result );
}

/*---------------------------------------------------------------------------*
  Name:         InitRenderer

  Description:  Renderer �� Surface �����������܂��B

  Arguments:    pRenderer:      ���������� Renderer �ւ̃|�C���^�B
                pMainSurface:   ���������郁�C����� Surface �ւ̃|�C���^�B
                pSubSurface:    ����������T�u��� Surface �ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitRenderer(
                NNSG2dRendererInstance* pRenderer,
                NNSG2dRenderSurface*    pMainSurface,
                NNSG2dRenderSurface*    pSubSurface)
{
    SDK_NULL_ASSERT( pRenderer );
    SDK_NULL_ASSERT( pMainSurface );
    SDK_NULL_ASSERT( pSubSurface );

    NNS_G2dInitRenderSurface( pMainSurface );
    NNS_G2dInitRenderSurface( pSubSurface );

    // ���C����� Surface �����������܂�
    {
        NNSG2dViewRect* pRect = &(pMainSurface->viewRect);

        // �\����`
        pRect->posTopLeft = SURFACE_MAIN_INIT_POS;
        pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
        pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;

        // �R�[���o�b�N�֐�
        pMainSurface->pFuncOamRegister          = CallBackAddOamMain;
        pMainSurface->pFuncOamAffineRegister    = CallBackAddAffineMain;
        pMainSurface->pFuncVisibilityCulling    = CallBackCulling;

        // �\����
        pMainSurface->type                      = NNS_G2D_SURFACETYPE_MAIN2D;
    }

    // �T�u��� Surface �����������܂�
    {
        NNSG2dViewRect* pRect = &(pSubSurface->viewRect);

        // �\����`
        pRect->posTopLeft = SURFACE_SUB_INIT_POS;
        pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
        pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;

        // �R�[���o�b�N�֐�
        pSubSurface->pFuncOamRegister           = CallBackAddOamSub;
        pSubSurface->pFuncOamAffineRegister     = CallBackAddAffineSub;
        pSubSurface->pFuncVisibilityCulling     = CallBackCulling;

        // �\����
        pSubSurface->type                       = NNS_G2D_SURFACETYPE_SUB2D;
    }

    // Renderer �����������܂�
    {
        NNS_G2dInitRenderer( pRenderer );

        // Surface �o�^
        NNS_G2dAddRendererTargetSurface( pRenderer, pMainSurface );
        NNS_G2dAddRendererTargetSurface( pRenderer, pSubSurface );

        // �v���L�V�o�^
        NNS_G2dSetRendererImageProxy( pRenderer, &sImageProxy, &sPaletteProxy );

        // Z �I�t�Z�b�g�������w��
        NNS_G2dSetRendererSpriteZoffset( pRenderer, - FX32_ONE );
    }
}

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  �A�j���[�V�����f�[�^�A�Z���o���N�A�L�����N�^�f�[�^�A
                �p���b�g�f�[�^���t�@�C������ǂݍ��݁A�L�����N�^�f�[�^��
                �p���b�g�f�[�^�����ꂼ��VRAM�Ƀ��[�h���܂��B

  Arguments:    �Ȃ��B

  Returns:      �ǂݍ��񂾃Z���o���N�ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
static NNSG2dCellDataBank* LoadResources(void)
{
    NNSG2dCellDataBank* pCellBank;
    void* pBuf;

    NNS_G2dInitImageProxy( &sImageProxy );
    NNS_G2dInitImagePaletteProxy( &sPaletteProxy );

    //------------------------------------------------------------------------------
    // load cell data
    {
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/Renderer_2LCD.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // �Z���o���N�͍Ō�܂Ń��C����������Ŏg�p����̂�
        // ���� pBuf �͊J�����܂���B
    }

    //------------------------------------------------------------------------------
    // load animation data
    {
        // �Z���A�j���[�V�����f�[�^�͍Ō�܂Ń��C�����������
        // �g�p����̂ł��� pBuf �͊J�����܂���B
        pBuf = G2DDemo_LoadNANR( &spAnimBank, "data/Renderer_2LCD.NANR" );
        SDK_NULL_ASSERT( pBuf );

        //
        // �Z���A�j���[�V�����̎��̂����������܂�
        //
        {
            spCellAnim = G2DDemo_GetNewCellAnimation(1);
            SDK_NULL_ASSERT( spCellAnim );

            SDK_NULL_ASSERT( NNS_G2dGetAnimSequenceByIdx(spAnimBank, 0) );

            NNS_G2dInitCellAnimation(
                spCellAnim,
                NNS_G2dGetAnimSequenceByIdx(spAnimBank, 0),
                pCellBank );
        }
    }

    //------------------------------------------------------------------------------
    // load character data for 2D
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/Renderer_2LCD.NCGR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For Main 2D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            CHARA_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sImageProxy );

        // Loading For Sub 2D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            CHARA_BASE,
            NNS_G2D_VRAM_TYPE_2DSUB,
            &sImageProxy );

        // �L�����N�^�f�[�^�� VRAM �ɃR�s�[�����̂�
        // ���� pBuf �͊J�����܂��B�ȉ������B
        G2DDemo_Free( pBuf );
    }

    // load character data for 3D (software sprite)
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCBR( &pCharData, "data/Renderer_2LCD.NCBR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 3D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            CHARA_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &sImageProxy );

        G2DDemo_Free( pBuf );
    }

    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/Renderer_2LCD.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For Main 2D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            PLTT_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sPaletteProxy);

        // Loading For Sub 2D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            PLTT_BASE,
            NNS_G2D_VRAM_TYPE_2DSUB,
            &sPaletteProxy);

        // Loading For 3D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            PLTT_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &sPaletteProxy);

        G2DDemo_Free( pBuf );
    }

    return pCellBank;
}


/*---------------------------------------------------------------------------*
  Name:         ProcessInput

  Description:  �L�[���͂ɉ������������s���܂��B

  Arguments:    pCellPos:       �Z���̈ʒu�ւ̃|�C���^�B
                pOutputType:    �o�͕��@���i�[�����o�b�t�@�ւ̃|�C���^�B
                pMainPos:       ���C����� Surface �̈ʒu�ւ̃|�C���^�B
                pSubPos:        �T�u��� Surface �̈ʒu�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void ProcessInput(NNSG2dFVec2* pCellPos,
                            NNSG2dSurfaceType* pOutputType,
                            NNSG2dFVec2* pMainPos,
                            NNSG2dFVec2* pSubPos)
{
    int dx = 0;
    int dy = 0;
    SDK_NULL_ASSERT( pCellPos );
    SDK_NULL_ASSERT( pOutputType );

    G2DDemo_ReadGamePad();

    // �\���L�[�̓��͂��擾
    {
        if ( G2DDEMO_IS_PRESS(PAD_KEY_UP) )
        {
            dy--;
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_DOWN) )
        {
            dy++;
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_LEFT) )
        {
            dx--;
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
        {
            dx++;
        }
    }

    if( G2DDEMO_IS_PRESS(PAD_BUTTON_Y) )
    {
        // ���C����� Surface �ړ�
        pMainPos->x += (dx << FX32_SHIFT);
        pMainPos->y += (dy << FX32_SHIFT);
    }
    else if( G2DDEMO_IS_PRESS(PAD_BUTTON_B) )
    {
        // �T�u��� Surface �ړ�
        pSubPos->x += (dx << FX32_SHIFT);
        pSubPos->y += (dy << FX32_SHIFT);
    }
    else
    {
        // �Z���ړ�
        pCellPos->x += (dx << FX32_SHIFT);
        pCellPos->y += (dy << FX32_SHIFT);
    }

    // Reset
    if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_X) )
    {
        *pMainPos = SURFACE_MAIN_INIT_POS;
        *pSubPos  = SURFACE_SUB_INIT_POS;
        *pCellPos = CELL_A_INIT_POS;
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
  Name:         PrintSurfaceRect

  Description:  Surface �̈ʒu����\�����܂��B

  Arguments:    x:          �\���ʒu x�B
                y:          �\���ʒu y�B
                pSurface:   �\���Ώۂ� Surface �ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void PrintSurfaceRect(u8 x, u8 y, const NNSG2dRenderSurface* pSurface)
{
    const NNSG2dViewRect* pRect = &(pSurface->viewRect);

    G2DDemo_PrintOutf(x, y, "(%3d, %3d)-(%3d, %3d)",
        pRect->posTopLeft.x >> FX32_SHIFT,
        pRect->posTopLeft.y >> FX32_SHIFT,
        (pRect->posTopLeft.x + pRect->sizeView.x - FX32_ONE) >> FX32_SHIFT,
        (pRect->posTopLeft.y + pRect->sizeView.y - FX32_ONE) >> FX32_SHIFT);
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C���֐��ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    NNSG2dRendererInstance  render;         // �`��p Renderer
    NNSG2dRenderSurface     mainSurface;    // ���C����� Surface
    NNSG2dRenderSurface     subSurface;     // �T�u��� Surface
    NNSG2dCellDataBank*     pCellBank;      // Cell �f�[�^


    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();

        NNS_G2dInitOamManagerModule();
        InitOamManager( &sMainOamMan, NNS_G2D_OAMTYPE_MAIN );
        InitOamManager( &sSubOamMan, NNS_G2D_OAMTYPE_SUB );
        InitRenderer( &render, &mainSurface, &subSurface );
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
        static NNSG2dFVec2 cellAPos = CELL_A_INIT_POS;  // Cell A �`��ʒu


        // ���͂�ǂݎ�� Cell �� Surface �̈ʒu�A���C����ʂ̏o�͕��@���X�V���܂�
        {
            ProcessInput(
                &cellAPos,
                &(mainSurface.type),
                &(mainSurface.viewRect.posTopLeft),
                &(subSurface.viewRect.posTopLeft));
        }

        // �`��
        //
        // Renderer ���W���[����p���� Cell ��`�悵�܂��B
        //
        {
            NNS_G2dBeginRendering( &render );
                // �Z�� A: �����l�ł̓��C����ʂɕ\���B�L�[����ɂ��ړ��\�B
                NNS_G2dPushMtx();
                    NNS_G2dTranslate( cellAPos.x, cellAPos.y, 0 );
                    NNS_G2dDrawCellAnimation( spCellAnim );
                NNS_G2dPopMtx();

                // �Z�� B: �����l�ł̓��C����ʂɕ\���B�ʒu�Œ�B
                NNS_G2dPushMtx();
                    NNS_G2dTranslate( CELL_B_POS.x, CELL_B_POS.y, 0 );
                    NNS_G2dDrawCellAnimation( spCellAnim );
                NNS_G2dPopMtx();

                // �Z�� C: �����l�ł̓T�u��ʂɕ\���B�ʒu�Œ�B
                NNS_G2dPushMtx();
                    NNS_G2dTranslate( CELL_C_POS.x, CELL_C_POS.y, 0 );
                    NNS_G2dDrawCellAnimation( spCellAnim );
                NNS_G2dPopMtx();
            NNS_G2dEndRendering();
        }

        // �\�����̏o��
        {
            G2DDemo_PrintOutf(0, 0, "cell pos: (%3d, %3d)",
                cellAPos.x >> FX32_SHIFT, cellAPos.y >> FX32_SHIFT);
            G2DDemo_PrintOut(0, 1, "main surface:");
            PrintSurfaceRect(1, 2, &mainSurface);
            G2DDemo_PrintOut(0, 3, "sub surface:");
            PrintSurfaceRect(1, 4, &subSurface);

            G2DDemo_PrintOut(0, 23, (mainSurface.type == NNS_G2D_SURFACETYPE_MAIN2D) ?
                   "OBJ           ": "SoftwareSprite");
        }

        // V Blank �҂�
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();


        //
        // �o�b�t�@�̓��e��HW�ɏ����o���܂�
        //
        {
            // �\�����
            G2DDemo_PrintApplyToHW();

            // Cell
            NNS_G2dApplyAndResetOamManagerBuffer( &sMainOamMan );
            NNS_G2dApplyAndResetOamManagerBuffer( &sSubOamMan );
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


