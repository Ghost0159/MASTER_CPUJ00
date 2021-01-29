#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - FS - demos - async
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
# Revision 1.4  2004/09/04 02:47:20  yosizaki
# change to use default component(mongoose).
#
# Revision 1.3  2004/07/19 11:42:00  yosizaki
# change to use libfsdemo.
#
# Revision 1.2  2004/07/01 02:25:30  yasu
# .bin -> .srl
#
# Revision 1.1  2004/05/20 10:01:10  yosizaki
# (none)
#
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=	../libfsdemo


#----------------------------------------------------------------------------

SRCS		=	main.c
TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

NITRO_MAKEROM	=	TRUE
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
