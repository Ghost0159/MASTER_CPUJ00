#! make -f
#----------------------------------------------------------------------------
# Project:  NitroWiFi - demos - sitedefs
# File:     Makefile
#
# Copyright 2005 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.5  2005/12/21 03:27:42  adachi_hiroaki
# SITEDEFSのファイルを再定義できるようにした
#
# Revision 1.4  2005/11/16 01:23:03  adachi_hiroaki
# 内部用と外部公開用の分岐条件をファイルの有無にした
#
# Revision 1.3  2005/11/16 00:52:34  adachi_hiroaki
# 内部公開用と外部公開用を分けて整理
#
# Revision 1.2  2005/11/02 04:13:42  terui
# Fix comment.
#
# Revision 1.1  2005/08/22 11:39:46  yasu
# デモが動作する前提となるネットワーク設定を一元化する
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------
TARGET_LIB	= libsitedefs$(NITRO_LIBSUFFIX).a


SITEDEFS	?= $(shell \
			if [ -r _sitedefs_custom.c ];then \
				echo _sitedefs_custom.c;\
			else \
				echo sitedefs.c;\
			fi)

SRCS		= $(SITEDEFS)

include	$(NITROWIFI_ROOT)/build/buildtools/commondefs

#----------------------------------------------------------------------------

do-build:	$(TARGETS)

include	$(NITROWIFI_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
