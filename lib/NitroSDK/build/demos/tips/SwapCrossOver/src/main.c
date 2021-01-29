/*---------------------------------------------------------------------------*
  Project:  NitroSDK - tips - SwapCrossOver
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.6  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.5  2005/11/21 11:05:48  kitase_hirotake
  SVC_WaitVBlankIntr ‚ð OS_WaitVBlankIntr ‚É•ÏX

  Revision 1.4  2005/02/28 05:26:23  yosizaki
  do-indent.

  Revision 1.3  2004/07/20 07:38:09  yada
  SVC_WaitIntr() -> OS_WaitInterrupt()

  Revision 1.2  2004/06/08 00:21:38  yada
  add '#pragma unused()' for FINALROM

  Revision 1.1  2004/03/31 13:57:25  takano_makoto
  First upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample for detect SwapBuffer of geometry engine cross over vblank.
// 
// If you send SwapBuffer command to geometry command FIFO just before the vblank,
// you need detect the command will be executed before vblank or after vblank
// to sync 2D and 3D graphics.
// This sample detect SwapBuffer has been executed by the vblank.
// 
// HOWTO:
// 1. If it is possible that no polygon are drawn, you draw invisible polygon
//    by DrawDummyPolygon().
// 2. If SwapBuffer has been executed by the vblank, polygon list RAM and vertex
//    list RAM count become 0. you can check these counts after 8 cycle from vblank.
// 
// Operation:
//    Left, Right : Move a cube.
//    A           : Print parameters at the time called swapbuffer and after vblank.
//    B           : Send SwapBuffer command just before vblank, and print parameters.
//    Select      : Switch BG on/off.
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"
#include "data.h"


static void VCountEqIntr();

static vs32 frame_cnt = 0;

typedef struct
{
    s32     vcount;                    // VCount
    s32     fifo_cnt;                  // Geometry command fifo count.
    s32     frame_cnt;                 // frame count
    s32     polygon_num;               // count of PolygonList RAM.
    s32     vertex_num;                // count of VertexList RAM.
}
t_geometry_stat;



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




/*---------------------------------------------------------------------------*
  Name:         DrawDummyPolygon

  Description:  Set a unvisible polygon, to Polygon List RAM and Vertex RAM.
                Please call this function with any POSITION VECTOR matrix stack.

  Arguments:    0 if PROJECTION matrix stack is full.
                1 if PROJECTION matrix stack has more area.

  Returns:      none
 *---------------------------------------------------------------------------*/
static void DrawDummyPolygon(s32 pj_stacked)
{
    MtxFx44 mtx_pj;

    // move current matrix, and load identity matrix.
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_PushMtx();
    G3_Identity();

    G3_MtxMode(GX_MTXMODE_PROJECTION);

    if (pj_stacked)
    {
        /* if stack is full, load current matrix */
        while (G3X_GetClipMtx(&mtx_pj) != 0)
        {
        };
    }
    else
    {
        /* if stack has more area push matrix. */
        G3_PushMtx();
    }
    G3_Identity();

    // draw unvisible polygon.
    G3_PolygonAttr(GX_LIGHTMASK_NONE,  // no lights
                   GX_POLYGONMODE_MODULATE,     // modulation mode
                   GX_CULL_NONE,       // cull back
                   0,                  // polygon ID(0 - 63)
                   0,                  // alpha(0 - 31)
                   GX_POLYGON_ATTR_MISC_DISP_1DOT       // OR of GXPolygonAttrMisc's value
        );

    G3_Begin(GX_BEGIN_TRIANGLES);
    {
        G3_Vtx(0, 0, -FX32_ONE);
        G3_Vtx(0, 0, -FX32_ONE);
        G3_Vtx(0, 0, -FX32_ONE);
    }
    G3_End();


    // restore current matrix.
    G3_MtxMode(GX_MTXMODE_PROJECTION);
    if (pj_stacked)
    {
        G3_LoadMtx44(&mtx_pj);
    }
    else
    {
        G3_PopMtx(1);
    }
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_PopMtx(1);
}




//---------------------------------------------------------------------------
// Translate many geometry command to make time lag.
//---------------------------------------------------------------------------
static void PushHugeCommands()
{
    s32     i;
    enum
    { COMMAND_NUM = 95 };

    for (i = 0; i < COMMAND_NUM; i++)
    {
        G3_Direct3(G3OP_BOX_TEST, 0, 0, 0);
    }
}


//---------------------------------------------------------------------------
// Send vertex data from table.
//---------------------------------------------------------------------------
static void vtx(int idx)
{
    G3_Vtx(gCubeGeometry[idx * 3], gCubeGeometry[idx * 3 + 1], gCubeGeometry[idx * 3 + 2]);
}


//---------------------------------------------------------------------------
// Send quad polygon data.
//---------------------------------------------------------------------------
static void quad(int idx0, int idx1, int idx2, int idx3)
{
    vtx(idx0);
    vtx(idx1);
    vtx(idx2);
    vtx(idx3);
}


//---------------------------------------------------------------------------
// Draw cube function.
//---------------------------------------------------------------------------
static void drawCube(int alpha, u16 Rotate)
{

    {
        fx16    s = FX_SinIdx(Rotate);
        fx16    c = FX_CosIdx(Rotate);

        G3_RotX(s, c);
        G3_RotY(s, c);
        G3_RotZ(s, c);
    }
    G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), // diffuse
                            GX_RGB(16, 16, 16), // ambient
                            TRUE       // use diffuse as vtx color if TRUE
        );

    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), // specular
                            GX_RGB(0, 0, 0),    // emission
                            FALSE      // use shininess table if TRUE
        );

    G3_PolygonAttr(GX_LIGHTMASK_NONE,  // no lights
                   GX_POLYGONMODE_MODULATE,     // modulation mode
                   GX_CULL_BACK,       // cull back
                   0,                  // polygon ID(0 - 63)
                   alpha,              // alpha(0 - 31)
                   0                   // OR of GXPolygonAttrMisc's value
        );


    G3_Begin(GX_BEGIN_QUADS);
    {
        quad(2, 0, 4, 6);
        quad(7, 5, 1, 3);
        quad(6, 4, 5, 7);
        quad(3, 1, 0, 2);
        quad(5, 4, 0, 1);
        quad(6, 7, 3, 2);
    }
    G3_End();
}


//---------------------------------------------------------------------------
// Init display settings.
//---------------------------------------------------------------------------
static void InitDisplay()
{

    G3X_Init();                        // initialize the 3D graphics states
    G3X_InitTable();                   // initialize tables

    G3X_InitMtxStack();                // initialize the matrix stack

    GX_SetBankForTex(GX_VRAM_TEX_0_A); // VRAM-A for texture images

    GX_SetBankForBG(GX_VRAM_BG_128_D); // VRAM-D for BGs

    GX_SetBankForTexPltt(GX_VRAM_TEXPLTT_0123_E);       // VRAM-E for texture palettes

    GX_SetBankForOBJ(GX_VRAM_OBJ_32_FG);        // VRAM-FG for OBJ

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // graphics mode
                       GX_BGMODE_0,    // BGMODE is 0
                       GX_BG0_AS_3D);  // BG #0 is for 3D

    GX_SetVisiblePlane(GX_PLANEMASK_BG0 |       // BG #0 for 3D
                       GX_PLANEMASK_BG1 |       // BG #1 for text
                       GX_PLANEMASK_OBJ // OBJ
        );

    G2_SetBG0Priority(0);

    G2_SetBG1Control(GX_BG_SCRSIZE_TEXT_256x256,        // 256pix x 256pix text
                     GX_BG_COLORMODE_256,       // use 16 colors mode
                     GX_BG_SCRBASE_0x0000,      // screen base offset + 0x3800 is the address for BG #1 screen
                     GX_BG_CHARBASE_0x04000,    // character base offset + 0x00000 is the address for BG #1 characters
                     GX_BG_EXTPLTT_01  // use BGExtPltt slot #1 if BGExtPltt is enabled
        );

    G2_SetBG1Priority(1);

    G2_BG1Mosaic(FALSE);

    G3X_SetShading(GX_SHADING_TOON);   // shading mode is toon

    G3X_AntiAlias(TRUE);               // enable antialias(without additional computing costs)

    G2_BlendNone();                    // no 2D alpha blending or brightness change

    //---------------------------------------------------------------------------
    // After you start the geometry engine and the rendering one
    // (by GX_SetPower etc.), G3_SwapBuffers must be called once before use
    //---------------------------------------------------------------------------
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

    //---------------------------------------------------------------------------
    // Set clear color, depth and polygon ID.
    //---------------------------------------------------------------------------
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
        fx32    right = FX32_ONE * 2;
        fx32    left = FX32_ONE;
        fx32    top = FX32_ONE * 3 / 4;
        fx32    bottom = 0;
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
}





//---------------------------------------------------------------------------
// Record current geometry state.
//---------------------------------------------------------------------------
static void RecordCurrentStat(t_geometry_stat * stat)
{

    stat->vcount = GX_GetVCount();
    stat->fifo_cnt = G3X_GetCommandFifoCount();
    stat->frame_cnt = frame_cnt;
}

//---------------------------------------------------------------------------
// Print geometry stat.
//---------------------------------------------------------------------------
static void PrintStat(t_geometry_stat * stat1, t_geometry_stat * stat2)
{
#ifdef SDK_FINALROM
#pragma unused( stat1, stat2 )
#endif

    OS_Printf("[Before VBlank]\n");
    OS_Printf("PolygonListRAM = %d/2048\t", stat1->polygon_num);
    OS_Printf("VertexListRAM  = %d/6144\n", stat1->vertex_num);
    OS_Printf("fr%x: VCOUNT=%d, FIFO_COUNT=%d\n", stat1->frame_cnt, stat1->vcount, stat1->fifo_cnt);

    OS_Printf("[After VBlank]\n");
    OS_Printf("PolygonListRAM = %d/2048\t", stat2->polygon_num);
    OS_Printf("VertexListRAM  = %d/6144\n", stat2->vertex_num);
    OS_Printf("fr%x: VCOUNT=%d, FIFO_COUNT=%d\n", stat2->frame_cnt, stat2->vcount, stat2->fifo_cnt);
}


//---------------------------------------------------------------------------
// Main Loop.
//---------------------------------------------------------------------------
void NitroMain(void)
{
    u16     Rotate = 0;                // for rotating cubes(0-65535)
    u32     bg = 1;
    s32     vp_x1 = 0, vp_y1 = 0, vp_x2 = 255, vp_y2 = 191;
    fx32    cam_y = FX32_ONE, cam_z = FX32_ONE;
    t_geometry_stat stat_before, stat_after;

    //---------------------------------------------------------------------------
    // Initialize:
    // They enable IRQ interrupts, initialize VRAM, and set BG #0 for 3D mode, 
    // and BG #1 for 2D mode.
    //---------------------------------------------------------------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    InitDisplay();

    OS_SetIrqFunction(OS_IE_V_COUNT, VCountEqIntr);
    GX_SetVCounterEqVal(191);
    (void)OS_EnableIrqMask(OS_IE_V_COUNT);
    (void)GX_VCounterEqIntr(TRUE);

    // Load 2D graphic data
    GX_LoadBG1Char(d_natsunoumi_schDT, 0, sizeof(d_natsunoumi_schDT));
    GX_LoadBGPltt(d_natsunoumi_sclDT, 0, sizeof(d_natsunoumi_sclDT));
    GX_LoadBG1Scr(d_natsunoumi_sscDT, 0, sizeof(d_natsunoumi_sscDT));

    // not displayed if alpha is equal or less than 24
    DEMOStartDisplay();

    while (1)
    {
        G3_ViewPort(vp_x1, vp_y1, vp_x2, vp_y2);

        DEMOReadKey();

        // switch BG ON/OFF.
        if (DEMO_IS_TRIG(PAD_BUTTON_SELECT))
        {
            bg = !bg;
        }

        if (bg)
        {
            GX_SetVisiblePlane(GX_PLANEMASK_BG0 |       // BG #0 for 3D
                               GX_PLANEMASK_BG1 |       // BG #1 for text
                               GX_PLANEMASK_OBJ // OBJ
                );
        }
        else
        {
            GX_SetVisiblePlane(GX_PLANEMASK_BG0 |       // BG #0 for 3D
                               GX_PLANEMASK_OBJ // OBJ
                );
        }

        // move position of cube.
        if (DEMO_IS_PRESS(PAD_KEY_LEFT))
        {
            cam_y += FX32_ONE;
        }
        else if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
        {
            cam_y -= FX32_ONE;
        }


        G3X_Reset();

        // Draw invisible polygon to Polygon RAM.
        DrawDummyPolygon(FALSE);

        //---------------------------------------------------------------------------
        // Set up the camera matrix
        //---------------------------------------------------------------------------
        {
            VecFx32 Eye = { 0, 4 * FX32_ONE, FX32_ONE };        // camera position
            VecFx32 at = { 0, 0, -5 * FX32_ONE };       // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            at.x = cam_y / 16;
            at.x = cam_y / 16;

            G3_LookAt(&Eye, &vUp, &at, NULL);   // set camera

        }

        G3_PushMtx();
        G3_Translate(0, 0, -5 * FX32_ONE);
        //---------------------------------------------------------------------------
        // Send Cube
        //---------------------------------------------------------------------------
        Rotate += 256;

        G3_PushMtx();
        G3_Translate(3 * (FX32_ONE >> 1), 0, 0);

        drawCube(31, Rotate);

        G3_PopMtx(1);

        G3_PopMtx(1);

        // If B button pushed, Wait vcount just before VBlank Inter.
        if (DEMO_IS_PRESS(PAD_BUTTON_B))
        {
            OS_WaitInterrupt(1, OS_IE_V_COUNT);
        }

        PushHugeCommands();

        stat_before.polygon_num = G3X_GetPolygonListRamCount();
        stat_before.vertex_num = G3X_GetVtxListRamCount();

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        RecordCurrentStat(&stat_before);

        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt

        RecordCurrentStat(&stat_after);

        // get PolygonListRAM count after more than 8 cycle from vblank.
        stat_after.polygon_num = G3X_GetPolygonListRamCount();
        stat_after.vertex_num = G3X_GetVtxListRamCount();

        // if vertex_num is not 0, SwapBuffers executed after VBlank.
        if (stat_after.vertex_num != 0)
        {
            PrintStat(&stat_before, &stat_after);

            OS_Printf(" SwapBuffer cross over a vblank!\n\n");
        }
        else if (DEMO_IS_PRESS(PAD_BUTTON_A))
        {
            PrintStat(&stat_before, &stat_after);

            OS_Printf("OK!\n\n");
        }
    }
}


//---------------------------------------------------------------------------
// VBlank interrupt function:
//---------------------------------------------------------------------------
void VBlankIntr(void)
{
    frame_cnt++;
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // checking VBlank interrupt
}


//---------------------------------------------------------------------------
// VCount Equal interrupt function:
//---------------------------------------------------------------------------
static void VCountEqIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_COUNT); // checking VCount interrupt
}
