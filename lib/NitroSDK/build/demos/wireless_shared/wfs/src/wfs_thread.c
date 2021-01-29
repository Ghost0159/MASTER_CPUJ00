/*---------------------------------------------------------------------------*
  Project:  NitroSDK - wireless_shared - demos - wfs
  File:     wfs_thread.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wfs_thread.c,v $
  Revision 1.10  2007/06/15 04:21:46  okubata_ryoma
  Copyright fix

  Revision 1.9  2007/06/14 13:17:03  yosizaki
  moved to the WFS library implementation.

  Revision 1.8  2006/05/22 02:35:24  yosizaki
  add temporary test code.

  Revision 1.7  2006/02/27 12:50:33  yosizaki
  change FS_OpenFileDirect to FS_CreateFileFromRom

  Revision 1.6  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.5  2006/01/10 02:51:04  adachi_hiroaki
  16進表記の出力には全て明示的に0xをつけた。/([^xX])(%\d*[xX][^hH])/ -> \10x\2

  Revision 1.4  2005/07/22 07:25:46  yosizaki
  fix around OSThredQueue.

  Revision 1.3  2005/05/09 04:31:40  yosizaki
  change around SDK_NO_THREAD.

  Revision 1.2  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.1  2004/11/16 01:12:55  yosizaki
  moved from /demos/wbt/wbt-fs.


 -[old log at /demos/wbt/wbt-fs/wfs]-

  Revision 1.9  2004/09/29 07:11:23  yosizaki
  merge from branch.

  Revision 1.8  2004/09/15 09:10:25  yosizaki
  add Seek&Read.

  Revision 1.7  2004/09/09 05:03:18  yosizaki
  fix file page-cache.

  Revision 1.6  2004/09/07 03:05:51  yosizaki
  source code Maintenance.

  Revision 1.5  2004/09/06 06:23:37  yosizaki
  add double-buffering system.

  Revision 1.4  2004/09/04 08:29:53  yosizaki
  change around PREPARE-response.

  Revision 1.3  2004/09/04 01:45:21  yosizaki
  delete obsolete codes and waste parameters.

  Revision 1.2  2004/09/03 11:35:48  yosizaki
  support page-cache (test version)

  Revision 1.1  2004/09/03 09:58:10  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
