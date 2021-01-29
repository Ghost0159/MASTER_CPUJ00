#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - demos - OS - forKorea-1
# File:     Makefile
#
# Copyright 2006 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.1  2006/10/19 01:32:51  okubata_ryoma
# ƒTƒ“ƒvƒ‹ƒfƒ‚forKorea-1‚Ì’Ç‰Á
#
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS			=

#----------------------------------------------------------------------------

SRCS			=	main.c font.c

TARGET_NEF		=	main.nef
TARGET_BIN		=	main.srl

NITRO_MAKEROM		=	TRUE
ROM_SPEC		=	forKorea.rsf

include	$(NITROSDK_ROOT)/build/buildtools/commondefs

#----------------------------------------------------------------------------

do-build:	$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
