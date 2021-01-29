/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-fs
  File:     wfs_private.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wfs_private.h,v $
  Revision 1.18  2005/02/28 05:26:34  yosizaki
  do-indent.

  Revision 1.17  2004/11/16 01:11:48  yosizaki
  this file was moved to /demos/wireless_shared/wfs.

  Revision 1.16  2004/10/25 04:18:14  yosizaki
  change WFSi_LoadTables.

  Revision 1.15  2004/09/29 07:10:11  yosizaki
  merge from branch.

  Revision 1.14  2004/09/15 09:10:45  yosizaki
  add Seek&Read.

  Revision 1.13  2004/09/15 01:11:12  yosizaki
  add WFS_SetPacketSize().

  Revision 1.12  2004/09/11 04:13:35  yosizaki
  add comments.
  unify MP-size definition.

  Revision 1.11  2004/09/09 05:04:17  yosizaki
  change to block memory-allocating critical sections.

  Revision 1.10  2004/09/07 04:08:53  yosizaki
  change to delay ack-timing.

  Revision 1.9  2004/09/07 03:03:14  yosizaki
  optimize around message.

  Revision 1.8  2004/09/06 06:23:46  yosizaki
  add double-buffering system.

  Revision 1.7  2004/09/04 13:25:46  yosizaki
  change to load TABLE-memory with WFSi_Alloc.

  Revision 1.6  2004/09/04 08:29:58  yosizaki
  change around PREPARE-response.

  Revision 1.5  2004/09/04 01:45:14  yosizaki
  delete obsolete codes and waste parameters.

  Revision 1.4  2004/09/03 12:22:10  yosizaki
  support page-cache. (wbt.c Revision 1.12 or later)

  Revision 1.3  2004/09/03 11:35:41  yosizaki
  support page-cache (test version)

  Revision 1.2  2004/09/03 09:57:50  yosizaki
  add wfs_thread.c

  Revision 1.1  2004/09/03 01:12:10  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
