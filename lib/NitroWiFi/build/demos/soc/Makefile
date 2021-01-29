#! make -f
#----------------------------------------------------------------------------
# Project:  NitroWiFi - demos - SOC
# File:     Makefile
#
# Copyright 2005,2006 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.13  2006/01/12 03:02:58  adachi_hiroaki
# telnetデモの追加
#
# Revision 1.12  2006/01/12 02:34:28  okubata_ryoma
# add ftpclient
#
# Revision 1.11  2006/01/09 23:53:30  kitase_hirotake
# shared の追加
#
# Revision 1.10  2006/01/06 00:42:24  kitase_hirotake
# httpserver の追加
#
# Revision 1.9  2005/12/22 04:31:52  adachi_hiroaki
# SOCLを使ったデモの入れ替え
#
# Revision 1.8  2005/11/17 23:42:56  adachi_hiroaki
# checksを非公開へ
#
# Revision 1.7  2005/11/02 04:13:42  terui
# Fix comment.
#
# Revision 1.6  2005/08/24 11:27:00  yasu
# udp_drop 追加
#
# Revision 1.5  2005/08/09 09:59:32  yasu
# noblock の追加
#
# Revision 1.4  2005/07/25 00:21:48  yasu
# echo の削除
#
# Revision 1.3  2005/07/23 14:19:42  yasu
# checks 追加
#
# Revision 1.2  2005/07/23 05:32:09  yasu
# インデント修正および shared ファイル使用
#
# Revision 1.1  2005/07/20 05:15:58  yasu
# make チェインの連結
#
# $NoKeywords: $
#----------------------------------------------------------------------------

include	$(NITROWIFI_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

SUBDIRS			= netconnect shared httpclient dhcp_monkey httpserver \
			  ftpclient telnet

#----------------------------------------------------------------------------

include	$(NITROWIFI_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
