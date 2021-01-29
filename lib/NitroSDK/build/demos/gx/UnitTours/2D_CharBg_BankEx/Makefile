#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - GX - demos - UnitTours/2D_CharBg_BankEx
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
# Revision 1.1  2004/11/25 11:41:02  takano_makoto
# initial update.
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

SRCS		=	main.c data.c
TARGET_BIN	=	main.srl
LINCLUDES	=	../DEMOLib/include
LLIBRARY_DIRS	=	../DEMOLib/lib/$(NITRO_BUILDTYPE)
LLIBRARIES	=	libDEMO.a

#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
