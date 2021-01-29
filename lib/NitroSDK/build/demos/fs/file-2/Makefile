#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - FS - demos - file-2
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
# Revision 1.10  2004/09/04 02:49:22  yosizaki
# change to use default component(mongoose).
#
# Revision 1.9  2004/07/01 02:25:30  yasu
# .bin -> .srl
#
# Revision 1.8  2004/05/13 09:33:36  yosizaki
# fix ferret.nef -> ferret_sub.nef
#
# Revision 1.7  2004/05/12 05:21:15  yosizaki
# change components to ferret.
#
# Revision 1.6  2004/05/06 10:18:17  yasu
# fix typo
#
# Revision 1.5  2004/04/20 04:25:41  yosizaki
# fix setting for TEG.thumb.
#
# Revision 1.4  2004/04/19 06:47:22  yosizaki
# change to use components/fs.
#
# Revision 1.3  2004/04/13 12:08:24  yosizaki
# fix header comment.
#
# Revision 1.2  2004/04/06 06:06:01  yasu
# unite all rsf files to main.rsf
#
# Revision 1.1  2004/04/01 11:26:07  yosizaki
# (none)
#
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		=	main.c
TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

NITRO_MAKEROM	=	TRUE
ROM_SPEC	=	main.rsf

#LCFILE_SPEC	=	# using default
#SRCDIR		=	# using default


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
