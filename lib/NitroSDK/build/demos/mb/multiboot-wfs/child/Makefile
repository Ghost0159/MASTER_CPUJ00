#! make -f
#----------------------------------------------------------------------------
# Project:  Project:  NitroSDK - MB - demos - multiboot-wfs
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
# Revision 1.1  2005/06/23 09:05:39  yosizaki
# initial upload.
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------


include	../make_common


###############################################################################
# 子機固有の設定.

SRCS			+=	main.c child.c
# オーバーレイテスト用モジュール.
SRCS_OVERLAY	+=	overlay_test/func_1.c	\
					overlay_test/func_2.c	\
					overlay_test/func_3.cpp

MAKEROM_ROMROOT		=	.
MAKEROM_ROMFILES	=	./data


include	$(NITROSDK_ROOT)/build/buildtools/commondefs

#----------------------------------------------------------------------------

INSTALL_TARGETS		=	$(BINDIR)/$(TARGET_BIN)
INSTALL_DIR	=		 ../parent/data

do-build:	$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====

