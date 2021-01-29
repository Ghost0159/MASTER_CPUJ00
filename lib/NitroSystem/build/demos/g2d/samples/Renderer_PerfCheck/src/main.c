/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - Renderer_PerfCheck
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.8 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      Renderer ���W���[���̃p�t�H�[�}���X���r���܂��B
//
//      �Renderer ��p���ă}���`�Z���A�j���[�V������`�悵�`��ɂ����������Ԃ�
//       �\�����܂��B
//
//      ��R���p�C���X�C�b�` USE_OPZ_API(�K��l 0)����`����Ă��܂��B
//       USE_OPZ_API �� 1�ɒ�`����ƁA�œK���q���g���T�|�[�g����A
//       NNS_G2dBeginRenderingEx()���g�p���܂��B
//       (�������ANNS_G2dBeginRenderingEx()���T�|�[�g�̃o�[�W�����ł̓r���h�s��
//       �ƂȂ�܂��B)
//
//      �A�{�^���������Ƃ����Ƃ������ɕ`�悪�\�� NNS_G2dMakeSimpleMultiCellTo
//       Oams()�ł̕`��֐؂�ւ��܂��B
//       NNS_G2dMakeSimpleMultiCellToOams()�`�掞�ɂ̓\�t�g�E�F�A�X�v���C�g�`��
//       �͍s���܂���B
//
// 
//  ������@
//      START �{�^��    Cell �̏o�͕��@�� OBJ <=> SoftwareSprite ��
//                      �؂�ւ��܂��B
//      X �{�^��        Cell �̊g��k���ړ���]��Ԃ����Z�b�g���܂��B
//      �\���L�[        Cell ���ړ����܂��B
//      A �{�^��        �����_�����g�p�����`��ƁA
//                      NNS_G2dMakeSimpleMultiCellToOams() �֐��𗘗p�����`���
//                      �؂�ւ��܂��B
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

//------------------------------------------------------------------------------
// �萔��`
#define SCREEN_WIDTH                256                     // ��ʕ�
#define SCREEN_HEIGHT               192                     // ��ʍ���

#define CELL_INIT_POS_X             (120 << FX32_SHIFT)     // Cell �̏����ʒu X
#define CELL_INIT_POS_Y             (120 << FX32_SHIFT)     // Cell �̏����ʒu Y

#define INIT_OUTPUT_TYPE            NNS_G2D_SURFACETYPE_MAIN2D   // �o�͕��@�̏����l

#define NUM_OF_OAM                  128      // OAM �}�l�[�W���Ɋ��蓖�Ă� OAM �̐�

#define CHARA_BASE                  0x0      // �L�����N�^�C���[�W�x�[�X�A�h���X
#define PLTT_BASE                   0x0      // �p���b�g�x�[�X�A�h���X

//
// �{��`���P�ɂ����2004/11/10�ł��V�����ǉ����ꂽ�A�œK��API���g�p���܂��B
// �i�������A�œK��API���T�|�[�g����Ȃ��o�[�W�����ł̓R���p�C�����s�\�ɂȂ�܂��B
//   �܂��A�A�t�B���p�����[�^�ɑ΂��鑀������f����Ȃ��Ȃ�܂��B�j
//
#define USE_OPZ_API 1

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


//------------------------------------------------------------------------------
// �v���g�^�C�v�錾

void VBlankIntr(void);
static void ProcessInput(CellInfo* pCellInfo, NNSG2dSurfaceType* pOutputType, u16 numCells);
static void MakeAffineMatrix(MtxFx22* pMtx, MtxFx22* pMtx2D, const CellInfo* pCellInfo);
static BOOL CallBackAddOam( const GXOamAttr* pOam, u16 affineIndex, BOOL bDoubleAffine );
static u16 CallBackAddAffine( const MtxFx22* mtx );
static void InitOamManager(NNSG2dOamManagerInstance*   pObjOamManager);
static void InitRenderer(
                NNSG2dRendererInstance* pRenderer,
                NNSG2dRenderSurface*    pSurface);


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
static void InitOamManager(NNSG2dOamManagerInstance*   pOamManager)
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
    

    pRect->posTopLeft.x = FX32_ONE * 0;
    pRect->posTopLeft.y = FX32_ONE * 0;
    pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
    pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;

    pSurface->pFuncOamRegister       = CallBackAddOam;
    pSurface->pFuncOamAffineRegister = CallBackAddAffine;

    NNS_G2dAddRendererTargetSurface( pRenderer, pSurface );
    NNS_G2dSetRendererImageProxy( pRenderer, &sImageProxy, &sPaletteProxy );
    NNS_G2dSetRendererSpriteZoffset( pRenderer, -FX32_ONE >> 2 );

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
  Name:         LoadResources

  Description:  �}���`�Z���A�j���[�V�����̍\�z�ƃL�����N�^�f�[�^�A
                �p���b�g�f�[�^�̓ǂݍ��݂��s���܂��B

  Arguments:    ppMCAnim:   �\�z�����}���`�Z���A�j���[�V�����ւ̃|�C���^��
                            �󂯎��|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadResources(NNSG2dMultiCellAnimation** ppMCAnim)
{
    void* pBuf;
    
    NNS_G2dInitImageProxy( &sImageProxy );
    NNS_G2dInitImagePaletteProxy( &sPaletteProxy );
    
    //
    // MultiCell �֘A������
    //
    {
        NNSG2dCellDataBank*             pCellBank = NULL;   // �Z���f�[�^
        NNSG2dCellAnimBankData*         pAnimBank = NULL;   // �Z���A�j���[�V����
        NNSG2dMultiCellDataBank*        pMCBank = NULL;     // �}���`�Z���f�[�^
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
        //
        {
            const NNSG2dMultiCellAnimSequence* pSequence;

            // �Đ�����V�[�P���X���擾
            pSequence = NNS_G2dGetAnimSequenceByIdx( pMCABank, 0 );
            SDK_ASSERT( pSequence );

            // �}���`�Z���A�j���[�V�������\�z
            *ppMCAnim = G2DDemo_GetNewMultiCellAnimation( pAnimBank,
                                                          pCellBank,
                                                          pMCBank,
                                                          NNS_G2D_MCTYPE_SHARE_CELLANIM );
            SDK_NULL_ASSERT( *ppMCAnim );


            // �}���`�Z���A�j���[�V�����ɍĐ�����V�[�P���X���Z�b�g
            NNS_G2dSetAnimSequenceToMCAnimation( *ppMCAnim, pSequence);
        }
    }

    //
    // VRAM �֘A�̏�����
    //
    {
        //------------------------------------------------------------------------------
        // load character data for 2D
        {
            NNSG2dCharacterData* pCharData;

            pBuf = G2DDemo_LoadNCGR( &pCharData, "data/MultiCellAnimation.NCGR" );
            SDK_NULL_ASSERT( pBuf );

            // Loading For 2D Graphics Engine.
            NNS_G2dLoadImage2DMapping(
                pCharData,
                CHARA_BASE,
                NNS_G2D_VRAM_TYPE_2DMAIN,
                &sImageProxy );

            // �L�����N�^�f�[�^�� VRAM �ɃR�s�[�����̂�
            // ���� pBuf �͊J�����܂��B�ȉ������B
            G2DDemo_Free( pBuf );
        }

        // load character data for 3D (software sprite)
        {
            NNSG2dCharacterData* pCharData;

            pBuf = G2DDemo_LoadNCBR( &pCharData, "data/MultiCellAnimation.NCBR" );
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

            pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/MultiCellAnimation.NCLR" );
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

            G2DDemo_Free( pBuf );
        }
    }
}

//------------------------------------------------------------------------------
// �����_�����W���[����API���g�p����
// �}���`�Z����`�悵�܂��B
static void DrawUsingRendererAPI
(
    NNSG2dRendererInstance*         pRender,       // �`��p Renderer
    const NNSG2dFVec2*              pPos,          // �ʒu���
    const NNSG2dMultiCellAnimation* pMCAnim        // MultiCellAnimation ����
)
{
    //
    // USE_OPZ_API ����`����Ă����ꍇ�́A
    // �œK���q���g��^���܂��B�����ł́A
    // �A�t�B���ϊ����g�p���Ȃ��B
    // �p�����[�^�ύX���s��Ȃ��B���w�肵�Ă��܂��B
    //            
#if USE_OPZ_API
    NNS_G2dBeginRenderingEx( pRender, NNS_G2D_RDR_OPZHINT_NOT_SR | 
                                      NNS_G2D_RDR_OPZHINT_LOCK_PARAMS );
#else// USE_OPZ_API
    NNS_G2dBeginRendering( pRender );
#endif//USE_OPZ_API

        NNS_G2dPushMtx();

            NNS_G2dTranslate( pPos->x, pPos->y, 0 );
            NNS_G2dDrawMultiCellAnimation( pMCAnim );

        NNS_G2dPopMtx();
    NNS_G2dEndRendering();
}

//------------------------------------------------------------------------------
// NNS_G2dMakeSimpleMultiCellToOams() ���g�p����
// �}���`�Z����`�悵�܂��B
static void DrawUsingSimpleAPI
(
    NNSG2dOamManagerInstance*       pOamManager,   // 2D�`�掞��OAM�}�l�[�W��
    const NNSG2dFVec2*              pPos,          // �ʒu���
    const NNSG2dMultiCellAnimation* pMCAnim        // MultiCellAnimation ����
)
{
    static GXOamAttr    temp[NUM_OF_OAM];   
    u16                 numOamDrawn = 0;           // �`�� OAM ��
    //
    // MultiCell �Ɠ����� OBJ���X�g���e���|�����ɏ����o���܂�
    //
    numOamDrawn = NNS_G2dMakeSimpleMultiCellToOams(
                        temp,                           // �o�͐�Oam�o�b�t�@
                        NUM_OF_OAM,                     // �o�͐�o�b�t�@��
                        &pMCAnim->multiCellInstance,    // �o�͑Ώ� MultiCell
                        NULL,                           // Affine �ϊ�
                        pPos,                           // �I�t�Z�b�g�ʒu
                        NULL,                           // Affine Index
                        FALSE );                        // �{�pAffine���H

    SDK_ASSERT( numOamDrawn < NUM_OF_OAM );

    // OAM�}�l�[�W����OBJ���X�g��o�^���܂�
    (void)NNS_G2dEntryOamManagerOam(
            pOamManager,
            temp,
            numOamDrawn );
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C���֐��ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    // �`��p Renderer
    NNSG2dRendererInstance      render;         
    // ���C����� Surface
    NNSG2dRenderSurface         surface;        
    // 2D�`�掞��OAM�}�l�[�W��
    NNSG2dOamManagerInstance    oamManager;     
    // �`����@�̎��
    NNSG2dSurfaceType           outputType = INIT_OUTPUT_TYPE;
    // �}���`�Z���A�j���[�V���� ����
    NNSG2dMultiCellAnimation*   pMCAnim = NULL; 
    
    //------------------------------------------
    // �A�v���P�[�V�����̏�����
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();
        InitOamManager( &oamManager );
        InitRenderer( &render, &surface );
        LoadResources( &pMCAnim );
        
        spCurrentOam = &oamManager;
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
        // �����_�����g�p���ĕ`��������Ȃ����H
        static BOOL     bUsingRenderer = TRUE;
        // �`��ɂ����鎞��
        static OSTick   timeDraw       = 0;
        // �Z���̈ʒu���Ȃ�
        static CellInfo cellInfo = {
            0,                                  // �`�� Cell �ԍ�
            0,                                  // ��]�p
            {FX32_ONE, FX32_ONE},               // �g�嗦
            {CELL_INIT_POS_X, CELL_INIT_POS_Y}  // Cell �`��ʒu
        };

        //------------------------------------------
        // ���͂�ǂݎ�� Cell �̕\�����Əo�͕��@���X�V���܂�
        {
            ProcessInput(&cellInfo, &outputType, 0);
            surface.type = outputType;
            
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_A) )
            {
                bUsingRenderer = bUsingRenderer^TRUE;
            }
        }
        

        //------------------------------------------
        // �`��
        // �`�揈���ɂ����������Ԃ��v�����Ă��܂�
        {
            timeDraw = OS_GetTick();
            
            // �����_�����g�p���邩�H
            if( bUsingRenderer )
            {
                DrawUsingRendererAPI( &render, &cellInfo.pos, pMCAnim );
            }else{
                //
                // �\�t�g�E�G�A�X�v���C�g�`��Ȃ�Ώ������Ȃ��B
                //
                if( outputType != NNS_G2D_SURFACETYPE_MAIN3D )
                {
                    DrawUsingSimpleAPI( &oamManager, &cellInfo.pos, pMCAnim );
                }
            }
                
            timeDraw = OS_GetTick() - timeDraw;
        }
        
        
        //------------------------------------------
        // �\�����̏o��
        {
            // ��ʏ㕔
            G2DDemo_PrintOutf(0, 0, "pos:   x=%3d     y=%3d",
                cellInfo.pos.x >> FX32_SHIFT, cellInfo.pos.y >> FX32_SHIFT);
            
            // ��ʉ���
            G2DDemo_PrintOutf( 0, 21, "TIME-draw  :%06ld usec\n", OS_TicksToMicroSeconds(timeDraw) );
            G2DDemo_PrintOut(0, 22, (outputType == NNS_G2D_SURFACETYPE_MAIN2D) ?
                   "OBJ           ": "SoftwareSprite");
            G2DDemo_PrintOut(0, 23, (bUsingRenderer) ?
                   "UsingRenderer           ": "UsingMakeCellToOams");
            
            // NNS_G2dMakeSimpleMultiCellToOams()�`�掞�ɂ̓\�t�g�E�F�A�X�v���C�g�`��͍s���܂���B
            if( outputType != NNS_G2D_SURFACETYPE_MAIN2D && !bUsingRenderer )
            {
                G2DDemo_PrintOutf( 5, 10, "Not supported." );
            }else{
                G2DDemo_PrintOutf( 5, 10, "              " );
            }
        }
        
        //------------------------------------------
        // V Blank �҂�
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();

        //------------------------------------------
        //
        // �o�b�t�@�̓��e��HW�ɏ����o���܂�
        //
        {
            // �\�����
            G2DDemo_PrintApplyToHW();

            // Cell
            NNS_G2dApplyOamManagerToHW( &oamManager );
            NNS_G2dResetOamManagerBuffer( &oamManager );
        }
        
        //------------------------------------------
        //
        // �A�j���[�V�������X�V���܂�
        //
        NNS_G2dTickMCAnimation( pMCAnim, FX32_ONE );
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


