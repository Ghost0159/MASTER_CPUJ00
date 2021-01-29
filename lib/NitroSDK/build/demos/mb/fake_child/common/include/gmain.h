/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - fake_child
  File:     gmain.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: gmain.h,v $
  Revision 1.3  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.2  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.1  2004/11/22 13:00:19  takano_makoto
  multiboot-Modelのものと同一のファイルとして登録

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef MB_DEMO_GMAIN_H_
#define MB_DEMO_GMAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro.h>


typedef struct
{
    u32     count;                     // フレーム情報
    u16     key;                       // キー情報
    u16     padding;
}
GShareData;


//============================================================================
//  関数宣言
//============================================================================

void    GInitDataShare(void);
void    GStepDataShare(s32 frame);
void    GMain(void);


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // MB_DEMO_GMAIN_H_
