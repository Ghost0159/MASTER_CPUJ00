#! make -f
#----------------------------------------------------------------------------
# Project:  NitroWiFi - demos - soc - telnet
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
# Revision 1.2  2006/05/26 06:40:03  adachi_hiroaki
# 必要なときだけ NitroSystem のファイルを参照するようにした。
#
# Revision 1.1  2006/01/12 03:01:23  adachi_hiroaki
# telnetデモの追加
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

TARGET_BIN	= main.srl
SRCS		= main.c font.c vt100.c console.c telnet.c softkbd.c

#SoftKeyboardライブラリがある場合は場所を指定する
#WITH_KBD	= d:/dev/release/SoftKeyboard/kbd

#----------------------------------------------------------------------------

#SoftKeyboardライブラリはNitroSystemが必要
ifdef WITH_KBD
include	$(NITROSYSTEM_ROOT)/build/buildtools/commondefs
else
include	$(NITROSDK_ROOT)/build/buildtools/commondefs
endif

include	$(NITROWIFI_ROOT)/build/buildtools/commondefs
include	$(NITROWIFI_ROOT)/build/demos/soc/netconnect/commondefs.add-ins.netconnect

ifdef WITH_KBD
LINCLUDES	+= $(WITH_KBD)/include
LLIBRARY_DIRS	+= $(WITH_KBD)/lib/$(NITRO_BUILDTYPE)
LLIBRARIES	+= libkbd$(NITRO_LIBSUFFIX).a
LMACRO_FLAGS	+= -DWITH_KBD
else
SRCS            += kbd_tiny.c
endif


do-build:		$(TARGETS)

ifdef WITH_KBD
include	$(NITROSYSTEM_ROOT)/build/buildtools/modulerules
else
include	$(NITROSDK_ROOT)/build/buildtools/modulerules
endif

include	$(NITROWIFI_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
