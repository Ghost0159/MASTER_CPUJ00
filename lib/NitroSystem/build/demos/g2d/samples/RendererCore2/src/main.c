/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - RendererCore2
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
//      RendererCore ���W���[����p���ăZ���A�j���[�V������`�悷��T���v��
//      
//      �RendererCore1�Ƃقړ���̓��e�����T���v���f���ł��B
//
//      ������_�����W���[���𗘗p���Ȃ���A�K�v�ɉ����ă����_���R�A���W���[��API��
//       ���p����悤�ȃP�[�X���f�����Ă��܂��B
//       (�����_�����W���[���͓����Ń����_���R�A���W���[�����g�p���Ă��邽�߁A
//        ��L�̂悤�ȉ^�p���\�ł��j      
//  
//      ������_�����W���[��API
//       �����_���R�A���W���[��API
//       ���̑��`��API �̃p�t�H�[�}���X�̔�r���s���܂��B
//
//  ������@
//      START �{�^��    Cell �̏o�͕��@�� OBJ <=> SoftwareSprite ��
//                      �؂�ւ��܂��B
//                      SoftwareSprite �̏ꍇ MakeCellToOams�ł̕`��͍s���܂���B
//      R �{�^��        �\������ Cell ��؂�ւ��܂��B(�ʏ�̃Z���� �P��摜����\�������Z��)
//      L �{�^��        �`����@��ύX���܂��B
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


#define SCREEN_WIDTH                256                     // ��ʕ�
#define SCREEN_HEIGHT               192                     // ��ʍ���

#define CELL_INIT_POS_X             (60 << FX32_SHIFT)     // Cell �̏����ʒu X
#define CELL_INIT_POS_Y             (60 << FX32_SHIFT)     // Cell �̏����ʒu Y

#define INIT_OUTPUT_TYPE            NNS_G2D_SURFACETYPE_MAIN2D   // �o�͕��@�̏����l

#define NUM_OF_OAM                  128                 // OAM �}�l�[�W���Ɋ��蓖�Ă� OAM �̐�

#define CHARA_BASE                  0x0                 // �L�����N�^�C���[�W�x�[�X�A�h���X
#define PLTT_BASE                   0x0                 // �p���b�g�x�[�X�A�h���X



//------------------------------------------------------------------------------
// �`��Z���Ɋւ���萔
// NUM_CELL_X * NUM_CELL_Y �� �`��Z�����ł����A��OBJ����128�𒴂����ꍇ�́A
// MyDrawMethod == MYDRAWMETHOD_CELLTOOAM �ȊO�̏ꍇ�A
// ���������f�������m�ȃp�t�H�[�}���X��r���ł��Ȃ��Ȃ�̂Œ��ӂ��K�v�ł��B
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

typedef enum 
{
    MYDRAWMETHOD_RENDERER,
    MYDRAWMETHOD_RENDERERCORE,
    MYDRAWMETHOD_CELLTOOAM,
    MYDRAWMETHOD_MAX

}MyDrawMethod;

const char*         drawTypeStrTbl[MYDRAWMETHOD_MAX]=
{
    "MYDRAWMETHOD_RENDERER    ",
    "MYDRAWMETHOD_RENDERERCORE",
    "MYDRAWMETHOD_CELLTOOAM   ",

};

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
  Name:         CallBackAddAffine

  Description:  �����_���R�A �`��֐������� OAM ��ǉ����邽�߂�
                �Ă΂��֐��ł��B
                ����OAM���ꊇ���ēo�^���܂��B
                EndRendering �̃^�C�~���O�ň�񂾂��Ă΂�܂��B

  Arguments:    pOam:           �ǉ����ׂ� OAM�� �ւ̃|�C���^�B
                num:            �ǉ�����OAM���B

  Returns:      ���������ꍇ�� TRUE�A�����łȂ���� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static BOOL CallBackAddOamBlock( const GXOamAttr* pOam, u16 num )
{
    return NNS_G2dEntryOamManagerOam( spCurrentOam, pOam, num );
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
static void ResetRendererSurface( NNSG2dRenderSurface*    pSurface )
{
    SDK_NULL_ASSERT( pSurface );
    {
        NNSG2dViewRect* pRect = &(pSurface->viewRect);
        
        pRect->posTopLeft.x = FX32_ONE * 0;
        pRect->posTopLeft.y = FX32_ONE * 0;
        pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
        pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;

        pSurface->pFuncOamRegister       = CallBackAddOam;
        pSurface->pFuncOamAffineRegister = CallBackAddAffine;
        
        pSurface->type                   = NNS_G2D_SURFACETYPE_MAIN2D;
    }
    
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
                NNSG2dRenderSurface*    pSurface )
{
    NNSG2dViewRect* pRect = &(pSurface->viewRect);
    SDK_NULL_ASSERT( pRenderer );
    SDK_NULL_ASSERT( pSurface );

    NNS_G2dInitRenderer( pRenderer );
    
    NNS_G2dInitRenderSurface( pSurface );
    
    ResetRendererSurface( pSurface );
    
    NNS_G2dAddRendererTargetSurface( pRenderer, pSurface );
    NNS_G2dSetRendererImageProxy( pRenderer, &sImageProxy, &sPaletteProxy );
    
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
// �����_�����W���[�����g�p���ĕ`����s���܂��B
static void DrawByRenderer
( 
    NNSG2dRendererInstance*     pRdr, 
    const CellInfo*             pCellInf 
)
{
    int                         i,j;
    //
    // NNS_G2dBeginRendering()�̑���ɍœK���I�v�V�������w�肷��
    // NNS_G2dBeginRenderingEx �� �g�p���Ă��܂��B
    //
    NNS_G2dBeginRenderingEx( pRdr, NNS_G2D_RDR_OPZHINT_NOT_SR | 
                                   NNS_G2D_RDR_OPZHINT_LOCK_PARAMS );
                                   
    for( i = 0; i < NUM_CELL_X; i++ )
    {
        for( j = 0; j < NUM_CELL_Y; j++ )
        {
            //
            // �{�f���̏ꍇ�Ȃ�΁APush Pop �͕s�v�ł��B
            // ���̂悤�ȏꍇ�́A���L�̂悤�ɋL�q����Ə����������悭�Ȃ�܂��B
            //
            NNS_G2dSetTrans( pCellInf->pos.x + FX32_ONE * STEP_CELL_X*i, 
                             pCellInf->pos.y + FX32_ONE * STEP_CELL_Y*j, 
                                  0 );
            NNS_G2dDrawCellAnimation( pCellInf->pCellAnim );
        }
    }
    NNS_G2dEndRendering();
}          
  


//------------------------------------------------------------------------------
// �����_���R�A���W���[����API���g�p���ĕ`����s���܂��B
// �����_���R�A���W���[���̓����_�����W���[���̕`�揈�������������郂�W���[���ł��B
// �����_���R�A���W���[���𒼐ڎg�p����ꍇ�́A
// �����_�����W���[�����s���Ă���A�s��X�^�b�N�Ǘ��A�p�����[�^���������A�������炪�s���K�v������܂��B
static void DrawByRndCore
( 
    NNSG2dRendererInstance*     pRdr, 
    const CellInfo*             pCellInf 
)
{
    NNSG2dRndCoreInstance*    pRndCore = &pRdr->rendererCore;
    NNSG2dRenderSurface*      pSurface = pRdr->pTargetSurfaceList;
    MtxFx32                   mtx;
    int                       i,j;
    
    MtxSetIdentity( &mtx );
    
    
    //
    // �����_�R�A�ɁA�T�[�t�F�X�ݒ���s��
    //
    NNS_G2dSetRndCoreSurface( pRndCore, &pSurface->coreSurface );
    
    //
    // 2D �T�[�t�F�X�ɑ΂���`��Ȃ��...
    //
    if( pSurface->type != NNS_G2D_SURFACETYPE_MAIN3D )
    {
        //
        // 2D �`��p�ɓo�^�֐��̐ݒ�
        //
        NNS_G2dSetRndCoreOamRegisterFunc  ( pRndCore, 
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
                // �����_���Ƀ}�g���N�X�L���b�V����ݒ肵�܂��B
                // (���O��Z�l��ݒ肵�Ă����K�v������܂��B)
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
        //
        // 3D �p Z�l��ݒ肵�܂��B
        //
        NNS_G2dSetRndCore3DSoftSpriteZvalue( pRndCore, -1 );
        NNS_G2dRndCoreBeginRendering( pRndCore );
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
                // (���O��Z�l��ݒ肵�Ă����K�v������܂��B)
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

//------------------------------------------------------------------------------
// NNS_G2dMakeCellToOams ���g�p���ĕ`����s���܂��B
// �����Ƃ������ɓ��삵�܂����A�t���@�\�����Ȃ��Ȃ��Ă��܂��B
static void DrawByMakeOamsFunc
( 
    NNSG2dOamManagerInstance*   pOamMgr, 
    const CellInfo*             pCellInf 
)
{   
    static GXOamAttr          temp[NUM_OF_OAM];
    u16                       numOamBuffer = NUM_OF_OAM;                  // �c�� OAM ��
    u16                       numDrawn;
    GXOamAttr*                pBuffer = temp;
    NNSG2dFVec2               trans;
    int                       i,j;
    
    
    
    
    for( i = 0; i < NUM_CELL_X; i++ )
    {
        for( j = 0; j < NUM_CELL_Y; j++ )
        {
            trans.x = pCellInf->pos.x + FX32_ONE * STEP_CELL_X*i;
            trans.y = pCellInf->pos.y + FX32_ONE * STEP_CELL_Y*j;
            
            if( numOamBuffer <= 0 )
            {
                return;
            }
            //
            // MultiCell �Ɠ����� OBJ���X�g���e���|�����ɏ����o���܂�
            //
            numDrawn = NNS_G2dMakeCellToOams(
                                pBuffer,           // �o�͐�Oam�o�b�t�@
                                numOamBuffer,   // �o�͐�o�b�t�@��
                                NNS_G2dGetCellAnimationCurrentCell( pCellInf->pCellAnim ), // �o�͑Ώ� Cell
                                NULL,           // Affine �ϊ�
                                &trans,         // �I�t�Z�b�g�ʒu
                                NULL,           // Affine Index
                                FALSE );        // �{�pAffine���H
            
            numOamBuffer    -= numDrawn;
            pBuffer         += numDrawn;
            
            SDK_ASSERT(numOamBuffer < NUM_OF_OAM);
        }
    }
    // OAM�}�l�[�W����OBJ���X�g��o�^���܂�
    (void)NNS_G2dEntryOamManagerOam(
            pOamMgr,
            temp,
            (u16)( NUM_OF_OAM - numOamBuffer ) );
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
    BOOL                        bDrawByCore = FALSE;
    MyDrawMethod                drawMethod = MYDRAWMETHOD_RENDERERCORE;
    
    OSTick                      time;
    u16                         anmSeqIdx = 0;
    
    // Initilizing App.
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();
        InitOamManager( &oamManager );
        InitRenderer( &render, &surface );
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
            {CELL_INIT_POS_X, CELL_INIT_POS_Y},  // Cell �`��ʒu
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
                    NNS_G2dSetRendererFlipMode( &render, FALSE, FALSE );
                }
                if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
                {
                    cellInfo.pos.x += FX32_ONE;
                    NNS_G2dSetRendererFlipMode( &render, TRUE, FALSE );
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
            
        
            // �`����@�̕ύX
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_L) )
            {
                drawMethod = (MyDrawMethod)(++drawMethod % MYDRAWMETHOD_MAX);
                //
                // �ēx�T�[�t�F�X�̏��������s��
                // �����_�����W���[�������Őݒ肵�Ă��郌���_�R�A�p�R�[���o�b�N�������Z�b�g���邽�߁B
                //
                {
                    if( drawMethod == MYDRAWMETHOD_RENDERER )
                    {
                        NNS_G2dInitRenderSurface( &surface );
                        ResetRendererSurface( &surface );
                        
                    }else if( drawMethod == MYDRAWMETHOD_RENDERERCORE ){
                        
                        NNS_G2dInitRndCoreSurface( &surface.coreSurface );
                        ResetRendererSurface( &surface );
                    }
                }
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
        // �l�X�ȕ`��API��p���� Cell ��`�悵�܂��B
        //
        time = OS_GetTick();   
        {
            switch( drawMethod )
            {
            case MYDRAWMETHOD_RENDERER:
                //
                // �����_���ɂ��`��
                //                
                DrawByRenderer( &render, &cellInfo );break;            
            case MYDRAWMETHOD_RENDERERCORE:
                //
                // �����_���R�A�ɂ��`��
                //
                DrawByRndCore( &render, &cellInfo );break;            
            case MYDRAWMETHOD_CELLTOOAM:
                //
                // NNS_G2dMakeCellToOams() ���g�p�����`��
                //     �\�t�g�E�F�A�X�v���C�g�`��̏ꍇ�͏������܂���B
                if( surface.type != NNS_G2D_SURFACETYPE_MAIN3D )
                {
                    DrawByMakeOamsFunc( &oamManager, &cellInfo );break;
                }
            }
        }
        time = OS_GetTick() - time;
        
        //------------------------------------------------------------------------------
        // �\�����̏o��
        {
            G2DDemo_PrintOutf(0, 0, "pos:   x=%3d     y=%3d",
                cellInfo.pos.x >> FX32_SHIFT, cellInfo.pos.y >> FX32_SHIFT);   
            G2DDemo_PrintOut(0, 1, drawTypeStrTbl[drawMethod] );
            G2DDemo_PrintOut(0, 2, (surface.type == NNS_G2D_SURFACETYPE_MAIN2D) ?
                   "OBJ           ": "SoftwareSprite"); 
            
            G2DDemo_PrintOutf( 0, 22, "OBJ_CNT:%04ld", 
                cellInfo.pCellAnim->pCurrentCell->numOAMAttrs * NUM_CELL_X * NUM_CELL_Y );
            G2DDemo_PrintOutf( 0, 23, "TIME   :%06ld usec\n", OS_TicksToMicroSeconds(time) );
            
            // NNS_G2dMakeSimpleMultiCellToOams()�`�掞�ɂ̓\�t�g�E�F�A�X�v���C�g�`��͍s���܂���B
            if( surface.type == NNS_G2D_SURFACETYPE_MAIN3D && drawMethod == MYDRAWMETHOD_CELLTOOAM )
            {
                G2DDemo_PrintOutf( 5, 10, "Not supported." );
            }else{
                G2DDemo_PrintOutf( 5, 10, "              " );
            }
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
