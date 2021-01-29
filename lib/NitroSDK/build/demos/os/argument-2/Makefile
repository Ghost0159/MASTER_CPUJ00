#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - OS - demos - argument-2
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
# Revision 1.1  2005/09/09 05:19:51  yada
# initial release
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		=	main.c
TARGET_BIN	=	main.srl

#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


BURYARG		=	$(ROOT)/tools/bin/buryarg.exe

#----------------------------------------------------------------------------

do-build:		$(TARGETS)
ifndef	NITRO_FINALROM
			@echo "--- replace argument data" $(NITRO_STDOUT)
			$(BURYARG) -q $(TARGETS)           "-x (C) NINTENDO" 2005
			$(BURYARG) -q $(TARGETS:.srl=.nlf) "-x (C) NINTENDO" 2005
			@echo --- done. try to execute $(TARGET_BIN). $(NITRO_STDOUT)
endif

include	$(NITROSDK_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
