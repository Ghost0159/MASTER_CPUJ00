#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - WBT - demos - wbt-fs
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
# Revision 1.12  2006/09/18 23:22:05  okubata_ryoma
# Copyright fix
#
# Revision 1.11  2006/09/15 06:17:17  okubata_ryoma
# undo
#
# Revision 1.10  2006/09/15 06:14:04  okubata_ryoma
# WC_DIR WFS_DIRを相対パスに変更
#
# Revision 1.9  2004/11/16 01:12:07  yosizaki
# moved wfs to /demos/wireless_shared.
#
# Revision 1.8  2004/11/08 04:09:47  yosizaki
# set NITRO_DIGEST option.
#
# Revision 1.7  2004/10/28 14:06:18  yosizaki
# change to use wireless_shared.
#
# Revision 1.6  2004/10/25 10:16:38  yosizaki
# add parent.c and child.c
#
# Revision 1.5  2004/09/29 07:01:10  yosizaki
# change func_3.c to func_3.cpp.
#
# Revision 1.4  2004/09/09 05:02:43  yosizaki
# fix SRC_OVERLAY
#
# Revision 1.3  2004/09/04 13:28:52  yosizaki
# add overlay.
#
# Revision 1.2  2004/09/03 09:57:20  yosizaki
# add wfs_thread.c
#
# Revision 1.1  2004/09/03 01:09:52  yosizaki
# (none)
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------


# デモのアプリケーションフレームワーク.
SRCS		=	main.c util.c font.c parent.c child.c

# wc モジュールは wireless_shared へ移動しました。
WC_DIR		=	$(ROOT)/build/demos/wireless_shared/wc
SRCS		+=	$(WC_DIR)/wc.c
LINCLUDES	=	$(WC_DIR)

# 無線ファイルシステム (WFS) モジュールは wireless_shared へ移動しました。
WFS_DIR		=	$(ROOT)/build/demos/wireless_shared/wfs
SRCS		+=	$(WFS_DIR)/src/wfs_common.c	\
				$(WFS_DIR)/src/wfs_archive.c	\
				$(WFS_DIR)/src/wfs_thread.c
LINCLUDES	+=	$(WFS_DIR)/include

# オーバーレイテスト用モジュール.
SRCS_OVERLAY	+=	overlay_test/func_1.c	\
					overlay_test/func_2.c	\
					overlay_test/func_3.cpp


# マルチブート子機がオーバーレイを使用する場合,
# NITRO_DIGEST オプションを指定する必要があります.
NITRO_DIGEST	= TRUE

NITRO_MAKEROM	=	TRUE
TARGET_BIN		=	main.srl
ROM_SPEC		=	main.rsf
LCFILE_SPEC		=	main.lsf

ifeq	($(NITRO_PLATFORM),TEG)
MAKELCF_FLAGS	+=	-DADDRESS_MAIN=0x02004000
else
MAKELCF_FLAGS	+=	-DADDRESS_MAIN=0x02000000
endif

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====

