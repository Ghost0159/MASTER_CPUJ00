#! make -f
#----------------------------------------------------------------------------
# Project:  NitroWiFi - WCM - demos - wcm-list
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
# Revision 1.2  2005/08/29 06:40:09  yosizaki
# change to use ../../shared.
#
# Revision 1.1  2005/07/21 08:21:06  adachi_hiroaki
# êVãKí«â¡
#
#
# $NoKeywords: $
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------

TARGET_BIN	=	main.srl

SRCS		=	main.c			\
				font.c			\
				screen.c

# for WCM control
SHARED_DIR	 = $(NWIFI_ROOT)/build/demos/shared
SRCS		+= $(SHARED_DIR)/wcm_control.c		\
		   $(SHARED_DIR)/ap_info.c
LINCLUDES	+= $(SHARED_DIR)

#----------------------------------------------------------------------------

include	$(NITROWIFI_ROOT)/build/buildtools/commondefs

do-build:	$(TARGETS)

include	$(NITROWIFI_ROOT)/build/buildtools/modulerules

#----------------------------------------------------------------------------

#===== End of Makefile =====
