#! make -f
#----------------------------------------------------------------------------
# Project:  NitroWiFi - 
# File:     Makefile
#
# Copyright 2006 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.2  2006/01/12 02:15:49  okubata_ryoma
# ftp.c‚Ì’Ç‰Á
#
# Revision 1.1  2006/01/06 00:24:09  kitase_hirotake
# Initial Upload
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=	

#----------------------------------------------------------------------------

NITRO_CODEGEN_ALL 	?= True
NITRO_PROC		= ARM9

SRCS			=	padkeyboard.c font.c output.c screen.c ftp.c

TARGET_LIB		=	libkeypad$(NITRO_LIBSUFFIX).a

#----------------------------------------------------------------------------


include	$(NITROWIFI_ROOT)/build/buildtools/commondefs
#----------------------------------------------------------------------------

do-build:       $(TARGETS) 

test:

#----------------------------------------------------------------------------

include	$(NITROWIFI_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
