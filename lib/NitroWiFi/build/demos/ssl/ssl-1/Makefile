#! make -f
#----------------------------------------------------------------------------
# Project:  NitroWiFi - SSL - demos - ssl-1
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
# Revision 1.6  2005/11/02 04:13:42  terui
# Fix comment.
#
# Revision 1.5  2005/07/29 10:00:50  seiki_masashi
# NITROWIFI_NOSUPPORT の参照位置を修正
#
# Revision 1.4  2005/07/25 08:56:16  yasu
# 著作年度記載の修正
#
# Revision 1.3  2005/07/23 05:32:09  yasu
# インデント修正および shared ファイル使用
#
# Revision 1.2  2005/07/21 12:13:54  seiki_masashi
# 共有の wcm_control.c を使用するように変更
#
# Revision 1.1  2005/07/21 11:47:03  seiki_masashi
# ssl/ssl-1 に移動
#
# Revision 1.1  2005/07/18 08:10:26  seiki_masashi
# ssl-1 デモの追加
#
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

TARGET_BIN	= main.srl

SRCS		= main.c tcp_client.c \
		  font.c screen.c \
		  ca_test.c

# for WCM control
SHARED_DIR	 = $(NWIFI_ROOT)/build/demos/shared
SRCS		+= $(SHARED_DIR)/wcm_control.c		\
		   $(SHARED_DIR)/ap_info.c
LINCLUDES	+= $(SHARED_DIR)

#----------------------------------------------------------------------------

include	$(NITROWIFI_ROOT)/build/buildtools/commondefs

ifdef NITROWIFI_NOSUPPORT
SRCS		+= $(SHARED_DIR)/_ap_info_custom.c
endif

do-build:		$(TARGETS)

include	$(NITROWIFI_ROOT)/build/buildtools/modulerules

#----------------------------------------------------------------------------

#===== End of Makefile =====
