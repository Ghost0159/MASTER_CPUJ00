#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - GX - demos - UnitTours/2D_BmpBg_Vram
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
# Revision 1.10  2004/07/28 07:38:34  yasu
# Let pictures exchangable
#
# Revision 1.9  2004/07/01 02:25:31  yasu
# .bin -> .srl
#
# Revision 1.8  2004/05/06 10:18:17  yasu
# fix typo
#
# Revision 1.7  2004/04/07 01:23:55  yada
# fix header comment
#
# Revision 1.6  2004/04/06 12:48:08  yada
# fix header comment
#
# Revision 1.5  2004/02/05 07:09:02  yasu
# change SDK prefix iris -> nitro
#
# Revision 1.4  2004/01/07 09:38:05  nishida_kenji
# revises comments
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		= main.c \
		  image.c

PICTURES	= ../2D_BmpBg_MainRam/pictures/picture_256_192_a.ppm \
		  ../2D_BmpBg_MainRam/pictures/picture_256_192_b.ppm \
		  ../2D_BmpBg_MainRam/pictures/picture_256_192_c.ppm \
		  ../2D_BmpBg_MainRam/pictures/picture_256_192_d.ppm

PICTURES_C	= $(PICTURES:.ppm=.autogen.c)

TARGET_BIN	= main.srl
LINCLUDES       = ../DEMOLib/include ../2D_BmpBg_MainRam/pictures
LLIBRARY_DIRS   = ../DEMOLib/lib/$(NITRO_BUILDTYPE)
LLIBRARIES      = libDEMO.a

LDIRT_CLEAN	= $(PICTURES_C)

#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:	$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules

%.autogen.c:	%.ppm
	$(NITRO_TOOLSDIR)/bin/ppmconv.exe $< > $@

$(OBJDIR)/image.o: image.c $(PICTURES_C)

#===== End of Makefile =====
