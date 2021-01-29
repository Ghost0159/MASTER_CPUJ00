/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-PowerSave
  File:     common.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: common.h,v $
  Revision 1.2  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.1  2005/04/28 07:15:17  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#if !defined(NITRO_MB_DEMO_MULTIBOOT_POWERSAVE_COMMON_H_)
#define NITRO_MB_DEMO_MULTIBOOT_POWERSAVE_COMMON_H_


#include <nitro.h>


/*****************************************************************************/
/* variable */

/* 省電力モード設定 */
extern BOOL g_power_save_mode;


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         ReadKeySetTrigger

  Description:  キートリガ検出.

  Arguments:    None.

  Returns:      前回呼び出し時に押下状態でなく, かつ
                今回呼び出し時に押下状態であったキー入力のビット集合.
 *---------------------------------------------------------------------------*/
u16     ReadKeySetTrigger(void);

/*---------------------------------------------------------------------------*
  Name:         MenuMode

  Description:  DSダウンロード親機初期設定メニュー.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MenuMode(void);

/*---------------------------------------------------------------------------*
  Name:         ParentMode

  Description:  DSダウンロードプレイ親機処理.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ParentMode(void);


#endif /* !defined(NITRO_MB_DEMO_MULTIBOOT_POWERSAVE_COMMON_H_) */
