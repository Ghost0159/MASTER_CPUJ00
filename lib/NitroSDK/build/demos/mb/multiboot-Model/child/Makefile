#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - MB - demos
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
# Revision 1.4  2004/10/28 01:27:43  takano_makoto
# wmhighをwireless_sharedのwhへ移行
#
# Revision 1.3  2004/10/21 00:42:52  yosizaki
# add SHARED_WH switch.
#
# Revision 1.2  2004/09/21 06:47:39  takano_makoto
# gmain.cを追加
#
# Revision 1.1  2004/09/16 14:13:05  takano_makoto
# Initial Update
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

INCDIR		=	../common/include ./include
SRCDIR		=	./src ../common/src 

SRCS		= \
		main.c		\
		font.c		\
		disp.c		\
		common.c	\
		gmain.c

SHARED_WH	:= TRUE

ifdef	SHARED_WH
# WH モジュール は wireless_shared へ移動しました.

WH_DIR		=	$(ROOT)/build/demos/wireless_shared/wh
SRCDIR		+=	$(WH_DIR)
INCDIR		+=	$(WH_DIR)
SRCS		+=	$(WH_DIR)/wh.c
MACRO_FLAGS	+= -DSHARED_WH

else
# 従来のソースコードもまだ残してあります.

SRCS		+=	wmhigh.c

endif	#SHARED_WH


TARGET_BIN	=	child.srl

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#-------------------------------#
#   install target		#
#-------------------------------#
INSTALL_TARGETS	=	$(TARGETS)

INSTALL_DIR	= ../data


#----------------------------------------------------------------------------

do-build:		$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules



#===== End of Makefile =====
