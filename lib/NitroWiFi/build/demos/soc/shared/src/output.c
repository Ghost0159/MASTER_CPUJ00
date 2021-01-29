/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - shared
  File:     output.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: output.c,v $
  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/06 00:23:20  kitase_hirotake
  Initial Upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

#include "output.h"

/*****************************************************************************/

/* variable */
static int  log_cur_x, log_cur_y;
static char log_line[32][32 + 1];
static void (*log_output_api) (const char*) = NULL;
static u16  sub_text_scr[32][32] ATTRIBUTE_ALIGN(32);
static BOOL sub_text_modified;

static char current_output[32];

/*****************************************************************************/

/* function */

/*---------------------------------------------------------------------------*
  Name:         DebugOutputHook

  Description:  デバッグ出力をフックしてログに追加.

  Arguments:    s                出力文字列.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DebugOutputHook(const char* s)
{
    /* 本来の出力先を呼び出す */
    (*log_output_api) (s);

    /* 1 行単位で出力を保存 */
    DebugOutput(s);
}

/*---------------------------------------------------------------------------*
  Name:         InitDebugOutputHook

  Description:  ログ出力用表示初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void InitDebugOutputHook(void)
{
    OSIntrMode  bak_cpsr = OS_DisableInterrupts();
#ifndef SDK_FINALROM
    if (!log_output_api)
    {
        void (**volatile p_output_api) (const char*) = &OS_PutString;

        void (**volatile p_output_bak) (const char*) = &log_output_api;
        (**p_output_api) ("");
        *p_output_bak = *p_output_api;
        *p_output_api = DebugOutputHook;
    }
#endif
    log_cur_x = log_cur_y = 0;
    MI_CpuClear32(log_line, sizeof(log_line));

    MI_CpuClear32(sub_text_scr, sizeof(sub_text_scr));
    sub_text_modified = FALSE;

    (void)OS_RestoreInterrupts(bak_cpsr);
}

/*---------------------------------------------------------------------------*
  Name:         DebugOutput

  Description:  デバッグ出力をフックしてログに追加.

  Arguments:    s                出力文字列.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void DebugOutput(const char* s)
{
    int x, y;

    /* 1 行単位で出力を保存 */
    x = log_cur_x;
    y = log_cur_y;
    for (;; ++s)
    {
        if (*s == '\n')
        {
            log_line[y & 31][x] = '\0';
            x = 32;
        }
        else
        {
            log_line[y & 31][x] = *s;
        }

        if (*s == '\0')
        {
            break;
        }
        else if (++x >= 32)
        {
            x = 0;
            ++y;
        }
    }

    log_cur_x = x;
    log_cur_y = y;
    sub_text_modified = TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         CurrentOutput

  Description:  カレント内容をこっそりセット.

  Arguments:    s                出力文字列.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CurrentOutput(const char* s)
{
    MI_CpuCopy8(s, current_output, 32);
    sub_text_modified = TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         FlushDebugOutput

  Description:  ログ出力を画面に描画.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FlushDebugOutput(void)
{
    OSIntrMode  bak_cpsr = OS_DisableInterrupts();
    BOOL        modified = sub_text_modified;
    if (modified)
    {
        sub_text_modified = FALSE;
    }
    (void)OS_RestoreInterrupts(bak_cpsr);

    /* バックグラウンドでの自動更新を考え, ここで割り込みは禁止しない */
    if (modified)
    {
        //int     base = (log_cur_y <= 23) ? 0 : (log_cur_y - 23);
        int base = (log_cur_y <= 20) ? 0 : (log_cur_y - 20);
        int x, y;

        //for (y = 0; y < 23; ++y)
        for (y = 0; y < 20; ++y)
        {
            const char*     src = log_line[(base + y) & 31];
            u16*        dst = sub_text_scr[y];
            for (x = 0; src[x]; ++x)
            {
                dst[x] = (u8) src[x];
            }

            for (; x < 32; ++x)
            {
                dst[x] = 0;
            }
        }

        {
            const char*     src = current_output;
            u16*        dst = sub_text_scr[22];
            for (x = 0; src[x]; ++x)
            {
                dst[x] = (u8) src[x];
            }

            for (; x < 32; ++x)
            {
                dst[x] = 0;
            }
        }

        DC_FlushRange(sub_text_scr, sizeof(sub_text_scr));
        GXS_LoadBG0Scr(sub_text_scr, 0, sizeof(sub_text_scr));
    }
}
