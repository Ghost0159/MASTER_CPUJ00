#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - GX - demos - UnitTours/ClearDepthOnly
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
# Revision 1.1  2004/10/04 13:36:23  takano_makoto
# Initial update.
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=
LINCLUDES       =       ../DEMOLib/include
LLIBRARY_DIRS   =       ../DEMOLib/lib/$(NITRO_BUILDTYPE)
LLIBRARIES      =       libDEMO.a

#----------------------------------------------------------------------------

SRCS		=	main.c tex_32768.c sea.c
TARGET_BIN	=	main.srl

#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
