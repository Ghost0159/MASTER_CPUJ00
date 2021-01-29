#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - MB - demos - multiboot-wfs
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
# Revision 1.1  2005/06/23 09:06:03  yosizaki
# initial upload.
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------


include	../make_common


###############################################################################
# 親機固有の設定.

SRCS			+=	main.c parent.c multiboot.c

# このサンプルは MBP モジュールを使用しています.
MBP_DIR			=	$(ROOT)/build/demos/wireless_shared/mbp
LINCLUDES		+=	$(MBP_DIR)
SRCS			+=	$(MBP_DIR)/mbp.c

MAKEROM_ROMROOT		=	.
MAKEROM_ROMFILES	=	./data


include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====

