#! make -f
#----------------------------------------------------------------------------
# Project:  NitroDWC - demos - dwc_connect_inet
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
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

SRCS	= \
	main.c		\
	init.c		\
	utility.c

TARGET_BIN	=	dwc_connect_inet_sample.srl
ROM_SPEC	=	dwc_connect_inet_sample.rsf
LCFILE_SPEC	=	dwc_connect_inet_sample.lsf

LDEPENDS_NEF = $(NITRODWC_LIB_DEPENDS)

NITRODWC_LIB_MODULES_   = base auth ac bm account
NITRODWC_LIB_GSMODULES_ = common ghttp gp gstats gt2 natneg qr2 serverbrowsing chat

LINCLUDES     = ../dbs/include
LLIBRARIES    = libdwcdbs$(NITRO_LIBSUFFIX).a
LLIBRARY_DIRS = ../dbs/lib/$(NITRO_BUILDTYPE)

CCFLAGS_MSGSTYLE = -msgstyle gcc

#----------------------------------------------------------------------------
include	$(NITRODWC_ROOT)/build/buildtools/commondefs

do-build:		$(TARGETS)

include	$(NITRODWC_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
