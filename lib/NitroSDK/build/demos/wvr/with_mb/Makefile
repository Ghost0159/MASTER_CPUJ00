#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - WVR - demos
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
# $Log: Makefile,v $
# Revision 1.1  2005/01/14 06:25:50  terui
# tests以下からdemos以下に移動。
#
# $NoKeywords: $
#----------------------------------------------------------------------------

# 子機ブート用バイナリを作成
SUBDIRS		=	child


# MBライブラリのサンプルはマルチブート機能を使うため、
# 同じ通信環境(無線あるいは有線)どうしの開発機材を複数必要とします。
# $NitroSDK/bin/ARM9-TS/Release/ ディレクトリにある
# mb_child.bin プログラムが最終実機におけるマルチブート子機と同等の
# 機能を提供するサンプルになっていますので、
# このバイナリをサンプルプログラムと同じ方法で他の機材に読み込み、
# 一緒に実行してください。


#----------------------------------------------------------------------------

INCDIR		=	common/include parent/include
SRCDIR		=	common/src parent/src
SRCS		=	main.c      \
			common.c    \
			disp.c      \
			font.c      \
			gmain.c

LLIBRARIES	=	libwvr$(CODEGEN).a

# WH モジュール は wireless_shared へ移動しました.

WH_DIR		=	$(ROOT)/build/demos/wireless_shared/wh
SRCDIR		+=	$(WH_DIR)
INCDIR		+=	$(WH_DIR)
SRCS		+=	$(WH_DIR)/wh.c

# MBP モジュール は wireless_shared へ移動しました.

MBP_DIR		=	$(ROOT)/build/demos/wireless_shared/mbp
SRCDIR		+=	$(MBP_DIR)
INCDIR		+=	$(MBP_DIR)
SRCS		+=	$(MBP_DIR)/mbp.c


TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

ROM_SPEC	=	main.rsf

# MBのコールバック内で複雑な処理をする場合には、lcfファイルで
# IRQスタックサイズを少し大きめに設定しておいた方が安全です。
# 特にOS_Printf()は大量のスタックを消費するので、コールバック内では
# できるだけ軽量版のOS_TPrintf()を使用するようにしてください。

# ifeq	($(NITRO_PLATFORM),TEG)
# MAKELCF_FLAGS	+=	-DADDRESS_MAIN=0x02004000
# else
# MAKELCF_FLAGS	+=	-DADDRESS_MAIN=0x02000000
# endif
# LCFILE_SPEC	=	main.lsf

# makerom settings
MAKEROM_ROMROOT		=	./data
MAKEROM_ROMFILES	=	*.srl

#------------------------------------------------------#
# VRAM内に無線ライブラリが配置されるコンポーネント定義 #
MY_COMPONENT_NAME	= ichneumon
MAKEROM_ARM7	= $(NITRO_COMPONENTSDIR)/$(MY_COMPONENT_NAME)/$(NITRO_BUILDTYPE_ARM7)/$(MY_COMPONENT_NAME)_sub$(TS_VERSION).nef
#------------------------------------------------------#

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
