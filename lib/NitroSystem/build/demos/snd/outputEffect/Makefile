#! make -f
#----------------------------------------------------------------------------
# Project:  NITRO-System - demos - snd - outputEffect
# File:     Makefile
#
# Copyright 2004-2007 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Revision: 1.4 $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

SRCS		=	main.c

TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

MAKEROM_ROMROOT  = ../data
MAKEROM_ROMFILES = sound_data.sdat

#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSYSTEM_ROOT)/build/buildtools/commondefs

#----------------------------------------------------------------------------

do-build:		$(TARGETS)

include	$(NITROSYSTEM_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
