#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - FS - demos - arc-2
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
# Revision 1.2  2004/09/04 02:47:05  yosizaki
# change to use default component(mongoose).
#
# Revision 1.1  2004/07/12 13:18:09  yosizaki
# (none)
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=	../libfsdemo


#----------------------------------------------------------------------------

SRCS		=	main.c
TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

ROM_SPEC	=	main.rsf

#LCFILE_SPEC	=	# using default
#SRCDIR		=	# using default

LINCLUDES	+=	../libfsdemo/include

LLIBRARY_DIRS   +=       ../libfsdemo/lib/$(NITRO_BUILDTYPE)
LLIBRARIES      +=       libfsdemo.a


include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:	$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules

ifdef	NITRO_ELFTOBIN
$(TARGETS): $(ELFTOBIN_ARM7)

else	#NITRO_ELFTOBIN
$(TARGETS): $(MAKEROM_ARM7)

endif	#NITRO_ELFTOBIN


#===== End of Makefile =====
