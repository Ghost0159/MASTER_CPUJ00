#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - GX - demos - UnitTours/2D_BmpBg_MainRam
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
# Revision 1.8  2004/07/28 08:43:43  yasu
# Small fix
#
# Revision 1.7  2004/07/28 07:05:54  yasu
# Let pictures exchangable
#
# Revision 1.6  2004/07/01 02:25:31  yasu
# .bin -> .srl
#
# Revision 1.5  2004/05/06 10:18:17  yasu
# fix typo
#
# Revision 1.4  2004/04/07 01:23:55  yada
# fix header comment
#
# Revision 1.3  2004/04/06 12:48:08  yada
# fix header comment
#
# Revision 1.2  2004/02/05 07:09:02  yasu
# change SDK prefix iris -> nitro
#
# Revision 1.1  2004/01/10 02:17:57  nishida_kenji
# directory renamed
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		= main.c \
		  image.c

PICTURES	= pictures/picture_256_192_a.ppm \
		  pictures/picture_256_192_b.ppm \
		  pictures/picture_256_192_c.ppm \
		  pictures/picture_256_192_d.ppm

PICTURES_C	= $(PICTURES:.ppm=.autogen.c)

TARGET_BIN	= main.srl
LINCLUDES       = ../DEMOLib/include pictures
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
