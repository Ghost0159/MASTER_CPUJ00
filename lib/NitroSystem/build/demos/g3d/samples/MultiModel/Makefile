#! make -f
#----------------------------------------------------------------------------
# Project:  NITRO-System - demos - g3d - samples - MultiModel
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
# $Revision: 1.5 $
#----------------------------------------------------------------------------

G3D_DEMOLIB		=	$(NITROSYSTEM_ROOT)/build/demos/g3d/demolib

LINCLUDES       =   $(G3D_DEMOLIB)/include
LLIBRARY_DIRS   =   $(G3D_DEMOLIB)/lib/$(NITRO_BUILDTYPE)
LLIBRARIES      =   libg3d_demo.a

#----------------------------------------------------------------------------


#----------------------------------------------------------------------------

SUBDIRS	= 

SRCS		=	main.c
TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

MAKEROM_ROMROOT  = ./
MAKEROM_ROMFILES = data/*.*

#----------------------------------------------------------------------------
include	$(NITROSYSTEM_ROOT)/build/buildtools/commondefs


do-build:binData $(TARGETS)
binData:
	$(NITROSYSTEM_ROOT)/tools/win/bin/g3dcvtr.exe data/src/cmn_tex_cube1.imd data/src/cmn_tex_cube2.imd -o data/two_cubes


include	$(NITROSYSTEM_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
