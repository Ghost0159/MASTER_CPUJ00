#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - WM - demos - wmPadRead-child
# File:     Makefile
#
# Copyright 2007 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.3  2007/11/15 02:15:25  seiki_masashi
# 不要なファイルを削除
#
# Revision 1.2  2007/10/27 14:52:51  seiki_masashi
# WH を使用するように変更
#
# Revision 1.1  2007/04/18 09:27:22  kitase_hirotake
# Initial Upload
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

INCDIR		=	./include
SRCDIR		=	./src

SRCS		=	main.c data.c tpdata.c

WH_DIR		=	$(ROOT)/build/demos/wireless_shared/wh
SRCDIR		+=	$(WH_DIR)
INCDIR		+=	$(WH_DIR)
SRCS		+=	$(WH_DIR)/wh.c
MACRO_FLAGS	+= -DSHARED_WH

TARGET_BIN	=	wmPadRead.srl

#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
