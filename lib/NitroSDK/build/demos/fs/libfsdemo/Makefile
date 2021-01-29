#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - FS - demos - libfsdemo
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
# Revision 1.2  2004/09/04 02:49:49  yosizaki
# fix typo.
#
# Revision 1.1  2004/07/09 00:57:15  yosizaki
# (none)
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		=	libfsdemo_util.c
TARGET_LIB	=	libfsdemo.a

include	$(NITROSDK_ROOT)/build/buildtools/commondefs

ifdef	NITROSDK_INSTALL_ROOT
INSTALL_TARGETS	=	$(TARGETS)
INSTALL_DIR	=	$(NITRO_INSTALL_ROOT)/build/demos/fs/libfsdemo/lib/$(NITRO_BUILDTYPE)
endif

#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
