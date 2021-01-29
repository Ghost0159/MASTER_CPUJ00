#! make -f
#----------------------------------------------------------------------------
# Project:  NitroWiFi - 
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
# Revision 1.1  2005/12/22 04:32:58  adachi_hiroaki
# ä»à’é©ìÆê⁄ë±ÉâÉCÉuÉâÉäÇÉfÉÇÇ∆ÇµÇƒí«â¡
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=	

#----------------------------------------------------------------------------

NITRO_CODEGEN_ALL 	?= True
NITRO_PROC		= ARM9

SRCS			=	util.c padkeyboard.c font.c output.c screen.c

# for WCM control
SHARED_DIR	 = $(NWIFI_ROOT)/build/demos/shared
SRCS		+= $(SHARED_DIR)/wcm_control.c
LINCLUDES	+= $(SHARED_DIR)


TARGET_LIB		=	libinet$(NITRO_LIBSUFFIX).a

#----------------------------------------------------------------------------


include	$(NITROWIFI_ROOT)/build/buildtools/commondefs
include	$(NITROWIFI_ROOT)/build/demos/SiteDefs/commondefs.add-ins.sitedefs

#----------------------------------------------------------------------------

do-build:       $(TARGETS) 

test:

#----------------------------------------------------------------------------

include	$(NITROWIFI_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
