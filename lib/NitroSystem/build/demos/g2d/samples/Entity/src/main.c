/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - Entity
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.10 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      �G���e�B�e�B��p���ăZ���A�j���[�V������\������B
//      �J���[�p���b�g��؂�ւ��ăL�����N�^�̐F��ύX����B
//
//  ������@
//      START �{�^��    Cell �̏o�͕��@�� OBJ <=> SoftwareSprite ��
//                      �؂�ւ��܂��B
//      A �{�^��        �A�j���[�V�����V�[�P���X�؂�ւ�
//      X �{�^��        �J���[�p���b�g�؂�ւ�
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>
#include <string.h>

#include "g2d_demolib.h"


#define SCREEN_WIDTH                256     // ��ʕ�
#define SCREEN_HEIGHT               192     // ��ʍ���

#define NUM_OF_OAM                 128      // OAM �}�l�[�W�����Ǘ����� OAM �̐�
#define NUM_OF_AFFINE               1       // OAM �}�l�[�W�����Ǘ����� Affine �p�����[�^�̐�
#define NUM_OF_ALT_PLT              4       // �p���b�g�̒u�������Ɏg����p���b�g�̐�
#define PLT_SWAP_TARGET             8       // �u���������̃p���b�g�ԍ�



//------------------------------------------------------------------------------
// �O���[�o���ϐ�

// OAM �}�l�[�W��
static NNSG2dOamManagerInstance         sOamManager;

//------------------------------------------------------------------------------
// �v���g�^�C�v�錾

static inline u16 GetNumOfEntitySequence( NNSG2dEntity* pEntity );
static BOOL CallBackAddOam( const GXOamAttr* pOam, u16 affineIndex, BOOL bDoubleAffine );
static u16 CallBackAddAffine( const MtxFx22* mtx );
static void LoadEntity(
    NNSG2dEntity* pEntity,
    const char* pathbase
);
static void LoadResources(
                NNSG2dEntity*               pEntity,
                NNSG2dImageProxy*           pImgProxy,
                NNSG2dImagePaletteProxy*    pPltProxy);
static void InitializeOamManager(NNSG2dOamManagerInstance* pManager);
static void InitRenderer(
                NNSG2dRendererInstance*     pRenderer,
                NNSG2dRenderSurface*        pSurface,
                NNSG2dImageProxy*           pImgProxy,
                NNSG2dImagePaletteProxy*    pPltProxy);


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
    SDK_NULL_ASSERT( pOam );
    
    return NNS_G2dEntryOamManagerOamWithAffineIdx( &sOamManager, pOam, affineIndex );
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
    SDK_NULL_ASSERT( mtx );

    return NNS_G2dEntryOamManagerAffine( &sOamManager, mtx );
}


/*---------------------------------------------------------------------------*
  Name:         LoadEntity

  Description:  ���\�[�X�t�@�C����ǂݍ��� Entity ���\�z���܂��B

  Arguments:    pEntity:    �ǂݍ��� Entity ���󂯎�� Entity �ւ̃|�C���^�B
                pathbase:   Entity �̊e���\�[�X�t�@�C��������g���q��
                            ������������ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadEntity(
    NNSG2dEntity* pEntity,
    const char* pathbase
){
    size_t length = strlen( pathbase );
    char*                   filepath;
    char*                   extPos;
    NNSG2dCellDataBank*     pCellBank = NULL;
    NNSG2dAnimBankData*     pAnimBank = NULL;
    NNSG2dEntityDataBank*   pEntityBank = NULL;
    SDK_NULL_ASSERT( pEntity );
    SDK_NULL_ASSERT( pathbase );


    filepath = (char*)G2DDemo_Alloc( length + 6 );
    SDK_NULL_ASSERT( filepath );

    extPos = filepath + length;
    (void)strcpy( filepath, pathbase );

    // load entity resource
    {
        void* pBuf;

        (void)strcpy( extPos, ".NCER" );
        pBuf = G2DDemo_LoadNCER( &pCellBank, filepath );
        SDK_NULL_ASSERT( pBuf );

        (void)strcpy( extPos, ".NANR" );
        pBuf = G2DDemo_LoadNANR( &pAnimBank, filepath );
        SDK_NULL_ASSERT( pBuf );

        (void)strcpy( extPos, ".NENR" );
        pBuf = G2DDemo_LoadNENR( &pEntityBank, filepath );
        SDK_NULL_ASSERT( pBuf );
    }

    // construct an entity
    {
        NNSG2dCellAnimation *pCellAnim = G2DDemo_GetNewCellAnimation(1);
        SDK_NULL_ASSERT( pCellAnim );

        NNS_G2dInitCellAnimation(
            pCellAnim,
            NNS_G2dGetAnimSequenceByIdx( pAnimBank, 0 ),
            pCellBank );

        NNS_G2dInitEntity(
            pEntity,
            pCellAnim,
            NNS_G2dGetEntityDataByIdx( pEntityBank, 0 ),
            pAnimBank );

        // ��L���������ł� Entity �̃A�j���[�V�����V�[�P���X������������������܂���B
        // NNS_G2dSetEntityCurrentAnimation �ŏ����l���w�肷��K�v������܂��B
        NNS_G2dSetEntityCurrentAnimation( pEntity, 0 );

        SDK_ASSERT( NNS_G2dIsEntityValid( pEntity ) );
    }

    G2DDemo_Free( filepath );
}



/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  ���\�[�X��ǂݍ��݂܂��B

  Arguments:    pEntity:    �ǂݍ��� Entity ���󂯎�� Entity �ւ̃|�C���^�B
                pImgProxy:  �ǂݍ��񂾃L�����N�^�̏����i�[���� ImageProxy�B
                pPltProxy:  �ǂݍ��񂾃p���b�g�̏����i�[���� PaletteProxy�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadResources(
                NNSG2dEntity*               pEntity,
                NNSG2dImageProxy*           pImgProxy,
                NNSG2dImagePaletteProxy*    pPltProxy)
{
    void* pBuf;

    NNS_G2dInitImageProxy( pImgProxy );
    NNS_G2dInitImagePaletteProxy( pPltProxy );

    // load resources.

    //------------------------------------------------------------------------------
    // load entity
    {
        LoadEntity( pEntity, "data/Entity" );
    }

    //------------------------------------------------------------------------------
    // load character data for 2D
    {
        NNSG2dCharacterData*        pCharData = NULL;

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/Entity.NCGR" );
        SDK_NULL_ASSERT( pBuf );

        // For 2D Graphics Engine
        NNS_G2dLoadImage2DMapping( pCharData, 0, NNS_G2D_VRAM_TYPE_2DMAIN, pImgProxy );

        G2DDemo_Free( pBuf );
    }

    // load character data for 3D (for software sprite)
    {
        NNSG2dCharacterData*        pCharData = NULL;

        pBuf = G2DDemo_LoadNCBR( &pCharData, "data/Entity.NCBR" );
        SDK_NULL_ASSERT( pBuf );

        // For 3D Graphics Engine
        NNS_G2dLoadImage2DMapping( pCharData, 0, NNS_G2D_VRAM_TYPE_3DMAIN, pImgProxy );

        G2DDemo_Free( pBuf );
    }


    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData*        pPltData = NULL;

        pBuf = G2DDemo_LoadNCLR( &pPltData, "data/Entity.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // For 3D Graphics Engine
        NNS_G2dLoadPalette( pPltData, 0, NNS_G2D_VRAM_TYPE_3DMAIN, pPltProxy );
        // For 2D Graphics Engine
        NNS_G2dLoadPalette( pPltData, 0, NNS_G2D_VRAM_TYPE_2DMAIN, pPltProxy );

        G2DDemo_Free( pBuf );
    }
}



/*---------------------------------------------------------------------------*
  Name:         InitRenderer

  Description:  Renderer �� Surface �����������܂��B

  Arguments:    pRenderer:  ���������� Renderer �ւ̃|�C���^�B
                pSurface:   ���������ARenderer �ɐݒ肷�� Surface �ւ̃|�C���^�B
                pImgProxy:  Renderer �ɐݒ肷�� ImageProxy �ւ̃|�C���^�B
                pPltProxy:  Renderer �ɐݒ肷�� PaletteProxy�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitRenderer(
                NNSG2dRendererInstance*     pRenderer,
                NNSG2dRenderSurface*        pSurface,
                NNSG2dImageProxy*           pImgProxy,
                NNSG2dImagePaletteProxy*    pPltProxy)
{
    NNSG2dViewRect* pRect = &(pSurface->viewRect);
    SDK_NULL_ASSERT( pRenderer );
    SDK_NULL_ASSERT( pSurface );
    SDK_NULL_ASSERT( pImgProxy );
    SDK_NULL_ASSERT( pPltProxy );

    NNS_G2dInitRenderer( pRenderer );
    NNS_G2dInitRenderSurface( pSurface );

	// �T�[�t�F�C�X�͈̔͂�ݒ肵�܂�
    pRect->posTopLeft.x = FX32_ONE * 0;
    pRect->posTopLeft.y = FX32_ONE * 0;
    pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
    pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;

    pSurface->pFuncOamRegister       = CallBackAddOam;
    pSurface->pFuncOamAffineRegister = CallBackAddAffine;
    pSurface->type = NNS_G2D_SURFACETYPE_MAIN3D;

	// Renderer �ɐݒ肵�܂�
    NNS_G2dAddRendererTargetSurface( pRenderer, pSurface );
    NNS_G2dSetRendererImageProxy( pRenderer, pImgProxy, pPltProxy );
}



/*---------------------------------------------------------------------------*
  Name:         GetNumOfEntitySequence

  Description:  Entity �ɓo�^����Ă���V�[�P���X�̐����擾����B

  Arguments:    pEntity:    �����Ώۂ� Entity�B

  Returns:      Entity �ɓo�^����Ă���V�[�P���X�̐�
*---------------------------------------------------------------------------*/
static inline u16 GetNumOfEntitySequence( NNSG2dEntity* pEntity )
{
    return pEntity->pEntityData->animData.numAnimSequence;
}



/*---------------------------------------------------------------------------*
  Name:         InitializeOamManager

  Description:  OAM �}�l�[�W�������������܂��B

  Arguments:    pManager:   �����Ώۂ� OAM �}�l�[�W��

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitializeOamManager(NNSG2dOamManagerInstance* pManager)
{
    BOOL    bSuccess;

    bSuccess = NNS_G2dGetNewManagerInstance( pManager, 0, NUM_OF_OAM - 1, NNS_G2D_OAMTYPE_MAIN );
    SDK_ASSERT( bSuccess );

    bSuccess = NNS_G2dInitManagerInstanceAffine( pManager, 0, NUM_OF_AFFINE - 1 );
    SDK_ASSERT( bSuccess );
}



/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C���֐��ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    static NNSG2dEntity                 entity;     // Entity
    static NNSG2dRendererInstance       renderer;   // Renderer
    static NNSG2dRenderSurface          surface;    // Surface
    static NNSG2dImageProxy             imgProxy;   // ImageProxy
    static NNSG2dImagePaletteProxy      pltProxy;   // PaletteProxy
    static NNSG2dPaletteSwapTable       pltSwpTbl;  // �p���b�g�u���e�[�u��

    // Initialize App.
    {
        u16 i;
        u16* pTbl = pltSwpTbl.paletteIndex;

        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();

        LoadResources( &entity, &imgProxy, &pltProxy );
        NNS_G2dInitOamManagerModule();
        InitializeOamManager( &sOamManager );
        InitRenderer( &renderer, &surface, &imgProxy, &pltProxy );

    	// �p���b�g�u���e�[�u����������
        for( i = 0; i < NNS_G2D_NUM_COLOR_PALETTE; ++i )
        {
            pTbl[i] = i;
        }
        NNS_G2dSetEntityPaletteTable( &entity, &pltSwpTbl );
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
        static u16      rotate      = 0;
        static fx32     animSpeed   = FX32_ONE;
        static u16      animIdx    = 0;
        static u16      pltIdx     = 0;

        //
        // Key handling.
        //
        {
            G2DDemo_ReadGamePad();

            // �V�[�P���X�؂�ւ�
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_A ) )
            {
                animIdx++;
                if( animIdx >= GetNumOfEntitySequence( &entity ) )
                {
                    animIdx = 0;
                }
                NNS_G2dSetEntityCurrentAnimation( &entity, animIdx );
            }

            // �`�惂�[�h�ؑ�
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_START ) )
            {
                if( surface.type == NNS_G2D_SURFACETYPE_MAIN2D )
                {
                    surface.type = NNS_G2D_SURFACETYPE_MAIN3D;
                }else{
                    surface.type = NNS_G2D_SURFACETYPE_MAIN2D;
                }
            }

            // �p���b�g�؂�ւ�
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_X ) )
            {
                pltIdx++;
                if( pltIdx > NUM_OF_ALT_PLT )
                {
                    pltIdx = 0;
                }

                if( pltIdx == 0 )
                {
                	// �u�����s��Ȃ�
                    pltSwpTbl.paletteIndex[PLT_SWAP_TARGET] = PLT_SWAP_TARGET;
                }
                else
                {
                    pltSwpTbl.paletteIndex[PLT_SWAP_TARGET] = pltIdx;
                }
            }
        }

        //
        // draw
        //
        {
            NNS_G2dBeginRendering( &renderer );
                NNS_G2dPushMtx();
                    NNS_G2dTranslate( FX32_ONE * 120, FX32_ONE * 100, 0 );
                    NNS_G2dDrawEntity( &entity );
                NNS_G2dPopMtx();
            NNS_G2dEndRendering();
        }

        // �\����Ԃ��o��
        {
            G2DDemo_PrintOutf(0, 0, "sequence no.: %d", animIdx);
            G2DDemo_PrintOutf(0, 1, "palette swap: %d", pltIdx);

            G2DDemo_PrintOut(0, 23, (surface.type == NNS_G2D_SURFACETYPE_MAIN2D) ?
                                "OBJ           ": "SoftwareSprite");
        }

        // SoftwareSprite �̏o�͔��f
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);

        // V �u�����N�҂�
        SVC_WaitVBlankIntr();

        //
        // copy Oam datas from manager's buffer to 2D HW.
        //
        {
            // BG�i�\����ԕ�����j�𔽉f
            G2DDemo_PrintApplyToHW();

            // OBJ �𔽉f
            NNS_G2dApplyOamManagerToHW( &sOamManager );
            NNS_G2dResetOamManagerBuffer( &sOamManager );
        }

        //
        // Tick animation
        //
        {
            NNS_G2dTickEntity( &entity, animSpeed );
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


