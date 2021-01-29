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

/*===========================================================================*/

#ifdef	__cplusplus
}          /* extern "C" */
#endif

#endif /* SCREEN_H_ */
