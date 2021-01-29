#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - demos - PRC
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
# Revision 1.2  2005/10/04 03:55:14  seiki_masashi
# patterns.c が include するファイル名の修正
#
# Revision 1.1  2005/10/04 02:55:39  seiki_masashi
# 新規追加
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

SRCS		=
PATTERN_SOURCE = src/patterns.c
PATTERN_HEADER = include/patterns.h
PATTERN_TARGETS	= $(PATTERN_SOURCE) $(PATTERN_HEADER)
PATTERNS	= data/patterns.txt
LNEWDIRS	= ./src ./include
#LDIRT_CLEAN	= $(LNEWDIRS)
NITRO_FORCE_BUILD = 1

#----------------------------------------------------------------------------
include	$(NITROSDK_ROOT)/build/buildtools/commondefs

do-build:		$(PATTERN_TARGETS)

pattern:		$(PATTERN_TARGETS)

$(PATTERN_SOURCE): $(PATTERNS)
	$(PERL) $(NITROSDK_ROOT)/tools/bin/pdic2c.pl -c -C $@ -H $(PATTERN_HEADER) 64 $<

$(PATTERN_HEADER): $(PATTERNS)
	$(PERL) $(NITROSDK_ROOT)/tools/bin/pdic2c.pl -h -H $@ 64 $<

include	$(NITROSDK_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
