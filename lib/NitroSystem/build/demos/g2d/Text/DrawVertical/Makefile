#! make -f
#----------------------------------------------------------------------------
# Project:  NITRO-System - demos - g2d - Text - DrawVertical
# File:     Makefile
#
# Copyright 2004-2007 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Revision: 1.3 $
#----------------------------------------------------------------------------

G2D_TEXTDEMOLIB	=	../textdemolib

LINCLUDES       =   $(G2D_TEXTDEMOLIB)/include
LLIBRARY_DIRS   =   $(G2D_TEXTDEMOLIB)/lib/$(NITRO_BUILDTYPE)
LLIBRARIES      =   libg2d_textdemo.a

#----------------------------------------------------------------------------

SRCS		=	main.c
TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

MAKEROM_ROMROOT  = ../
MAKEROM_ROMFILES = ./data/*.*

#----------------------------------------------------------------------------

FCNV_DIR	= ../../../../../tools/win/fontcvtr
FONTCVTR	= $(FCNV_DIR)/fontcvtrc.exe
FONT_SRC	= ../data/src
FONT_OUT	= ../data
FONT_XLOR	= $(FONT_SRC)/demofont2.xlor
FONT_BPP	= 4
FONT_WIN_H	= "‚l‚r ‚oƒSƒVƒbƒN"
FONT_WIN_V	= "@‚l‚r ‚oƒSƒVƒbƒN"
FONT_SIZE	= 12

FONT_H0		= h_0.NFTR
FONT_H90	= h_90.NFTR
FONT_H180	= h_180.NFTR
FONT_H270	= h_270.NFTR

FONT_V0		= v_0.NFTR
FONT_V90	= v_90.NFTR
FONT_V180	= v_180.NFTR
FONT_V270	= v_270.NFTR

FONT_BIN	= $(FONT_H0) $(FONT_H90) $(FONT_H180) $(FONT_H270) \
			  $(FONT_V0) $(FONT_V90) $(FONT_V180) $(FONT_V270)

FONT_TARGET = $(addprefix $(FONT_OUT)/,$(FONT_BIN))

LDEPENDS_NEF += $(FONT_TARGET)


FONT_TTC	= $(shell find `cygpath -W`/Fonts/ -iname msgothic.ttc)

ifeq	($(FONT_TTC),)
ENV_CHK		= env-error
else
ENV_CHK		= $(FONT_TTC)
endif

#----------------------------------------------------------------------------
include	$(NITROSYSTEM_ROOT)/build/buildtools/commondefs

.PHONY:		env-error
.PRECIOUS:	$(FONT_OUT)/%.bmp

do-build:		$(TARGETS) $(FONT_TARGET)

env-error:
	-echo Sorry. This demo requires japanese Windows environment.
	false

# win -> bmp for h
$(FONT_OUT)/h.bmp: $(FONT_XLOR) $(FONTCVTR) Makefile $(ENV_CHK)
	echo $(FONT_WIN_H) -\> $@
	$(FONTCVTR) -i win -in $(FONT_WIN_H) -is $(FONT_SIZE) -ib $(FONT_BPP)	\
	            -o bmp -of $@ -oo $(FONT_XLOR)

# win -> bmp for v
$(FONT_OUT)/v.bmp: $(FONT_XLOR) $(FONTCVTR) Makefile $(ENV_CHK)
	echo $(FONT_WIN_V) -\> $@
	$(FONTCVTR) -i win -in $(FONT_WIN_V) -is $(FONT_SIZE) -ib $(FONT_BPP)	\
	            -o bmp -of $@ -oo $(FONT_XLOR)

# bmp -> nftr for h
$(FONT_OUT)/h_%.NFTR:	$(FONT_OUT)/h.bmp $(FONT_XLOR) $(FONTCVTR) Makefile $(ENV_CHK)
	echo $< -\> $@
	$(FONTCVTR) -i bmp -if $< -io $(FONT_XLOR) -ib $(FONT_BPP) \
	            -o nitro -of $@ -oe sjis -oc $*

# bmp -> nftr for v
$(FONT_OUT)/v_%.NFTR:	$(FONT_OUT)/v.bmp $(FONT_XLOR) $(FONTCVTR) Makefile $(ENV_CHK)
	echo $< -\> $@
	$(FONTCVTR) -i bmp -if $< -io $(FONT_XLOR) -ib $(FONT_BPP) \
	            -o nitro -of $@ -oe sjis -oc $*


include	$(NITROSYSTEM_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
