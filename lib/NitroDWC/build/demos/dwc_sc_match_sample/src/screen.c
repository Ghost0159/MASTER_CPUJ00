#include  <nitro.h>
#include  "screen.h"
#include  "font.h"

u16 gScreen[32 * 32];

void InitScreen(void)
{
	GX_SetBankForBG( GX_VRAM_BG_32_FG );
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);

    G2_SetBG0Control( GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                       GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000,
                       GX_BG_EXTPLTT_01 );

    G2_SetBG0Priority(0);
    
    G2_BG0Mosaic(FALSE);
    
    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));

    //---- screen
    MI_CpuFillFast((void *)gScreen, 0, sizeof(gScreen));
    DC_FlushRange(gScreen, sizeof(gScreen));
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));
}


// ** these code are refer to rtc sample. thanks.
/*---------------------------------------------------------------------------*
  Name:         ClearScreen

  Description:  clear screen buffer

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ClearScreen(void)
{
    MI_CpuClearFast((void *)gScreen, sizeof(gScreen));
}

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  enter string into screen buffer
                string must be within 32 chars

  Arguments:    x       : x
                y       : y
                palette : color (0-15)
                text    : string. end mark is NULL

  Returns:      None.
 *---------------------------------------------------------------------------*/
void PrintString(s16 x, s16 y, u8 palette, char *text, ...)
{
    va_list vlist;
    char temp[32 + 2], *tempPtr;
    s32 i;
    u16 *p, *pLimit;

    va_start(vlist, text);
    (void)vsnprintf(temp, 33, text, vlist);
    va_end(vlist);

    *(u16 *)(&temp[32]) = 0;
    p = &gScreen[((y * 32) + x) % (32 * 32)];
    pLimit = &gScreen[32 * 32];
    tempPtr = &temp[0];

    for (i = 0; *tempPtr; i++, tempPtr++)
    {
        *p = (u16)((palette << 12) | *tempPtr);
        if (++p >= pLimit)
        {
            p = &gScreen[0];
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         ColorString

  Description:  change string color which is put in screen buffer

  Arguments:    x       : x
                y       : y
                length  : number of characters to change color
                palette : color (0-15)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ColorString(s16 x, s16 y, s16 length, u8 palette)
{
    s32 i;
    u16 *p, *pLimit;

    if (length < 0)
        return;

    p = &gScreen[((y * 32) + x) % (32 * 32)];
    pLimit = &gScreen[32 * 32];

    for (i = 0; i < length; i++)
    {
        u16 temp = *p;
        temp &= 0x0fff;
        temp |= (palette << 12);
        *p = temp;

        if (++p >= pLimit)
        {
            p = &gScreen[0];
        }
    }
}
