#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - FS - demos - overlay-staticinit
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
# Revision 1.1  2005/01/19 07:44:35  yosizaki
# initial update.
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=	


#----------------------------------------------------------------------------


# メインのアプリケーションフレームワーク
SRCS			=	main.c data.c draw.c

# オーバーレイとして構成された各モード
SRCS_OVERLAY	=	top_menu.c mode_1.c mode_2.c mode_3.c

NITRO_MAKEROM	=	TRUE
TARGET_BIN	=	main.srl
LCFILE_SPEC	=	main.lsf

ifeq	($(NITRO_PLATFORM),TEG)
MAKELCF_FLAGS	+=	-DADDRESS_MAIN=0x02004000
else
MAKELCF_FLAGS	+=	-DADDRESS_MAIN=0x02000000
endif

#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
