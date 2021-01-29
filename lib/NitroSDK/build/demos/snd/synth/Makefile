#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - SND - demos - synth
# File:     Makefile
#
# Copyright 2005 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.2  2005/04/13 14:10:11  yosizaki
# fix NITROSYSTEM_ROOT -> NITROSDK_ROOT
#
# Revision 1.1  2005/04/13 03:29:59  ida
# SNDÇNITRO-SDKÇ÷à⁄ê›
#
# Revision 1.2  2005/03/08 07:45:24  kyuma_koichi
# èâî≈
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		=	main.c

TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
