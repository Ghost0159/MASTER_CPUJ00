#! make -f
#----------------------------------------------------------------------------
# Project:  NITRO-System - demos - g2d - bg - ShareBg
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
# $Revision: 1.7 $
#----------------------------------------------------------------------------

SRCS		=	main.c loader.c
TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

MAKEROM_ROMROOT  = ./
MAKEROM_ROMFILES = data/*.*

G2D_TEST_RES_SRC = $(wildcard data/src/Screen/*.*)
G2D_TEST_CVTR_OUTDIR = data

G2D_TEST_RES_NSC = $(G2D_TEST_RES_SRC)
G2D_TEST_RES_NSCR = $(G2D_TEST_RES_NSC:.nsc=.NSCR)


G2D_TEST_RES_TARGET = $(subst data/src/Screen/,data/,$(G2D_TEST_RES_NSCR))

BINRES_FILES = $(G2D_TEST_CVTR_OUTDIR)/*.NCER 
BINRES_FILES := $(BINRES_FILES) $(G2D_TEST_CVTR_OUTDIR)/*.NMCR 
BINRES_FILES := $(BINRES_FILES) $(G2D_TEST_CVTR_OUTDIR)/*.NCGR 
BINRES_FILES := $(BINRES_FILES) $(G2D_TEST_CVTR_OUTDIR)/*.NCBR 
BINRES_FILES := $(BINRES_FILES) $(G2D_TEST_CVTR_OUTDIR)/*.NCLR 
BINRES_FILES := $(BINRES_FILES) $(G2D_TEST_CVTR_OUTDIR)/*.NANR 
BINRES_FILES := $(BINRES_FILES) $(G2D_TEST_CVTR_OUTDIR)/*.NMAR 
BINRES_FILES := $(BINRES_FILES) $(G2D_TEST_CVTR_OUTDIR)/*.NSCR

LDIRT_CLEAN = $(BINRES_FILES)

#----------------------------------------------------------------------------
include	$(NITROSYSTEM_ROOT)/build/buildtools/commondefs

data/%.NSCR: data/src/Screen/%.nsc
	$(NITROSYSTEM_ROOT)/tools/win/bin/g2dcvtr.exe $< -o/$(G2D_TEST_CVTR_OUTDIR)

reconv: $(G2D_TEST_RES_TARGET)

do-build: reconv $(TARGETS)

include	$(NITROSYSTEM_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
