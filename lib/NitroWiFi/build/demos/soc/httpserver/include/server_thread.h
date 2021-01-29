/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos
  File:     server_thread.h

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: server_thread.h,v $
  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/06 00:43:17  kitase_hirotake
  httpserver の追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>

/*---------------------------------------------------------------------------*
  Name:         CreateServerThread

  Description:  サーバスレッドを作成.

  Arguments:    None.

  Returns:      なし.
 *---------------------------------------------------------------------------*/
void    CreateServerThread(void);

/*---------------------------------------------------------------------------*
  Name:         ServerThread

  Description:  http クライアントとの通信.

  Arguments:    arg.

  Returns:      なし.
 *---------------------------------------------------------------------------*/
void    ServerThread(void* arg);
