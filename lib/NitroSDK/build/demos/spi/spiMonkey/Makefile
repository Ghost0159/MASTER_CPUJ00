#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - SPI - demos
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
# Revision 1.3  2004/07/31 08:16:22  terui
# Change to use default component.
#
# Revision 1.2  2004/07/01 02:25:48  yasu
# .bin -> .srl
#
# Revision 1.1  2004/06/02 09:52:45  terui
# Initial upload.
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

SRCS		=	main.c monkey.c snd_data.c

TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

LINCLUDES	=	./include

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
