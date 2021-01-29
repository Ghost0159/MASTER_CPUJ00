/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - Renderer_CharChange
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
//      Renderer ���W���[����p���ăZ���A�j���[�V������`�悷��T���v��
//        �Renderer �̎g�p���� �摜�v���N�V�i2�j��؂�ւ��ĕ`����s���܂��B
//        ��摜�v���N�V�̂����̂ЂƂ́AVRAM�ɓǂݍ��܂ꂽ�摜�f�[�^�̈ꕔ��
//         ���������������܂��B
//        ����̍ۂɁAg2dcvtr.exe(g2d�o�C�i���R���o�[�^) 
//         �� -cr/�I�v�V���� (�L�����N�^�t�@�C���ϊ��̈�w��I�v�V����)�𗘗p����
//         �f�[�^���쐬���Ă��܂��B
//
//      �{�v���W�F�N�g�̃��C�N�t�@�C���ɋL�q����Ă���R���o�[�g�I�v�V������A
//      data/src �t�H���_�ȉ��̃��\�[�X�f�[�^�����킹�ĎQ�Ƃ��Ă��������ƁA
//      �������������Ă��������₷���Ǝv���܂��B        
//
//  ������@
//                      
//      A �{�^��      �g�p����摜�v���N�V�̐؂�ւ�  
//                      
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

//------------------------------------------------------------------------------
// �萔��`
#define SCREEN_WIDTH                256                     // ��ʕ�
#define SCREEN_HEIGHT               192                     // ��ʍ���

#define NUM_OF_OAM                  128                     // OAM �}�l�[�W���Ɋ��蓖�Ă� OAM �̐�

#define CHARA_BASE                  0x0                     // �L�����N�^�C���[�W�x�[�X�A�h���X
#define PLTT_BASE                   0x0                     // �p���b�g�x�[�X�A�h���X

#define APP_NUM_IMG_PROXY           2                       // �摜�v���N�V�̐�

//------------------------------------------------------------------------------
// �O���[�o���ϐ�

static NNSG2dImageProxy             sImageProxy[APP_NUM_IMG_PROXY];    // Cell �p�L�����N�^/�e�N�X�`���v���L�V
static NNSG2dImagePaletteProxy      sPaletteProxy;                     // Cell �p�p���b�g�v���L�V

static NNSG2dOamManagerInstance     oamManager;     // �����_�����g�p����OAM�}�l�[�W��

static NNSG2dAnimBankData*          spAnimBank;     // �A�j���[�V�����o���N
static NNSG2dCellAnimation*         spCellAnim;     // �Z���A�j���[�V����
static NNSG2dCellDataBank*          spCellBank;     // �Z�����o���N

//------------------------------------------------------------------------------
// �v���g�^�C�v�錾



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

    return NNS_G2dEntryOamManagerOamWithAffineIdx( &oamManager, pOam, affineIndex );
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
    return NNS_G2dEntryOamManagerAffine( &oamManager, mtx );
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
static void InitRenderer
(
    NNSG2dRendererInstance* pRenderer,
    NNSG2dRenderSurface*    pSurface
)
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
    
    // �R�[���o�b�N�̏�����
    pSurface->pFuncOamRegister       = CallBackAddOam;
    pSurface->pFuncOamAffineRegister = CallBackAddAffine;

    pSurface->type = NNS_G2D_SURFACETYPE_MAIN2D;
    
    // �`��ΏۃT�[�t�F�X�̐ݒ�
    NNS_G2dAddRendererTargetSurface( pRenderer, pSurface );
    
    // �摜�v���N�V�A�p���b�g�v���N�V�̐ݒ�A
    NNS_G2dSetRendererImageProxy( pRenderer, &sImageProxy[0], &sPaletteProxy );
}


//------------------------------------------------------------------------------
static u32 InitImageProxy
( 
    NNSG2dImageProxy* pImageProxy,
    const char*       pNcgrFileName,
    u32               characterBaseOffset
)
{
    SDK_NULL_ASSERT( pNcgrFileName );
    NNS_G2dInitImageProxy( pImageProxy );
    
    //------------------------------------------------------------------------------
    // load character data for 2D
    {
        NNSG2dCharacterData* pCharData  = NULL;
        void*                pBuf       = NULL;
        
        pBuf = G2DDemo_LoadNCGR( &pCharData, pNcgrFileName );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            characterBaseOffset,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            pImageProxy );
            
        // �L�����N�^�f�[�^�� VRAM �ɃR�s�[�����̂�
        // ���� pBuf �͊J�����܂��B
        G2DDemo_Free( pBuf );
        
        return pCharData->szByte;
    }
}

/*---------------------------------------------------------------------------*
  Name:         OverWrriteVramImage_

  Description:  �L�����N�^�f�[�^�𕔕��I�ɏ㏑�����܂�

  Arguments:    �Ȃ��B

  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
static void OverWrriteVramImage_( void* pNcgrFile )
{
    NNSG2dCharacterPosInfo* pCharPosInfo = NULL;
    
    if( NNS_G2dGetUnpackedCharacterPosInfo( pNcgrFile, &pCharPosInfo ) )
    {
    
    }
}

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  �A�j���[�V�����f�[�^�A�Z���o���N�A�L�����N�^�f�[�^�A
                �p���b�g�f�[�^���t�@�C������ǂݍ��݁A�L�����N�^�f�[�^��
                �p���b�g�f�[�^�����ꂼ��VRAM�Ƀ��[�h���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
static void LoadResources(void)
{
    void* pBuf;

    
    NNS_G2dInitImagePaletteProxy( &sPaletteProxy );

    //------------------------------------------------------------------------------
    // load cell data
    {
        pBuf = G2DDemo_LoadNCER( &spCellBank, "data/PartialCharCvt.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // �Z���o���N�͍Ō�܂Ń��C����������Ŏg�p����̂�
        // ���� pBuf �͊J�����܂���B
    }

    //------------------------------------------------------------------------------
    // load animation data
    {
        // �Z���A�j���[�V�����f�[�^�͍Ō�܂Ń��C�����������
        // �g�p����̂ł��� pBuf �͊J�����܂���B
        pBuf = G2DDemo_LoadNANR( &spAnimBank, "data/PartialCharCvt.NANR" );
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
                NNS_G2dGetAnimSequenceByIdx( spAnimBank, 0), spCellBank );
        }
    }

    //------------------------------------------------------------------------------
    // load character data for 2D
    {
        int i = 0;
        u32 szLoaded = 0;
        
        const char* ncgrFileNames [] = 
        {
            "data/PartialCharCvt0.NCGR",
            "data/PartialCharCvt0.NCGR"
        };
        
        for( i = 0 ; i < APP_NUM_IMG_PROXY; i++ )
        { 
            szLoaded += InitImageProxy( &sImageProxy[i], 
                                        ncgrFileNames[i], 
                                        CHARA_BASE + szLoaded );
        }
        
        //
        // sImageProxy[1] ���Ǘ����Ă���VRAM�̈�𕔕��I�ɏ㏑�����܂��B
        //
        {
            const u32 addr = NNS_G2dGetImageLocation( &sImageProxy[1], NNS_G2D_VRAM_TYPE_2DMAIN );

            NNSG2dCharacterData*    pCharData       = NULL;
            NNSG2dCharacterPosInfo* pCharPosInfo    = NULL;
            void*                   pBuf            = NULL;
        
            pBuf = G2DDemo_LoadNCGREx( &pCharData, &pCharPosInfo, "data/PartialCharCvt1.NCGR" );
            SDK_NULL_ASSERT( pBuf );
            
            DC_FlushAll();
            {
                const int szChar = ( pCharData->pixelFmt == GX_TEXFMT_PLTT16 ) ? 32 : 64;
                const int offset = ( pCharPosInfo->srcPosY * pCharPosInfo->srcW ) * szChar;
                GX_LoadOBJ( (void*)pCharData->pRawData, addr + offset, pCharData->szByte );
            }
            
            // �L�����N�^�f�[�^�� VRAM �ɃR�s�[�����̂�
            // ���� pBuf �͊J�����܂��B
            G2DDemo_Free( pBuf );
        }
    }
    

    
    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/PartialCharCvt.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            PLTT_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sPaletteProxy);

        G2DDemo_Free( pBuf );
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
    int                         imgProxyIdx = 0;
    
    // �A�v���P�[�V���������������܂�
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();
        InitOamManager( &oamManager );
        InitRenderer( &render, &surface );
        LoadResources();
    }

    // �`����J�n���܂�
    {
        SVC_WaitVBlankIntr();
        GX_DispOn();
        GXS_DispOn();
    }

    // Main loop
    while( TRUE )
    {
        // �p�b�h���͏���
        {
            G2DDemo_ReadGamePad();
            
            // A �{�^���ŁA�g�p����摜�v���N�V��ύX���܂�
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_A) )
            {
                imgProxyIdx++;
                imgProxyIdx &= (APP_NUM_IMG_PROXY - 1);
                
                NNS_G2dSetRendererImageProxy( &render, 
                                              &sImageProxy[imgProxyIdx], 
                                              &sPaletteProxy );
            }
        }
        

        // �`��
        //
        // Renderer ���W���[����p���� Cell ��`�悵�܂��B
        //
        {
            NNS_G2dBeginRendering( &render );
                NNS_G2dPushMtx();
                    NNS_G2dTranslate( 120 * FX32_ONE, 120 * FX32_ONE, 0 );
                    NNS_G2dDrawCellAnimation( spCellAnim );
                NNS_G2dPopMtx();
            NNS_G2dEndRendering();
        }

        // �\�����̏o��
        {
            G2DDemo_PrintOutf(0, 0, "ImageProxyIdx : %d", imgProxyIdx );
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


