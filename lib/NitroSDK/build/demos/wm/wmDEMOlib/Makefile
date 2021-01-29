#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - wmDEMOlib - libraries
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
# Revision 1.1  2004/07/23 15:13:38  terui
# êÆóùÇµÇƒêVãKí«â¡
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS			=


#----------------------------------------------------------------------------

SRCS			=	wm_lib.c wm_tool.c ni.c

TARGET_LIB		=	libwmDEMO$(NITRO_LIBSUFFIX).a

include	$(NITROSDK_ROOT)/build/buildtools/commondefs

ifdef	NITROSDK_INSTALL_ROOT
INSTALL_TARGETS	=	$(TARGETS)
INSTALL_DIR		=	$(NITRO_INSTALL_ROOT)/build/demos/wm/wmDEMOlib/lib/$(NITRO_BUILDTYPE)
endif

override GDIRT_CLEAN_DIRS	=	./obj ./depend ./lib/$(NITRO_BUILDTYPE)


#----------------------------------------------------------------------------

do-build:		$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
