#! make -f
#----------------------------------------------------------------------------
# Project:  NitroWiFi - demos
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
# Revision 1.15  2006/03/12 23:54:47  okubata_ryoma
# Copyright fix
#
# Revision 1.14  2006/02/28 08:48:44  okubata_ryoma
# NitroCrypto切り出しによる変更
#
# Revision 1.13  2005/12/27 09:02:27  yasu
# NOSSL に crypt を追加
#
# Revision 1.12  2005/12/13 12:03:26  seiki_masashi
# crypt ディレクトリを追加
#
# Revision 1.11  2005/09/14 00:25:41  yasu
# NOSSL の対応
#
# Revision 1.10  2005/08/22 12:11:04  yasu
# SiteDefs の追加
#
# Revision 1.9  2005/08/06 11:50:26  seiki_masashi
# ssl を復帰
#
# Revision 1.8  2005/07/22 14:04:21  yasu
# 一時的なコメントアウト
#
# Revision 1.7  2005/07/21 11:57:24  seiki_masashi
# 階層構造の修正
#
# Revision 1.6  2005/07/21 11:19:35  seiki_masashi
# ssl-1 と bsafe-1 をビルドツリーに追加
#
# Revision 1.5  2005/07/20 05:16:04  yasu
# make チェインの連結
#
# Revision 1.4  2005/07/15 09:45:24  yasu
# cps, ssl ライブラリ受け入れにともなうツリーの調整
#
# Revision 1.3  2005/07/11 12:04:00  terui
# SUBDIRS に wcm を追加。
#
# Revision 1.2  2005/06/23 11:13:17  yasu
# NitroWiFi の commondefs, modulerules を読み込む
#
# Revision 1.1  2005/06/22 11:07:57  yasu
# ソースツリーテンプレート作成
#
# $NoKeywords: $
#----------------------------------------------------------------------------

include	$(NITROWIFI_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

SUBDIRS			= SiteDefs	\
			  cps		\
			  wcm		\
			  soc		\

ifndef	NITROWIFI_NOSSL
SUBDIRS			+= ssl
endif

ifdef	NITRO_PRIVATE
#SUBDIRS		+= bsafe
endif

#----------------------------------------------------------------------------

include	$(NITROWIFI_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
