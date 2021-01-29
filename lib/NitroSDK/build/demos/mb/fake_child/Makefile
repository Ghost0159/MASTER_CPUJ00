#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - MB - demos - fake_child
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
# Revision 1.1  2004/11/22 12:59:15  takano_makoto
# Initial update.
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=	

#----------------------------------------------------------------------------

INCDIR		=	./common/include
SRCDIR		=	./src ./common/src

SRCS		= main.c font.c disp.c common.c gmain.c

WH_DIR		=	$(ROOT)/build/demos/wireless_shared/wh
SRCDIR		+=	$(WH_DIR)
INCDIR		+=	$(WH_DIR)
SRCS		+=	$(WH_DIR)/wh.c

TARGET_BIN	=	main.srl

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:	$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
