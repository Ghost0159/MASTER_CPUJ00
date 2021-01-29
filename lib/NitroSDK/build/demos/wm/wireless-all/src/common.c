/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless-all
  File:     common.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: common.c,v $
  Revision 1.2  2006/04/10 13:37:12  yosizaki
  beautify sourcecode indent.

  Revision 1.1  2006/04/10 13:07:32  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include "common.h"


/*****************************************************************************/
/* constant */

#define BGSTR_MAX_LENGTH        32

/* テキスト描画用データ */
extern const u16 _binary_bg_plt_dat[16][16];
extern const u32 _binary_bg_chr_dat[8 * 0x100];


/*****************************************************************************/
/* variable */

/* チャンネル */
static u16 current_channel = 0;

/* キー入力 */
static int own_aid;
static GShareData current_input[WH_PLAYER_MAX] ATTRIBUTE_ALIGN(32);
static BOOL valid_input[WH_PLAYER_MAX];
static GShareData next_input;
static u16 padPress;
static u16 padTrig;

/* 描画用変数 */
static u16 vscr[32 * 32];
static GXOamAttr oamBak[128];


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         IS_PAD_PRESS

  Description:  自身のキー判定.

  Arguments:    判定するキーフラグ.

  Returns:      指定したキーが押下されている場合は TRUE,
                されていない場合は FALSE.
 *---------------------------------------------------------------------------*/
BOOL IS_PAD_PRESS(u16 flag)
{
    return (padPress & flag) == flag;
}

/*---------------------------------------------------------------------------*
  Name:         IS_PAD_TRIGGER

  Description:  自身のキートリガ判定.

  Arguments:    判定するキーフラグ.

  Returns:      指定したキーのトリガが立っている場合は TRUE.
                立っていない場合は FALSE.
 *---------------------------------------------------------------------------*/
BOOL IS_PAD_TRIGGER(u16 flag)
{
    return (padTrig & flag) == flag;
}

/*---------------------------------------------------------------------------*
  Name:         UpdateInput

  Description:  キー入力の更新.
                ワイヤレス通信が確立している場合はデータシェアリングを試行し,
                そうでない場合は自身のキー入力だけを更新する.

  Arguments:    None.

  Returns:      キー入力更新に成功したら TRUE.
 *---------------------------------------------------------------------------*/
static BOOL UpdateInput(void)
{
    BOOL    retval = FALSE;

    {
        OSIntrMode bak_cpsr = OS_DisableInterrupts();
        next_input.key = PAD_Read();
        next_input.count = next_input.count + 1;
        /* データシェアリング以外なら自分のキー入力を設定 */
        if (WH_GetSystemState() != WH_SYSSTATE_DATASHARING)
        {
            own_aid = 0;
            current_input[0] = next_input;
            valid_input[0] = TRUE;
            retval = TRUE;
        }
        /* そうでなければデータシェアリングを試行 */
        else if (WH_StepDS(&next_input))
        {
            /* 成功すれば全員の入力データを反映 */
            u16     i;
            for (i = 0; i < WH_PLAYER_MAX; ++i)
            {
                void   *adr = WH_GetSharedDataAdr(i);
                if (adr)
                {
                    MI_CpuCopy8(adr, &current_input[i], sizeof(current_input[i]));
                    valid_input[i] = TRUE;
                }
                else
                {
                    valid_input[i] = FALSE;
                }
            }
            retval = TRUE;
            own_aid = WH_GetCurrentAid();
        }
        (void)OS_RestoreInterrupts(bak_cpsr);
    }

    /* 入力を更新できた場合はキートリガも更新 */
    if (retval && valid_input[own_aid])
    {
        padTrig = (u16)(~padPress & current_input[own_aid].key);
        padPress = current_input[own_aid].key;
    }

    return retval;
}

/*---------------------------------------------------------------------------*
  Name:         GetCurrentInput

  Description:  指定プレイヤーの現在のキー入力を取得.

  Arguments:    aid               キー入力を取得するプレイヤー.

  Returns:      最新のデータシェアリングに成功していれば有効なアドレス,
                そうでなければ NULL.
 *---------------------------------------------------------------------------*/
GShareData *GetCurrentInput(int aid)
{
    return valid_input[aid] ? &current_input[aid] : NULL;
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  キートリガ取得

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    /* OAM更新 */
    DC_FlushRange(oamBak, sizeof(oamBak));
    MI_DmaCopy32(3, oamBak, (void *)HW_OAM, sizeof(oamBak));
    /* BG更新 */
    DC_FlushRange(vscr, sizeof(vscr));
    GX_LoadBG0Scr(vscr, 0, sizeof(vscr));

    /* 割り込みチェックフラグ */
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         InitCommon

  Description:  共通初期化関数

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void InitCommon(void)
{
    /* OS 初期化 */
    OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    FX_Init();
    {
        void   *tempLo;
        OSHeapHandle hh;
        tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
        OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
        hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
        if (hh < 0)
        {
            OS_TPanic("ARM9: Fail to create heap...\n");
        }
        hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
    }

    /* キー入力の初期化 */
    MI_CpuClear8(&next_input, sizeof(next_input));
    MI_CpuClear8(&current_input, sizeof(current_input));
    own_aid = 0;
    (void)UpdateInput();

    /* FS 初期化 */
    if (!MB_IsMultiBootChild())
    {
        static u8 fs_tablework[0x100] ATTRIBUTE_ALIGN(32);
        FS_Init(FS_DMA_NOT_USE);
        (void)FS_LoadTable(fs_tablework, sizeof(fs_tablework));
    }

    /* GX 初期化 */
    GX_Init();
    GX_DispOff();
    GXS_DispOff();
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);
    /* OBJ (VRAM-B) */
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_B);
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
    MI_DmaFill32(3, oamBak, 0xC0, sizeof(oamBak));
    GX_LoadOBJ(_binary_bg_chr_dat, 0, sizeof(_binary_bg_chr_dat));
    GX_LoadOBJPltt(_binary_bg_plt_dat, 0, sizeof(_binary_bg_plt_dat));
    /* BG (VRAM-C) */
    GX_SetBankForBG(GX_VRAM_BG_128_C);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800,      /* SCR ベースブロック 31 */
                     GX_BG_CHARBASE_0x00000,    /* CHR ベースブロック 0 */
                     GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    G2_SetBG0Offset(0, 0);
    GX_LoadBG0Char(_binary_bg_chr_dat, 0, sizeof(_binary_bg_chr_dat));
    GX_LoadBGPltt(_binary_bg_plt_dat, 0, sizeof(_binary_bg_plt_dat));
    MI_CpuFillFast((void *)vscr, 0, sizeof(vscr));
    DC_FlushRange(vscr, sizeof(vscr));
    DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(vscr, 0, sizeof(vscr));
    GX_SetVisiblePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_OBJ);
    GX_DispOn();
    GXS_DispOn();

    /* Vブランク割込設定 */
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
}

/*---------------------------------------------------------------------------*
  Name:         GetCurrentChannel

  Description:  現在選択しているワイヤレスチャンネルを取得.

  Arguments:    None

  Returns:      現在選択しているワイヤレスチャンネル.
 *---------------------------------------------------------------------------*/
u16 GetCurrentChannel(void)
{
    return current_channel;
}

/*---------------------------------------------------------------------------*
  Name:         SetCurrentChannel

  Description:  現在のワイヤレスチャンネルを設定.

  Arguments:    channel           設定するチャンネル.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void SetCurrentChannel(u16 channel)
{
    current_channel = channel;
}

/*---------------------------------------------------------------------------*
  Name:         LoadLinkIcon

  Description:  リンクアイコンをVRAMにロード.

  Arguments:    id                ロード先のキャラクタID.
                palette           ロード先のパレット.
                level             リンク強度.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void LoadLinkIcon(int id, int palette, int level)
{
    /* *INDENT-OFF* */
    typedef struct Icon16Format
    {
        GXRgb   palette[16];
        u8      image[16 * 16 / 2];
    }
    IconFormat;
    extern const IconFormat _binary_linkimg0_dat[];
    extern const IconFormat _binary_linkimg1_dat[];
    extern const IconFormat _binary_linkimg2_dat[];
    extern const IconFormat _binary_linkimg3_dat[];
    static const IconFormat * const table[4] =
    { _binary_linkimg0_dat, _binary_linkimg1_dat, _binary_linkimg2_dat, _binary_linkimg3_dat, };
    /* *INDENT-ON* */
    GX_LoadOBJ(table[level]->image, (u32)(32 * id), 16 * 16 / 2);
    GX_LoadOBJPltt(table[level]->palette, sizeof(GXRgb) * 16 * palette,
                   sizeof(table[level]->palette));
}

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  BGテキストを描画.

  Arguments:    x                 描画 x グリッド位置.
                y                 描画 x グリッド位置.
                palette           パレットインデックス.
                format            書式文字列.

  Returns:      None
 *---------------------------------------------------------------------------*/
void PrintString(int x, int y, int palette, const char *format, ...)
{
    int     i;
    char    tmp[BGSTR_MAX_LENGTH + 1] = { 0, };
    va_list vlist;
    va_start(vlist, format);
    (void)OS_VSNPrintf(tmp, sizeof(tmp), format, vlist);
    va_end(vlist);
    y = y * 32 + x;
    for (i = 0; (i < BGSTR_MAX_LENGTH) && tmp[i]; ++i)
    {
        vscr[(y + i) & ((32 * 32) - 1)] = (u16)((palette << 12) | tmp[i]);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WaitNextFrame

  Description:  次のピクチャーフレームを待ち描画を初期化する.

  Arguments:    None.

  Returns:      キー入力が更新されたら TRUE.
 *---------------------------------------------------------------------------*/
BOOL WaitNextFrame(void)
{
    BOOL    retval;
    /* 次のピクチャーフレームを待機 */
    OS_WaitVBlankIntr();
    /* キー入力の更新 */
    retval = UpdateInput();
    /* テキストをクリア */
    MI_CpuClearFast(vscr, sizeof(vscr));
    /* リンク強度アイコンの表示 */
    LoadLinkIcon(512, PLTT_LINK_ICON, WM_GetLinkLevel());
    G2_SetOBJAttr(&oamBak[64], 3, 3, 0,
                  GX_OAM_MODE_NORMAL, FALSE, GX_OAM_EFFECT_NONE,
                  GX_OAM_SHAPE_16x16, GX_OAM_COLOR_16, 512, PLTT_LINK_ICON, 0);
    return retval;
}

/*---------------------------------------------------------------------------*
  Name:         WaitWHState

  Description:  WH の指定のステートになるまで待機.

  Arguments:    state             遷移を待つステート.

  Returns:      None
 *---------------------------------------------------------------------------*/
void WaitWHState(int state)
{
    while (WH_GetSystemState() != state)
    {
        (void)WaitNextFrame();
        PrintString(3, 10, PLTT_WHITE, "WH working...");
        PrintString(10, 16, PLTT_WHITE, "( to state %d )", state);
    }
}
