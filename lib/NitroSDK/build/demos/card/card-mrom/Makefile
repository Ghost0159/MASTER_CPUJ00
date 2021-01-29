#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - CARD - demos - card-mrom
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
# Revision 1.1  2005/04/05 11:44:37  yosizaki
# initial upload.
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=	


#----------------------------------------------------------------------------


SRCS		=	main.c

TARGET_BIN	=	main.srl

#SRCDIR		=	# using default
#LCFILE		=	# using default


# NITRO_ROMSPEED を明示的に指定するとデフォルトの .rsf ファイルへ反映されます.
# 独自に用意した .rsf ファイルがある場合は, Property  セクション内に
#  RomSpeedType  {1TROM|MROM}
# と記述します.
NITRO_ROMSPEED	= MROM



include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
