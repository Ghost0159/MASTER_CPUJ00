#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - WM - demos
# File:     Makefile
#
# Copyright 2003-2005 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: makefile,v $
# Revision 1.16  2005/11/22 01:06:16  yosizaki
# copyright の年を更新.
#
# Revision 1.15  2005/11/21 00:41:10  adachi_hiroaki
# 親機選択画面に参加人数とニックネームを表示するようにした
#
# Revision 1.14  2004/11/05 04:27:40  sasakis
# ロビー画面、親機選択画面の追加と、それに必要な改造（scan関連など）。
#
# Revision 1.13  2004/11/02 07:15:01  sasakis
# makefile の修正忘れ。
#
# Revision 1.12  2004/11/02 07:12:55  sasakis
# コード整理。
#
# Revision 1.11  2004/11/01 04:11:44  sasakis
# 処理落ち対策コードなどの追加（作業中コードを含む暫定版）。
#
# Revision 1.10  2004/10/22 07:35:18  sasakis
# 共有化した wh に対応。
#
# Revision 1.9  2004/10/21 00:43:23  yosizaki
# add SHARED_WH switch.
#
# Revision 1.8  2004/10/06 05:11:35  sasakis
# グラフ表示の追加など。
#
# Revision 1.7  2004/09/30 00:00:29  takano_makoto
# ローカル設定を削除
#
# Revision 1.6  2004/09/22 09:39:33  sasakis
# デバグ画面を追加。
#
# Revision 1.5  2004/09/17 09:13:30  sasakis
# 親機時、MeasureChannelを使用して、最も混雑していないチャンネルを
# 選択するように改造した（ただし条件がまだ決定していない）。
#
# Revision 1.4  2004/09/15 10:23:40  sasakis
# 電波受信強度アイコンの追加と、データサイズ可変化のための改造など。
#
# Revision 1.3  2004/09/10 08:44:52  sasakis
# ガイドライン準拠処理の追加、エラー関係の見直しなど。
#
# Revision 1.2  2004/09/08 01:52:52  sasakis
# エラー・切断関係改良。
#
# Revision 1.1  2004/09/07 04:59:42  sasakis
# 初版登録。
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS         =

#----------------------------------------------------------------------------

INCDIR			=	./include
SRCDIR			=	./src
SRCS            = \
                main.c          \
                print.c         \
		key.c		\
		graphics.c	\
                font.c          \
		menu.c		\
		unicode.c	\
                icon.c

WH_DIR		=  $(ROOT)/build/demos/wireless_shared/wh
SRCDIR		+= $(WH_DIR)
INCDIR		+= $(WH_DIR)
SRCS		+= $(WH_DIR)/wh.c
MACRO_FLAGS	+= -DSHARED_WH

TARGET_BIN      = main.srl

include $(NITROSDK_ROOT)/build/buildtools/commondefs

#----------------------------------------------------------------------------

do-build:       $(TARGETS)

include $(NITROSDK_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
