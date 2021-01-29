/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - favorite-color
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.7  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.6  2005/11/21 10:54:19  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.5  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.4  2005/03/25 01:29:48  yosizaki
  fix FillRect() to use explicit Z-sort.
  add comment.

  Revision 1.3  2005/02/28 05:26:10  yosizaki
  do-indent.

  Revision 1.2  2004/10/08 00:40:48  yosizaki
  fix V-BlankIntr during initialization.
  small fix around symbols' name, comments, and so on.

  Revision 1.1  2004/10/07 10:20:14  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include "DEMO.h"


/*****************************************************************************/
/* prototype */

/* �F���\���p��� */
typedef struct ColorProp_tag
{
    int     index;
    const char *name;
    GXRgb   color;
    u8      padding[4 - sizeof(GXRgb)];
}
ColorProp;


/*****************************************************************************/
/* macro */

#define	COLOR_PROP_DEFINE(name)       \
{                                     \
    OS_FAVORITE_COLOR_ ## name,       \
    "OS_FAVORITE_COLOR_" #name,       \
    OS_FAVORITE_COLOR_VALUE_ ## name, \
}


/*****************************************************************************/
/* const */

extern const u32 d_CharData[8 * 256];
extern const u32 d_PaletteData[8 * 16];

/* ��ʕ\���p�e��萔 */
enum
{
    ICON_MAX_X = 4,
    ICON_MAX_Y = OS_FAVORITE_COLOR_MAX / 4,
    ICON_WX = 32, ICON_WY = 12,
    SPACE_X = 16, SPACE_Y = 16,
    BORDER = 2,
    MARGIN = 16,
    FRAME_OX = 24, FRAME_OY = 48,
    FRAME_WX = ICON_WX * ICON_MAX_X + SPACE_X * (ICON_MAX_X - 1) + MARGIN * 2,
    FRAME_WY = ICON_WY * ICON_MAX_Y + SPACE_Y * (ICON_MAX_Y - 1) + MARGIN * 2
};

/* �F�e�[�u����� */
static const ColorProp prop[OS_FAVORITE_COLOR_MAX] = {
    COLOR_PROP_DEFINE(GRAY),
    COLOR_PROP_DEFINE(BROWN),
    COLOR_PROP_DEFINE(RED),
    COLOR_PROP_DEFINE(PINK),
    COLOR_PROP_DEFINE(ORANGE),
    COLOR_PROP_DEFINE(YELLOW),
    COLOR_PROP_DEFINE(LIME_GREEN),
    COLOR_PROP_DEFINE(GREEN),
    COLOR_PROP_DEFINE(DARK_GREEN),
    COLOR_PROP_DEFINE(SEA_GREEN),
    COLOR_PROP_DEFINE(TURQUOISE),
    COLOR_PROP_DEFINE(BLUE),
    COLOR_PROP_DEFINE(DARK_BLUE),
    COLOR_PROP_DEFINE(PURPLE),
    COLOR_PROP_DEFINE(VIOLET),
    COLOR_PROP_DEFINE(MAGENTA),
};


/*****************************************************************************/
/* variable */

static u16 gScreen[32 * 32];           /* ���z�X�N���[�� */


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         ClearString

  Description:  �\���p���z�X�N���[���̏�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ClearString(void)
{
    MI_CpuClearFast(gScreen, sizeof(gScreen));
}


/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  ���z�X�N���[����32�����ȓ��̕������\��.

  Arguments:    x       - ������̐擪��z�u���� x ���W / 8 dot
                y       - ������̐擪��z�u���� y ���W / 8 dot
                palette - �����F���w�肷��p���b�g�ԍ�
                text    - �\��������
                ...     - �㑱�̉ψ���

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrintString(int x, int y, int palette, const char *text, ...)
{
    va_list vlist;
    static char temp[64 + 2];
    u8      d;
    int     i;

    va_start(vlist, text);
    (void)OS_VSNPrintf(temp, sizeof(temp) - 1, text, vlist);
    va_end(vlist);
    *(u16 *)&temp[32] = 0x0000;
    for (i = 0; (d = MI_ReadByte(temp + i)) != '\0'; ++i)
    {
        if ((d == '\r') || (d == '\n'))
            break;
        gScreen[((y * 32) + x + i) % (32 * 32)] = (u16)((palette << 12) | d);
    }
}


/*---------------------------------------------------------------------------*
  Name:         FillRect

  Description:  �w��ʒu�ɋ�`��`��.

  Arguments:    x       - ��`���[�� x ���W
                y       - ��`��[�� y ���W
                z       - ��`�̃��C�����w�肷�� z ���W
                wx      - ��` x ��
                wy      - ��` y ��
                col     - ��`�\���F

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void FillRect(int x, int y, int z, int wx, int wy, GXRgb col)
{
    G3_PolygonAttr(GX_LIGHTMASK_NONE,  // no lights
                   GX_POLYGONMODE_MODULATE,     // modulation mode
                   GX_CULL_NONE,       // cull none
                   0,                  // polygon ID(0 - 63)
                   31,                 // alpha(0 - 31)
                   0                   // OR of GXPolygonAttrMisc's value
        );

    G3_Begin(GX_BEGIN_QUADS);
    {
        fx16    ox = (fx16)(-FX16_ONE + (x * FX16_ONE * 2) / GX_LCD_SIZE_X);
        fx16    oy = (fx16)(+FX16_ONE - (y * FX16_ONE * 2) / GX_LCD_SIZE_Y);
        fx16    oz = (fx16)(+z);
        fx16    tx = (fx16)(ox + (wx * FX16_ONE * 2) / GX_LCD_SIZE_X);
        fx16    ty = (fx16)(oy - (wy * FX16_ONE * 2) / GX_LCD_SIZE_Y);

        G3_Color(col);
        G3_Vtx(ox, oy, oz);
        G3_Color(col);
        G3_Vtx(ox, ty, oz);
        G3_Color(col);
        G3_Vtx(tx, ty, oz);
        G3_Color(col);
        G3_Vtx(tx, oy, oz);
    }
    G3_End();
}


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  �A�v���P�[�V�����̃��C���G���g��.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    //---------------------------------------------------------------------------
    // Initialization:
    // This sample set the basic 3D settings, DEMOInit() not called.
    //---------------------------------------------------------------------------
    DEMOInitCommon();

    (void)OS_DisableIrq();

    DEMOInitVRAM();
    DEMOInitDisplay3D();

    //---------------------------------------------------------------------------
    // 2D �e�L�X�g�ݒ�.
    //---------------------------------------------------------------------------
    GX_LoadBG1Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));

    //---------------------------------------------------------------------------
    // Start display
    //---------------------------------------------------------------------------
    (void)OS_EnableIrq();
    DEMOStartDisplay();


    {                                  /*
                                        * ��`�l�ƕ\��������̑Ή����{���ɐ�����������.
                                        * (���̏����� SDK �����̐��������m�F���邽�߂̂��̂ł�)
                                        */
        const GXRgb *p_col = OS_GetFavoriteColorTable();
        int     i;
        for (i = 0; i < OS_FAVORITE_COLOR_MAX; ++i)
        {
            if (prop[i].index != i)
            {
                OS_TPanic("ColorProp[%d] definition is wrong!", i);
            }
            else if (prop[i].color != p_col[i])
            {
                OS_TPanic("ColorProp[%d] definition or OS_GetFavoriteColorTable() is wrong!", i);
            }
            else if (prop[i].color != OS_GetFavoriteColor(i))
            {
                OS_TPanic("OS_GetFavoriteColor(i) is wrong!", i);
            }
        }
    }

    {                                  /*
                                        * �e�F�l��\��.
                                        * �����L�[�őI�����ړ���, �F��`����\��.
                                        */
        int     i;
        int     cur_x = 0, cur_y = 0;
        int     frame = 0;

        DEMOReadKey();
        for (;; ++frame)
        {
            /* ��`���C���̑��ΊǗ� */
            int     layer = 3;
            //---------------------------------------------------------------------------
            // Reset the states of 3D graphics
            //---------------------------------------------------------------------------
            G3X_Reset();

            //---------------------------------------------------------------------------
            // In this sample, the camera matrix and the projection matrix are set as
            // an identity matrices for simplicity.
            //---------------------------------------------------------------------------
            G3_MtxMode(GX_MTXMODE_PROJECTION);
            G3_Identity();
            G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
            G3_Identity();

            ClearString();

            DEMOReadKey();

            /* �w�i��`�� */
            FillRect(FRAME_OX, FRAME_OY, layer--, FRAME_WX, FRAME_WY, GX_RGB(16, 16, 16));
            FillRect(FRAME_OX + BORDER, FRAME_OY + BORDER, layer--,
                     FRAME_WX - BORDER * 2, FRAME_WY - BORDER * 2, GX_RGB(31, 31, 31));

            /* �����L�[�ŃJ�[�\���ړ� */
            if (DEMO_IS_TRIG(PAD_KEY_RIGHT))
            {
                if (++cur_x >= ICON_MAX_X)
                    cur_x -= ICON_MAX_X;
            }
            if (DEMO_IS_TRIG(PAD_KEY_LEFT))
            {
                if (--cur_x < 0)
                    cur_x += ICON_MAX_X;
            }
            if (DEMO_IS_TRIG(PAD_KEY_DOWN))
            {
                if (++cur_y >= ICON_MAX_Y)
                    cur_y -= ICON_MAX_Y;
            }
            if (DEMO_IS_TRIG(PAD_KEY_UP))
            {
                if (--cur_y < 0)
                    cur_y += ICON_MAX_Y;
            }

            {                          /* �I��F�ƈꗗ��\�� */
                const int index = cur_x + cur_y * ICON_MAX_X;

                PrintString(1, 1, 0xF, "index : %d", index);
                PrintString(1, 3, 0xF, "%s", prop[index].name);

                for (i = 0; i < OS_FAVORITE_COLOR_MAX; ++i)
                {
                    const int x = FRAME_OX + MARGIN + (i % ICON_MAX_X) * (ICON_WX + SPACE_X);
                    const int y = FRAME_OY + MARGIN + (i / ICON_MAX_X) * (ICON_WY + SPACE_Y);
                    GXRgb   gnd;
                    if (i == index)
                    {
                        const int grad = ((frame & 32) == 0) ? (frame & 31) : 31 - (frame & 31);
                        gnd = GX_RGB(grad, grad, grad);
                    }
                    else
                    {
                        gnd = GX_RGB(31, 31, 31);
                    }
                    FillRect(x - MARGIN / 2, y - MARGIN / 2, layer - 0,
                             ICON_WX + MARGIN, ICON_WY + MARGIN, gnd);
                    FillRect(x - MARGIN / 4, y - MARGIN / 4, layer - 1,
                             ICON_WX + MARGIN / 2, ICON_WY + MARGIN / 2, GX_RGB(31, 31, 31));
                    FillRect(x, y, layer - 2, ICON_WX, ICON_WY, prop[i].color);
                }
            }

            // swapping the polygon list RAM, the vertex RAM, etc.
            G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);

            OS_WaitVBlankIntr();       // Waiting the end of VBlank interrupt
        }
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
    /* ���z�X�N���[����VRAM�ɔ��f */
    DC_FlushRange(gScreen, sizeof(gScreen));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG1Scr(gScreen, 0, sizeof(gScreen));

    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // checking VBlank interrupt
}
