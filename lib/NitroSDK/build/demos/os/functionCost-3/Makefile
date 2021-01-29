#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - OS - demos - functionCost-3
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
# Revision 1.2  2004/07/01 02:25:48  yasu
# .bin -> .srl
#
# Revision 1.1  2004/05/19 04:12:04  yada
# initial release
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		=	main.c
TARGET_BIN	=	main.srl

NITRO_PROFILE		=	TRUE
NITRO_PROFILE_TYPE	=	FUNCTIONCOST

#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
