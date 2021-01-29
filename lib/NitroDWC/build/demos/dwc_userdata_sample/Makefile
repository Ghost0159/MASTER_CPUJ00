#! make -f
#----------------------------------------------------------------------------
# Project:  NitroDWC - test - dwc_userdata_sample
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
# Revision 1.1  2005/11/18 05:38:53  onozawa
# DWCÉâÉCÉuÉâÉäçÏê¨
#
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

SRCS	= \
	init.c		\
	utility.c	\
	userdata.c	\
	main.c		\
	font.c		\
	screen.c	\
	mp.c		\
	wc.c		\

TARGET_BIN	=	dwc_userdata_sample.srl
ROM_SPEC	=	dwc_userdata_sample.rsf
LCFILE_SPEC	=	dwc_userdata_sample.lsf

LDEPENDS_NEF = $(NITRODWC_LIB_DEPENDS)

NITRODWC_LIB_MODULES_   = bm account
NITRODWC_LIB_GSMODULES_ = common

LINCLUDES     = ../dbs/include
LLIBRARIES    = libdwcdbs$(NITRO_LIBSUFFIX).a
LLIBRARY_DIRS = ../dbs/lib/$(NITRO_BUILDTYPE)

CCFLAGS_MSGSTYLE = -msgstyle gcc

#----------------------------------------------------------------------------
include	$(NITRODWC_ROOT)/build/buildtools/commondefs

do-build:		$(TARGETS)

include	$(NITRODWC_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
