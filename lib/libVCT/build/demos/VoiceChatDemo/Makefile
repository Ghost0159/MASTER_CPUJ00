#! make -f
#----------------------------------------------------------------------------
# Project:  libVCT - build
# File:     Makefile
#
# Copyright (c) 2005-2006 Abiosso Networks, Inc. ALL RIGHTS RESERVED.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Abiosso Networks, Inc., and are protected
# by Federal copyright law.  They may not be disclosed to third parties
# or copied or duplicated in any form, in whole or in part,  without
# the prior written consent of Abiosso.
#
#----------------------------------------------------------------------------

SUBDIRS		=

#----------------------------------------------------------------------------

SRCDIR  = src

SRCS    = \
	gamemain.c \
	logon.c \
	main.c \
	registerfriend.c \
	sound.c \
	transport.c \
	debug_menu.cpp \
	demo_util.cpp \
	thread_statistics.c \
	phone.cpp \
	transceiver.cpp \
	conference.cpp \
	font.c \
	init.c \
	mp.c \
	screen.c \
	utility.c \
	wc.c \
	tp_input.c \
	demo_menu.cpp \
	vct_menu.cpp

EXT_OBJS           += ./data/sound_data.o

TARGET_BIN	=	main.srl

LDEPENDS_NEF = $(NITRODWC_LIB_DEPENDS)

NITRODWC_LIB_MODULES_   = base auth ac bm util account
NITRODWC_LIB_GSMODULES_ = common gp gstats gt2 natneg qr2 serverbrowsing

LINCLUDES	= ./include $(NITRODWC_ROOT)/build/demos/dbs/include
LLIBRARIES	= libdwcdbs$(NITRO_LIBSUFFIX).a
LLIBRARY_DIRS	= $(NITRODWC_ROOT)/build/demos/dbs/lib/$(NITRO_BUILDTYPE)

CCFLAGS_MSGSTYLE = -msgstyle gcc

ifeq ($(PR_USERDATA), TRUE)
MACRO_FLAGS	+=	-DUSE_PRIVATE_USERDATA
endif

ifdef NITRO_PROFILE
NITRO_PROFILE_TYPE  = FUNCTIONCOST
SRCS +=	cost.c
endif

#----------------------------------------------------------------------------
include	$(NITROLIBVCT_ROOT)/build/buildtools/commondefs

do-build:		$(TARGETS)

include	$(NITROLIBVCT_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
