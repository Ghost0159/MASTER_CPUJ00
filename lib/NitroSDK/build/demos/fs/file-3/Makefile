#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - FS - demos - file-3
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
# Revision 1.1  2005/06/28 02:12:50  yosizaki
# initial upload.
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS				=	main.c
TARGET_NEF			=	main.nef
TARGET_BIN			=	main.srl

NITRO_MAKEROM		=	TRUE
MAKEROM_ROMROOT		=	./src
MAKEROM_ROMFILES	=	*

#ROM_SPEC			=	# using default
#LCFILE_SPEC		=	# using default
#SRCDIR				=	# using default


include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:	$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
