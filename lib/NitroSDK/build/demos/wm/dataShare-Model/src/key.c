/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - dataShare-Model
  File:     key.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: key.c,v $
  Revision 1.3  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.2  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.1  2004/11/01 04:11:44  sasakis
  処理落ち対策コードなどの追加（作業中コードを含む暫定版）。

  $NoKeywords$
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include "key.h"

static KeyInfo sKeyInfo;

static void keyRead(KeyInfo * pKey)
{
    static u16 repeat_count[12];
    int     i;
    u16     r;

    r = PAD_Read();
    pKey->trg = 0x0000;
    pKey->up = 0x0000;
    pKey->rep = 0x0000;

    for (i = 0; i < 12; i++)
    {
        if (r & (0x0001 << i))
        {
            if (!(pKey->cnt & (0x0001 << i)))
            {
                pKey->trg |= (0x0001 << i);
                repeat_count[i] = 1;

            }
            else
            {
                if (repeat_count[i] > KEY_REPEAT_START)
                {
                    pKey->rep |= (0x0001 << i);
                    repeat_count[i] = KEY_REPEAT_START - KEY_REPEAT_SPAN;
                }
                else
                {
                    repeat_count[i]++;
                }
            }
        }
        else
        {
            if (pKey->cnt & (0x0001 << i))
            {
                pKey->up |= (0x0001 << i);
            }
        }
    }

    pKey->cnt = r;
}

void updateKeys(void)
{
    keyRead(&sKeyInfo);
}

KeyInfo *getKeyInfo(void)
{
    return &sKeyInfo;
}
