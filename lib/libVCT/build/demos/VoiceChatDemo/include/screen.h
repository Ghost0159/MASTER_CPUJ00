#ifndef	__SCREEN_H_
#define	__SCREEN_H_

#ifdef	__cplusplus
extern "C" {
#endif

/*===========================================================================*/
#include	<nitro/types.h>

extern u16 gScreen[32 * 32];

void InitScreen(void);
void ClearScreen(void);
void PrintString(s16 x, s16 y, u8 palette, char *text, ...);
void ColorString(s16 x, s16 y, s16 length, u8 palette);

#define PutMainScreen PrintString
enum {
    FONT_COLOR_BLACK,
    FONT_COLOR_RED,
    FONT_COLOR_GREEN,
    FONT_COLOR_BLUE,
    FONT_COLOR_YELLOW,
    FONT_COLOR_PURPLE,
    FONT_COLOR_LIGHT_BLUE,
    FONT_COLOR_DARK_RED,
    FONT_COLOR_DARK_GREEN,
    FONT_COLOR_DARK_BLUE,
    FONT_COLOR_DARK_YELLOW,
    FONT_COLOR_DARK_PURPLE,
    FONT_COLOR_DARK_LIGHT_BLUE,
    FONT_COLOR_GRAY,
    FONT_COLOR_DARK_GRAY,
    FONT_COLOR_WHITE
};

/*===========================================================================*/

#ifdef	__cplusplus
}          /* extern "C" */
#endif

#endif /* SCREEN_H_ */
