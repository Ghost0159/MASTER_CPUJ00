#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - GX - demos - UnitTours/2D_Oam_Bmp1D
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
# Revision 1.2  2004/03/01 09:32:14  takano_makoto
# Correct mistake in file header
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		=	main.c data.c
TARGET_BIN	=	main.srl
LINCLUDES       =       ../DEMOLib/include
LLIBRARY_DIRS   =       ../DEMOLib/lib/$(NITRO_BUILDTYPE)
LLIBRARIES      =       libDEMO.a

#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
