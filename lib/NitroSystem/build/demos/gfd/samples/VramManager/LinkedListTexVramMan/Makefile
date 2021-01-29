#! make -f
#----------------------------------------------------------------------------
# Project:  NITRO-System - demos - gfd - samples - VramManager - LinkedListTexVramMan
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
# $Revision: 1.2 $
#----------------------------------------------------------------------------


GFD_DEMOLIB		=	$(NITROSYSTEM_ROOT)/build/demos/gfd/demolib

LINCLUDES       =   $(GFD_DEMOLIB)/include
LLIBRARY_DIRS   =   $(GFD_DEMOLIB)/lib/$(NITRO_BUILDTYPE)
LLIBRARIES      =   libgfd_demo.a


#----------------------------------------------------------------------------

SRCS		=	main.c							 \


#TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

#NITRO_MAKEROM	=	TRUE
#ROM_SPEC	=	main.rsf


#----------------------------------------------------------------------------
include	$(NITROSYSTEM_ROOT)/build/buildtools/commondefs


do-build:		$(TARGETS)


include	$(NITROSYSTEM_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
