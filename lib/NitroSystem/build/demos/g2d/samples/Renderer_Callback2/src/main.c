/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - Renderer_Callback2
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.7 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      Renderer ���W���[����OBJ�`��O�R�[���o�b�N���g�p���āA
//      OBJ�P�ʂ̉��J�����O���s���T���v���ł��B
//      
//      ��ʂ�2��ʂɂ܂�����傫�ȉ��M�̃Z�����\������܂��B
//      �T���v���f���́ANNS_G2dDrawCellAnimation() �̏����ɂ����鎞�Ԃ��f�o�b�N�\�����܂��B
//
//  ������@
//      START �{�^��    �J�����O�� ON,OFF
//      X �{�^��        Cell �y�� Surface �̈ʒu�����Z�b�g���܂��B
//      �\���L�[        Cell ���ړ����܂��B
//      Y + �\���L�[    ���C�� Surface ���ړ����܂��B
//      A               �Z���̉�]�� ON OFF���܂��B
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"


//------------------------------------------------------------------------------
// �萔��`
#define SCREEN_WIDTH        256                 // ��ʕ�
#define SCREEN_HEIGHT       192                 // ��ʍ���

#define NUM_OF_OAM          128                 // OAM �}�l�[�W���Ɋ��蓖�Ă� OAM �̐�
#define NUM_OF_AFFINE       (NUM_OF_OAM / 4)    // OAM �}�l�[�W���Ɋ��蓖�Ă� Affine�p�����[�^ �̐�

#define CHARA_BASE          0x0                 // �L�����N�^�C���[�W�x�[�X�A�h���X
#define PLTT_BASE           0x0                 // �p���b�g�x�[�X�A�h���X

#define INIT_OUTPUT_TYPE    NNS_G2D_SURFACETYPE_MAIN2D   // �o�͕��@�̏����l

// �T�[�t�F�X�����ʒu
#define SURFACE_MAIN_INIT_POS ((NNSG2dFVec2){0 << FX32_SHIFT, 0 << FX32_SHIFT})             
// �T�[�t�F�X�����ʒu
#define SURFACE_SUB_INIT_POS ((NNSG2dFVec2){0 << FX32_SHIFT, SCREEN_HEIGHT << FX32_SHIFT})  
// �Z�������ʒu
#define CELL_INIT_POS ((NNSG2dFVec2){150 << FX32_SHIFT, 120 << FX32_SHIFT})   



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

//------------------------------------------------------------------------------
// ��`��View�������肷��B
static BOOL IsRectInView_
( 
    const fx32  px,
    const fx32  py,
    const fx32 _min_x, 
    const fx32 _max_x,
    const fx32 _min_y,
    const fx32 _max_y,
    const MtxFx32*          pMtx,
    const NNSG2dViewRect*   pViewRect
)
{
        s32 max_x, min_x;
    s32 max_y, min_y;
    s32 temp;
    
    // ��]�g��k���ϊ���̕\���ő�ŏ�XY���W�����߂�
    min_x = FX_Mul(_min_x, pMtx->_00) + FX_Mul(_min_y, pMtx->_10);  // ����
    max_x = min_x;
    
    temp  = FX_Mul(_min_x, pMtx->_00) + FX_Mul(_max_y, pMtx->_10);  // ����
    if      (temp < min_x) { min_x = temp; }
    else if (temp > max_x) { max_x = temp; }
    temp  = FX_Mul(_max_x, pMtx->_00) + FX_Mul(_min_y, pMtx->_10);  // �E��
    if      (temp < min_x) { min_x = temp; }
    else if (temp > max_x) { max_x = temp; }
    temp  = FX_Mul(_max_x, pMtx->_00) + FX_Mul(_max_y, pMtx->_10);  // �E��
    if      (temp < min_x) { min_x = temp; }
    else if (temp > max_x) { max_x = temp; }
    
    min_y = FX_Mul(_min_x, pMtx->_01) + FX_Mul(_min_y, pMtx->_11);  // ����
    max_y = min_y;
    
    temp  = FX_Mul(_min_x, pMtx->_01) + FX_Mul(_max_y, pMtx->_11);  // ����
    if      (temp < min_y) { min_y = temp; }
    else if (temp > max_y) { max_y = temp; }
    
    temp  = FX_Mul(_max_x, pMtx->_01) + FX_Mul(_min_y, pMtx->_11);  // �E��
    if      (temp < min_y) { min_y = temp; }
    else if (temp > max_y) { max_y = temp; }
    
    temp  = FX_Mul(_max_x, pMtx->_01) + FX_Mul(_max_y, pMtx->_11);  // �E��
    if      (temp < min_y) { min_y = temp; }
    else if (temp > max_y) { max_y = temp; }
    
    // �Z���̕\���ʒu���_�����Z����
    min_x  += px;
    max_x  += px;
    min_y  += py;
    max_y  += py;
    
    // �T�[�t�F�[�X���O����
    if( (max_y > 0) && (min_y < pViewRect->sizeView.y) )
    {
        if( (max_x > 0) && (min_x < pViewRect->sizeView.x) )
        {
            return TRUE;
        }
    }
    return FALSE;
}
//------------------------------------------------------------------------------
// OBJ �`��O�R�[���o�b�N�Ƃ���OBJ�J�����O�֐�����������
// OBJ��CELL �Ɣ�r����� �����Ȃ̂ŁA�J�����O�ɂ�鏈�����ׂ̑����ʂ�
// �����_�ɒ��ӂ���K�v������܂��B
static void OBJCullingFunc_
(
    struct NNSG2dRendererInstance*         pRend,
    struct NNSG2dRenderSurface*            pSurface,
    const  NNSG2dCellData*                 pCell,
    u16                                    oamIdx,
    const MtxFx32*                         pMtx
)
{
#pragma unused( pCell )
#pragma unused( oamIdx )
    {
        const GXOamAttr* pOam = &pRend->rendererCore.currentOam;
        const GXOamShape shape = NNS_G2dGetOAMSize( pOam ); 
        
        const fx32 minX = NNS_G2dRepeatXinCellSpace( (s16)pOam->x ) * FX32_ONE ;
        const fx32 maxX = ( NNS_G2dRepeatXinCellSpace( (s16)pOam->x ) + NNS_G2dGetOamSizeX( &shape ) ) * FX32_ONE;
        const fx32 minY = NNS_G2dRepeatYinCellSpace( (s16)pOam->y ) * FX32_ONE;
        const fx32 maxY = ( NNS_G2dRepeatYinCellSpace( (s16)pOam->y ) + NNS_G2dGetOamSizeY( &shape ) ) * FX32_ONE;
        
        const fx32  px = pMtx->_20 - pSurface->viewRect.posTopLeft.x;
        const fx32  py = pMtx->_21 - pSurface->viewRect.posTopLeft.y;
                    
        if( IsRectInView_( px, py, minX, maxX, minY, maxY, pMtx, &pSurface->viewRect ) )
        {
            pRend->rendererCore.bDrawEnable = TRUE;
        }else{
            pRend->rendererCore.bDrawEnable = FALSE;
        }
    }   
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
        
        // �\����
        pMainSurface->type                      = NNS_G2D_SURFACETYPE_MAIN2D;
        
        pMainSurface->pBeforeDrawOamBackFunc = OBJCullingFunc_;
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
        
        // �\����
        pSubSurface->type                       = NNS_G2D_SURFACETYPE_SUB2D;
        
        pSubSurface->pBeforeDrawOamBackFunc = OBJCullingFunc_;
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
  Name:         LoadResource

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
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/Rdr_CallBack2.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // �Z���o���N�͍Ō�܂Ń��C����������Ŏg�p����̂�
        // ���� pBuf �͊J�����܂���B
    }

    //------------------------------------------------------------------------------
    // load animation data
    {
        // �Z���A�j���[�V�����f�[�^�͍Ō�܂Ń��C�����������
        // �g�p����̂ł��� pBuf �͊J�����܂���B
        pBuf = G2DDemo_LoadNANR( &spAnimBank, "data/Rdr_CallBack2.NANR" );
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

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/Rdr_CallBack2.NCGR" );
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

    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/Rdr_CallBack2.NCLR" );
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
        G2DDemo_Free( pBuf );
    }

    return pCellBank;
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


    // Initilizing App.
    {
        G2DDemo_CommonInit();
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
        static NNSG2dFVec2 cellAPos = CELL_INIT_POS;  // Cell A �`��ʒu
        static u16         priority = 0;
        static u16         plttNo   = 0;
        static BOOL        bRotate = FALSE;
        static u16         rot = 0x0;
        static OSTick      time;

        //------------------------------------------------------
        // �p�b�h���͂�ǂݎ��A�f�[�^���X�V���܂�
        {
            int dx = 0;
            int dy = 0;
            
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
                mainSurface.viewRect.posTopLeft.x += (dx << FX32_SHIFT);
                mainSurface.viewRect.posTopLeft.y += (dy << FX32_SHIFT);
            }
            else
            {
                // �Z���ړ�
                cellAPos.x += (dx << FX32_SHIFT);
                cellAPos.y += (dy << FX32_SHIFT);
            }

            // ���Z�b�g
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_X) )
            {
                cellAPos = CELL_INIT_POS;
                mainSurface.viewRect.posTopLeft = SURFACE_MAIN_INIT_POS;
                subSurface.viewRect.posTopLeft  = SURFACE_SUB_INIT_POS;
            }
            
            // ��] ON/OFF           
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_A) )
            {
                bRotate = bRotate^TRUE;
            }
            
            //
            // �I�u�W�F�P�ʂ̉��J�����O�֐� ON/OFF�B
            //
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_START) )
            {
                if( mainSurface.pBeforeDrawOamBackFunc == NULL )
                {
                    mainSurface.pBeforeDrawOamBackFunc  = OBJCullingFunc_;
                    subSurface.pBeforeDrawOamBackFunc   = OBJCullingFunc_;
                }else{
                    mainSurface.pBeforeDrawOamBackFunc  = NULL;
                    subSurface.pBeforeDrawOamBackFunc   = NULL;
                }
            }
        }
        //------------------------------------------------------
        // �`��
        //
        // Renderer ���W���[����p���� Cell ��`�悵�܂��B
        //
        {
            NNS_G2dBeginRendering( &render );
                    
                // �Z�� �L�[����ɂ��ړ��\�B
                NNS_G2dPushMtx();
                    NNS_G2dTranslate( cellAPos.x, cellAPos.y, 0 );
                    
                    if( bRotate )
                    {
                        NNS_G2dRotZ( FX_SinIdx( rot ), FX_CosIdx( rot ) );
                    }
                    
                    // �`��ɂ����鎞�Ԃ��v��
                    time = OS_GetTick();
                        NNS_G2dDrawCellAnimation( spCellAnim );
                    time = OS_GetTick() - time;
                    
                NNS_G2dPopMtx();
                
            NNS_G2dEndRendering();
        }
        //------------------------------------------------------
        // �\�����̏o��
        {
            G2DDemo_PrintOutf( 0, 0, "TimeForDraw  : %06ld usec\n", OS_TicksToMicroSeconds(time) );
            G2DDemo_PrintOutf( 0, 1, "OBJ-Culling  : %s", ( mainSurface.pBeforeDrawOamBackFunc != NULL ) ? "ON ":"OFF");
            G2DDemo_PrintOutf( 0, 2, "Rotate       : %s", ( bRotate ) ? "ON ":"OFF");
        }
        //------------------------------------------------------
        // V Blank �҂�
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();

        //------------------------------------------------------
        //
        // �o�b�t�@�̓��e��HW�ɏ����o���܂�
        //
        {
            // Cell
            NNS_G2dApplyAndResetOamManagerBuffer( &sMainOamMan );
            NNS_G2dApplyAndResetOamManagerBuffer( &sSubOamMan );
            
            // �\�����
            G2DDemo_PrintApplyToHW();
        }
        
        //------------------------------------------------------
        // �V�[���X�V
        rot += 0x100;
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


