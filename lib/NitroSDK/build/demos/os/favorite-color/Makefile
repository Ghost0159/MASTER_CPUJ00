#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - OS - demos - favorite-color
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
# Revision 1.3  2004/10/12 02:23:23  yasu
# Don't use SUBDIRS for building DEMOLib.a to avoid problems around building twice.
#
# Revision 1.2  2004/10/12 01:51:19  yosizaki
# change NITROSDK_ROOT to ROOT (DEMOLIB_DIR)
#
# Revision 1.1  2004/10/07 10:20:05  yosizaki
# (none)
#
# $NoKeywords: $
#----------------------------------------------------------------------------

DEMOLIB_DIR	= $(ROOT)/build/demos/gx/UnitTours/DEMOLib

SUBDIRS 	=
LINCLUDES       = $(DEMOLIB_DIR)/include
LLIBRARY_DIRS   = $(DEMOLIB_DIR)/lib/$(NITRO_BUILDTYPE)
LLIBRARIES      = libDEMO.a

DEMOLIB		= $(DEMOLIB_DIR)/lib/$(NITRO_BUILDTYPE)/libDEMO.a
LDEPENDS_NEF	= $(DEMOLIB)

#----------------------------------------------------------------------------

SRCS		= main.c font.c
TARGET_BIN	= main.srl

#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs

#----------------------------------------------------------------------------

do-build:	$(TARGETS)

$(DEMOLIB):
		$(MAKE) -C $(DEMOLIB_DIR)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
