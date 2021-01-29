#! make -f
#----------------------------------------------------------------------------
# Project:  NITRO-System - demos - g3d - demolib
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
# $Revision: 1.3 $
#----------------------------------------------------------------------------

G3D_DEMOLIB		=	$(NITROSYSTEM_ROOT)/build/demos/g3d/demolib

LINCLUDES       =   $(G3D_DEMOLIB)/include
#LLIBRARY_DIRS  =   $(G3D_DEMOLIB)/lib/$(NITRO_BUILDTYPE)
#LLIBRARIES     =   libg3d_demo.a


SUBDIRS     =


#----------------------------------------------------------------------------

SRCS        =   system.c \
                camera.c \
				graphics.c \
				print.c \
				font.c

TARGET_LIB  =   libg3d_demo.a

#SRCDIR     =   # using default
#LCFILE     =   # using default

include $(NITROSYSTEM_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:       $(TARGETS)


include $(NITROSYSTEM_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
