#! make -f
#----------------------------------------------------------------------------
# Project:  NitroWiFi - SOC - demos - dhcp_monkey
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
# Revision 1.1  2005/12/22 04:28:44  adachi_hiroaki
# dhcp_monkey‚ÌSOC”Å
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

TARGET_BIN	= dhcp_monkey.srl
SRCS		= dhcp_monkey.c

# for WCM control
SHARED_DIR	 = $(NWIFI_ROOT)/build/demos/shared
SRCS		+= $(SHARED_DIR)/wcm_control.c
LINCLUDES	+= $(SHARED_DIR) 

#----------------------------------------------------------------------------
include	$(NITROWIFI_ROOT)/build/buildtools/commondefs
include	$(NITROWIFI_ROOT)/build/demos/SiteDefs/commondefs.add-ins.sitedefs

do-build:		$(TARGETS)

include	$(NITROWIFI_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
