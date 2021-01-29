#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - FS - demos - file-1
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
# Revision 1.11  2004/09/08 12:25:38  yasu
# switch main address by TEG/TS setting
#
# Revision 1.10  2004/07/10 04:16:35  yasu
# Stop building with elftobin
#
# Revision 1.9  2004/07/01 02:25:30  yasu
# .bin -> .srl
#
# Revision 1.8  2004/05/06 10:18:17  yasu
# fix typo
#
# Revision 1.7  2004/04/13 12:08:41  yosizaki
# fix header comment.
#
# Revision 1.6  2004/04/06 05:58:17  yasu
# unite all rsf files to main.rsf
#
# Revision 1.5  2004/04/02 13:07:00  yasu
# merged with 1.00 branch
#
# Revision 1.4.2.1  2004/04/01 13:20:02  yasu
# add NITRO_MAKEROM=TRUE
#
# Revision 1.4  2004/02/10 12:54:40  yasu
# support thumb version
#
# Revision 1.3  2004/02/05 07:09:02  yasu
# change SDK prefix iris -> nitro
#
# Revision 1.2  2004/01/18 01:17:46  yasu
# Made rom spec file for each target Debug, Release and Rom
#
# Revision 1.1  2004/01/15 07:32:17  yasu
# Test for ROM Image construction
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		=	main.c
SRCS_OVERLAY	=	boo.c foo.c woo.c

TARGET_BIN	=	main.srl

NITRO_MAKEROM	=	TRUE
NITRO_ELFTOBIN	=

ROM_SPEC	=	main.rsf
LCFILE_SPEC	=	main.lsf

ifeq	($(NITRO_PLATFORM),TEG)
MAKELCF_FLAGS	+=	-DADDRESS_MAIN=0x02004000
else
MAKELCF_FLAGS	+=	-DADDRESS_MAIN=0x02000000
endif

#SRCDIR		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/modulerules

do-build:	$(TARGETS)

#===== End of Makefile =====
