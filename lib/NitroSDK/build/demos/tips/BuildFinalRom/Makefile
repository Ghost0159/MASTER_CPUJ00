#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - GX - demos - tips/BuildFinalRom
# File:	    Makefile
#
# Copyright 2003,2004 Nintendo.	 All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.2  2004/09/21 03:33:19  yasu
# Add comments for quick guide
#
# Revision 1.1  2004/09/21 02:02:53  yasu
# Demo for building final rom
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		= etc/banner


#----------------------------------------------------------------------------

SRCS		= main.c data.c
TARGET_BIN	= main.srl
LINCLUDES	= ../../gx/UnitTours/DEMOLib/include
LLIBRARY_DIRS	= ../../gx/UnitTours/DEMOLib/lib/$(NITRO_BUILDTYPE)
LLIBRARIES	= libDEMO.a

#SRCDIR		= # using default
#LCFILE		= # using default

###--------------------------------------------------------------------------
###  Setting for finalrom
###--------------------------------------------------------------------------
#### BEGIN
####
####  Please get your "libsyscall.a" and "rom header template" from Nintendo.
####  And put them in ./etc directory.
####
#
# ROM SPEC FILE
#
ROM_SPEC	= ./ROM-TS.rsf
#
# LIBSYSCALL: Provided to every product by NINTENDO
#
LIBSYSCALL	= ./etc/libsyscall.a
###
###
###
#### END

#----------------------------------------------------------------------------
include	$(NITROSDK_ROOT)/build/buildtools/commondefs

#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
