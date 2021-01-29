/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - include
  File:     file.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: myrom.h,v $
  Revision 1.4  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.3  2005/03/01 01:57:00  yosizaki
  copyright の年を修正.

  Revision 1.2  2005/02/28 05:26:27  yosizaki
  do-indent.

  Revision 1.1  2004/07/09 00:59:51  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#if	!defined(NITRO_FS_DEMO_ARC_1_MYROM_H_)
#define NITRO_FS_DEMO_ARC_1_MYROM_H_

#include <nitro.h>
#include <nitro/fs.h>
#include <nitro/card.h>


#ifdef __cplusplus
extern "C" {
#endif


/* 複製 ROM アーカイブ構造体 */
typedef struct MyRomArchive
{
    /* アーカイブ本体 */
    FSArchive arc[1];
    /* 使用する DMA チャンネル */
    u32     default_dma_no;
    /* カード / カートリッジのロック ID */
    u32     card_lock_id;

}
MyRomArchive;


/* 複製 ROM アーカイブの初期化 */
void    MyRom_Init(MyRomArchive * p_rom);

/* DMA の指定およびアーカイブの登録 / ロード */
BOOL    MyRom_Create(MyRomArchive * p_rom,
                     const char *name, int name_len,
                     u32 base, const CARDRomRegion *fnt, const CARDRomRegion *fat);

/* 現在のデフォルト DMA を取得 */
u32     MyRom_GetDefaultDMA(const MyRomArchive * p_rom);

/* デフォルト DMA を設定 */
u32     MyRom_SetDefaultDMA(MyRomArchive * p_rom, u32 dma_no);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* NITRO_FS_FILE_H_ */
