#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - demos - MI - UnComp_Stream_BmpBG
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
# Revision 1.2  2004/11/30 08:01:24  takano_makoto
# small fix in comment.
#
# Revision 1.1  2004/11/30 08:00:05  takano_makoto
# Initial update.
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		=	main.c
TARGET_BIN	=	main.srl
LINCLUDES       =       ../../gx/UnitTours/DEMOLib/include
LLIBRARY_DIRS   =       ../../gx/UnitTours/DEMOLib/lib/$(NITRO_BUILDTYPE)
LLIBRARIES      =       libDEMO.a

NITRO_MAKEROM	=	TRUE
ROM_SPEC	=	main.rsf


#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
