#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - WVR - demos
# File:     Makefile
#
# Copyright 2003-2005 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.1  2005/01/14 06:25:50  terui
# tests以下からdemos以下に移動。
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

INCDIR		=	../common/include ./include
SRCDIR		=	./src ../common/src 

LLIBRARIES	=	libwvr$(CODEGEN).a

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

#------------------------------------------------------#
# VRAM内に無線ライブラリが配置されるコンポーネント定義 #
MY_COMPONENT_NAME	= ichneumon
MAKEROM_ARM7	= $(NITRO_COMPONENTSDIR)/$(MY_COMPONENT_NAME)/$(NITRO_BUILDTYPE_ARM7)/$(MY_COMPONENT_NAME)_sub$(TS_VERSION).nef
#------------------------------------------------------#

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
