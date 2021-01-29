/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - IDE - IDE_G2dDemoLib
  File:     system.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/
#include "g2d_demolib.h"
#include <nnsys/g2d/g2d_Softsprite.h>
#include <nnsys/g2d/g2d_Load.h>

// ���\�[�X�v�[���̃T�C�Y
#define SIZE_OF_RES_POOL        1000

// �A���C�����g�ɂ��킹�Đ؂�グ
#define ROUND_UP(value, alignment) \
    (((u32)(value) + (alignment-1)) & ~(alignment-1))

// �A���C�����g�ɂ��킹�Đ؂艺��
#define ROUND_DOWN(value, alignment) \
    ((u32)(value) & ~(alignment-1))

G2DDemoGamePad         gGamePad;         // �Q�[���p�b�h
NNSFndHeapHandle       gHeapHandle;      // �q�[�v�n���h��

/*---------------------------------------------------------------------------*
    �������֌W�B
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         Init3DStuff_

  Description:  �\�t�g�E�F�A�X�v���C�g�̂��߂̃O���t�B�b�N�X�G���W����
                ���������s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void Init3DStuff_(void)
{
    G3X_Init();                                            // initialize the 3D graphics states
    G3X_InitMtxStack();                                    // initialize the matrix stack

    // ���C�����
    {
        GX_SetBankForTex(GX_VRAM_TEX_0_A);                          // VRAM-A for texture images
        GX_SetBankForOBJ(GX_VRAM_OBJ_128_B);                        // VRAM-B for OBJ
        GX_SetBankForBG(GX_VRAM_BG_128_C);                          // VRAM-C for BGs
        GX_SetBankForTexPltt(GX_VRAM_TEXPLTT_0123_E);               // VRAM-E for texture palettes
        GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,                    // graphics mode
                           GX_BGMODE_4,                             // BGMODE is 4
                           GX_BG0_AS_3D);                           // BG #0 is for 3D
        GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D );             // 2D mapping mode
        GX_SetVisiblePlane( GX_PLANEMASK_BG0 | GX_PLANEMASK_OBJ );  // display BG0 & OBJ
    }

    // �T�u���
    {
        GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_128_D);            // VRAM-D for OBJ
        GXS_SetGraphicsMode(GX_BGMODE_0);                      // BGMODE is 0
        GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);        // 2D mapping mode
        GXS_SetVisiblePlane(GX_PLANEMASK_OBJ);                 // display only OBJ
    }


    G2_SetBG0Priority(1);

    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaBlend(TRUE);

    // 3D �ʂ͓������OFF�ł�����BG�ʂƃ��u�����f�B���O���s���܂����A
    // ���̂��߂ɂ̓��u�����f�B���O�Ώۖʂ̎w����s���K�v������܂��B
    // SDK �ɂ͑Ώۖʂ݂̂�ݒ肷�� API �����݂��Ȃ��̂ŁA����� G2_SetBlendAlpha ���g���܂��B
    // ���̏ꍇ G2_SetBlendAlpha �̌��2�̈����͖�������܂��B
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BD, 0, 0);

    // clear color
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 0, FALSE );

    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_CommonInit

  Description:  OS�A�O���t�B�b�N�X�G���W���A�q�[�v�A�t�@�C���V�X�e����
                ���������s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G2DDemo_CommonInit(void)
{
    // Common initialization.
    {
        OS_Init();

        FX_Init();

        GX_Init();

        GX_DispOff();
        GXS_DispOff();

        OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
        (void)OS_EnableIrqMask( OS_IE_V_BLANK );
        (void)OS_EnableIrq();

        (void)GX_VBlankIntr(TRUE);
    }

    // Clear VRAM
    {
        //---------------------------------------------------------------------------
        // All VRAM banks to LCDC
        //---------------------------------------------------------------------------
        GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);

        //---------------------------------------------------------------------------
        // Clear all LCDC space
        //---------------------------------------------------------------------------
        MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);

        //---------------------------------------------------------------------------
        // Disable the banks on LCDC
        //---------------------------------------------------------------------------
        (void)GX_DisableBankForLCDC();

        MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);      // clear OAM
        MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);        // clear the standard palette

        MI_CpuFillFast((void*)HW_DB_OAM, 192, HW_DB_OAM_SIZE); // clear OAM
        MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);  // clear the standard palette
    }

    //------------------------------------------------------------------------------
    // Initializing the 3D Sprite Engine.
    //------------------------------------------------------------------------------
    Init3DStuff_();
    NNS_G2dSetupSoftwareSpriteCamera();


    //------------------------------------------------------------------------------
    // misc.
    {
        *(GXRgb*)(HW_BG_PLTT) = GX_RGB(15, 15, 15);
        *(GXRgb*)(HW_DB_BG_PLTT) = GX_RGB(15, 15, 15);
        OS_InitTick();
    }
    //------------------------------------------------------------------------------
    // Heap �m��
    {
        u32   arenaLow      = ROUND_UP  (OS_GetMainArenaLo(), 16);
        u32   arenaHigh     = ROUND_DOWN(OS_GetMainArenaHi(), 16);
        u32   heapSize      = arenaHigh - arenaLow;
        void* heapMemory    = OS_AllocFromMainArenaLo(heapSize, 16);

        gHeapHandle = NNS_FndCreateExpHeap(heapMemory, heapSize);
        SDK_ASSERT( gHeapHandle != NNS_FND_HEAP_INVALID_HANDLE );
    }

    //------------------------------------------------------------------------------
    // File System
    {
        (void)OS_EnableIrqMask(OS_IE_FIFO_RECV);

        /* initialize file-system */
        FS_Init( FS_DMA_NOT_USE );

        /* always preload FS table for faster directory access. */
        {
            const u32   need_size = FS_GetTableSize();
            void    *p_table = G2DDemo_Alloc(need_size);
            SDK_ASSERT(p_table != NULL);
            (void)FS_LoadTable(p_table, need_size);
        }
    }
    
    // �Q�[���p�b�h����ǂ݂��܂��B
    G2DDemo_ReadGamePad();
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_CameraSetup

  Description:  �J�����̋��ʐݒ���s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G2DDemo_CameraSetup(void)
{
    {
        // �P�ʍs��Ɠ���
        VecFx32 Eye = { 0, 0, 0 };                  // Eye position
        VecFx32 vUp = { 0, FX32_ONE, 0 };           // Up
        VecFx32 at = { 0, 0, -FX32_ONE };           // Viewpoint

        // Matrix mode is changed to GX_MTXMODE_POSITION_VECTOR internally,
        // and the camera matrix is loaded to the current matrix.
        G3_LookAt(&Eye, &vUp, &at, NULL);
    }
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_MaterialSetup

  Description:  �}�e���A���̋��ʐݒ���s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G2DDemo_MaterialSetup(void)
{
    // for software sprite-setting
    {
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),        // diffuse
                                GX_RGB(16, 16, 16),        // ambient
                                TRUE                       // use diffuse as vtx color if TRUE
                                );

        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),        // specular
                                GX_RGB(0, 0, 0),           // emission
                                FALSE                      // use shininess table if TRUE
                                );

        G3_PolygonAttr(GX_LIGHTMASK_NONE,                  // no lights
                       GX_POLYGONMODE_MODULATE,            // modulation mode
                       GX_CULL_NONE,                       // cull back
                       0,                                  // polygon ID(0 - 63)
                       31,                                 // alpha(0 - 31)
                       0                                   // OR of GXPolygonAttrMisc's value
                       );
    }
}


/*---------------------------------------------------------------------------*
    ���̑�
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_GetNewCellAnimation

  Description:  �v�[������ NNSG2dCellAnimation ���擾���܂��B

  Arguments:    num:  �擾���� NNSG2dCellAnimation �̐��B

  Returns:      �擾���� NNSG2dCellAnimation �ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
NNSG2dCellAnimation* G2DDemo_GetNewCellAnimation( u16 num )
{
    static NNSG2dCellAnimation        sPoolAnimatedObj[SIZE_OF_RES_POOL];
    static u16                        sNumAnimatedObj = 0;

    const u16 retIdx = sNumAnimatedObj;

    sNumAnimatedObj += num;
    SDK_ASSERT( sNumAnimatedObj <= SIZE_OF_RES_POOL );

    return &sPoolAnimatedObj[ retIdx ];
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_GetNewNode

  Description:  �v�[������ NNSG2dNode ���擾���܂��B

  Arguments:    num:  �擾���� NNSG2dNode �̐��B

  Returns:      �擾���� NNSG2dNode �ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
NNSG2dNode* G2DDemo_GetNewNode( u16 num )
{
    static NNSG2dNode                 sPoolObjNode[SIZE_OF_RES_POOL];
    static u16                        sNumUsedPoolObjNode = 0;

    const u16 retIdx = sNumUsedPoolObjNode;

    sNumUsedPoolObjNode += num;
    SDK_ASSERT( sNumUsedPoolObjNode <= SIZE_OF_RES_POOL );

    return &sPoolObjNode[ retIdx ];
}

/*---------------------------------------------------------------------------*
  Name:         getNewMultiCellAnim_

  Description:  �v�[������ NNSG2dMultiCellAnimation ���擾���܂��B

  Arguments:    num:  �擾���� NNSG2dMultiCellAnimation �̐��B

  Returns:      �擾���� NNSG2dMultiCellAnimation �ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
static NNSG2dMultiCellAnimation*     GetNewMultiCellAnim_( u16 num )
{
    static NNSG2dMultiCellAnimation   sMcAnimPool_[SIZE_OF_RES_POOL];
    static u16                        sNumUsedMcAnimPool = 0;

    const u16 retIdx = sNumUsedMcAnimPool;

    sNumUsedMcAnimPool += num;
    SDK_ASSERT( sNumUsedMcAnimPool <= SIZE_OF_RES_POOL );

    return &sMcAnimPool_[retIdx];
}


/*---------------------------------------------------------------------------*
  Name:         G2DDemo_GetNewMultiCellAnimation

  Description:  �V���� NNSG2dMultiCellAnimation ���擾���ď��������܂��B

  Arguments:    numNode:            �m�[�h��
                pAnimBank:          �A�j���[�V�����f�[�^�o���N
                pCellDataBank:      �Z���f�[�^�o���N
                pMultiCellDataBank: �}���`�Z���f�[�^�o���N

  Returns:      �v�[������擾�����������ς݂� NNSG2dMultiCellAnimation ��
                �̃|�C���^
 *---------------------------------------------------------------------------*/
/* �Â�API���g�p����ȑO�̃o�[�W����
NNSG2dMultiCellAnimation*     G2DDemo_GetNewMultiCellAnimation
(
    u16                                numNode,
    const NNSG2dCellAnimBankData*      pAnimBank,
    const NNSG2dCellDataBank*          pCellDataBank ,
    const NNSG2dMultiCellDataBank*     pMultiCellDataBank
)
{
    // �v�[������ NNSG2dMultiCellAnimation ���擾
    NNSG2dMultiCellAnimation* pMCAnim = GetNewMultiCellAnim_( 1 );

    // �v�[������ numNode �Ԃ�� NNSG2dNode �� NNSG2dCellAnimation ���擾
    NNSG2dNode*               pNodeArray     = G2DDemo_GetNewNode( numNode );
    NNSG2dCellAnimation*      pCellAnimArray = G2DDemo_GetNewCellAnimation( numNode );

    NNS_G2dInitMCAnimation( pMCAnim,              // �}���`�Z���A�j���[�V����
                                  pNodeArray,           // �m�[�h�z��
                                  pCellAnimArray,       // �Z���A�j���[�V�����z��
                                  numNode,              // �m�[�h��
                                  pAnimBank,            // �A�j���[�V�����f�[�^�o���N
                                  pCellDataBank,        // �Z���f�[�^�o���N
                                  pMultiCellDataBank ); // �}���`�Z���f�[�^�o���N
    return pMCAnim;
}
*/

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_GetNewMultiCellAnimation

  Description:  �V���� NNSG2dMultiCellAnimation ���擾���ď��������܂��B

  Arguments:    pAnimBank:          �A�j���[�V�����f�[�^�o���N
                pCellDataBank:      �Z���f�[�^�o���N
                pMultiCellDataBank: �}���`�Z���f�[�^�o���N
                mcType:             �}���`�Z�����̂̎��
                
  Returns:      �v�[������擾�����������ς݂� NNSG2dMultiCellAnimation ��
                �̃|�C���^
 *---------------------------------------------------------------------------*/
NNSG2dMultiCellAnimation*     G2DDemo_GetNewMultiCellAnimation
(
    const NNSG2dCellAnimBankData*      pAnimBank,
    const NNSG2dCellDataBank*          pCellDataBank ,
    const NNSG2dMultiCellDataBank*     pMultiCellDataBank,
    NNSG2dMCType                       mcType
)
{
    const u32 szWork = NNS_G2dGetMCWorkAreaSize( pMultiCellDataBank, mcType );
    void* pWorkMem   = NNS_FndAllocFromExpHeap( gHeapHandle, szWork );
    
    
    // �v�[������ NNSG2dMultiCellAnimation ���擾
    NNSG2dMultiCellAnimation* pMCAnim = GetNewMultiCellAnim_( 1 );

    // ������
    NNS_G2dInitMCAnimationInstance( pMCAnim, 
                                    pWorkMem, 
                                    pAnimBank, 
                                    pCellDataBank, 
                                    pMultiCellDataBank, 
                                    mcType );
    return pMCAnim;
}




/*---------------------------------------------------------------------------*
    �Q�[���p�b�h�֌W�B
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_ReadGamePad

  Description:  �L�[��ǂݍ��݁A�g���K�ƃ����[�X�g���K�����߂܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G2DDemo_ReadGamePad(void)
{
    u16 status = PAD_Read();

    gGamePad.trigger = (u16)(status          & (status ^ gGamePad.button));
    gGamePad.release = (u16)(gGamePad.button & (status ^ gGamePad.button));
    gGamePad.button  = status;
}

