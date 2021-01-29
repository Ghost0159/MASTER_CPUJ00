#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - tips - demos - MultiLink
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
# Revision 1.5  2004/11/04 08:20:17  yasu
# fix typo
#
# Revision 1.4  2004/09/29 02:38:17  yasu
# Fix dependencies
#
# Revision 1.3  2004/07/01 09:05:04  yasu
# add dependency for LLIBRARIES
#
# Revision 1.2  2004/07/01 02:25:48  yasu
# .bin -> .srl
#
# Revision 1.1  2004/06/25 11:38:55  yasu
# demo for hierarchy makefile to do breakdown module onto small modules
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		= main ext extobj

#----------------------------------------------------------------------------
TARGET_BIN	= main.srl

LLIBRARY_DIRS	= main/$(LIBDIR)		\
		  ext/$(LIBDIR)

LLIBRARIES	= mainSegment.a 		\
		  extSegment.a

EXT_OBJS	= extobj/$(OBJDIR)/extobj.o

# Do 'link' when followings are modified
LDEPENDS_NEF	= main/$(LIBDIR)/mainSegment.a	\
		  ext/$(LIBDIR)/extSegment.a	\
		  $(EXT_OBJS)

# Do 'makerom' when followings are modified
LDEPENDS_BIN	=


include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
