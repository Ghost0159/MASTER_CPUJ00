/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - OamManagerEx3
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
//      �g�� OAM �}�l�[�W���ƃ����_����g�ݍ��킹�Ďg�p����T���v���ł��B
//      �EOBJ ���������ŕ\�����A��������A���ۂ̎g�pOAM�� ��葽���� OBJ��\�����܂��B
//      �EOBJ�̕`�揇�w���p���\���D��x���� OBJ ��\�����܂��B
//      �E����̕\���D��x���w�肳�ꂽ�ꍇ�́A�A�t�B���ϊ����ꂽOAM�ƒʏ��OAM�̕`�揇���[���ɂ��Ď����܂��B
//      �E�������\���̃A���S���Y���ɂ��āA�g�p����n�[�h�E�F�A���\�[�X��(OBJ,�A�t�B���p�����[�^ �̐�)
//        ��v���C�I���e�B��ω������Ȃ��玦���܂��B
//
//  ������@
//      �\���L�[���E    �L�����N�^�����E�ɓ������܂��B
//      �\���L�[�㉺    �L�����N�^��O��ɓ������܂��B
//      A               OMA�}�l�[�W�����g�p����OBJ �̐��𑝂₵�܂��B
//      B               OMA�}�l�[�W�����g�p����OBJ �̐������炵�܂��B
//      X               OMA�}�l�[�W�����g�p����A�t�B���p�����[�^ �̐��𑝂₵�܂��B
//      Y               OMA�}�l�[�W�����g�p����A�t�B���p�����[�^ �̐������炵�܂��B
//      START           �g��OAM�}�l�[�W���̕`�揇�Ԃ̐ؑւ������܂��B          
//      SELECT          �E�[�̃Z���̕`��v���C�I���e�B�����̑�2�̃Z���Ƃ͈قȂ�l�Ɏw�肵�܂��B
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

// ��ʂ̑傫��
#define SCREEN_WIDTH            256
#define SCREEN_HEIGHT           192




#define NUM_OF_OAM_CHUNK        32   // �g��OAM�}�l�[�W���̎���OAM �`�����N��
#define NUM_OF_AFFINE_PROXY     32   // �g��OAM�}�l�[�W���̎��A�t�B���p�����[�^�v���N�V�̐�
#define NUM_OF_ORDERING_TBL     2    // �g��OAM�}�l�[�W���̎��� �I�[�_�����O�e�[�u����
                                     // (=�w��\�ȃv���C�I���e�B��)
                                     

// ���\�[�X�̃��[�h�A�h���X�I�t�Z�b�g
#define CHARA_BASE              0x0000
#define PLTT_BASE               0x0000



//-----------------------------------------------------------------------------
// �\���̒�`

// �ʒu��\���\����
typedef struct Position
{
    s16 x;
    s16 y;
} Position;



//-----------------------------------------------------------------------------
// �O���[�o���ϐ�

//-----------------------------------------------------------------------------
// �g��OAM�}�l�[�W��
static NNSG2dOamManagerInstanceEx       myOamMgrEx_;
// �`�����N
// OAM ���i�[���郊�X�g�̂��߂̃R���e�i�ł��B
static NNSG2dOamChunk                   sOamChunk[NUM_OF_OAM_CHUNK];
// �`�����N���X�g
// �\���D��x���Ƃ̃`�����N���X�g��ێ����܂��B
// ���p����\���D��x�̐������K�v�ł��B
static NNSG2dOamChunkList               sOamChunkList[NUM_OF_ORDERING_TBL];
// Affine�p�����[�^�v���L�V
// Affine �p�����[�^���i�[�� HW ���f���܂� AffineIndex �̌����x�������܂��B
static NNSG2dAffineParamProxy           sAffineProxy[NUM_OF_AFFINE_PROXY];



//-----------------------------------------------------------------------------
// OAM�}�l�[�W��
static NNSG2dOamManagerInstance         myOamMgr_;

//-----------------------------------------------------------------------------
// �����_���֘A
static NNSG2dRendererInstance           myRenderer_;
static NNSG2dRenderSurface              myRenderSurface_;
static NNSG2dImageProxy                 myImageProxy_;
static NNSG2dImagePaletteProxy          myPaletteProxy_;


//-----------------------------------------------------------------------------
// 
// �Z���A�j���[�V����
static NNSG2dCellAnimation*             pMyCellAnim_;     

// �g��OAM�}�l�[�W���o�^���Ɏg�p�����
// �\���D��x
static u8                               oamPriority_ = 0; 

// �g��OAM�}�l�[�W�����g�p����OAM�A�g���r���[�g��
static u16 numOamReservedForManager_           =   8;     
// �g��OAM�}�l�[�W�����g�p����A�t�B���p�����[�^��
static u16 numOamAffineReservedForManager_     =   1;     

// �g��OAM�}�l�[�W���ɂ���Ďg�p���ꂽOAM�̐�
static u16 numOamUsedByManager_         = 0;              
// �g��OAM�}�l�[�W���ɂ���Ďg�p���ꂽ�A�t�B���p�����[�^�̐�
static u16 numOamAffineUsedByManager_   = 0;              

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
void NitroMain(void);
void VBlankIntr(void);

//------------------------------------------------------------------------------
// �����_�����g�p����AOAM�o�^�֐�
//
// �g��OAM�}�l�[�W����OAM�o�^�֐����Ăяo���܂��B
//
static BOOL RndrCBFuncEntryOam_
( 
    const GXOamAttr* pOam, 
    u16 affineIndex, 
    BOOL /*bDoubleAffine*/ 
)
{
    SDK_NULL_ASSERT( pOam );
    
    numOamUsedByManager_++;
    
    if( NNS_G2D_OAM_AFFINE_IDX_NONE == affineIndex )
    {
        return NNS_G2dEntryOamManExOam( &myOamMgrEx_, pOam, oamPriority_ );
    }else{
        return NNS_G2dEntryOamManExOamWithAffineIdx( &myOamMgrEx_, pOam, oamPriority_, affineIndex );
    }
}

//------------------------------------------------------------------------------
// �����_�����g�p����AOAM�A�t�B���p�����[�^�o�^�֐�
//
// �g��OAM�}�l�[�W����OAM�A�t�B���p�����[�^�o�^�֐����Ăяo���܂��B
//
static u16 RndrCBFuncEntryOamAffine_( const MtxFx22* mtx )
{
    SDK_NULL_ASSERT( mtx );
    
    numOamAffineUsedByManager_++;
    
    return NNS_G2dEntryOamManExAffine( &myOamMgrEx_, mtx );
}




//------------------------------------------------------------------------------
// �g��OAM�}�l�[�W�����O�����W���[����OAM�A�g���r���[�g��A�t�B���p�����[�^
// ��o�^����Ƃ��Ɏg�p����֐��Q
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// �g��OAM�}�l�[�W�������p�\�ȁAOAM�A�g���r���[�g�̌����擾���܂�
// �{�֐��ł́AOAM�A�g���r���[�g�̌��́A���ۂɃn�[�h�E�F�AOAM�Ƀ��[�h�\�Ȍ���\���܂��B
// OAM�`�����N�̌�(NUM_OF_OAM_CHUNK)�Ƃ͈قȂ�_�ɂ����ӂ��������B
// 
// �o�^�`�����N�� > OamMgrExCBGetCapacityOam_()�Ԃ�l �̂Ƃ��ɁA�g��OAM�}�l�[�W���͎������\�����s���܂��B
// 
static u16 OamMgrExCBGetCapacityOam_(void)
{
    return numOamReservedForManager_;
}
//------------------------------------------------------------------------------
// �g��OAM�}�l�[�W�������p�\�ȁA�A�t�B���p�����[�^�̌����擾���܂�
static u16 OamMgrExCBGetCapacityOamAffine_(void)
{
    return numOamAffineReservedForManager_;
}

static BOOL OamMgrExCBEntryOam_(const GXOamAttr* pOam, u16 /*index*/)
{
    return NNS_G2dEntryOamManagerOam( &myOamMgr_, pOam, 1 );
}

static u16 OamMgrExCBEntryOamAffine_( const MtxFx22* pMtx, u16 /*index*/ )
{
    return NNS_G2dEntryOamManagerAffine( &myOamMgr_, pMtx );
}



//------------------------------------------------------------------------------
static void LoadResources(void)
{
    NNSG2dCellDataBank* pCellBank;
    void*               pBuf;

    NNSG2dAnimBankData*          pAnimBank = NULL;
    
    NNS_G2dInitImageProxy( &myImageProxy_ );
    NNS_G2dInitImagePaletteProxy( &myPaletteProxy_ );

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
        pBuf = G2DDemo_LoadNANR( &pAnimBank, "data/Renderer_2LCD.NANR" );
        SDK_NULL_ASSERT( pBuf );

        //
        // �Z���A�j���[�V�����̎��̂����������܂�
        //
        {
            pMyCellAnim_ = G2DDemo_GetNewCellAnimation(1);
            SDK_NULL_ASSERT( pMyCellAnim_ );

            SDK_NULL_ASSERT( NNS_G2dGetAnimSequenceByIdx( pAnimBank, 0 ) );

            NNS_G2dInitCellAnimation(
                pMyCellAnim_,
                NNS_G2dGetAnimSequenceByIdx(pAnimBank, 0),
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
            &myImageProxy_ );

        // �L�����N�^�f�[�^�� VRAM �ɃR�s�[�����̂�
        // ���� pBuf �͊J�����܂��B�ȉ������B
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
            &myPaletteProxy_ );

        G2DDemo_Free( pBuf );
    }
}

//------------------------------------------------------------------------------
// �A�v���P�[�V�����̏������������s���܂�
static void InitApp_()
{
    
    //
    // �`��f�[�^�f�ނ�ǂݍ��݂܂��B�܂��AVRAM�Ǘ����s���f�[�^�̏��������s���܂��B
    //
    LoadResources();
    
    //
    // �g��OAM�}�l�[�W���̏��������s���܂��B
    // �ȉ��̗�ł́A�g��OAM�}�l�[�W����OAM�o�^��Ƃ����������郂�W���[���Ƃ��āA
    // OAM�}�l�[�W�����W���[�����g�p���Ă��܂��B
    // ���̂��߁AOAM�}�l�[�W�����W���[�������������Ă��܂��B
    //
    {
        BOOL                        bSuccess;
        

        //
        // OAM�}�l�[�W���̏�����
        //
        NNS_G2dInitOamManagerModule();
        bSuccess = NNS_G2dGetNewOamManagerInstanceAsFastTransferMode( &myOamMgr_, 0, 128, NNS_G2D_OAMTYPE_MAIN );
        SDK_ASSERT( bSuccess );

        

        // �g�� OAM �}�l�[�W�����̂̏�����
        bSuccess = NNS_G2dGetOamManExInstance(
                        &myOamMgrEx_,
                        sOamChunkList,
                        (u8)NUM_OF_ORDERING_TBL,
                        NUM_OF_OAM_CHUNK,
                        sOamChunk,
                        NUM_OF_AFFINE_PROXY,
                        sAffineProxy);

        SDK_ASSERT( bSuccess );
        
        // NNS_G2dApplyOamManExToBaseModule() �Ăяo���ŃR�[���o�b�N�����֐��Q��o�^���܂��B
        // �{�T���v���ł́A�R�[���o�b�N�֐��̎�����OAM�}�l�[�W��API�𗘗p���Ă��܂��B    
        {
            NNSG2dOamExEntryFunctions   funcs;
            funcs.getOamCapacity    = OamMgrExCBGetCapacityOam_;
            funcs.getAffineCapacity = OamMgrExCBGetCapacityOamAffine_;
            funcs.entryNewOam       = OamMgrExCBEntryOam_;
            funcs.entryNewAffine    = OamMgrExCBEntryOamAffine_;

            NNS_G2dSetOamManExEntryFunctions( &myOamMgrEx_, &funcs );
        }
    }
    
    //
    // �����_�����W���[�������������܂�
    // �{�T���v���ł́AOAM�o�^�������s�����W���[���Ɋg��OAM�}�l�[�W�����g�p���Ă��܂��B
    //
    {
        // �����_���̏�����
        NNS_G2dInitRenderer( &myRenderer_ );
        NNS_G2dInitRenderSurface( &myRenderSurface_ );
        
        // ���C����� Surface �����������܂�
        {
            NNSG2dViewRect* pRect = &(myRenderSurface_.viewRect);

            // �\����`
            pRect->posTopLeft.x = 0;
            pRect->posTopLeft.y = 0;
            pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
            pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;

            // �R�[���o�b�N�֐�
            myRenderSurface_.pFuncOamRegister          = RndrCBFuncEntryOam_;
            myRenderSurface_.pFuncOamAffineRegister    = RndrCBFuncEntryOamAffine_;

            // �\����
            myRenderSurface_.type                      = NNS_G2D_SURFACETYPE_MAIN2D;
        }
        
        NNS_G2dAddRendererTargetSurface( &myRenderer_, &myRenderSurface_ );
        
        // �v���L�V�o�^
        NNS_G2dSetRendererImageProxy( &myRenderer_, &myImageProxy_, &myPaletteProxy_ );
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
    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_PrintInit();
        
        InitApp_();
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
        // �L�����N�^�ʒu
        static Position pos = { 64, 128 };

        static OSTick          timeForApply;
        static u16             rotation = 0x0;
        static u8              priority = 0;
        
        static NNSG2dOamExDrawOrder drawOrderType = NNSG2D_OAMEX_DRAWORDER_BACKWARD;
        
        // ���͂̏���
        {
            G2DDemo_ReadGamePad();

            // �L�����N�^�̈ړ�
            if( G2DDEMO_IS_PRESS( PAD_KEY_LEFT ) )
            {
                pos.x--;
            }
            if( G2DDEMO_IS_PRESS( PAD_KEY_RIGHT ) )
            {
                pos.x++;
            }
            if( G2DDEMO_IS_PRESS( PAD_KEY_UP ) )
            {
                pos.y--;
            }
            if( G2DDEMO_IS_PRESS( PAD_KEY_DOWN ) )
            {
                pos.y++;
            }
            
            if( G2DDEMO_IS_PRESS( PAD_BUTTON_L ) )
            {
                rotation -= 0xFF;
            }
            if( G2DDEMO_IS_PRESS( PAD_BUTTON_R ) )
            {
                rotation += 0xFF;
            }
            
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_START ) )
            {
                //
                // �g��OAM�}�l�[�W���̕`�揇�Ԃ�؂�ւ��܂�
                //
                if( drawOrderType == NNSG2D_OAMEX_DRAWORDER_BACKWARD )
                {   
                    drawOrderType = NNSG2D_OAMEX_DRAWORDER_FORWARD;
                }else{
                    drawOrderType = NNSG2D_OAMEX_DRAWORDER_BACKWARD;
                }
                NNSG2d_SetOamManExDrawOrderType( &myOamMgrEx_, drawOrderType );
            }
            
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_A ) )
            {
                if( numOamReservedForManager_+1 < 127 )
                {
                    numOamReservedForManager_++;
                }
            }
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_B ) )
            {
                if( numOamReservedForManager_ > 1 )
                {
                    numOamReservedForManager_--;
                }
            }
            
            
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_X ) )
            {
                if( numOamAffineReservedForManager_+1 < 31)
                {
                    numOamAffineReservedForManager_++;
                }
            }
            
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_Y ) )
            {
                if( numOamAffineReservedForManager_ > 0 )
                {
                    numOamAffineReservedForManager_--;
                }
            }
            
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_SELECT ) )
            {
                if( priority == 0 )
                {
                    priority = 1;
                }else{
                    priority = 0;
                }
            }
        }
        
        // �`��
        {
            NNS_G2dBeginRendering( &myRenderer_ );
                NNS_G2dPushMtx();
                    NNS_G2dTranslate( FX32_ONE*pos.x, FX32_ONE*pos.y, 0 );                        
                    // ��
                    oamPriority_ = 0;
                    NNS_G2dTranslate( FX32_ONE*30, 0, 0 );
                    NNS_G2dDrawCellAnimation( pMyCellAnim_ );
                    
                    // ����
                    oamPriority_ = 0;
                    NNS_G2dTranslate( FX32_ONE*30, 0, 0 );
                    NNS_G2dScale( FX32_ONE + (FX32_ONE >> 1), FX32_ONE + (FX32_ONE >> 1), 0 );
                    NNS_G2dDrawCellAnimation( pMyCellAnim_ );
                    
                    // �E
                    oamPriority_ = priority;
                    NNS_G2dTranslate( FX32_ONE*30, 0, 0 );
                    if( rotation != 0 )
                    {
                        NNS_G2dRotZ( FX_SinIdx( rotation ), FX_CosIdx( rotation ) );
                    }
                    NNS_G2dDrawCellAnimation( pMyCellAnim_ );
                NNS_G2dPopMtx();
            NNS_G2dEndRendering();
        }

        // �f�o�b�N������o��
        {
            G2DDemo_PrintOutf(0, 0, "OAM(used/Max)=(%3d/%3d)", 
                                    numOamUsedByManager_, 
                                    numOamReservedForManager_ );
                                    
            G2DDemo_PrintOutf(0, 1, "AFF(used/Max)=(%3d/%3d)", 
                                    numOamAffineUsedByManager_, 
                                    numOamAffineReservedForManager_ );        
                  
            G2DDemo_PrintOutf(0, 2, "DrawOrderType = %s", 
                                    (drawOrderType == NNSG2D_OAMEX_DRAWORDER_BACKWARD) ?
                                    "Backward":
                                    "Forward " );        
        }
        
        // V Blank �҂�
        SVC_WaitVBlankIntr();

        //
        // �o�b�t�@�̓��e��HW�ɏ����o���܂�
        //
        {
            timeForApply = OS_GetTick();
            // �\�����������o���܂�
            G2DDemo_PrintApplyToHW();

            
            // OBJ�������o���܂�
            NNS_G2dApplyOamManExToBaseModule( &myOamMgrEx_ );

            // �g�� OAM �}�l�[�W�������Z�b�g���܂��B
            // ���Z�b�g���Ă��������\���Ɋւ�����͈ێ�����܂��B
            NNS_G2dResetOamManExBuffer( &myOamMgrEx_ );
            timeForApply = OS_GetTick() - timeForApply;
            
            NNS_G2dApplyOamManagerToHW( &myOamMgr_ );
            NNS_G2dResetOamManagerBuffer( &myOamMgr_ );
            
            numOamUsedByManager_         = 0;
            numOamAffineUsedByManager_   = 0;
        }
        
        // �A�j���[�V�����̍X�V
        NNS_G2dTickCellAnimation( pMyCellAnim_, FX32_ONE );
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
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );    // checking VBlank interrupt
}


