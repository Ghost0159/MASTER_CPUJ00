/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - OamSoftwareSpriteDraw
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.4 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      Oam �\�t�g�E�G�A�X�v���C�g�`��̃T���v��
//      �e�����ɂ����Ă̕`�敉�ׂ̈Ⴂ���r���܂��B
//      NUM_OBJ_X * NUM_OBJ_Y ���̃X�v���C�g��`�悵�܂��B
//  ������@
//
//      A �{�^��        �\������ OBJ ��؂�ւ��܂�(�؂�ւ���OBJ�����݂���Ȃ��)�B
//      B �{�^��        �`�������؂�ւ��܂�
//      �㉺���E        �`�� �X�v���C�g���𑝌����܂��B
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

//------------------------------------------------------------------------------
// �e��萔�̒�`
#define AFFINE_IDX                  3                       // Cell �p Affine Index
#define CELL_ROTATE_UNIT            0xFF                    // Cell �̒P�ʉ�]��
#define CELL_SCALE_UNIT             ((FX32_ONE >> 7) * 3)   // Cell �̒P�ʊg��k����
                                                            // scale �l�� 0 �t�߂̒l�ɂȂ�Ȃ��悤�ɂ��Ă���܂�
#define CELL_INIT_POS_X             (120 << FX32_SHIFT)     // Cell �̏����ʒu X
#define CELL_INIT_POS_Y             (120 << FX32_SHIFT)     // Cell �̏����ʒu Y
#define NUM_OBJ_X                   48                       
#define NUM_OBJ_Y                   32
#define STEP_OBJ                    10

#define INIT_OUTPUT_TYPE            NNS_G2D_OAMTYPE_MAIN    // �o�͕��@�̏����l

#define NUM_OF_OAM                  128                     // OAM �}�l�[�W���Ɋ��蓖�Ă� OAM �̐�
#define NUM_OF_AFFINE               (NUM_OF_OAM / 4)        // OAM �}�l�[�W���Ɋ��蓖�Ă� Affine �p�����[�^�̐�

#define TEX_BASE                    0x0                     // �e�N�X�`���x�[�X�A�h���X
#define TEX_PLTT_BASE               0x0                     // �e�N�X�`���p���b�g�x�[�X�A�h���X



//------------------------------------------------------------------------------
// �\���̒�`



//------------------------------------------------------------------------------
// �O���[�o���ϐ�

static NNSG2dImageProxy         sImageProxy;    // Cell �p�L�����N�^/�e�N�X�`���v���L�V
static NNSG2dImagePaletteProxy  sPaletteProxy;  // Cell �p�p���b�g�v���L�V

//------------------------------------------------------------------------------
// �v���g�^�C�v�錾

void VBlankIntr(void);
static NNSG2dCellDataBank* LoadResources(void);

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
// �`����
typedef enum MyDrawMetod
{
    MyDrawMetod_Normal,         // �ʏ�
    MyDrawMetod_Fast,           // ����
    MyDrawMetod_UsingCache,     // �L���b�V�����p�A����
    MyDrawMetod_Max
}MyDrawMetod;

const char* strDrawMetod[]=
{
    "Normal    ",
    "Fast      ",
    "UsingCache",
    "NG",
};

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

    //------------------------------------------------------------------------------
    // load cell data
    {
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/OamSoftwareSpriteDraw.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // �Z���o���N�͍Ō�܂Ń��C����������Ŏg�p����̂�
        // ���� pBuf �͊J�����܂���B
    }


    // load character data for 3D (software sprite)
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCBR( &pCharData, "data/OamSoftwareSpriteDraw.NCBR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 3D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            TEX_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &sImageProxy );

        G2DDemo_Free( pBuf );
    }

    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/OamSoftwareSpriteDraw.NCLR" );
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
  Name:         NitroMain

  Description:  ���C���֐��ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    NNSG2dCellDataBank*         pCellBank;      // �Z���f�[�^�o���N

    // Initialize App.
    {
        BOOL result = TRUE;

        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();
        
        pCellBank = LoadResources();
        G2_SetBG1Priority( 0 );
        G2_SetBG0Priority( 1 );
    }

    // start display
    {
        SVC_WaitVBlankIntr();
        GX_DispOn();
        GXS_DispOn();
    }

    //----------------------------------------------------
    // Main loop
    while( TRUE )
    {
        static CellInfo cellInfo = 
        {
            0,                                  // �`�� Cell �ԍ�
            0,                                  // ��]�p
            {FX32_ONE, FX32_ONE},               // �g�嗦
            {CELL_INIT_POS_X, CELL_INIT_POS_Y}  // Cell �`��ʒu
        };
        static NNSG2dOamType        outputType = INIT_OUTPUT_TYPE;  // Cell �o�͕��@
        static OSTick time;
        static MyDrawMetod          drawMethod = MyDrawMetod_Normal;
        static int numX = 24, numY = 16;
        //
        // �L�[����
        //
        {
            G2DDemo_ReadGamePad();

            // Change current Cell
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_A) )
            {
                cellInfo.cellIdx++;
                if( cellInfo.cellIdx >= pCellBank->numCells )
                {
                    cellInfo.cellIdx = 0;
                }
            }
            
            if ( G2DDEMO_IS_TRIGGER(PAD_BUTTON_B) )
            {
                drawMethod++;
                drawMethod %= MyDrawMetod_Max;
            }
            
            if ( G2DDEMO_IS_PRESS(PAD_KEY_UP) )
            {
                numY--;
            }
            if ( G2DDEMO_IS_PRESS(PAD_KEY_DOWN) )
            {
                numY++;
            }
            if ( G2DDEMO_IS_PRESS(PAD_KEY_LEFT) )
            {
                numX--;
            }
            if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
            {
                numX++;
            }
            
        }   

        if( NUM_OBJ_X < numX )
        {
            numX = NUM_OBJ_X;
        }
        
        if( 0 > numX )
        {
            numX = 0;
        }
        
        if( NUM_OBJ_Y < numY )
        {
            numY = NUM_OBJ_Y;
        }
        
        if( 0 > numY )
        {
            numY = 0;
        }  

        // �`��
        //
        // OBJ��`�悵�܂��B
        // �܂��A�`�揈���ɂ����������Ԃ��v�����܂��B
        //
        //
        time = OS_GetTick();
        {
            GXOamAttr      tempOam;
            int i, j;
            
            // 0�Ԗڂ�OBJ��`�悵�܂�
            NNS_G2dCopyCellAsOamAttr( NNS_G2dGetCellDataByIdx( pCellBank, cellInfo.cellIdx ), 0, &tempOam );
            NNS_G2dSetupSoftwareSpriteCamera();
            
            switch( drawMethod )
            {
            //------------------------------------------------------------------------------
            // �ʏ�̕`��
            case MyDrawMetod_Normal:
                for( i = 0; i < numX; i ++ )
                {
                    for( j = 0;j < numY; j++ )
                    {
                        NNS_G2dDrawOneOam3DDirectWithPos( (s16)(STEP_OBJ*i), (s16)(STEP_OBJ*j), (s16)(-1), &tempOam, 
                                           &sImageProxy.attr,
                                           NNS_G2dGetImageLocation( &sImageProxy, NNS_G2D_VRAM_TYPE_3DMAIN ),
                                           NNS_G2dGetImagePaletteLocation( &sPaletteProxy, NNS_G2D_VRAM_TYPE_3DMAIN ) );
                    }
                }
                break;
            //------------------------------------------------------------------------------
            // �����`��F�J�����g�s��̃X�^�b�N�ւ̑ޔ��A�������s��Ȃ�
            case MyDrawMetod_Fast:
                G3_PushMtx();
                for( i = 0; i < numX; i ++ )
                {
                    for( j = 0;j < numY; j++ )
                    {
                        // �J�����g�s�񂪕ۑ�����Ȃ��̂ŁA�J�����g�s��̏��������`��O�ɕK�v�ł���B
                        G3_Identity();
                        NNS_G2dDrawOneOam3DDirectWithPosFast( (s16)(STEP_OBJ*i), (s16)(STEP_OBJ*j), (s16)(-1), &tempOam, 
                                           &sImageProxy.attr,
                                           NNS_G2dGetImageLocation( &sImageProxy, NNS_G2D_VRAM_TYPE_3DMAIN ),
                                           NNS_G2dGetImagePaletteLocation( &sPaletteProxy, NNS_G2D_VRAM_TYPE_3DMAIN ) );
                    }
                }
                G3_PopMtx(1);
                break;
            //------------------------------------------------------------------------------
            // �L���b�V�����p�����`��F�ł������A����������̉摜������OBJ�����`��ł��Ȃ�
            // �i�T�C�Y�͈قȂ��Ă��悢�j                
            case MyDrawMetod_UsingCache: 
                NNS_G2dSetOamSoftEmuSpriteParamCache( &tempOam, 
                                           &sImageProxy.attr,
                                           NNS_G2dGetImageLocation( &sImageProxy, NNS_G2D_VRAM_TYPE_3DMAIN ),
                                           NNS_G2dGetImagePaletteLocation( &sPaletteProxy, NNS_G2D_VRAM_TYPE_3DMAIN ) );    
                G3_PushMtx();
                for( i = 0; i < numX; i ++ )
                {
                    for( j = 0;j < numY; j++ )
                    {
                        // �J�����g�s�񂪕ۑ�����Ȃ��̂ŁA�J�����g�s��̏��������`��O�ɕK�v�ł���B
                        G3_Identity();
                        NNS_G2dDrawOneOam3DDirectUsingParamCacheFast( (s16)(STEP_OBJ*i), (s16)(STEP_OBJ*j), (s16)(-1), &tempOam );
                    }
                }
                G3_PopMtx(1);
                break;
            }
        }
        time = OS_GetTick() - time;

        // �\�����̏o��
        {
            
            G2DDemo_PrintOutf( 0, 21, "TIME:%06ld usec\n", OS_TicksToMicroSeconds(time) );
            G2DDemo_PrintOut ( 0, 22, strDrawMetod[drawMethod] );
            G2DDemo_PrintOutf( 0, 23, "NUM_SPRITE:%04ld usec\n", numX * numY );
        }

        //
        // �}�l�[�W���̓��e��HW�ɏ����o���܂�
        //
        {
            // V Blank �҂�
            G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
            SVC_WaitVBlankIntr();
            
            // �\�����
            G2DDemo_PrintApplyToHW();
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

