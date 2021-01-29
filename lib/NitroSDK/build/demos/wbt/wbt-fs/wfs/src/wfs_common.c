/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-fs - wfs
  File:     wfs_common.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wfs_common.c,v $
  Revision 1.25  2005/02/28 05:26:34  yosizaki
  do-indent.

  Revision 1.24  2004/11/16 01:11:42  yosizaki
  this file was moved to /demos/wireless_shared/wfs.

  Revision 1.23  2004/10/25 04:20:09  yosizaki
  add WFS_InitParent and WFS_InitChild.
  add some getter functions.

  Revision 1.22  2004/10/18 00:17:29  yosizaki
  fix WFSi_OnChildSystemCallback().

  Revision 1.21  2004/10/15 09:02:09  yosizaki
  merge from branch.

  Revision 1.20  2004/10/13 02:17:39  yosizaki
  fix WFS_end in child-mode. (add async-cancelling)

  Revision 1.19  2004/09/29 07:31:45  yosizaki
  change WFS_Init.
  change assertion of getter functions.

  Revision 1.18  2004/09/29 07:11:38  yosizaki
  merge from branch.

  Revision 1.17  2004/09/15 09:10:25  yosizaki
  add Seek&Read.

  Revision 1.16  2004/09/15 01:11:06  yosizaki
  add WFS_SetPacketSize().

  Revision 1.15  2004/09/11 08:11:01  yosizaki
  fix around recv_idle.

  Revision 1.14  2004/09/11 04:13:10  yosizaki
  unify MP-size definition.

  Revision 1.13  2004/09/09 14:15:48  yosizaki
  fix WFS_GetCurrentDownloadProgress().

  Revision 1.12  2004/09/09 05:03:18  yosizaki
  fix file page-cache.

  Revision 1.11  2004/09/07 04:08:59  yosizaki
  change to delay ack-timing.

  Revision 1.10  2004/09/07 03:05:51  yosizaki
  source code Maintenance.

  Revision 1.9  2004/09/06 08:47:48  yosizaki
  fix archive. (around result of FS_ReadFile())

  Revision 1.8  2004/09/06 06:23:41  yosizaki
  add double-buffering system.

  Revision 1.7  2004/09/04 13:23:55  yosizaki
  change to load TABLE-memory with WFSi_Alloc.

  Revision 1.6  2004/09/04 08:29:53  yosizaki
  change around PREPARE-response.

  Revision 1.5  2004/09/04 01:45:21  yosizaki
  delete obsolete codes and waste parameters.

  Revision 1.4  2004/09/03 12:24:05  yosizaki
  change names. (WBT_RegisterUserDefinedBlock, WBT_UnRegisterBlock)

  Revision 1.3  2004/09/03 11:35:48  yosizaki
  support page-cache (test version)

  Revision 1.2  2004/09/03 09:58:38  yosizaki
  add wfs_thread.c

  Revision 1.1  2004/09/03 01:10:17  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
