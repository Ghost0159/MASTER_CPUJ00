#! make -f
#----------------------------------------------------------------------------
# Project:  NitroDWC - demos - dwc_sample
# File:     Makefile
#
# Copyright 2003,2004 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.7  2007/02/14 11:10:15  takayama
# make PR_USERDATA=TRUE に関する内容を削除。
#
# Revision 1.6  2006/08/09 12:00:31  takayama
# コメントの文字コードをSJISに統一。
#
# Revision 1.5  2006/02/22 00:29:56  sotoike
# dwc_sample, friend_match, ghttp_cat_sample, ghttp_sample
# の各サンプルから、不要なiwmanager.c, h を(リポジトリから)削除
# NitroDWC/build/demos/ssk/dwcsample/include/iwmanager.h は放置
#
# Revision 1.4  2006/02/21 02:02:16  sotoike
# dwc_sample, dwc_sc_match_sample, friend_match, ghttp_sample ､ﾎmakefile ､ﾎ
# NITRODWC_LIB_MODULES から util の記述を削除。
# (オーバレイ用モジュールとして不要な記述であったため)
#
# Revision 1.3  2005/10/11 12:18:18  onozawa
# base64をDWC_Utilへ変更
#
# Revision 1.2  2005/09/13 00:25:36  onozawa
# 汎用的なサンプルに変更
#
# Revision 1.6  2005/09/03 07:16:14  sasakit
# support for dbs outputs.
#
# Revision 1.5  2005/08/31 02:37:30  sasakit
# accountモジュールの追加に対する修正。
#
# Revision 1.4  2005/08/23 13:56:29  arakit
# デバッグ用に固定ユーザデータを使用できるようにした。
#
# Revision 1.3  2005/08/19 12:09:38  sasakit
# リリースに向けて修正
#
# Revision 1.2  2005/08/19 07:38:33  arakit
# リンクするモジュールにauth, ac, bm を追加した。
#
# Revision 1.1.1.1  2005/08/02 08:02:31  sasakit
# First import of NitroDWC source tree.
#
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

SRCS	= \
	main.c		\
	init.c		\
	utility.c

TARGET_BIN	=	ghttp_sample.srl
ROM_SPEC	=	ghttp_sample.rsf
LCFILE_SPEC	=	ghttp_sample.lsf

LDEPENDS_NEF = $(NITRODWC_LIB_DEPENDS)

NITRODWC_LIB_MODULES_   = base auth ac bm account
NITRODWC_LIB_GSMODULES_ = common ghttp gp gstats gt2 natneg qr2 serverbrowsing chat

LINCLUDES     = ../dbs/include
LLIBRARIES    = libdwcdbs$(NITRO_LIBSUFFIX).a
LLIBRARY_DIRS = ../dbs/lib/$(NITRO_BUILDTYPE)

CCFLAGS_MSGSTYLE = -msgstyle gcc

#----------------------------------------------------------------------------
include	$(NITRODWC_ROOT)/build/buildtools/commondefs

do-build:		$(TARGETS)

include	$(NITRODWC_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
