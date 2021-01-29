/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - OamManagerEx2
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
//      �g��OAM�}�l�[�W�����g�p���āA�Z���A�j���[�V������`�悷��T���v���ł��B
//      �g��OAM�}�l�[�W����OAM�}�l�[�W����g�ݍ��킹�Ďg�p���܂��B
//
//  ������@
//      A               �����A�j���[�V�������Đ����܂��B
//
// ============================================================================


#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"


//-----------------------------------------------------------------------------
//
// �G�t�F�N�g�\����
//
struct MyEffectObject;
typedef struct MyEffectObject
{
    
    NNSG2dCellAnimation*        pCellAnm;   // �Z���A�j���[�V����
    NNSG2dFVec2                 pos;        // �ʒu
    BOOL                        bActive;    // �A�N�e�B�u���ǂ����H
    struct MyEffectObject*      pNext;      // ���̃G�t�F�N�g
    
}MyEffectObject;


//-----------------------------------------------------------------------------
#define LENGTH_EFFECT_ARRAY     32              // �G�t�F�N�g�\���̂̃v�[���p�z��̒���

#define NUM_OF_OAM              16              // OAM�}�l�[�W���Ǘ��Ώۂ�OAM��
#define NUM_OF_AFFINE           (NUM_OF_OAM/4)  // OAM�}�l�[�W���Ǘ��Ώۂ�Affine�p�����[�^��

#define TEX_BASE                0x0             // �e�N�X�`���x�[�X�A�h���X
#define TEX_PLTT_BASE           0x0             // �e�N�X�`���p���b�g�x�[�X�A�h���X


#define NUM_OF_OAM_CHUNK        256             // OAM�`�����N��
                                                // (�g��OAM�}�l�[�W���ɓo�^�\��OAM�̍ő吔)
#define NUM_OF_CHUNK_LIST      (u8) 1           // �v���C�I���e�B
#define NUM_OF_AFFINE_PROXY     1               // �A�t�B���v���N�V��
                                                // (�g��OAM�}�l�[�W���ɓo�^�\��
                                                //  �A�t�B���p�����[�^�̍ő吔)

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�

static MyEffectObject    effectArray_[LENGTH_EFFECT_ARRAY]; // �G�t�F�N�g�\���̂̃v�[���p�z��

static MyEffectObject*   pActiveEffectListHead = NULL;      // �A�N�e�B�u��Ԃł���G�t�F�N�g�̃��X�g
static MyEffectObject*   pFreeEffectListHead   = NULL;      // ��A�N�e�B�u��Ԃł���G�t�F�N�g�̃��X�g

// �`�����N
// OAM ���i�[���郊�X�g�̂��߂̃R���e�i�ł��B
static NNSG2dOamChunk           oamChunk_[NUM_OF_OAM_CHUNK];

// �`�����N���X�g
// �\���D��x���Ƃ̃`�����N���X�g��ێ����܂��B
// ���p����\���D��x�̐������K�v�ł��B
static NNSG2dOamChunkList       oamChunkList_[NUM_OF_CHUNK_LIST];

// Affine�p�����[�^�v���L�V
// Affine �p�����[�^���i�[�� HW ���f���܂� AffineIndex �̌����x�������܂��B
// ���̃f���ł͗��p���Ă��܂���B
static NNSG2dAffineParamProxy   affineProxy_[NUM_OF_AFFINE_PROXY];


//
// OAM�}�l�[�W���A�g��OAM�}�l�[�W��
//
static NNSG2dOamManagerInstance         oamManager_;
static NNSG2dOamManagerInstanceEx       oamExManager_;


    
//------------------------------------------------------------------------------
// �v���g�^�C�v�錾


/*---------------------------------------------------------------------------*
  Name:         CallBackFunction

  Description:  �A�j���[�V�����R���g���[������R�[���o�b�N�����֐��ł��B
                
                �p�����[�^�ɂ́A�A�j���[�V�������Đ����Ă���G�t�F�N�g�\����
                �ւ̃|�C���^���ݒ肳��Ă��܂��B
                �R�[���o�b�N�֐����ł́A���̃G�t�F�N�g�\���̂̃A�N�e�B�u�t���O��
                FALSE�ɐݒ肵�Ă��܂��B

  Arguments:    param:          �R�[���o�b�N�֐��o�^���Ɏw�肵��
                                ���[�U�p�����[�^�ł��B
                currentFrame:   �R�[���o�b�N�^�C�v��
                                NNS_G2D_ANMCALLBACKTYPE_LAST_FRM �̏ꍇ��
                                �A�j���[�V�����R���g���[���� currentTime ��
                                fx32 �^�œn����܂��B
                                ����ȊO�ł� u16 �^�̌��݂̃t���[���ԍ���
                                �n����܂��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void CallBackFunction( u32 param, fx32 currentFrame )
{
#pragma unused( currentFrame )
    
    MyEffectObject*     pE = (MyEffectObject*)param;
    pE->bActive = FALSE;
}


/*---------------------------------------------------------------------------*
  Name:         CallBackGetOamCapacity

  Description:  �g�� OAM �}�l�[�W������R�[���o�b�N����A�g�� OAM �}�l�[�W����
                ���p�\�� OAM ����`���܂��B

  Arguments:    �Ȃ��B

  Returns:      ���p�ł��� OAM ���B
 *---------------------------------------------------------------------------*/
static u16 CallBackGetOamCapacity(void)
{
    return NNS_G2dGetOamManagerOamCapacity( &oamManager_ );
}



/*---------------------------------------------------------------------------*
  Name:         CallBackGetAffineCapacity

  Description:  �g�� OAM �}�l�[�W������R�[���o�b�N����A�g�� OAM �}�l�[�W����
                ���p�\�� Affine �p�����[�^�i�[�̈搔��`���܂��B

  Arguments:    �Ȃ��B

  Returns:      ���p�ł��� Affine �p�����[�^�i�[�̈搔�B
 *---------------------------------------------------------------------------*/
static u16 CallBackGetAffineCapacity(void)
{
    return 0;
}



/*---------------------------------------------------------------------------*
  Name:         CallBackEntryNewOam

  Description:  �g�� OAM �}�l�[�W������R�[���o�b�N����AOAM ���o�͂��܂��B

  Arguments:    pOam:   �o�͂��ׂ� OAM �ւ̃|�C���^�B
                index:  pOam �� OBJ �i���o�[

  Returns:      OAM �̏o�͂ɐ����������ǂ����B
 *---------------------------------------------------------------------------*/
static BOOL CallBackEntryNewOam(const GXOamAttr* pOam, u16 index )
{
#pragma unused( index )
    return NNS_G2dEntryOamManagerOam( &oamManager_, pOam, 1 );
}



/*---------------------------------------------------------------------------*
  Name:         CallBackEntryNewAffine

  Description:  �g�� OAM �}�l�[�W������R�[���o�b�N����AAffine �p�����[�^��
                �o�^���܂��B

  Arguments:    pMtx:   �o�^���ׂ� Affine �p�����[�^�B
                index:  pMtx �� AffineIndex�B

  Returns:      ���ۂɓo�^���� pMtx �� AffineIndex�B
 *---------------------------------------------------------------------------*/
static u16 CallBackEntryNewAffine( const MtxFx22* pMtx, u16 index )
{
#pragma unused( pMtx )
#pragma unused( index )
	// ���̊֐��͏�ɖ����Ȓl��Ԃ��܂�
    return NNS_G2D_OAMEX_HW_ID_NOT_INIT;
}

//------------------------------------------------------------------------------
static void InitApp()
{
    BOOL        result = TRUE;
    
    //
    // OAM�}�l�[�W�������������܂�
    //
    {
        // OAM�}�l�[�W�� ���W���[�� �̏�����
        NNS_G2dInitOamManagerModule();
        // OAM�}�l�[�W�� ���� �̏�����
        result &= NNS_G2dGetNewOamManagerInstanceAsFastTransferMode( &oamManager_, 
                                                                     0, 
                                                                     NUM_OF_OAM,
                                                                     NNS_G2D_OAMTYPE_MAIN );    
    }
    
    
    // �g�� OAM �}�l�[�W�����̂̏�����
    {
        NNSG2dOamExEntryFunctions       funcs;// !
        
        result &= NNS_G2dGetOamManExInstance( &oamExManager_, 
                                                oamChunkList_, NUM_OF_CHUNK_LIST,
                                                NUM_OF_OAM_CHUNK, oamChunk_,
                                                NUM_OF_AFFINE_PROXY, affineProxy_ );

        // NNS_G2dApplyOamManExToBaseModule() �Ăяo���ŃR�[���o�b�N�����֐��Q��o�^���܂��B
        funcs.getOamCapacity    = CallBackGetOamCapacity;
        funcs.getAffineCapacity = CallBackGetAffineCapacity;
        funcs.entryNewOam       = CallBackEntryNewOam;
        funcs.entryNewAffine    = CallBackEntryNewAffine;

        NNS_G2dSetOamManExEntryFunctions( &oamExManager_, &funcs );
    }
    SDK_ASSERT( result );
}

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  �ȉ��̏��������s���܂��B
                  �Z���A�j���[�V�����f�[�^�̓ǂݍ���
                  �Z���A�j���[�V�����̏�����
                  �L�����N�^�f�[�^�A�p���b�g�f�[�^�̓ǂݍ���

  Arguments:    ppAnimBank: �Z���A�j���[�V�����f�[�^�ւ̃|�C���^��
                            �󂯎��|�C���^�B
                ppCellAnim: �Z���A�j���[�V�����ւ̃|�C���^���󂯎��|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadResources_( )
{
    //
    // Cell �֘A������
    //
    {
        void* pBuf;
        
        NNSG2dCellDataBank*          pCellBank  = NULL;
        NNSG2dAnimBankData*          pAnimBank  = NULL;
        
        // �Z���f�[�^����уZ���A�j���[�V�����f�[�^�����[�h�B
        // �Z���f�[�^����уZ���A�j���[�V�����f�[�^�͍Ō�܂�
        // ���C����������Ŏg�p����̂ł����� pBuf �͉�����܂���B
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/bomb.NCER" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNANR( &pAnimBank, "data/bomb.NANR" );
        SDK_NULL_ASSERT( pBuf );
        SDK_NULL_ASSERT( NNS_G2dGetAnimSequenceByIdx( pAnimBank, 0) );
        
        //
        // �Z���A�j���[�V�����̎��̂����������܂�
        //
        {
            
            NNSG2dCellAnimation*         pCellAnm = NULL;
            int i = 0;
            
            for( i = 0; i < LENGTH_EFFECT_ARRAY; i++ )
            {
                effectArray_[i].pCellAnm = G2DDemo_GetNewCellAnimation(1);
                pCellAnm = effectArray_[i].pCellAnm;
                
                
                SDK_NULL_ASSERT( pCellAnm );
                
                NNS_G2dInitCellAnimation( pCellAnm, NNS_G2dGetAnimSequenceByIdx( pAnimBank, 0), pCellBank );
                
                //
                // �R�[���o�b�N�̐ݒ�
                //
                NNS_G2dSetAnimCtrlCallBackFunctor(
                    NNS_G2dGetCellAnimationAnimCtrl(pCellAnm),
                    NNS_G2D_ANMCALLBACKTYPE_LAST_FRM,
                    (u32)&effectArray_[i],
                    CallBackFunction );
            }
        }
    }

    //
    // VRAM �֘A�̏�����
    //
    {
        void* pBuf;

        // load character data for 2D
        {
            NNSG2dCharacterData*        pCharData;

            pBuf = G2DDemo_LoadNCGR( &pCharData, "data/bomb.NCGR" );
            SDK_NULL_ASSERT( pBuf );
            
            // Loading For 2D Graphics Engine.
            DC_FlushRange( pCharData->pRawData, pCharData->szByte );
            GX_LoadOBJ( (void*)pCharData->pRawData, TEX_BASE, pCharData->szByte );
                
            // �L�����N�^�f�[�^�� VRAM �ɃR�s�[�����̂�
            // ���� pBuf �͊J�����܂��B�ȉ������B
            G2DDemo_Free( pBuf );
        }

        //------------------------------------------------------------------------------
        // load palette data
        {
            NNSG2dPaletteData*        pPlttData;

            pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/bomb.NCLR" );
            SDK_NULL_ASSERT( pBuf );

            // Loading For 2D Graphics Engine.
            DC_FlushRange( pPlttData->pRawData, pPlttData->szByte );
            GX_LoadOBJPltt( (void*)pPlttData->pRawData, TEX_PLTT_BASE, pPlttData->szByte );
            
            G2DDemo_Free( pBuf );
        }
    }
}

//------------------------------------------------------------------------------
// �V�[���̃G�t�F�N�g�o�^���X�g�����Z�b�g���܂�
static void ResetEffectList_()
{
    int i;
    for( i = 0; i < LENGTH_EFFECT_ARRAY - 1; i++ )
    {
        effectArray_[i].bActive = FALSE;
        effectArray_[i].pNext = &effectArray_[i+1];
        NNS_G2dSetCellAnimationCurrentFrame( effectArray_[i].pCellAnm, 0 );
    }
    effectArray_[LENGTH_EFFECT_ARRAY - 1].bActive = FALSE;
    effectArray_[LENGTH_EFFECT_ARRAY - 1].pNext = NULL;
    
    pActiveEffectListHead   = NULL;
    pFreeEffectListHead     = effectArray_;
}

//------------------------------------------------------------------------------
// �V�����G�t�F�N�g���V�[���ɒǉ����܂�
static void AddNewEffect_()
{
    MyEffectObject*             pE = pFreeEffectListHead;
    if( pE )
    {
        SDK_ASSERT( pE->bActive == FALSE );
        
        pE->bActive = TRUE;
        pE->pos.x = (fx32)((OS_GetTick() % 255) * FX32_ONE);
        pE->pos.y = (fx32)((OS_GetTick() % 192) * FX32_ONE);
        
        
        // �t���[���X�g������o��
        pFreeEffectListHead = pE->pNext;
        
        // �A�N�e�B�u���X�g�擪�ɒǉ�
        pE->pNext             = pActiveEffectListHead;
        pActiveEffectListHead = pE;
    }
}

//------------------------------------------------------------------------------
// �V�[�����X�V���܂��B
static void TickScene_()
{
    MyEffectObject*             pE = pActiveEffectListHead;
    MyEffectObject*             pPreE = NULL;
    MyEffectObject*             pNext = NULL;
    
    
    while( pE )
    {    
        // �Z���A�j���[�V�����̍X�V
        NNS_G2dTickCellAnimation( pE->pCellAnm, FX32_ONE );
        pE      = pE->pNext;
    }
    
    pE = pActiveEffectListHead;
    while( pE )
    {
        pNext = pE->pNext;
        if( !pE->bActive )    
        {    
            // �A�N�e�B�u���X�g������o��
            if( pPreE )
            {
                pPreE->pNext          = pNext;
            }else{
                pActiveEffectListHead = pNext;
            }
            // �t���[���X�g�擪�ɒǉ�
            pE->pNext           = pFreeEffectListHead;
            pFreeEffectListHead = pE;
            
            pE      = pNext;
        }else{
            pPreE   = pE;
            pE      = pNext;
        }
    }   
}

//------------------------------------------------------------------------------
// �ЂƂ̔����G�t�F�N�g��`�悵�܂�
static u16 DrawOneEffect_( const MyEffectObject* pE )
{
    {
        int i;
        u16                     numOamBuffer;       // �c�� OAM ��
        static GXOamAttr        temp[NUM_OF_OAM];   // �e���|���� OAM �o�b�t�@
        const NNSG2dCellData*   pCell;              // �\������ Cell

        pCell = NNS_G2dGetCellAnimationCurrentCell( pE->pCellAnm );
        SDK_NULL_ASSERT( pCell );

        //
        // Cell �Ɠ����� OBJ���X�g���e���|�����ɏ����o���܂�
        //
        numOamBuffer = NNS_G2dMakeCellToOams(
                            temp,           // �o�͐�Oam�o�b�t�@
                            NUM_OF_OAM,     // �o�͐�o�b�t�@��
                            pCell,          // �o�͑Ώ� Cell
                            NULL,           // Affine �ϊ�
                            &pE->pos,       // �I�t�Z�b�g�ʒu
                            NULL,           // Affine Index
                            FALSE );        // �{�pAffine���H

        SDK_ASSERT( numOamBuffer < NUM_OF_OAM);

        for( i = 0; i < numOamBuffer; i++ )
        {
            if( !NNS_G2dEntryOamManExOam( &oamExManager_, &temp[i], 0 ) )
            {   
                return (u16)i;
            }
        }
        return numOamBuffer;
    }
    
}

//------------------------------------------------------------------------------
// �V�[����`�悵�܂�
// �g�p����OAM�̐����Ԃ�l�ƂȂ�܂��B
//
static u16 DrawScene_()
{
    u16     totalOam = 0;
    MyEffectObject*     pE;
    pE = pActiveEffectListHead;
    while( pE )
    {
        totalOam += DrawOneEffect_( pE );
        pE = pE->pNext;
    }
    
    return totalOam;
}

//------------------------------------------------------------------------------
// �����G�t�F�N�g�̐��𐔂��܂�
static u16 GetNumEffect_( MyEffectObject* pListHead )
{
    u16 count = 0;
    MyEffectObject*     pE = pListHead;
    while( pE )
    {
        count++;
        pE = pE->pNext;
    }
    return count;
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C���֐��ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_PrintInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        
        InitApp();
        LoadResources_();
        ResetEffectList_();
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
        static OSTick               time;                           // OAM�}�l�[�W���p�t�H�[�}���X�v���Ɏg�p���܂��B
        u16     totalOam = 0;
        //
        // Pad handling.
        //
        {
            G2DDemo_ReadGamePad();
            // �V�[���� �V���� �G�t�F�N�g�� �ǉ����܂�
            if (G2DDEMO_IS_TRIGGER(PAD_BUTTON_A ))
            {
                AddNewEffect_();
            }
            
            if (G2DDEMO_IS_TRIGGER(PAD_BUTTON_B ))
            {
                ResetEffectList_();
            }
            
            if (G2DDEMO_IS_TRIGGER(PAD_BUTTON_L ))
            {
                OS_Printf( "num-active = %d\n", GetNumEffect_( pActiveEffectListHead ) );
                OS_Printf( "num-free   = %d\n", GetNumEffect_( pFreeEffectListHead ) );
            }
        }
        
        // �V�[����`�悵�܂�
        // �g�p����OAM�̐����Ԃ�l�ƂȂ�܂��B
        totalOam = DrawScene_();
        

        //
        // �f�o�b�N������`��
        //
        {
            G2DDemo_PrintOutf( 0, 0, "Total-OAM:%d", totalOam );
        }
        
        //
        // �}�l�[�W���̓��e��HW�ɏ����o���܂�
        // �܂��A���̍ۂɂ����������Ԃ��v�����A�f�o�b�N���Ƃ��ďo�͂��܂��B
        //            
        {
            // V Blank �҂�
            G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
            SVC_WaitVBlankIntr();
            
            time = OS_GetTick();
            {
                NNS_G2dApplyOamManExToBaseModule( &oamExManager_ );
                NNS_G2dResetOamManExBuffer( &oamExManager_ );
                
                NNS_G2dApplyOamManagerToHW( &oamManager_ );
                NNS_G2dResetOamManagerBuffer( &oamManager_ );
            }
            time = OS_GetTick() - time;
        }
        
        //
        // �A�j���[�V��������HW�ɏ����o���܂�   
        //
        G2DDemo_PrintApplyToHW();   
        
        //
        // �V�[�����X�V���܂��B
        // 
        TickScene_();   
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

