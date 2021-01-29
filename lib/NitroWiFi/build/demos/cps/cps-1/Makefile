#! make -f
#----------------------------------------------------------------------------
# Project:  NitroWiFi - CPS - demos
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
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

SRCS		= main.c httpd.c httpsd.c nbns.c telnet.c httpc.c ping.c \
		  font.c screen.c \
		  ca_jeys.c ca_mac.c ca_nintendo.c ca_rsa.c ca_rsaclass3.c \
		  mycert.c mykey.c

TARGET_BIN	= main.srl

SHARED_DIR	= $(NWIFI_ROOT)/build/demos/shared

SRCS		+= $(SHARED_DIR)/wcm_control.c
SRCS		+= $(SHARED_DIR)/ap_info.c
LINCLUDES	+= $(SHARED_DIR)

#----------------------------------------------------------------------------

include	$(NITROWIFI_ROOT)/build/buildtools/commondefs

ifdef NITROWIFI_NOSUPPORT
SRCS		+= $(SHARED_DIR)/_ap_info_custom.c
endif

do-build:	$(TARGETS)

include	$(NITROWIFI_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
