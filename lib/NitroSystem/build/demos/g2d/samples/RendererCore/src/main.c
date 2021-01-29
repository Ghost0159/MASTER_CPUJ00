/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - RendererCore
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
//      �����_���R�A ���W���[����P�Ƃŗp���ăZ���A�j���[�V������`�悷��T���v��
//      ������_���R�A ���W���[�����g�p���āA2D �`��� 3D�`����s���܂��B
//      �3D�`�� �� ����̃X�v���C�g�������ɕ`�悷�邽�߂�API���g�p���ĕ`����s���܂��B
//       (���̂��߁A������ނ�OBJ����\�������Z���ł͕`�悪�������s���܂���)

//      ������_���R�A ���W���[���̎g�p���@�ɂ��ẮA���C�u�����R�[�h�̃����_����
//       �W���[�����̃R�[�h���Q�l�ɂȂ�܂��B�i���ɐU�镑���̃J�X�^�}�C�Y�Ȃ�...�j
//      
//  ������@
//      START �{�^��    Cell �̏o�͕��@�� OBJ <=> SoftwareSprite ��
//                      �؂�ւ��܂��B
//                      SoftwareSprite �̏ꍇ MakeCellToOams�ł̕`��͍s���܂���B
//      R �{�^��        �\������ Cell ��؂�ւ��܂��B(�ʏ�̃Z���� �P��摜����\�������Z��)
//      �\���L�[        Cell ���ړ����܂��B(�����Ƀ����_���̃t���b�v��Ԃ��ύX���܂��B
//                                          MakeCellToOams�֐� �ł͔��f����܂���B)
//
// 
//  �\�t�g�E�F�A�X�v���C�g�`��Ń����_���R�A�� 
//  �P��摜����\�������Z���ɓ��������`��֐����g�p���ĕ`��������Ȃ��܂��̂ŁA
//  �ʏ�̃Z����`�悵���ꍇ�A�������`�悳��Ȃ��s����m�F���邱�Ƃ��ł��܂��B
//  ���̏ꍇ�� R �{�^�� �� �\���Z���� �P��摜����\�������Z�� �� �ύX���Ă݂Ă��������B
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

//------------------------------------------------------------------------------
// �萔��`
#define SCREEN_WIDTH                256                     // ��ʕ�
#define SCREEN_HEIGHT               192                     // ��ʍ���

#define CELL_INIT_POS_X             (60 << FX32_SHIFT)     // Cell �̏����ʒu X
#define CELL_INIT_POS_Y             (60 << FX32_SHIFT)     // Cell �̏����ʒu Y

#define INIT_OUTPUT_TYPE            NNS_G2D_SURFACETYPE_MAIN2D   // �o�͕��@�̏����l

#define NUM_OF_OAM                  128                 // OAM �}�l�[�W���Ɋ��蓖�Ă� OAM �̐�

#define CHARA_BASE                  0x0                 // �L�����N�^�C���[�W�x�[�X�A�h���X
#define PLTT_BASE                   0x0                 // �p���b�g�x�[�X�A�h���X

// �`��Z���Ɋւ���萔
// NUM_CELL_X * NUM_CELL_Y �� �`��Z�����ł��B
// 
#define NUM_CELL_X 5        
#define NUM_CELL_Y 5
#define STEP_CELL_X 24
#define STEP_CELL_Y 24
  
//------------------------------------------------------------------------------
// �\���̒�`

typedef struct CellInfo
{
    NNSG2dFVec2                 scale;     // �g�嗦
    NNSG2dFVec2                 pos;       // Cell �`��ʒu
    NNSG2dCellAnimation*        pCellAnim; // �Z���A�j��
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
                �Ԃ��܂��B
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

//------------------------------------------------------------------------------
static void ResetRendererSurface( NNSG2dRndCoreSurface*    pSurface )
{
    SDK_NULL_ASSERT( pSurface );
    {
        NNSG2dViewRect* pRect = &(pSurface->viewRect);
        
        pRect->posTopLeft.x = FX32_ONE * 0;
        pRect->posTopLeft.y = FX32_ONE * 0;
        pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
        pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;
        
        pSurface->type                   = NNS_G2D_SURFACETYPE_MAIN2D;
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
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/RendererCore.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // �Z���o���N�͍Ō�܂Ń��C����������Ŏg�p����̂�
        // ���� pBuf �͊J�����܂���B
    }

    //------------------------------------------------------------------------------
    // load animation data
    {
        // �Z���A�j���[�V�����f�[�^�͍Ō�܂Ń��C�����������
        // �g�p����̂ł��� pBuf �͊J�����܂���B
        pBuf = G2DDemo_LoadNANR( &spAnimBank, "data/RendererCore.NANR" );
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

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/RendererCore.NCGR" );
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

    // load charactor data for 3D (software sprite)
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/RendererCore.NCBR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 3D Graphics Engine.
        NNS_G2dLoadImage1DMapping(
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

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/RendererCore.NCLR" );
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

    return pCellBank;
}

//------------------------------------------------------------------------------
static void MtxSetIdentity( MtxFx32* pM )
{
    pM->_00 = FX32_ONE;
    pM->_01 = 0;
    pM->_10 = 0;
    pM->_11 = FX32_ONE;
    pM->_20 = 0;
    pM->_21 = 0;
}


//------------------------------------------------------------------------------
// �����_���R�A���W���[����API���g�p���ĕ`����s���܂��B
// �����_���R�A���W���[���̓����_�����W���[���̕`�揈�������������郂�W���[���ł��B
// �����_���R�A���W���[���𒼐ڎg�p����ꍇ�́A
// �����_�����W���[�����s���Ă���A�s��X�^�b�N�Ǘ��A�p�����[�^���������A�������炪�s���K�v������܂��B
static void DrawByRndCore
( 
    NNSG2dRndCoreInstance*     pRndCore, 
    NNSG2dRndCoreSurface*      pSurface,
    const CellInfo*            pCellInf 
)
{
    MtxFx32                   mtx;
    int                       i,j;
    
    MtxSetIdentity( &mtx );
    //
    // �����_�R�A�ɁA�T�[�t�F�X�ݒ���s��
    //
    NNS_G2dSetRndCoreSurface( pRndCore, pSurface );
    
    //
    // 2D �T�[�t�F�X�ɑ΂���`��Ȃ��...
    //
    if( pSurface->type != NNS_G2D_SURFACETYPE_MAIN3D )
    {
        //
        // 2D �`��p�ɓo�^�֐��̐ݒ�
        //
        NNS_G2dSetRndCoreOamRegisterFunc( pRndCore, 
                                          CallBackAddOam,
                                          CallBackAddAffine );
                                          
        NNS_G2dRndCoreBeginRendering( pRndCore );
        for( i = 0; i < NUM_CELL_X; i++ )
        {
            for( j = 0; j < NUM_CELL_Y; j++ )
            {
                // �`��ʒu�̐ݒ�������_���R�A�ɐݒ肷��s��ɑ΂��čs���܂��B
                mtx._20 = pCellInf->pos.x + FX32_ONE * STEP_CELL_X*i;
                mtx._21 = pCellInf->pos.y + FX32_ONE * STEP_CELL_Y*j;
                
                //
                // �����_���ɍs���ݒ肵�܂��B
                //
                NNS_G2dSetRndCoreCurrentMtx2D( &mtx, NULL );
                
                //
                // �����_�R�AAPI���g�p���ĕ`�揈���������Ȃ��܂��B
                //
                NNS_G2dRndCoreDrawCellFast2D( NNS_G2dGetCellAnimationCurrentCell( pCellInf->pCellAnim ) );
            }
        }
        NNS_G2dRndCoreEndRendering( );
    }else{
       
        // 3D �p Z�l��ݒ肵�܂��B
        NNS_G2dSetRndCore3DSoftSpriteZvalue( pRndCore, -1 );
        NNS_G2dRndCoreBeginRendering( pRndCore );
        // ����̃X�v���C�g�������ɕ`�悷�邽�߂�API���g�p���܂��B
        NNS_G2dSetRndCoreCellCloneSource3D( NNS_G2dGetCellAnimationCurrentCell( pCellInf->pCellAnim ) );
        for( i = 0; i < NUM_CELL_X; i++ )
        {
            for( j = 0; j < NUM_CELL_Y; j++ )
            {
                // �`��ʒu�̐ݒ�������_���R�A�ɐݒ肷��s��ɑ΂��čs���܂��B
                mtx._20 = pCellInf->pos.x + FX32_ONE * STEP_CELL_X*i;
                mtx._21 = pCellInf->pos.y + FX32_ONE * STEP_CELL_Y*j;
                //
                // �����_���Ƀ}�g���N�X�L���b�V����ݒ肵�܂��B
                // (���O��Z�l��
                // NNS_G2dSetRndCore3DSoftSpriteZvalue() �Őݒ肵�Ă����K�v������܂��B)
                //
                NNS_G2dSetRndCoreCurrentMtx3D( &mtx );
                //
                // �����_�R�AAPI���g�p���ĕ`�揈���������Ȃ��܂��B
                // ����̃X�v���C�g�������ɕ`�悷�邽�߂�API���g�p���܂��B
                //
                NNS_G2dRndCoreDrawCellClone3D( NNS_G2dGetCellAnimationCurrentCell( pCellInf->pCellAnim ) );
            }
        }
        NNS_G2dRndCoreEndRendering( );
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
    NNSG2dRndCoreInstance       rndCore;        // �����_���R�A
    NNSG2dRndCoreSurface        surface;        // �����_���R�A�T�[�t�F�X
    
    NNSG2dCellDataBank*         pCellBank;      // Cell �f�[�^
    NNSG2dOamManagerInstance    oamManager;     // 2D�`�掞��OAM�}�l�[�W��
    OSTick                      time;           // �p�t�H�[�}���X�v���p����
    u16                         anmSeqIdx = 0;  // �A�j���V�[�P���X�ԍ�
    
    // Initilizing App.
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();
        InitOamManager( &oamManager );
        
        //
        // �����_���̏�����
        //
        NNS_G2dInitRndCore( &rndCore );
        NNS_G2dInitRndCoreSurface( &surface );
        
        NNS_G2dSetRndCoreOamRegisterFunc( &rndCore, CallBackAddOam, CallBackAddAffine );
        
        ResetRendererSurface( &surface );
        NNS_G2dSetRndCoreImageProxy( &rndCore, &sImageProxy, &sPaletteProxy );
        
        
        pCellBank = LoadResources();
        
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
        static CellInfo cellInfo = {
            {FX32_ONE, FX32_ONE},               // �g�嗦
            {CELL_INIT_POS_X, CELL_INIT_POS_Y}, // Cell �`��ʒu
            NULL
        };

        cellInfo.pCellAnim = spCellAnim;
        //------------------------------------------------------------------------------
        // ���͂�ǂݎ�� Cell �̕\�����Əo�͕��@���X�V���܂�
        {
            G2DDemo_ReadGamePad();
            
            // Move Cell
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
                    NNS_G2dSetRndCoreFlipMode( &rndCore, FALSE, FALSE );
                }
                if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
                {
                    cellInfo.pos.x += FX32_ONE;
                    NNS_G2dSetRndCoreFlipMode( &rndCore, TRUE, FALSE );
                }
            }

            // Reset
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_X) )
            {
                cellInfo.pos = (NNSG2dFVec2){CELL_INIT_POS_X, CELL_INIT_POS_Y};
            }

            // Toggle Output Type
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_START) )
            {
                surface.type = (surface.type == NNS_G2D_SURFACETYPE_MAIN2D) ?
                                    NNS_G2D_SURFACETYPE_MAIN3D:
                                    NNS_G2D_SURFACETYPE_MAIN2D;
            }
            
            //
            // �A�j���؂�ւ�
            //
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_R) )
            {
                anmSeqIdx++;
                anmSeqIdx = (u16)(anmSeqIdx % NNS_G2dGetNumAnimSequence( spAnimBank ));
                
                NNS_G2dInitCellAnimation( spCellAnim, 
                                          NNS_G2dGetAnimSequenceByIdx(spAnimBank, anmSeqIdx ),
                                          pCellBank );
            }
        }
        
        
        //------------------------------------------------------------------------------
        // �`��
        //
        // Renderer ���W���[����p���� Cell ��`�悵�܂��B
        //
        time = OS_GetTick();   
        {
            //
            // �����_���R�A�ɂ��`��
            //
            DrawByRndCore( &rndCore, &surface, &cellInfo );
        }
        time = OS_GetTick() - time;
        
        //------------------------------------------------------------------------------
        // �\�����̏o��
        {
            G2DDemo_PrintOutf(0, 0, "pos:   x=%3d     y=%3d",
                cellInfo.pos.x >> FX32_SHIFT, cellInfo.pos.y >> FX32_SHIFT);   
            G2DDemo_PrintOut(0, 1, (surface.type == NNS_G2D_SURFACETYPE_MAIN2D) ?
                   "OBJ           ": "SoftwareSprite"); 
            
            G2DDemo_PrintOutf( 0, 22, "OBJ_CNT:%04ld", 
                cellInfo.pCellAnim->pCurrentCell->numOAMAttrs * NUM_CELL_X * NUM_CELL_Y );
            G2DDemo_PrintOutf( 0, 23, "TIME   :%06ld usec\n", OS_TicksToMicroSeconds(time) );
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
        
        // �Z���A�j���[�V�����̍X�V
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




