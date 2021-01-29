/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - Cell_Simple
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.13 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      Cell�̒P���ȕ`��������Ȃ��T���v��
//
//  ����
//      �OAM�}�l�[�W�����W���[���𗘗p���Ă��܂��B
//      �OBJ�ɂ��`��ƃ\�t�g�E�F�A�X�v���C�g�ɂ��`���؂�ւ��ĕ`�悵�Ă��܂��B
//      ��Z���̓A�j���[�V�������s���܂���B
//
//  ������@
//      START �{�^��    Cell �̏o�͕��@�� OBJ <=> SoftwareSprite ��
//                      �؂�ւ��܂��B
//      A �{�^��        �\������ Cell ��؂�ւ��܂�(�؂�ւ���Z�������݂���Ȃ�)�B
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


#define AFFINE_IDX                  3                       // Cell �p Affine Index
#define CELL_ROTATE_UNIT            0xFF                    // Cell �̒P�ʉ�]��
#define CELL_SCALE_UNIT             ((FX32_ONE >> 7) * 3)   // Cell �̒P�ʊg��k����
                                                            // scale �l�� 0 �t�߂̒l�ɂȂ�Ȃ��悤�ɂ��Ă���܂�
#define CELL_INIT_POS_X             (120 << FX32_SHIFT)     // Cell �̏����ʒu X
#define CELL_INIT_POS_Y             (120 << FX32_SHIFT)     // Cell �̏����ʒu Y

#define INIT_OUTPUT_TYPE            NNS_G2D_OAMTYPE_MAIN    // �o�͕��@�̏����l

#define NUM_OF_OAM                  128                     // OAM �}�l�[�W���Ɋ��蓖�Ă� OAM �̐�
#define NUM_OF_AFFINE               (NUM_OF_OAM / 4)        // OAM �}�l�[�W���Ɋ��蓖�Ă� Affine �p�����[�^�̐�

#define TEX_BASE                    0x0                     // �e�N�X�`���x�[�X�A�h���X
#define TEX_PLTT_BASE               0x0                     // �e�N�X�`���p���b�g�x�[�X�A�h���X



//------------------------------------------------------------------------------
// �\���̒�`

//------------------------------------------------------------------------------
//
// �Z���̏��
//
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

static NNSG2dCellDataBank*          spCellBank = NULL;
static NNSG2dOamManagerInstance     objOamManager;  // OBJ �o�͗p OAM �}�l�[�W��
static NNSG2dOamManagerInstance     ssOamManager;   // SoftwareSprite �o�͗p OAM �}�l�[�W��

static GXOamAttr                    tempOamBuffer[NUM_OF_OAM];           // �e���|���� OAM �o�b�t�@ 

static CellInfo cellInfo = {
    0,                                  // �`�� Cell �ԍ�
    0,                                  // ��]�p
    {FX32_ONE, FX32_ONE},               // �g�嗦
    {CELL_INIT_POS_X, CELL_INIT_POS_Y}  // Cell �`��ʒu
};


//------------------------------------------------------------------------------
// �v���g�^�C�v�錾
void VBlankIntr(void);

static NNSG2dCellDataBank* LoadResources(void);
static void ProcessInput(CellInfo* pCellInfo, NNSG2dOamType* pOutputType, u16 numCells);
static void MakeAffineMatrix(MtxFx22* pMtx, MtxFx22* pMtx2D, const CellInfo* pCellInfo);
static void InitApp( void );
static void AppDraw
( 
    NNSG2dOamManagerInstance*   pOamMgr, 
    const CellInfo*             pCellInfo,
    const MtxFx22*              pMtxAffineForCell,
    const MtxFx22*              pMtxAffineForOBJ
);
static void ApplayOamMgrToHW( NNSG2dOamType outputType );





/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  �Z���o���N�A�L�����N�^�f�[�^�A�p���b�g�f�[�^���t�@�C������
                �ǂݍ��݁A�L�����N�^�f�[�^�ƃp���b�g�f�[�^��
                ���ꂼ��VRAM�Ƀ��[�h���܂��B

  Arguments:    �Ȃ��B

  Returns:      �ǂݍ��񂾃Z���o���N�ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
static NNSG2dCellDataBank* LoadResources(void)
{
    NNSG2dCellDataBank* pCellBank;
    void* pBuf;

    NNS_G2dInitImageProxy( &sImageProxy );
    NNS_G2dInitImagePaletteProxy( &sPaletteProxy );

    //----------------------------------------------
    // load cell data
    {
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/Cell_Simple.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // �Z���o���N�͍Ō�܂Ń��C����������Ŏg�p����̂�
        // ���� pBuf �͊J�����܂���B
    }

    //----------------------------------------------
    // load character data for 2D
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/Cell_Simple.NCGR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            TEX_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sImageProxy );

        // �L�����N�^�f�[�^�� VRAM �ɃR�s�[�����̂�
        // ���� pBuf �͊J�����܂��B�ȉ������B
        G2DDemo_Free( pBuf );
    }
    //--------------------------------------------
    // load character data for 3D (software sprite)
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCBR( &pCharData, "data/Cell_Simple.NCBR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 3D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            TEX_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &sImageProxy );

        G2DDemo_Free( pBuf );
    }

    //--------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/Cell_Simple.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            TEX_PLTT_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sPaletteProxy);

        // Loading For 3D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            TEX_PLTT_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &sPaletteProxy);

        G2DDemo_Free( pBuf );
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
static void ProcessInput(CellInfo* pCellInfo, NNSG2dOamType* pOutputType, u16 numCells)
{
    SDK_NULL_ASSERT( pCellInfo );

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
        if( *pOutputType == NNS_G2D_OAMTYPE_MAIN )
        {
            // ��UOAM�����Z�b�g����
            NNS_G2dResetOamManagerBuffer( &objOamManager );
            NNS_G2dApplyOamManagerToHW( &objOamManager );
            
            *pOutputType = NNS_G2D_OAMTYPE_SOFTWAREEMULATION;
        }else{
            *pOutputType = NNS_G2D_OAMTYPE_MAIN;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         MakeAffineMatrix

  Description:  Cell �̕\���������� Affine �s����쐬���܂��B

  Arguments:    pMtx:       �ʏ�� Affine �s����󂯎�� MtxFx22 �ւ̃|�C���^�B
                pMtx2D:     2D�O���t�B�b�N�G���W���p�� Affine �s���
                            �󂯎�� MtxFx22 �ւ̃|�C���^�B
                pCellInfo:  Cell �̕\����������킷 CellInfo�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void MakeAffineMatrix(MtxFx22* pMtx, MtxFx22* pMtx2D, const CellInfo* pCellInfo)
{
    MtxFx22 tmpMtx;

    // ��]��2�̍s��ŋ��ʂł�
    G2DDemo_MTX_Rot22( &tmpMtx, FX_SinIdx( pCellInfo->rotate ), FX_CosIdx( pCellInfo->rotate ) );

    // �ʏ�� Affine �s����쐬���܂�
    MTX_ScaleApply22(
        &tmpMtx,
        pMtx,
        pCellInfo->scale.x,
        pCellInfo->scale.y );

    // 2D�O���t�B�b�N�G���W���p�� Affine �s����쐬���܂�
    // �X�P�[���������t���Ƃ��Đݒ肷��_�������I�ł��B
    // ����́A2D �O���t�B�b�N�X�G���W���̎d�l�ł��B
    // �ڂ����́A�v���O���~���O�}�j���A�����������������B
    MTX_ScaleApply22(
        &tmpMtx,
        pMtx2D,
        (FX32_ONE * FX32_ONE) / pCellInfo->scale.x,
        (FX32_ONE * FX32_ONE) / pCellInfo->scale.y );
}

//------------------------------------------------------------------------------
// �A�v���P�[�V�����̏��������s���܂�
//------------------------------------------------------------------------------
static void InitApp( void )
{
    BOOL result = TRUE;
    
    //
    // �f�����C�u�����̏��������s���܂�
    //
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();
    }
    
    //
    // OAM �}�l�[�W���̏��������s���܂�
    //
    // �o�b�t�@�]���̃p�t�H�[�}���X�͒ቺ���܂����A
    // ��莩�R�x�̍���OAM�}�l�[�W���̎��̂̏��������s��API
    // NNS_G2dGetNewOamManagerInstance()���p�ӂ���Ă��܂��B
    //
    {
        NNS_G2dInitOamManagerModule();
        
        // OBJ �p
        {
            result = NNS_G2dGetNewOamManagerInstanceAsFastTransferMode
                     ( 
                        &objOamManager, 
                        0, 
                        NUM_OF_OAM, 
                        NNS_G2D_OAMTYPE_MAIN 
                     );
            SDK_ASSERT( result );
        }
        
        // �\�t�g�E�F�A�X�v���C�g�p
        {
            result = NNS_G2dGetNewOamManagerInstanceAsFastTransferMode
                     ( 
                        &ssOamManager, 
                        0, 
                        NUM_OF_OAM, 
                        NNS_G2D_OAMTYPE_SOFTWAREEMULATION 
                     );
            SDK_ASSERT( result );
            // �\�t�g�E�F�A�X�v���C�g��Z�I�t�Z�b�g�������w��
            NNS_G2dSetOamManagerSpriteZoffsetStep( &ssOamManager, - FX32_ONE );
        }
    }
    
    //
    // ���\�[�X�ނ̃��[�h���s���܂�
    //
    spCellBank = LoadResources();
}

//------------------------------------------------------------------------------
// �`��
//
// �჌�x���ȕ`����@ NNS_G2dMakeCellToOams() �� Cell��`�悵�܂��B
// ��ʃ��W���[����G2dRenderer �� ���p����`����@������܂��B
//
//
//       NNS_G2dMakeCellToOams() API �� �Z�����Z����\��OBJ�Q�Ƃ��Ďw�肵���o�b�t�@��
//       �����o���܂��B�{�T���v���ł́A��U�ꎞ�o�b�t�@�ɏ����o�������ƁA���̈ꎞ�o�b
//       �t�@�̓��e��OAM�}�l�[�W�����W���[���ɓo�^���Ă��܂��B
//    
// ���ӁF�A�t�B���p�����[�^��\���s�� �Ƃ��� 
//       pMtxAffineForCell �� pMtxAffineForOBJ ��
//       ���݂��܂��B
//       pMtxAffineForOBJ �ŕ\�����s��� OBJ �̎Q�Ƃ���A�t�B���p�����[�^�Ƃ�
//       �Ďg�p����܂��B ���̂��߁AOAM�}�l�[�W���̎�ނ� 2D �̏ꍇ�ɂ́A�X�P�[
//       �����̂P�����A�s���n���K�v������܂��B
//
//       pMtxAffineForCell �ŕ\�����s��́A2D �O���t�B�b�N�X�G���W������Q��
//       ����邱�Ƃ͂Ȃ��A�Z���̒���OBJ�̈ʒu��ϊ�����ۂɎg�p����܂��B 
//
static void AppDraw
( 
    NNSG2dOamManagerInstance*   pOamMgr, 
    const CellInfo*             pCellInfo,
    const MtxFx22*              pMtxAffineForCell,
    const MtxFx22*              pMtxAffineForOBJ
)
{
    // �`�悳�ꂽOAM�̐�
    u16                     numOamDrawn = 0;  
    // affine �p�����[�^�̐ݒ�( affine Index )
    const u16               affineIdx 
        = NNS_G2dEntryOamManagerAffine( pOamMgr, pMtxAffineForOBJ );
    // �\������Z��
    const NNSG2dCellData*   pCell
        = NNS_G2dGetCellDataByIdx( spCellBank, pCellInfo->cellIdx );                    
    SDK_NULL_ASSERT( pCell );
    
    //----------------------------------------------------
    // Cell �Ɠ����� OBJ���X�g���e���|�����ɏ����o���܂�
    //
    numOamDrawn  = NNS_G2dMakeCellToOams(
                        tempOamBuffer,      // �o�͐�Oam�o�b�t�@
                        NUM_OF_OAM,         // �o�͐�o�b�t�@��
                        pCell,              // �o�͑Ώ�Cell
                        pMtxAffineForCell,  // Affine �ϊ�     <= �ʏ�� Affine �s����w�肵�܂��B
                        &pCellInfo->pos,    // �Z���̈ʒu         ���̍s���2D�O���t�B�b�N�G���W���ɂ�
                        affineIdx,          // Affine Index       �n���ꂸ�ACell��Affine�ϊ��ɗ��p����܂��B
                        TRUE );             // �{�pAffine���H

    SDK_ASSERT( numOamDrawn < NUM_OF_OAM );

    //---------------------------------------------------- 
    // �e���|�����ɏ����o����OBJ���X�g��OAM �}�l�[�W���֓o�^���܂�
    // 
    (void)NNS_G2dEntryOamManagerOam( pOamMgr,tempOamBuffer, numOamDrawn );
    
}

//------------------------------------------------------------------------------
//
// �}�l�[�W���̓��e��HW�ɏ����o���܂�
//
// outputType �̎�ނ��Ƃɏ��������򂵂Ă��܂��B
//
static void ApplayOamMgrToHW( NNSG2dOamType outputType )
{ 
    // ----- SoftwareSprite �� Cell ��`�悵�܂�
    if( outputType == NNS_G2D_OAMTYPE_SOFTWAREEMULATION )
    {
        u32 texBase = NNS_G2dGetImageLocation( &sImageProxy, NNS_G2D_VRAM_TYPE_3DMAIN );
        u32 pltBase = NNS_G2dGetImagePaletteLocation( &sPaletteProxy, NNS_G2D_VRAM_TYPE_3DMAIN );
        //
        // �{�T���v���ł́AV-Blank���Ƀ}�l�[�W���̓��e��HW�ɏ����o���Ă��܂���
        // �\�t�g�E�F�A�X�v���C�g�`��̏ꍇ�ɂ́A
        // �K�����������o��������V-Blank���ł���K�v�͂���܂���B
        //
        NNS_G2dApplyOamManagerToHWSprite( &ssOamManager, &sImageProxy.attr, texBase, pltBase );
        NNS_G2dResetOamManagerBuffer( &ssOamManager );
                
    }else{
    // ----- OBJ �� Cell ��`�悵�܂�
        NNS_G2dApplyOamManagerToHW( &objOamManager );
        NNS_G2dResetOamManagerBuffer( &objOamManager );
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
    //----------------------------------------------------
    // �A�v���P�[�V�����̏�����
    InitApp();
    // start display
    {
        SVC_WaitVBlankIntr();
        GX_DispOn();
        GXS_DispOn();
    }


    //----------------------------------------------------
    // ���C�����[�v
    while( TRUE )
    {
        static NNSG2dOamType outputType = INIT_OUTPUT_TYPE;  // Cell �o�͕��@
        MtxFx22              mtxAffine;                      // �ʏ� Cell affine �s��
        MtxFx22              mtxAffine2D;                    // OBJ�A�t�B���p�����[�^�p �s��
        
        //-------------------------------------------------
        // �X�V
        //
        // ���͂�ǂݎ�� Cell �̕\�����Əo�͕��@���X�V���܂�
        ProcessInput( &cellInfo, &outputType, spCellBank->numCells);
        // �A�t�B���ϊ��s��̐���
        MakeAffineMatrix( &mtxAffine, &mtxAffine2D, &cellInfo );
        
        
        //-------------------------------------------------
        // �`��
        //
        // OBJ �ŕ\�����邩 SoftwareSprite �ŕ\�����邩��
        // OAM �}�l�[�W���� OBJ�A�t�B���p�����[�^�p �s���؂�ւ��܂��B
        // 
        if( outputType == NNS_G2D_OAMTYPE_MAIN )
        {
            AppDraw( &objOamManager, &cellInfo, &mtxAffine, &mtxAffine2D );
        }else{               
            AppDraw( &ssOamManager, &cellInfo, &mtxAffine, &mtxAffine );
        }
        
        //-------------------------------------------------
        // �\�����̏o��
        {
            G2DDemo_PrintOutf(0, 0, "pos:   x=%3d     y=%3d",
                cellInfo.pos.x >> FX32_SHIFT, cellInfo.pos.y >> FX32_SHIFT);
            G2DDemo_PrintOutf(0, 1, "scale: x=%7.3f y=%7.3f",
                (double)cellInfo.scale.x / FX32_ONE, (double)cellInfo.scale.y / FX32_ONE);
            G2DDemo_PrintOutf(0, 2, "rot:   %5d (%6.2f)",
                cellInfo.rotate, 360.0 * cellInfo.rotate / 0x10000);
            G2DDemo_PrintOut(0, 23, (outputType == NNS_G2D_OAMTYPE_MAIN ) ?
                   "OBJ           ": "SoftwareSprite");
        }
        
        //-------------------------------------------------
        //
        // V Blank �҂�
        {
            G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
            SVC_WaitVBlankIntr();
        
            // �\�����̔��f�iBG�f�[�^�̑���j
            G2DDemo_PrintApplyToHW();
        
            //-------------------------------------------------
            //
            // �}�l�[�W���̓��e��HW�ɏ����o���܂�
            //
            ApplayOamMgrToHW( outputType );
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

