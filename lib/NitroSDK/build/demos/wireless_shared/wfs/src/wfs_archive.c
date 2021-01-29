/*---------------------------------------------------------------------------*
  Project:  NitroSDK - wireless_shared - demos - wfs
  File:     wfs_archive.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wfs_archive.c,v $
  Revision 1.12  2007/06/15 04:21:46  okubata_ryoma
  Copyright fix

  Revision 1.11  2007/06/14 13:16:51  yosizaki
  moved to the WFS library implementation.

  Revision 1.10  2006/08/25 08:36:22  yosizaki
  fix about mixed-FAT mode.

  Revision 1.9  2006/05/22 02:34:52  yosizaki
  small fix.

  Revision 1.8  2006/03/06 08:12:01  adachi_hiroaki
  HW_ROM_HEADER_BUF ‚ð CARD_GetRomHeader() ‚É’u‚«Š·‚¦

  Revision 1.7  2006/02/27 12:50:33  yosizaki
  change FS_OpenFileDirect to FS_CreateFileFromRom

  Revision 1.6  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.5  2005/11/14 08:32:35  yosizaki
  fix  about wfsi_work pointer.

  Revision 1.4  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.3  2005/02/18 09:14:36  yosizaki
  fix around hidden warnings.

  Revision 1.2  2004/12/15 02:06:04  yasu
  untabify

  Revision 1.1  2004/11/16 01:12:55  yosizaki
  moved from /demos/wbt/wbt-fs.


 -[old log at /demos/wbt/wbt-fs/wfs]-

  Revision 1.12  2004/10/28 14:50:21  yosizaki
  fix around FSi_LoadTables.

  Revision 1.11  2004/10/25 15:20:35  yosizaki
  fix WFSi_LoadTables.

  Revision 1.10  2004/10/25 04:19:32  yosizaki
  change WFSi_LoadTables.

  Revision 1.9  2004/09/29 07:11:38  yosizaki
  merge from branch.

  Revision 1.8  2004/09/15 09:10:25  yosizaki
  add Seek&Read.

  Revision 1.7  2004/09/15 01:11:06  yosizaki
  add WFS_SetPacketSize().

  Revision 1.6  2004/09/07 03:05:51  yosizaki
  source code Maintenance.

  Revision 1.5  2004/09/06 08:47:48  yosizaki
  fix archive. (around result of FS_ReadFile())

  Revision 1.4  2004/09/04 13:23:55  yosizaki
  change to load TABLE-memory with WFSi_Alloc.

  Revision 1.3  2004/09/04 01:45:21  yosizaki
  delete obsolete codes and waste parameters.

  Revision 1.2  2004/09/03 09:58:38  yosizaki
  add wfs_thread.c

  Revision 1.1  2004/09/03 01:10:17  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
