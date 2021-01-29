/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - WCM - demos - wcm-list
  File:     screen.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: screen.h,v $
  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2005/07/21 08:21:06  adachi_hiroaki
  �V�K�ǉ�


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef SCREEN_H_
#define SCREEN_H_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/
#include <nitro/types.h>

/*---------------------------------------------------------------------------*
    �֐� ��`
 *---------------------------------------------------------------------------*/
void    InitScreen(void);
void    ClearMainScreen(void);
void    PutMainScreen(s32 x, s32 y, u8 palette, char* text, ...);
void    PrintSubScreen(char* text, ...);
void    UpdateScreen(void);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif

#endif /* SCREEN_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
