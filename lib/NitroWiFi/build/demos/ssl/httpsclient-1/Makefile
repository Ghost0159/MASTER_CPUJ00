#! make -f
#----------------------------------------------------------------------------
# Project:  NitroWiFi - SSL - demos - httpsclient-1
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
# Revision 1.2  2005/11/02 04:13:42  terui
# Fix comment.
#
# Revision 1.1  2005/08/06 14:27:49  seiki_masashi
# httpsclient-1 ÉfÉÇÇÃí«â¡
#
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

TARGET_BIN	= main.srl
SRCS		= main.c

# for WCM control
SHARED_DIR	 = $(NWIFI_ROOT)/build/demos/shared
SRCS		+= $(SHARED_DIR)/wcm_control.c		\
		   $(SHARED_DIR)/ap_info.c
LINCLUDES	+= $(SHARED_DIR)

#----------------------------------------------------------------------------

include	$(NITROWIFI_ROOT)/build/buildtools/commondefs

#
# NITROWIFI_NOSUPPORT ÇÃílÇéQè∆Ç∑ÇÈÇÃÇ≈à»â∫ÇÃï∂ÇÕ commondefs ì«Ç›çûÇ›ÇÃ
# å„Ç…Ç®Ç≠Ç±Ç∆
#
ifdef NITROWIFI_NOSUPPORT
SRCS		+= $(SHARED_DIR)/_ap_info_custom.c
endif

do-build:		$(TARGETS)

include	$(NITROWIFI_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
