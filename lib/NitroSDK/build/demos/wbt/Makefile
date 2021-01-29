#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - WBT - demos
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
# Revision 1.3  2004/11/09 00:04:00  yosizaki
# ignore building wbt-fs in TEG. (avoid link error)
#
# Revision 1.2  2004/09/13 07:11:26  yosizaki
# add wbt-fs
#
# Revision 1.1  2004/08/23 09:47:09  yosizaki
# (none)
#
# $NoKeywords: $
#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

SUBDIRS =	wbt-1

ifneq		($(NITRO_PLATFORM),TEG)
SUBDIRS +=	wbt-fs
endif


#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
