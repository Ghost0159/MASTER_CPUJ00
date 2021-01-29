#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - WM - demos
# File:     Makefile
#
# Copyright 2003-2005 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: makefile,v $
# Revision 1.1  2005/03/25 07:41:37  seiki_masashi
# dataShare-Model Ç… WEP Key ÇÃìÆìIê›íËÇâ¡Ç¶ÅAwep-1 ÉfÉÇÇ∆ÇµÇƒêVãKìoò^
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS         =

#----------------------------------------------------------------------------

INCDIR			=	./include
SRCDIR			=	./src
SRCS            = \
                main.c          \
                print.c         \
		key.c		\
		graphics.c	\
                font.c          \
		menu.c		\
                icon.c

WH_DIR		=  $(ROOT)/build/demos/wireless_shared/wh
SRCDIR		+= $(WH_DIR)
INCDIR		+= $(WH_DIR)
SRCS		+= $(WH_DIR)/wh.c
MACRO_FLAGS	+= -DSHARED_WH

TARGET_BIN      = main.srl

include $(NITROSDK_ROOT)/build/buildtools/commondefs

#----------------------------------------------------------------------------

do-build:       $(TARGETS)

include $(NITROSDK_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
