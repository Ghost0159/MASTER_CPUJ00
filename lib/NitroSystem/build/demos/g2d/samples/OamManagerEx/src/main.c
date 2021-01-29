/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - OamManagerEx
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.11 $
 *---------------------------------------------------------------------------*/



// ============================================================================
//  �f���̓��e
//      �g�� OAM �}�l�[�W���𗘗p����
//      �E128 ��葽���� OBJ ���������ŕ\�����܂��B
//      �EOBJ�̕`�揇�w���p���\���D��x���� OBJ ��\�����܂��B
//
//  ������@
//      �\���L�[���E    �L�����N�^�����E�ɓ������܂��B
//      �\���L�[�㉺    �L�����N�^��O��ɓ������܂��B
//      A               OBJ �̐��𑝂₵�܂��B
//      B               OBJ �̐������炵�܂��B
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

// ��ʂ̑傫��
#define SCREEN_WIDTH            256
#define SCREEN_HEIGHT           192

#define HW_OAM_USE              128     // HW OAM �̗��p���鐔

#define INIT_NUM_OF_LAYER       10      // �O������ɕ��ԃo�[�̏�����
#define MAX_NUM_OF_LAYER        27      // �O������ɕ��ԃo�[�̍ő吔
#define NUM_OF_LANE             10      // ���E�����ɕ��ԃo�[�̐�

#define NUM_OF_OAM_CHUNK        (MAX_NUM_OF_LAYER * NUM_OF_LANE + 1)
#define NUM_OF_CHUNK_LIST       (MAX_NUM_OF_LAYER + 1)
#define NUM_OF_AFFINE_PROXY     1

// �L�����N�^�ƃo�[�� OBJ �T�C�Y
#define BAR_WIDTH               32
#define BAR_HEIGHT              16
#define CHARA_WIDTH             32
#define CHARA_HEIGHT            32

// �O������̃o�[�̕\���ʒu�̍�
#define BAR_SHIFT_X             12      // X ������12�����
#define BAR_SHIFT_Y             6       // ����� Y ������6�����

// ���E�����̃o�[�ƕ\���ʒu�̍�
#define LANE_SHIFT              (BAR_WIDTH + 24)

// �S�Ẵo�[������ŏ��̋�`�̈�T�C�Y
#define DRAW_AREA_WIDTH         (BAR_SHIFT_X * (MAX_NUM_OF_LAYER - 1) + LANE_SHIFT * (NUM_OF_LANE - 1) + BAR_WIDTH)
#define DRAW_AREA_HEIGHT        (BAR_SHIFT_Y * (MAX_NUM_OF_LAYER - 1) + BAR_HEIGHT)

// ��ԍ����̃o�[�̕\���ʒu
// ���̃o�[�̕\���ʒu�͂��̒l����v�Z�ŋ��߂��܂��B
#define BAR_OFFSET_X            ((SCREEN_WIDTH - DRAW_AREA_WIDTH) / 2)
#define BAR_OFFSET_Y            (((SCREEN_HEIGHT + DRAW_AREA_HEIGHT) / 2) + 10)

// �L�����̏����\���ʒu
// ��ԍ����̃o�[�����_�Ƃ��܂��B
#define CHARA_INIT_POS_X        ((LANE_SHIFT * (NUM_OF_LANE - 1) + BAR_WIDTH) / 2)
#define CHARA_INIT_POS_Y        (- (((BAR_SHIFT_Y * (MAX_NUM_OF_LAYER - 1)) * BAR_SHIFT_X / BAR_SHIFT_Y) / 2))


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

// �`�����N
// OAM ���i�[���郊�X�g�̂��߂̃R���e�i�ł��B
static NNSG2dOamChunk           sOamChunk[NUM_OF_OAM_CHUNK];

// �`�����N���X�g
// �\���D��x���Ƃ̃`�����N���X�g��ێ����܂��B
// ���p����\���D��x�̐������K�v�ł��B
static NNSG2dOamChunkList       sOamChunkList[NUM_OF_CHUNK_LIST];

// Affine�p�����[�^�v���L�V
// Affine �p�����[�^���i�[�� HW ���f���܂� AffineIndex �̌����x�������܂��B
// ���̃f���ł͗��p���Ă��܂���B
//static NNSG2dAffineParamProxy   sAffineProxy[NUM_OF_AFFINE_PROXY];


//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾

static u16 CallBackGetOamCapacity(void);
static void InitOamManagerEx(NNSG2dOamManagerInstanceEx* pExMan );
static void LoadResources(void);
static void InitTemplateOam( GXOamAttr* pChara, GXOamAttr* pBar );
static const Position*  GetPosition(void);
void NitroMain(void);
void VBlankIntr(void);



/*---------------------------------------------------------------------------*
  Name:         CallBackGetOamCapacity

  Description:  �g�� OAM �}�l�[�W������R�[���o�b�N����A�g�� OAM �}�l�[�W����
                ���p�\�� OAM ����`���܂��B

  Arguments:    �Ȃ��B

  Returns:      ���p�ł��� OAM ���B
 *---------------------------------------------------------------------------*/
static u16 CallBackGetOamCapacity(void)
{
    return HW_OAM_USE;
}

/*---------------------------------------------------------------------------*
  Name:         CallBackEntryNewOam

  Description:  �g�� OAM �}�l�[�W������R�[���o�b�N����AOAM ���o�͂��܂��B

  Arguments:    pOam:   �o�͂��ׂ� OAM �ւ̃|�C���^�B
                index:  pOam �� OBJ �i���o�[

  Returns:      OAM �̏o�͂ɐ����������ǂ����B
 *---------------------------------------------------------------------------*/
static BOOL CallBackEntryNewOam(const GXOamAttr* pOam, u16 index)
{
    const u32 szByte = sizeof( GXOamAttr );

    DC_FlushRange( (void*)pOam, szByte );
    GX_LoadOAM( pOam, index * szByte, szByte );

    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         InitOamManagerEx

  Description:  �g�� OAM �}�l�[�W�����̂����������܂��B

  Arguments:    pExMan:     ����������g�� OAM �}�l�[�W�����̂ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitOamManagerEx(NNSG2dOamManagerInstanceEx* pExMan )
{
    BOOL bSuccess;
    NNSG2dOamExEntryFunctions funcs;

    // �g�� OAM �}�l�[�W���̂ݗ��p�̏ꍇ��
    // NNS_G2dInitOamManagerModule() 
    // �̌Ăяo���͕s�v�ł��B
    // �g�� OAM �}�l�[�W���̃R�[���o�b�N�֐��̎�����OAM�}�l�[�W�����g�p
    // ����ꍇ�ɂ́ANNS_G2dInitOamManagerModule()�����s���Ă��������B

    // �g�� OAM �}�l�[�W�����̂̏�����
    bSuccess = NNS_G2dGetOamManExInstance(
                    pExMan,
                    sOamChunkList,
                    (u8)NUM_OF_CHUNK_LIST,
                    NUM_OF_OAM_CHUNK,
                    sOamChunk,
                    0,                      // �A�t�B���p�����[�^�͊Ǘ����Ȃ�
                    NULL);                  // �A�t�B���p�����[�^�͊Ǘ����Ȃ�

    SDK_ASSERT( bSuccess );
    
    // �g��OAM�}�l�[�W���ɓo�^�֐���ݒ肵�܂��B
    // �g��OAM�}�l�[�W���̓f�t�H���g�̓o�^�֐��̂������Ă��܂���̂ŁA
    // ���p�O�ɕK���{�֐������s���ēo�^�֐���ݒ肷��K�v������܂��B
    {
        // NNS_G2dApplyOamManExToBaseModule() �Ăяo����
        // �R�[���o�b�N�����֐��Q��o�^���܂��B
        funcs.getOamCapacity    = CallBackGetOamCapacity;
        funcs.entryNewOam       = CallBackEntryNewOam;
        funcs.getAffineCapacity = NULL;
        funcs.entryNewAffine    = NULL;
        
        NNS_G2dSetOamManExEntryFunctions( pExMan, &funcs );
    }
}



/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  �L�����N�^�f�[�^�ƃp���b�g�f�[�^���t�@�C������ǂݍ���
                VRAM �Ƀ��[�h���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadResources(void)
{
    void* pBuf;

    //-------------------------------------------------------------------------
    // load character data for 2D
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/OamManagerEx.NCGR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        DC_FlushRange( pCharData->pRawData, pCharData->szByte );
        GX_LoadOBJ( pCharData->pRawData, CHARA_BASE, pCharData->szByte );

        // �L�����N�^�f�[�^�� VRAM �ɃR�s�[�����̂�
        // ���� pBuf �͊J�����܂��B�ȉ������B
        G2DDemo_Free( pBuf );
    }

    //-------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/OamManagerEx.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        DC_FlushRange( pPlttData->pRawData, pPlttData->szByte );
        GX_LoadOBJPltt( pPlttData->pRawData, PLTT_BASE, pPlttData->szByte );

        G2DDemo_Free( pBuf );
    }
}



/*---------------------------------------------------------------------------*
  Name:         InitTemplateOam

  Description:  �\������ OBJ �̐��^�ɂȂ� OAM ���쐬���܂��B

  Arguments:    pChara:     �L�����N�^ OBJ �̐��^�ɂȂ� OAM �ւ̃|�C���^�B
                pBar:       �o�[ OBJ �̐��^�ɂȂ� OAM �ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitTemplateOam( GXOamAttr* pChara, GXOamAttr* pBar )
{
    // �\���L�[�œ�������L�����N�^�̐��^�ł�
    G2_SetOBJAttr(
        pChara,                 // OAM�f�[�^
        0,                      // X ���W
        0,                      // Y ���W
        0,                      // �\���D�揇��
        GX_OAM_MODE_NORMAL,     // OBJ ���[�h
        FALSE,                  // ���U�C�N
        GX_OAM_EFFECT_NONE,     // �G�t�F�N�g
        GX_OAM_SHAPE_32x32,     // �`��
        GX_OAM_COLORMODE_16,    // �J���[���[�h
        0x204,                  // �L�����N�^�l�[��
        0x8,                    // �J���[�p�����[�^
        0);                     // �A�t�B���C���f�b�N�X

    // �����z�u�����o�[�̐��^�ł�
    G2_SetOBJAttr(
        pBar,                   // OAM�f�[�^
        0,                      // X ���W
        0,                      // Y ���W
        0,                      // �\���D�揇��
        GX_OAM_MODE_NORMAL,     // OBJ ���[�h
        FALSE,                  // ���U�C�N
        GX_OAM_EFFECT_NONE,     // �G�t�F�N�g
        GX_OAM_SHAPE_32x16,     // �`��
        GX_OAM_COLORMODE_16,    // �J���[���[�h
        0x200,                  // �L�����N�^�l�[��
        0xD,                    // �J���[�p�����[�^
        0);                     // �A�t�B���C���f�b�N�X
}



/*---------------------------------------------------------------------------*
  Name:         ProcessInput

  Description:  �p�b�h���͂�ǂݎ��A�L�����N�^�̈ʒu�ƃo�[�̃��C���[����
                �ύX���܂��B

  Arguments:    pPos:       �L�����N�^�ʒu�Ƃ��ē��͂𔽉f������
                            Position �ւ̃|�C���^�B
                pNumLayer:  �o�[�̃��C���[���Ƃ��ē��͂𔽉f������
                            s8 �ւ̃|�C���^

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void ProcessInput(Position* pPos, s8* pNumLayer)
{
    G2DDemo_ReadGamePad();

    // �o�[���̑���
    if( G2DDEMO_IS_PRESS( PAD_BUTTON_A ) )
    {
        (*pNumLayer)++;
    }
    if( G2DDEMO_IS_PRESS( PAD_BUTTON_B ) )
    {
        (*pNumLayer)--;
    }
    if( *pNumLayer > MAX_NUM_OF_LAYER )
    {
        *pNumLayer = MAX_NUM_OF_LAYER;
    }
    if( *pNumLayer < 0 )
    {
        *pNumLayer = 0;
    }

    // �L�����N�^�̈ړ�
    if( G2DDEMO_IS_PRESS( PAD_KEY_LEFT ) )
    {
        pPos->x--;
    }
    if( G2DDEMO_IS_PRESS( PAD_KEY_RIGHT ) )
    {
        pPos->x++;
    }
    if( G2DDEMO_IS_PRESS( PAD_KEY_UP ) )
    {
        pPos->y--;
    }
    if( G2DDEMO_IS_PRESS( PAD_KEY_DOWN ) )
    {
        pPos->y++;
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
    NNSG2dOamManagerInstanceEx  oamManagerEx;   // �g�� OAM �}�l�[�W��
    GXOamAttr chara;                            // �L�����N�^���^ OAM
    GXOamAttr bar;                              // �o�[���^ OAM


    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_PrintInit();
        LoadResources();
        InitOamManagerEx( &oamManagerEx );
        InitTemplateOam( &chara, &bar );
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
        static Position pos = {CHARA_INIT_POS_X, CHARA_INIT_POS_Y};

        static s8 numLayer = INIT_NUM_OF_LAYER; // �o�[�̃��C���[��
        u8 charaPriority;                       // �L�����N�^�̕\���D��x
        int numObjDrawn = 0;                    // �\�����Ă���OBJ��
        static OSTick          timeForEntery, timeForApply;;
        


        // ���͂̏���
        {
            ProcessInput(&pos, &numLayer);
        }


        // �L�����̕\���D��x���v�Z���܂�
        {
            charaPriority = (u8)((- pos.y + BAR_SHIFT_X) / BAR_SHIFT_X);

            if( charaPriority >= MAX_NUM_OF_LAYER )
            {
                charaPriority = MAX_NUM_OF_LAYER;
            }
        }


        //
        // OAM �}�l�[�W���ւ̓o�^
        //
        {
            BOOL bSuccess;
            int i, r;

            // �L������ OAM �}�l�[�W���֓o�^���܂�
            {
                G2_SetOBJPosition(
                    &chara,
                    BAR_OFFSET_X + pos.x - pos.y,
                    BAR_OFFSET_Y - CHARA_HEIGHT + (pos.y * BAR_SHIFT_Y) / BAR_SHIFT_X
                );

                // �\���D��x���œo�^���܂�
                bSuccess = NNS_G2dEntryOamManExOam( &oamManagerEx, &chara, charaPriority );
                SDK_ASSERT( bSuccess );

                numObjDrawn++;
            }

            // �o�[�� OAM �}�l�[�W���֓o�^���܂�
            for( i = 0; i < numLayer; ++i )
            {
                int x, y;               // �o�[�̕\���ʒu
                u8 priority = (u8)i;    // �o�[�̕\���D��x

                // �D��x���L�����̕\���D��x�Ɣ��Ȃ��悤�ɂ���
                // �l�������������D��x�������i=�O�ɕ\�������j
                if( priority >= charaPriority )
                {
                    priority++;
                }

                // ��ʊO�̂��͕̂\�����Ȃ��iY�j
                y = BAR_OFFSET_Y - BAR_HEIGHT - i * BAR_SHIFT_Y;

                if( y < - BAR_HEIGHT || SCREEN_HEIGHT <= y )
                {
                    continue;
                }

                timeForEntery = OS_GetTick();
                for( r = 0; r < NUM_OF_LANE; ++r )
                {
                    // ��ʊO�̂��͕̂\�����Ȃ��iX)
                    x = BAR_OFFSET_X + i * BAR_SHIFT_X + r * LANE_SHIFT;

                    if( x < - BAR_WIDTH || SCREEN_WIDTH <= x )
                    {
                        continue;
                    }

                    G2_SetOBJPosition(&bar, x, y);

                    // �\���D��x���œo�^���܂�
                    bSuccess = NNS_G2dEntryOamManExOam( &oamManagerEx, &bar, priority );
                    SDK_ASSERT( bSuccess );

                    numObjDrawn++;
                }
                timeForEntery = OS_GetTick() - timeForEntery;
            }
        }


        // �\�������o�͂��܂�
        {
            G2DDemo_PrintOutf( 0, 0, "Chara Position: %4d / %4d", pos.x, pos.y);
            G2DDemo_PrintOutf( 0, 1, "Chara Priority: %4d", charaPriority);
            G2DDemo_PrintOutf( 0, 2, "Number of OBJ:  %4d", numObjDrawn);
            
            G2DDemo_PrintOutf( 0, 3, "Entry:%06ld usec\n", OS_TicksToMicroSeconds(timeForEntery) );
            G2DDemo_PrintOutf( 0, 4, "Apply:%06ld usec\n", OS_TicksToMicroSeconds(timeForApply) );
        }


        // V Blank �҂�
        SVC_WaitVBlankIntr();


        //
        // �o�b�t�@�̓��e��HW�ɏ����o���܂�
        //
        {
            // �\�����������o���܂�
            G2DDemo_PrintApplyToHW();

            timeForApply = OS_GetTick();
            // OBJ�������o���܂�
            NNS_G2dApplyOamManExToBaseModule( &oamManagerEx );

            // �g�� OAM �}�l�[�W�������Z�b�g���܂��B
            // ���Z�b�g���Ă��������\���Ɋւ�����͈ێ�����܂��B
            NNS_G2dResetOamManExBuffer( &oamManagerEx );
            timeForApply = OS_GetTick() - timeForApply;
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
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );    // checking VBlank interrupt
}


