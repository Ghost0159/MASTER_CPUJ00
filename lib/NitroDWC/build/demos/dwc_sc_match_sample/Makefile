#! make -f
#----------------------------------------------------------------------------
# Project:  NitroDWC - test - dwc_sc_test
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
# Revision 1.4  2007/02/14 11:10:14  takayama
# make PR_USERDATA=TRUE に関する内容を削除。
#
# Revision 1.3  2006/08/09 12:00:30  takayama
# コメントの文字コードをSJISに統一。
#
# Revision 1.2  2006/02/21 02:02:16  sotoike
# dwc_sample, dwc_sc_match_sample, friend_match, ghttp_sample のmakefile の
# NITRODWC_LIB_MODULES から util の記述を削除。
# (オーバレイ用モジュールとして不要な記述であったため)
#
# Revision 1.1  2005/11/18 05:43:48  onozawa
# DWCライブラリ作成
#
# Revision 1.1  2005/10/18 06:37:22  arakit
# (none)
#
# Revision 1.1  2005/09/23 10:47:39  sasakit
# 自動テスト追加
#
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

SRCS	= \
	init.c		\
	utility.c	\
	userdata.c	\
	main.c		\
	font.c		\
	screen.c	\

TARGET_BIN	=	dwc_sc_match_sample.srl
ROM_SPEC	=	dwc_sc_match_sample.rsf
LCFILE_SPEC	=	dwc_sc_match_sample.lsf

LDEPENDS_NEF = $(NITRODWC_LIB_DEPENDS)

NITRODWC_LIB_MODULES_   = base auth ac bm account
NITRODWC_LIB_GSMODULES_ = common ghttp gp gstats gt2 natneg qr2 serverbrowsing

LINCLUDES     = ../../demos/dbs/include
LLIBRARIES    = libdwcdbs$(NITRO_LIBSUFFIX).a
LLIBRARY_DIRS = ../../demos/dbs/lib/$(NITRO_BUILDTYPE)

CCFLAGS_MSGSTYLE = -msgstyle gcc

ISD=/cygdrive/C/Program Files/INTELLIGENT SYSTEMS/IS-NITRO-DEBUGGER/X86/bin/ISND.exe

#----------------------------------------------------------------------------
include	$(NITRODWC_ROOT)/build/buildtools/commondefs

do-build:		$(TARGETS)
#	"$(ISD)" /s 04070698 "$(shell cygpath -aw $(TARGETS))" &

include	$(NITRODWC_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
