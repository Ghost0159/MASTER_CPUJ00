#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - GX - demos - UnitTours/DEMOLib
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
# Revision 1.13  2004/07/12 02:19:45  yasu
# Make dummy directory to avoid a conflict on CVS updating
#
# Revision 1.12  2004/05/06 10:18:18  yasu
# fix typo
#
# Revision 1.11  2004/04/07 01:23:55  yada
# fix header comment
#
# Revision 1.10  2004/04/06 12:48:08  yada
# fix header comment
#
# Revision 1.9  2004/02/10 00:53:54  yasu
# add install settings
#
# Revision 1.8  2004/02/05 07:09:03  yasu
# change SDK prefix iris -> nitro
#
# Revision 1.7  2004/01/28 06:20:40  kitani_toshikazu
# Adjusted light intensity
#
# Revision 1.6  2004/01/07 09:38:05  nishida_kenji
# revises comments
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		=	DEMOKey.c DEMOInit.c DEMOUtility.c
TARGET_LIB	=	libDEMO.a

include	$(NITROSDK_ROOT)/build/buildtools/commondefs

ifdef	NITROSDK_INSTALL_ROOT
INSTALL_TARGETS	=	$(TARGETS)
INSTALL_DIR	=	$(NITRO_INSTALL_ROOT)/build/demos/gx/UnitTours/DEMOLib/lib/$(NITRO_BUILDTYPE)
endif

override GDIRT_CLEAN_DIRS =	./obj ./depend ./lib/$(NITRO_BUILDTYPE)


#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
