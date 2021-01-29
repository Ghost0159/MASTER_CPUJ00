#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - demos - OS - forChina-fs
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
# Revision 1.1  2005/06/02 07:57:22  yosizaki
# initial upload.
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS					=

#----------------------------------------------------------------------------

SRCS					=	main.c font.c

TARGET_NEF				=	main.nef
TARGET_BIN				=	main.srl

NITRO_MAKEROM			=	TRUE
ROM_SPEC				=	forChina.rsf


# FS を使用して各種定数データをロードするように
# OS_ShowAttentionChina() 関数の実装をオーバーライド.
SRCS					+=	os_china_override.c
MAKEROM_ROMROOT			=	$(NITROSDK_ROOT)/data/for_china
MAKEROM_ROMFILES		=	*


include	$(NITROSDK_ROOT)/build/buildtools/commondefs

do-build:	$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
