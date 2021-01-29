#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - WM - demos
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
# Revision 1.3  2004/10/22 11:04:46  terui
# wcモジュールを共有領域に切り出し。
#
# Revision 1.2  2004/07/26 02:08:09  terui
# libwm.aをローカルライブラリから削除
#
# Revision 1.1  2004/07/23 15:12:40  terui
# 整理して新規追加
#
# Revision 1.2  2004/07/23 14:48:04  terui
# 余分な記述を削除
#
# Revision 1.1  2004/07/15 12:48:12  terui
# initial upload
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

SRCS		=	main.c font.c

# wc モジュールは wireless_shared へ移動しました。
WC_DIR		=	$(ROOT)/build/demos/wireless_shared/wc
SRCS		+=	$(WC_DIR)/wc.c
LINCLUDES	=	$(WC_DIR)

TARGET_BIN	=	main.srl

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
