/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - IDE - IDE_Archive
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/

#include "sdk_init.h"
#include "nns_util.h"

// アーカイブのインデックス定義ファイル
#include "picture.naix"


static void	LoadPicture(void);

/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  アーカイブからＢＧデータを読み込み、画面を表示するサンプル
  				プログラムです。

  Arguments:    なし。

  Returns:      なし。
   ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    InitSystem();
	InitMemory();
    InitVRAM();

    InitDisplay();
	LoadPicture();
	G2_SetBG0Offset(0,20);

    for(;;)
    {
		SVC_WaitVBlankIntr();
		ReadGamePad();
	}
}

/*---------------------------------------------------------------------------*
  Name:         LoadPicture

  Description:  アーカイブを１つロードし、その中に格納されているキャラクタ、
  				カラーパレット、スクリーンの各データをVRAMに転送します。

  				各データをVRAMに転送した後、アーカイブを削除します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void
LoadPicture(void)
{
	NNSFndArchive* archive;

	// アーカイブをメインメモリに読み込み、ファイルシステムにマウントします。
	// LoadArchive()内でDC_FlushRange()の呼び出しが行われています。
	// LoadArchive()は、nns_util.c内にあります。
	if ((archive = LoadArchive("PCT", "/data/picture.narc")) != NULL)
	{
		// パス名を指定してデータを取得。
		void* chrData = NNS_FndGetArchiveFileByName("PCT:/picture/dog/dog256.chr");
		void* scnData = NNS_FndGetArchiveFileByName("PCT:/picture/dog/dog256.scn");

		// インデックスを指定してデータを取得。
		void* palData = NNS_FndGetArchiveFileByIndex(archive, NARC_picture_dog256_pal);

		// データをVRAMのロード。
		GX_LoadBG0Char(chrData, 0, 0x10000);
    	GX_LoadBG0Scr (scnData, 0, 0x00800);
		GX_LoadBGPltt (palData, 0, 0x00200);

		// VRAMにデータをロードした後は、メインメモリ上のデータは不要な為、
		// アーカイブを削除します。
		// RemoveArchive()は、nns_util.c内にあります。
		RemoveArchive(archive);
	}
}
