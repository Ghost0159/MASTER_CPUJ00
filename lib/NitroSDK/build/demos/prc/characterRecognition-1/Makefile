#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - demos - PRC
# File:     Makefile
#
# Copyright 2004-2005 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.4  2005/10/04 02:59:14  seiki_masashi
# パターンデータを共有化
#
# Revision 1.3  2004/09/03 13:28:31  seiki_masashi
# ferret から mongoose へ
#
# Revision 1.2  2004/07/01 02:25:48  yasu
# .bin -> .srl
#
# Revision 1.1  2004/06/23 12:02:11  seiki_masashi
# add 'prc/characterRecognition-1'
#
# Revision 1.3  2004/06/22 11:19:00  seiki_masashi
# 動作可能バージョン(simpleのみ)
#
# Revision 1.2  2004/06/17 02:31:57  seiki_masashi
# 認識アルゴリズムの差し替えを容易にするために prc_algo_* に分割しました
#
# Revision 1.1  2004/06/16 23:56:09  seiki_masashi
# rename ptrcg to prc.
#
# Revision 1.1  2004/06/15 08:10:55  seiki_masashi
# ダミーのライブラリでサンプルプログラムが動作可能に
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		= ../patterns

#----------------------------------------------------------------------------

TARGET_BIN	= main.srl
#TARGET_NEF	= main.nef

SRCDIR		= src ../patterns/src
INCDIR		= include ../patterns/include

SRCS		= main.c patterns.c

#----------------------------------------------------------------------------
include	$(NITROSDK_ROOT)/build/buildtools/commondefs

do-build:		$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
