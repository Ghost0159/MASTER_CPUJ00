/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/Sub_Double3D
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.22  2007/10/11 02:20:22  yosizaki
  fix comparator function.
  add check for the result of MATH_QSort().

  Revision 1.21  2007/04/26 04:08:30  okubata_ryoma
  2��ʂɕʁX��fog��ݒ肷��ۂ̃R�[�h��ǉ�

  Revision 1.20  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.19  2005/11/21 10:44:16  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.18  2005/10/07 09:02:12  okubata_ryoma
  ���C��

  Revision 1.17  2005/10/07 05:22:48  okubata_ryoma
  �R�����g�̏C��

  Revision 1.16  2005/10/07 05:11:45  okubata_ryoma
  �R�����g�̏C��

  Revision 1.14  2005/08/03 02:03:11  okubata_ryoma
  �ēx��肱�݂̕s��̏C��

  Revision 1.13  2005/07/07 06:16:11  okubata_ryoma
  �I�u�W�F�N�g�̉�荞�݌��ۂ��C�����܂���

  Revision 1.10  2005/07/07 04:47:46  okubata_ryoma
  ���C��

  Revision 1.8  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.7  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.6  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.5  2004/04/06 12:48:08  yada
  fix header comment

  Revision 1.4  2004/02/24 10:48:30  yasu
  fix initial value of "flag"

  Revision 1.3  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.2  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.1  2004/02/04 11:11:04  nishida_kenji
  new

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that displays moving 3D models on both LCDs.
//
// Both LCDs display a different rotating 3D cube.
// The sub 2D engine displays the captured image rendered by the main engine,
// making use of direct color bitmap BG/bitmap OBJ.
// Note that the output LCDs are swapped every frame.
//
// HOWTO:
// 1. Set up OAM for the sub 2D engine, to display captured images correctly.
// 2. (Frame 2N):   Assign VRAM-C to BG of the sub 2D engine,
//                  and display the captured image on VRAM-C
//                  through direct color bitmap BG.
//                  Capture the 3D plane and store it on VRAM-D.
//    (Frame 2N+1): Assign VRAM-D to OBJ of the sub 2D engine,
//                  and display the captured image on VRAM-D
//                  through bitmap OBJs.
//                  Capture the 3D plane and store it on VRAM-C.
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"

#define    STACK_SIZE     1024
#define    THREAD1_PRIO   (OS_THREAD_LAUNCHER_PRIORITY - 6)
void    proc1(void *p1);
OSThread thread1;
u32     stack1[STACK_SIZE / sizeof(u32)];

static GXOamAttr sOamBak[128];

BOOL    flip_flag = TRUE;              // flip�؂�ւ��t���O
BOOL    swap = FALSE;                  // SwapBuffers���s�t���O
int     attr = GX_POLYGON_ATTR_MISC_NONE;

s16     gCubeGeometry[3 * 8] = {
    FX16_ONE, FX16_ONE, FX16_ONE,
    FX16_ONE, FX16_ONE, -FX16_ONE,
    FX16_ONE, -FX16_ONE, FX16_ONE,
    FX16_ONE, -FX16_ONE, -FX16_ONE,
    -FX16_ONE, FX16_ONE, FX16_ONE,
    -FX16_ONE, FX16_ONE, -FX16_ONE,
    -FX16_ONE, -FX16_ONE, FX16_ONE,
    -FX16_ONE, -FX16_ONE, -FX16_ONE
};

GXRgb   gCubeColor[8] = {
    GX_RGB(31, 31, 31),
    GX_RGB(31, 31, 0),
    GX_RGB(31, 0, 31),
    GX_RGB(31, 0, 0),
    GX_RGB(0, 31, 31),
    GX_RGB(0, 31, 0),
    GX_RGB(0, 0, 31),
    GX_RGB(0, 0, 0)
};

static void Color(int idx)
{
    G3_Color(gCubeColor[idx]);
}

static void Vtx(int idx)
{
    G3_Vtx(gCubeGeometry[idx * 3], gCubeGeometry[idx * 3 + 1], gCubeGeometry[idx * 3 + 2]);
}

static void Quad(int idx0, int idx1, int idx2, int idx3)
{
    Vtx(idx0);
    Vtx(idx1);
    Vtx(idx2);
    Vtx(idx3);
}

static void ColVtxQuad(int idx0, int idx1, int idx2, int idx3)
{
    Color(idx0);
    Vtx(idx0);
    Color(idx1);
    Vtx(idx1);
    Color(idx2);
    Vtx(idx2);
    Color(idx3);
    Vtx(idx3);
}

static void drawLeftCube(u16 Rotate)
{
    G3_PushMtx();

    // Rotate and translate
    G3_Translate(-3 << (FX32_SHIFT - 1), 0, 0);
    {
        fx16    s = FX_SinIdx(Rotate);
        fx16    c = FX_CosIdx(Rotate);

        G3_RotX(s, c);
        G3_RotY(s, c);
        G3_RotZ(s, c);
    }

    G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), // diffuse
                            GX_RGB(16, 16, 16), // ambient
                            FALSE      // use diffuse as vtx color if TRUE
        );

    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), // specular
                            GX_RGB(0, 0, 0),    // emission
                            FALSE      // use shininess table if TRUE
        );

    G3_PolygonAttr(GX_LIGHTMASK_NONE,  // disable lights
                   GX_POLYGONMODE_MODULATE,     // modulation mode
                   GX_CULL_BACK,       // cull back
                   0,                  // polygon ID(0 - 63)
                   31,                 // alpha(0 - 31)
                   attr                // OR of GXPolygonAttrMisc's value
        );

    //---------------------------------------------------------------------------
    // Draw a cube:
    // Specify different colors for the planes.
    //---------------------------------------------------------------------------

    G3_Begin(GX_BEGIN_QUADS);

    {
        Color(3);
        Quad(2, 0, 4, 6);

        Color(4);
        Quad(7, 5, 1, 3);

        Color(5);
        Quad(6, 4, 5, 7);

        Color(2);
        Quad(3, 1, 0, 2);

        Color(6);
        Quad(5, 4, 0, 1);

        Color(1);
        Quad(6, 7, 3, 2);
    }

    G3_End();

    G3_PopMtx(1);
}

static void drawRightCube(u16 Rotate)
{
    G3_PushMtx();

    // Rotate and translate
    G3_Translate(3 << (FX32_SHIFT - 1), 0, 0);

    {
        fx16    s = FX_SinIdx(Rotate);
        fx16    c = FX_CosIdx(Rotate);

        G3_RotX(s, c);
        G3_RotY(s, c);
        G3_RotZ(s, c);
    }

    G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), // diffuse
                            GX_RGB(16, 16, 16), // ambient
                            FALSE      // use diffuse as vtx color if TRUE
        );

    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), // specular
                            GX_RGB(0, 0, 0),    // emission
                            FALSE      // use shininess table if TRUE
        );

    G3_PolygonAttr(GX_LIGHTMASK_NONE,  // disable lights
                   GX_POLYGONMODE_MODULATE,     // modulation mode
                   GX_CULL_BACK,       // cull back
                   0,                  // polygon ID(0 - 63)
                   31,                 // alpha(0 - 31)
                   attr                // OR of GXPolygonAttrMisc's value
        );

    //---------------------------------------------------------------------------
    // Draw a cube:
    // Specify different colors for the vertices.
    //---------------------------------------------------------------------------
    G3_Begin(GX_BEGIN_QUADS);
    {
        ColVtxQuad(2, 0, 4, 6);
        ColVtxQuad(7, 5, 1, 3);
        ColVtxQuad(6, 4, 5, 7);
        ColVtxQuad(3, 1, 0, 2);
        ColVtxQuad(5, 4, 0, 1);
        ColVtxQuad(6, 7, 3, 2);
    }
    G3_End();

    G3_PopMtx(1);

}

static void setupFrame2N(void)
{

    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);

    (void)GX_ResetBankForSubOBJ();
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);
    GX_SetBankForLCDC(GX_VRAM_LCDC_D);
    GX_SetCapture(GX_CAPTURE_SIZE_256x192,
                  GX_CAPTURE_MODE_A,
                  GX_CAPTURE_SRCA_3D, (GXCaptureSrcB)0, GX_CAPTURE_DEST_VRAM_D_0x00000, 16, 0);

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    G2_SetBG0Priority(0);

    GXS_SetGraphicsMode(GX_BGMODE_5);
    GXS_SetVisiblePlane(GX_PLANEMASK_BG2);
    G2S_SetBG2ControlDCBmp(GX_BG_SCRSIZE_DCBMP_256x256,
                           GX_BG_AREAOVER_XLU, GX_BG_BMPSCRBASE_0x00000);
    G2S_SetBG2Priority(0);
    G2S_BG2Mosaic(FALSE);
}

static void setupFrame2N_1(void)
{
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    (void)GX_ResetBankForSubBG();
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_128_D);
    GX_SetBankForLCDC(GX_VRAM_LCDC_C);
    GX_SetCapture(GX_CAPTURE_SIZE_256x192,
                  GX_CAPTURE_MODE_A,
                  GX_CAPTURE_SRCA_3D, (GXCaptureSrcB)0, GX_CAPTURE_DEST_VRAM_C_0x00000, 16, 0);

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    G2_SetBG0Priority(0);

    GXS_SetGraphicsMode(GX_BGMODE_5);
    GXS_SetVisiblePlane(GX_PLANEMASK_OBJ);
}

static void setupSubOAM(void)
{
    int     i;
    int     x, y;
    int     idx = 0;

    GXS_SetOBJVRamModeBmp(GX_OBJVRAMMODE_BMP_2D_W256);

    for (i = 0; i < 128; ++i)
    {
        sOamBak[i].attr01 = 0;
        sOamBak[i].attr23 = 0;
    }

    for (y = 0; y < 192; y += 64)
    {
        for (x = 0; x < 256; x += 64, idx++)
        {
            G2_SetOBJAttr(&sOamBak[idx],
                          x,
                          y,
                          0,
                          GX_OAM_MODE_BITMAPOBJ,
                          FALSE,
                          GX_OAM_EFFECT_NONE,
                          GX_OAM_SHAPE_64x64, GX_OAM_COLOR_16, (y / 8) * 32 + (x / 8), 15, 0);
        }
    }

    DC_FlushRange(&sOamBak[0], sizeof(sOamBak));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GXS_LoadOAM(&sOamBak[0], 0, sizeof(sOamBak));

}

//---------------------------------------------------------------------------
// SwapBuffers���s�t���O�𗧂Ă�
//---------------------------------------------------------------------------
static void SetSwapBuffersflag(void)
{
    OSIntrMode old = OS_DisableInterrupts();    // ���荞�݋֎~
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    swap = TRUE;
    (void)OS_RestoreInterrupts(old);
}

static BOOL IgnoreRemoval(void)
{
    return FALSE;
}

void NitroMain(void)
{
//#define CPU_TEST                      // CPU�ɕ��ׂ�^���܂�(�e�X�g�p)
//#define GE_TEST                       // �W�I���g���G���W���ɕ��ׂ�^���܂�(�e�X�g�p)

//#define SET_FOG                       // Double3D���g�p��2��ʂŕʁX��fog��ݒ肵�����ꍇ��
                                        // ����define��L���ɂ����ۂ̃R�[�h�����Q�l���������B

    u16     Rotate = 0;                // for rotating cubes(0-65535)

    int     i = 0;

#if defined(CPU_TEST) || defined(GE_TEST)
    MATHRandContext32 rnd;             // CPU�ɕ��ׂ�^���闐���𐶐����邽�߂̕ϐ��i�e�X�g�p�j
#endif
    //---------------------------------------------------------------------------
    // Initialize:
    // They enable IRQ interrupts, initialize VRAM, and set BG #0 for 3D mode.
    //---------------------------------------------------------------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    CARD_SetPulledOutCallback(IgnoreRemoval);

    OS_InitThread();
    OS_CreateThread(&thread1, proc1, NULL, stack1 + STACK_SIZE / sizeof(u32), STACK_SIZE,
                    THREAD1_PRIO);
    OS_WakeupThreadDirect(&thread1);

    G3X_Init();
    G3X_InitTable();
    G3X_InitMtxStack();
    G3X_AntiAlias(TRUE);
    setupSubOAM();

    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    G3X_SetClearColor(GX_RGB(0, 0, 0), // clear color
                      0,               // clear alpha
                      0x7fff,          // clear depth
                      63,              // clear polygon ID
                      FALSE            // fog
        );

    G3_ViewPort(0, 0, 255, 191);       // Viewport

    //---------------------------------------------------------------------------
    // Set up the projection matrix
    //---------------------------------------------------------------------------
    {
        fx32    right = FX32_ONE;
        fx32    top = FX32_ONE * 3 / 4;
        fx32    near = FX32_ONE;
        fx32    far = FX32_ONE * 400;

        //---------------------------------------------------------------------------
        // Switch MTXMODE to GX_MTXMODE_PROJECTION, and
        // set a projection matrix onto the current projection matrix on the matrix stack
        //---------------------------------------------------------------------------
        G3_Perspective(FX32_SIN30, FX32_COS30,  // sine and cosine of FOVY
                       FX32_ONE * 4 / 3,        // aspect
                       near,           // near
                       far,            // far
                       NULL            // a pointer to a matrix if you use it
            );

        G3_StoreMtx(0);
    }

#if defined(SET_FOG)
    /* �t�H�O�ݒ� */
    {
        u32     fog_table[8];
        int     i;

        // �t�H�O�����̐ݒ�
        G3X_SetFog(TRUE,               // �t�H�O��L����
                   GX_FOGBLEND_COLOR_ALPHA,     // �J���[�ƃA���t�@�Ƀt�H�O
                   GX_FOGSLOPE_0x2000, // �t�H�O���z�̐ݒ�
                   0x4800);            // �t�H�O�v�Z�f�v�X�l
        G3X_SetFogColor(GX_RGB(31, 31, 31), 0); // �t�H�O�J���[�̐ݒ�

        // �t�H�O�e�[�u���̐ݒ�(�l���傫���قǔZ���t�H�O�ɂȂ�)
        for (i = 0; i < 8; i++)
        {
            fog_table[i] = (u32)(((i * 16) << 0) |
                                 ((i * 16 + 4) << 8) |
                                 ((i * 16 + 8) << 16) | ((i * 16 + 12) << 24));
        }
        G3X_SetFogTable(&fog_table[0]);
    }
#endif

    DEMOStartDisplay();

#if defined(CPU_TEST) || defined(GE_TEST)
    MATH_InitRand32(&rnd, OS_GetVBlankCount()); // CPU�ɕ��ׂ�^���邽�߂̗����̐����i�e�X�g�p�j
#endif

    while (1)
    {
        DEMOReadKey();


        G3X_Reset();

        Rotate += 256;

        //---------------------------------------------------------------------------
        // Set up camera matrix
        //---------------------------------------------------------------------------
        {
            VecFx32 Eye = { 0, 0, FX32_ONE * 5 };       // Eye Position
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);
        }
        G3_PushMtx();

#if defined(SET_FOG)
        if(DEMO_IS_TRIG(PAD_BUTTON_A))
        {
            if(attr)
            {
                // fog �𖳌��ɂ��܂�
                attr = GX_POLYGON_ATTR_MISC_NONE;
            }
            else
            {
                // fog ��L���ɂ��܂�
                attr = GX_POLYGON_ATTR_MISC_FOG;
            }
        }
#endif

        if (flip_flag)
            drawRightCube(Rotate);
        else
            drawLeftCube(Rotate);
        G3_PopMtx(1);

#ifdef GE_TEST
        while (GX_GetVCount() != 191)
        {;
        }
#endif

#ifdef CPU_TEST
        // �����I��CPU�ɕ��ׂ�^���܂��i�e�X�g�p�j
        if (DEMO_IS_PRESS(PAD_BUTTON_R))
        {
            OS_SpinWait(MATH_Rand32(&rnd, 1000000));
        }
#endif

#ifdef GE_TEST
        // �����I�ɃW�I���g���G���W���ɕ��ׂ�^���܂� (�e�X�g�p)
        while (i <= MATH_Rand32(&rnd, 1000000))
        {
            G3_PushMtx();
            G3_PopMtx(1);
            i++;
        }
#endif

#if defined(SET_FOG)
        // V�J�E���g180�܂� wait
        // (����wait�̌ォ��V�u�����N�J�n���C���܂łɃt�H�O�̏�������������悤�ɂ���K�v������B
        //  ����̏ꍇ�͗]�T��������180���C���ɐݒ�)
        while (GX_GetVCount() != 180)
        {;
        }

        // SwapBuffers �R�}���h�͊m���ɏ����������������Ȃ��t���[���ł̂ݔ��s
        // (V�J�E���g180�ŕ`�悪�������Ă���Ώ��������͔������Ȃ�����)
        if (!G3X_IsGeometryBusy())
        {
            SetSwapBuffersflag();          // SwapBuffers �R�}���h�𔭍s
            
            if (flip_flag)                 // flip�̐؂�ւ��t���O
            {
                // ����ʂ̃t�H�O�̐F��ݒ�
                G3X_SetFogColor(GX_RGB(0, 0, 0), 0);
            }
            else
            {
                // ���ʂ̃t�H�O�̐F��ݒ�
                G3X_SetFogColor(GX_RGB(31, 31, 31), 0);
            }
        }
#else
        SetSwapBuffersflag();          // SwapBuffers���s�t���O�𗧂Ă�
#endif
        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt

    }
}

//---------------------------------------------------------------------------
// VBlank interrupt function:
//
// Interrupt handlers are registered on the interrupt table by OS_SetIRQFunction.
// OS_EnableIrqMask selects IRQ interrupts to enable, and
// OS_EnableIrq enables IRQ interrupts.
// Notice that you have to call 'OS_SetIrqCheckFlag' to check a VBlank interrupt.
//---------------------------------------------------------------------------
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // checking VBlank interrupt

    //---- �X���b�h�P�N��
    OS_WakeupThreadDirect(&thread1);
}
/*
---------------------------------------------------------------------------------------------------

��SwapBuffers�R�}���h�ƃW�I���g���G���W���̓���̐���

�ESwapBuffers�R�}���h��G3_SwapBuffers�֐����Ă񂾂Ƃ��ɁA�W�I���g���R�}���hFIFO�Ɋi�[����
  SwapBuffers�R�}���h�͂ǂ̃^�C�~���O�ŃW�I���g���R�}���hFIFO����Ă΂ꂽ�Ƃ��Ă��A
  ����V�u�����N�̊J�n���ɁA�o�b�t�@�̃X���b�v���������s����܂��B
  (�܂�AV�u�����N�̊J�n���ɂ������s�ł��܂���)
�E���̂��߁A�����`�揈���Ȃǂ�V�u�����N���ԓ��܂ł��ꍞ��ŁA
  SwapBuffers�R�}���h�̎��s��V�u�����N�̊J�n���܂łɂł��Ȃ������ꍇ�A
  ���̃t���[����V�u�����N�J�n���܂ŃW�I���g���G���W���̓r�W�[�̂܂ܖ�1�t���[���҂�������B
  ���̊Ԃ͉摜�̕`���X���b�v�Ȃǂ͂ł��Ȃ��ł��̃t���[���͑O�Ɠ����摜��\������B


s : SwapBuffers�R�}���h���s
S : �o�b�t�@�̃X���b�v�������s
w : SwapBuffers�R�}���h���s����o�b�t�@�̃X���b�v�������s�J�n�܂ł̃E�F�C�g
G : �W�I���g���G���W�����`��Ȃǂ̏�����

                          |
-VBlank(end)------------------------------------------
               +-----+    |     +-----+
               |     |    |     |     |
               |     |    |     |     |
               |  G  |    |     |  G  |
               |     |    |     |     |
               |     |    |     |     |
               |     |    |     |     |
               |     |    |     |     |
               |     |    |     |     |
               +-----+    |     |     |
               |  s  |    |     |     |
               +-----+    |     |     |
               |  w  |    |     |     |
-VBlank(start)-+-----+----+-----+     +-----------
  ��784(cycle) |  S  |    |     |     |            �����̃X���b�v������CPU�T�C�N����784�T�C�N��������܂�
               +-----+    |     +-----+           
    ��check  ��           |     |  s  |            ������check�̕�����G3X_IsGeometryBusy�֐���p����
                          |     +-----+              �X���b�v�������I��������ǂ����𔻒肵�Ă��܂��B�@�@
-VBlank(end)--------------+-----+     +---------
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |  w  |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
-VBlank(start)------------+-----+-----+----------
                          |     |  S  |
                          |     +-----+
                          |
                          |
-VBlank(end)--------------+-----------------------


����肪�����������
    �EV�u�����N���Ńo�b�t�@�̃X���b�v�������������Ă��Ȃ��̂ɁA�㉺��ʂ̐؂�ւ����s�����ꍇ
      �㉺�œ�����ʂ��\������Ă��܂��܂��B

�����_��h������V�u�����N���ł��ׂ�����
    �EV�u�����N�ɓ���O�܂łɁA���̉摜�̕`�悪�I�����Ă��邱�Ƃ��m�F
      �`�悪�I�����Ă���Ȃ�A����V�u�����N�J�n���Ƀo�b�t�@�̃X���b�v���������s����܂��B
    �EV�u�����N���ŁA�o�b�t�@�̃X���b�v�������I���������Ƃ��m�F
      �X���b�v�������I�����Ă���Ȃ�A�㉺��ʂ̐؂�ւ����s���܂��B

    ���̂��߂�
    �EV�u�����N�ɓ���O�ɁA���̉摜�̕`�悪�I�����Ă��邱��(����V�u�����N�ŃX���b�v�������m���Ɏn�܂�)
    �EV�u�����N���ŁA�o�b�t�@�̃X���b�v�������I����������
    �����̂Q���m�F����K�v������܂��B

����̓I��V�u�����N���ōs���Ă��鏈��

        �E if(GX_GetVCount() <= 193)               �X���b�v��������ɂ�1���C����������Ȃ��̂�
                                                   ���̎��_��193���C���ɒB���Ă���΁A
                                                   �X���b�v�����͏I����Ă���Ɣ��f�ł��A
                                                   OS_SpinWait�֐���784�T�C�N���҂K�v������܂���B
                                                   
        �E OS_SpinWait(784)                        784�T�C�N���҂��܂��B
        
        �E if (!G3X_IsGeometryBusy() && swap)      V�u�����N�O�� SwapBuffers �R�}���h�����s�ς݂ŁA
                                                   �W�I���g���G���W�����r�W�[�łȂ��ꍇ
                                                   (�摜�̕`�揈����o�b�t�@�̃X���b�v�������s���łȂ�)
                                                
�������̈Ӗ�
    �o�b�t�@�̃X���b�v����(�����_�����O�G���W�����Q�Ƃ���f�[�^��؂�ւ���)�ɂ́A
    CPU�T�C�N����784(392�~2)�T�C�N���K�v�Ȃ��߁A
    �����AV�u�����N�̊J�n���Ƀo�b�t�@�̃X���b�v�������J�n���ꂽ�̂��Ƃ�����A784�T�C�N���҂Ă�
    �o�b�t�@�̃X���b�v�����͏I����Ă���Ɣ��f�ł��܂��B

    V�u�����N�J�n����784�T�C�N���҂��ăW�I���g���G���W�����r�W�[�ȏꍇ
        V�u�����N�O�̉摜�̕`�揈�����x�ꂽ���߁A�o�b�t�@�̃X���b�v�������܂��I�����Ă��Ȃ��B
        �����̂Ƃ��̓X���b�v�������㉺��ʐ؂�ւ�������ׂ��ł͂���܂���B
    V�u�����N�J�n����784�T�C�N���҂��ăW�I���g���G���W�����r�W�[�łȂ��ꍇ
        �o�b�t�@�̃X���b�v�������摜�̕`�揈�����I�����Ă���B
        ���摜�̕`����X���b�v�������I�����Ă���̂ŁA�㉺��ʂ̐؂�ւ�������ׂ��ł��B

�����AV�u�����N���ł̂���784�T�C�N�����ǂ����Ă����������Ȃ��Ǝv���ꍇ��
���� OS_SpinWait(784) �̑O�ɉ����A784�T�C�N���ȏ�̏��������Ă��������B

�������́A���̕��@�̓|���S����1���`�悵�Ă��Ȃ��Ƃ��͎g���Ȃ����@�Ȃ̂ł����A

void VBlankIntr(void)
{
    if ( (G3X_GetVtxListRamCount() == 0) && swap )
    {
        if (flag)  // flip�̐؂�ւ�
        {
            setupFrame2N_1();
        }
        else
        {
            setupFrame2N();
        }


��̂悤�ɏC�����Ă�����h�����Ƃ��ł��܂��B
���̕��@�̓X���b�v�o�b�t�@������VBlank�J�n���Ɏ��s���ꂽ�Ƃ�
���_RAM�Ɋi�[����Ă��钸�_���� 0 �ɂȂ邱�Ƃ�
G3X_GetVtxListRamCount�֐�(���_RAM�Ɋi�[����Ă��钸�_�̐���Ԃ��֐�) �Ŋm�F���邱�Ƃɂ����
�X���b�v�o�b�t�@���������s���ꂽ�Ƃ�����������Ă��܂��B


���ӁF�����A���ɒ������荞�ݏ�����V�u�����N�̊��荞�ݑO�ɔ������āA
      V�u�����N�̊J�n���x��AV�u�����N�̎��Ԃ����ɒZ��(��3���C���ȉ�)�Ȃ��Ă��܂����ꍇ
      �\�����ɏ㉺��ʂ̐؂�ւ����s��ꂽ��A���̃t���[�������O�̉摜���\������邱�Ƃ�����܂��B

---------------------------------------------------------------------------------------------------
*/
void proc1(void *p1)
{
#pragma unused(p1)

    while (1)
    {
#define SPIN_WAIT                      // OS_SpinWait�֐���p������@�Ɨp���Ȃ����@��؂�ւ��܂��B

    // �摜�̕`����o�b�t�@�̃X���b�v�������������Ă��邱�Ƃ��m�F���܂��B
#ifdef SPIN_WAIT

    if (GX_GetVCount() <= 193)
    {
        OS_SpinWait(784);
    }
    if (!G3X_IsGeometryBusy() && swap)

#else

    if ((G3X_GetVtxListRamCount() == 0) && swap)

#endif
    // �摜�̕`����X���b�v�������������Ă���ꍇ�A�㉺��ʂ̐؂�ւ����s���܂��B
    {
        if (flip_flag)                 // flip�̐؂�ւ�(�㉺��ʂ̐؂�ւ�����)
        {
            setupFrame2N_1();
        }
        else
        {
            setupFrame2N();
        }
        swap = FALSE;
        flip_flag = !flip_flag;
    }
    OS_SleepThread(NULL);   // ���̃X���b�h�𖰂点��B���ɋN����̂� OS_WakeupThreadDirect() ���Ă΂ꂽ�Ƃ��B
    }
}
