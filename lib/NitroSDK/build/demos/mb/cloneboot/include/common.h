/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - cloneboot
  File:     common.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: common.h,v $
  Revision 1.3  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.2  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.1  2004/11/15 06:21:41  yosizaki
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef MB_DEMO_COMMON_H_
#define MB_DEMO_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro.h>

//============================================================================
//  関数宣言
//============================================================================


/*
 * この関数は multiboot-Model の親機側で NitroMain() だったものです.
 * 本サンプルでは MB_IsMultiBootChild() == FALSE 判定で呼び出されます.
 */
void    ParentMain(void);

/*
 * この関数は multiboot-Model の子機側で NitroMain() だったものです.
 * 本サンプルでは MB_IsMultiBootChild() == TRUE 判定で呼び出されます.
 */
void    ChildMain(void);

/*
 * この関数は親機専用領域 .parent セクションに配置されています.
 * 内容は, 単に ParentMain() を呼び出すだけです.
 */
void    ParentIdentifier(void);

/* それ以外は全て multiboot-Model と同じです */

void    CommonInit();
void    ReadKey(void);
u16     GetPressKey(void);
u16     GetTrigKey(void);
void    InitAllocateSystem(void);

/*---------------------------------------------------------------------------*
  Name:         IS_PAD_PRESS

  Description:  キー判定

  Arguments:    判定するキーフラグ

  Returns:      指定したキーが押下されている場合は TRUE
                されていない場合は FALSE
 *---------------------------------------------------------------------------*/
static inline BOOL IS_PAD_PRESS(u16 flag)
{
    return (GetPressKey() & flag) == flag;
}

/*---------------------------------------------------------------------------*
  Name:         IS_PAD_TRIGGER

  Description:  キートリガ判定

  Arguments:    判定するキーフラグ

  Returns:      指定したキーのトリガが立っている場合は TRUE
                立っていない場合は FALSE
 *---------------------------------------------------------------------------*/
static inline BOOL IS_PAD_TRIGGER(u16 flag)
{
    return (GetTrigKey() & flag) == flag;
}


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // MB_DEMO_COMMON_H_
