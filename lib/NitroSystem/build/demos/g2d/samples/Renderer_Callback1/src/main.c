/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - Renderer_Callback1
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      Renderer ���W���[����OBJ�`��R�[���o�b�N��p����
//      �e�Q�[���^�C�g���ŗL�̓��ꏈ�����s���T���v���ł��B
//
//      ��ʂɕ\�������L�����N�^��OBJ�̕\���v���C�I���e�B��ʒu���R�[���o
//      �b�N���ő��삵�܂��B
//
//  ������@
//      �\���L�[        Cell ���ړ����܂��B
//      B �{�^��        �����Ă���Ƃ��A�R�[���o�b�N�������ɂȂ�܂��B
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

//------------------------------------------------------------------------------
// �e��萔��`
#define SCREEN_WIDTH                256                     // ��ʕ�
#define SCREEN_HEIGHT               192                     // ��ʍ���

#define AFFINE_IDX                  3                       // Cell �p Affine Index
#define CELL_ROTATE_UNIT            0xFF                    // Cell �̒P�ʉ�]��
#define CELL_SCALE_UNIT             ((FX32_ONE >> 7) * 3 )  // Cell �̒P�ʊg��k����
                                                            // scale �l�� 0 �t�߂̒l�ɂȂ�Ȃ��悤�ɂ��Ă���܂�
#define CELL_INIT_POS_X             (120 << FX32_SHIFT)     // Cell �̏����ʒu X
#define CELL_INIT_POS_Y             (120 << FX32_SHIFT)     // Cell �̏����ʒu Y

#define INIT_OUTPUT_TYPE            NNS_G2D_SURFACETYPE_MAIN2D   // �o�͕��@�̏����l

#define NUM_OF_OAM                  128                 // OAM �}�l�[�W���Ɋ��蓖�Ă� OAM �̐�

#define CHARA_BASE                  0x0                 // �L�����N�^�C���[�W�x�[�X�A�h���X
#define PLTT_BASE                   0x0                 // �p���b�g�x�[�X�A�h���X



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

static NNSG2dScreenData*            pScrDataBG1;    // BG�p�X�N���[���f�[�^
static NNSG2dSVec2                  randomDiff;     // ���������_���ړ���

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
  Name:         CallBackBeforeOBJ

  Description:  �����_��OBJ�`��R�[���o�b�N�F
                �����_��OBJ�`��O�R�[���o�b�N�Ƃ��ēo�^���āA�e�Q�[���^�C�g���ŗL
                �̃n�b�L���O���s���܂��B
                �����ł́A�Z���̓��������̈ʒu�����炵�A�����g�����̕`��D��x��ύX���Ă��܂��B

  Arguments:    pRend               �����_��
                pSurface            �T�[�t�F�X
                pCell               �Z��
                oamIdx              OAM�ԍ�
                pMtx                �s��

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
static void CallBackBeforeOBJ
(
    struct NNSG2dRendererInstance*         pRend,
    struct NNSG2dRenderSurface*            pSurface,
    const NNSG2dCellData*                  pCell,
    u16                                    oamIdx,
    const MtxFx32*                         pMtx
)
{
#pragma unused( pSurface ) 
#pragma unused( pCell ) 
#pragma unused( pMtx ) 
    GXOamAttr*    pTempOam = &pRend->rendererCore.currentOam;
    //
    // �p�����[�^��������
    //
    {
        // 0 -- 3 �� ���� ������킷      
        if( oamIdx <= 3 )
        {
            // �ʒu�����炷
            pTempOam->x += randomDiff.x;                    
            pTempOam->y += randomDiff.y;
        }
        
        // 4��5�͉����g 6 �͉e ������킷
        if( oamIdx >= 4 )
        {
            // �`��D��x�ύX
            pTempOam->priority = 0;
        }
    }
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
    pSurface->type = NNS_G2D_SURFACETYPE_MAIN2D;

    NNS_G2dAddRendererTargetSurface( pRenderer, pSurface );
    NNS_G2dSetRendererImageProxy( pRenderer, &sImageProxy, &sPaletteProxy );
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
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/Rdr_CallBack1.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // �Z���o���N�͍Ō�܂Ń��C����������Ŏg�p����̂�
        // ���� pBuf �͊J�����܂���B
    }

    //------------------------------------------------------------------------------
    // load animation data
    {
        // �Z���A�j���[�V�����f�[�^�͍Ō�܂Ń��C�����������
        // �g�p����̂ł��� pBuf �͊J�����܂���B
        pBuf = G2DDemo_LoadNANR( &spAnimBank, "data/Rdr_CallBack1.NANR" );
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

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/Rdr_CallBack1_OBJ.NCGR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        NNS_G2dLoadImage1DMapping(
            pCharData,
            CHARA_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sImageProxy );

        // �L�����N�^�f�[�^�� VRAM �ɃR�s�[�����̂�
        // ���� pBuf �͊J�����܂��B�ȉ������B
        G2DDemo_Free( pBuf );
    }
    
    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/Rdr_CallBack1_OBJ.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            PLTT_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sPaletteProxy);

        G2DDemo_Free( pBuf );
    }
    
    //------------------------------------------------------------------------------
    // load screen data
    {
        NNSG2dPaletteData*      pPltData;
        NNSG2dCharacterData*    pChrData;
        NNSG2dScreenData*       pScrData;
        void*   pBuf2;
        void*   pBuf3;

        pBuf  = G2DDemo_LoadNSCR( &pScrDataBG1, "data/Rdr_CallBack1_BG0.NSCR" );
        pBuf2 = G2DDemo_LoadNCLR( &pPltData, "data/Rdr_CallBack1_BG.NCLR" );
        pBuf3 = G2DDemo_LoadNCGR( &pChrData, "data/Rdr_CallBack1_BG.NCGR" );
        
        SDK_NULL_ASSERT( pBuf );
        SDK_NULL_ASSERT( pBuf2 );
        SDK_NULL_ASSERT( pBuf3 );

        NNS_G2dBGSetup(
            NNS_G2D_BGSELECT_MAIN1,
            pScrDataBG1,
            pChrData,
            pPltData,
            GX_BG_SCRBASE_0x0000,
            GX_BG_CHARBASE_0x08000
        );
        G2_SetBG1Priority(0);
        
        pBuf  = G2DDemo_LoadNSCR( &pScrData, "data/Rdr_CallBack1_BG1.NSCR" );
        NNS_G2dBGSetup(
            NNS_G2D_BGSELECT_MAIN2,
            pScrData,
            pChrData,
            pPltData,
            GX_BG_SCRBASE_0x1800,
            GX_BG_CHARBASE_0x10000
        );
        G2_SetBG2Priority(1);
        G2DDemo_Free( pBuf2 );
        G2DDemo_Free( pBuf3 );
        
    }
    
    return pCellBank;
}

//------------------------------------------------------------------------------
// ��ʈʒu�̃X�N���[���f�[�^�̃L�����N�^�ԍ����擾����
static u16 GetBG1ScreenCharNo( int posX, int posY )
{
    const u16*  pData = (const u16*)pScrDataBG1->rawData;
    
    posX %= pScrDataBG1->screenWidth;
    posY %= pScrDataBG1->screenHeight;
    
    pData += (posY >> 3) * (pScrDataBG1->screenWidth >> 3) + (posX >> 3);
    
    // �L�����N�^�ԍ���Ԃ�
    return (u16)(*pData & 0x3FF);
}

//------------------------------------------------------------------------------
// �Z�������g���������H��Ɉʒu���邩�H
static BOOL IsOnLoad( int x, int y )
{
    const u16 bgNo = GetBG1ScreenCharNo( x, y );
    
    // ���H������킷�L�����N�^�ԍ����H
    if( bgNo == 0x20 || bgNo == 0x21 || bgNo == 0x40 || bgNo == 0x41 )
    {
        return TRUE;
    }else{
        return FALSE;
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
        
        GX_SetVisiblePlane( GX_PLANEMASK_BG0 | 
                            GX_PLANEMASK_BG1 | 
                            GX_PLANEMASK_BG2 | 
                            GX_PLANEMASK_OBJ );
        
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

        //
        // �p�b�h���͂�ǂݎ�� Cell �̕\�����Əo�͕��@���X�V���܂�
        // 
        {
            G2DDemo_ReadGamePad();    
            //
            // �Z���̈ړ�
            //
            {
                if ( G2DDEMO_IS_PRESS(PAD_KEY_UP) )
                {
                    cellInfo.pos.y -= FX32_ONE;
                }
                if ( G2DDEMO_IS_PRESS(PAD_KEY_DOWN) )
                {
                    cellInfo.pos.y += FX32_ONE;
                }
                if ( G2DDEMO_IS_PRESS(PAD_KEY_LEFT) )
                {
                    cellInfo.pos.x -= FX32_ONE;
                }
                if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
                {
                    cellInfo.pos.x += FX32_ONE;
                }
            }
            //
            // OBJ�P�ʃR�[���o�b�N�� ON/OFF
            //
            if( !G2DDEMO_IS_PRESS(PAD_BUTTON_B) )
            {
                const int x = cellInfo.pos.x >> FX32_SHIFT;
                const int y = cellInfo.pos.y >> FX32_SHIFT;
                const int OBJ_SIZE = 16;
                
                // �Z����4���̂������Âꂩ���A���H�̏�Ȃ��...
                if( IsOnLoad( x           , y               )  ||
                    IsOnLoad( x + OBJ_SIZE, y               )  ||
                    IsOnLoad( x           , y - OBJ_SIZE    )  ||
                    IsOnLoad( x + 16      , y - OBJ_SIZE)           )
                {
                    surface.pBeforeDrawOamBackFunc = CallBackBeforeOBJ;
                }else{
                    surface.pBeforeDrawOamBackFunc = NULL;
                }
            }else{
                surface.pBeforeDrawOamBackFunc = NULL;
            }
        }

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
        }
        
        
        // V Blank �҂�
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();


        //
        // �o�b�t�@�̓��e��HW�ɏ����o���܂�
        //
        {
            // Cell
            NNS_G2dApplyOamManagerToHW( &oamManager );
            NNS_G2dResetOamManagerBuffer( &oamManager );
        }
        
        // �X�V
        {
            //
            // �����̗h�炬�ʍX�V
            //
            {
                randomDiff.x = 0;
                randomDiff.y = 0;
                
                if( OS_GetTick() % 5 == 0 )
                {
                    randomDiff.x = 3;
                }
                
                if( OS_GetTick() % 7 == 0 )
                {
                    randomDiff.y = 3;
                }
            }
            //
            // �Z���A�j���X�V
            //
            NNS_G2dTickCellAnimation( spCellAnim, FX32_ONE );
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


