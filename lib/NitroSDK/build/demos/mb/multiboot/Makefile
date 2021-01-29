#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - MB - demos - multiboot
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
# Revision 1.4  2004/10/05 06:47:54  sato_masaki
# 設定メニューを追加。その他色々細かな修正。
#
# Revision 1.3  2004/09/14 02:54:17  miya
# サイン入り
#
# Revision 1.2  2004/09/01 00:10:51  yosizaki
# add LCFILE_SPEC to extend IRQ stack.
#
# Revision 1.1  2004/08/10 07:31:39  yosizaki
# (none)
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=	


# MBライブラリのサンプルはマルチブート機能を使うため、
# 同じ通信環境(無線あるいは有線)どうしの開発機材を複数必要とします。
# $NitroSDK/bin/ARM9-TS/Release/ ディレクトリにある
# mb_child.bin プログラムが最終実機におけるマルチブート子機と同等の
# 機能を提供するサンプルになっていますので、
# このバイナリをサンプルプログラムと同じ方法で他の機材に読み込み、
# 一緒に実行してください。


#----------------------------------------------------------------------------

SRCS		=	common.c data.c dispfunc.c parent.c main.c
TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

ROM_SPEC	=	main.rsf

LCFILE_SPEC	=	ARM9-$(NITRO_PLATFORM).lsf
#SRCDIR		=	# using default

# makerom settings
MAKEROM_ROMROOT		=	./data
MAKEROM_ROMFILES	=	*.srl

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:	$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules

ifdef	NITRO_ELFTOBIN
$(TARGETS): $(ELFTOBIN_ARM7)

else	#NITRO_ELFTOBIN
$(TARGETS): $(MAKEROM_ARM7)

endif	#NITRO_ELFTOBIN


#===== End of Makefile =====
