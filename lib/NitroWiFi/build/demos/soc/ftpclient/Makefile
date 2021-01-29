#! make -f
#----------------------------------------------------------------------------
# Project:  NitroWiFi - demo - soc - ftpclient
# File:     Makefile
#
# Copyright 2006 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.1  2006/01/12 02:17:39  okubata_ryoma
# ftp.ｃの追加
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

# SOC_SHARED ルートの定義.
SOC_SHARED		=  $(NWIFI_ROOT)/build/demos/soc/shared


TARGET_BIN	= main.srl
SRCS		= main.c

NITRO_MAKEROM	=	TRUE
ROM_SPEC	=	main.rsf

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
