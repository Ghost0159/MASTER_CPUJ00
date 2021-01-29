#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - OS - demos - exceptionDisplay-3
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
# Revision 1.6  2004/07/01 02:25:48  yasu
# .bin -> .srl
#
# Revision 1.5  2004/06/03 07:16:47  yada
# sorry, update 1.4 is mistake. ignore comment of 1.4.
# now, add NITRO_PROFILE_TYPE=CALLTRACE.
#
# Revision 1.4  2004/06/03 06:59:41  yada
# fix indent processing
#
# Revision 1.3  2004/05/06 10:18:18  yasu
# fix typo
#
# Revision 1.2  2004/04/14 07:09:18  yada
# add NITRO_PROFILE=TRUE
#
# Revision 1.1  2004/04/14 07:08:02  yada
# first release

# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		=	main.c
TARGET_BIN	=	main.srl

NITRO_PROFILE	=	TRUE
NITRO_PROFILE_TYPE =	CALLTRACE

#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs



#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
