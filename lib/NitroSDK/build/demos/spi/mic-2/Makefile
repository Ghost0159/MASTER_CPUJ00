#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - SPI - demos
# File:     Makefile
#
# Copyright 2003,2004,2007 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.4  2007/05/14 06:38:39  yosizaki
# add rule 'capture'
#
# Revision 1.3  2004/07/31 08:16:22  terui
# Change to use default component.
#
# Revision 1.2  2004/07/01 02:25:48  yasu
# .bin -> .srl
#
# Revision 1.1  2004/06/17 04:42:36  yosizaki
# (none)
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

SRCS		=	main.c

TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules

# サンプリングデータを .wav ファイルへ変換.
capture:
	rm -f *.wav
	echo now capturing MIC...
	$(NITRO_TOOLSDIR)/bin/loadrun.exe $(BINDIR)/$(TARGET_BIN) > ./log.txt
	$(NITRO_TOOLSDIR)/bin/mic2wav.exe ./log.txt


#===== End of Makefile =====
