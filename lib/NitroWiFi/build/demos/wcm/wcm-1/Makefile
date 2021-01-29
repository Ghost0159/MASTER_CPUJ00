#! make -f
#----------------------------------------------------------------------------
# Project:  NitroWiFi - WCM - demos - wcm-1
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
# Revision 1.4  2005/07/29 10:00:50  seiki_masashi
# NITROWIFI_NOSUPPORT の参照位置を修正
#
# Revision 1.3  2005/07/23 05:32:09  yasu
# インデント修正および shared ファイル使用
#
# Revision 1.2  2005/07/21 08:45:01  terui
# wcm_control.c を shared 以下のソースファイルを用いるように変更
#
# Revision 1.1  2005/07/11 12:06:53  terui
# 新規追加。
#
# $NoKeywords: $
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------

TARGET_BIN	= main.srl
SRCS		= main.c				\
		  font.c				\
		  screen.c

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

do-build:	$(TARGETS)

include	$(NITROWIFI_ROOT)/build/buildtools/modulerules

#----------------------------------------------------------------------------

#===== End of Makefile =====
