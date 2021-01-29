#! make -f
#----------------------------------------------------------------------------
# Project:  NitroWiFi - 
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
# Revision 1.2  2006/01/06 00:44:38  kitase_hirotake
# PAD入力対応版
#
# Revision 1.1  2005/12/22 04:34:06  adachi_hiroaki
# HTTPクライアントのデモをSOCで置き換え
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

# SOC_SHARED ルートの定義.
SOC_SHARED		=  $(NWIFI_ROOT)/build/demos/soc/shared


TARGET_BIN	= main.srl
SRCS		= main.c http.c ssl.c

# SOC デモ共通初期化ライブラリ.
LLIBRARIES		+= libkeypad$(NITRO_LIBSUFFIX).a
LLIBRARY_DIRS		+= $(SOC_SHARED)/lib/$(NITRO_BUILDTYPE)
LINCLUDES		+= $(SOC_SHARED)/include



#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/commondefs
include	$(NITROWIFI_ROOT)/build/buildtools/commondefs
include	$(NITROWIFI_ROOT)/build/demos/soc/netconnect/commondefs.add-ins.netconnect

do-build:		$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules
include	$(NITROWIFI_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
