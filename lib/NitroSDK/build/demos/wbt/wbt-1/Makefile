#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - WBT - demos - wbt-1
# File:     Makefile
#
# Copyright 2003,2004,2006 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.6  2006/09/18 23:22:12  okubata_ryoma
# Copyright fix
#
# Revision 1.5  2006/09/15 06:17:11  okubata_ryoma
# undo
#
# Revision 1.4  2006/09/15 06:12:55  okubata_ryoma
# WC_DIRを相対パスに変更
#
# Revision 1.3  2004/10/28 11:03:30  yosizaki
# merge wc with ../wireless_shared
#
# Revision 1.2  2004/10/19 10:27:02  miya
# Port受信コールバックの使用とプリント文をNITRO画面上に表示するように変更
#
# Revision 1.1  2004/08/23 09:47:14  yosizaki
# (none)
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

SRCS		=	main.c font.c bt.c text.c

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

