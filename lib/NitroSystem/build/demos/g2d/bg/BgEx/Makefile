#! make -f
#----------------------------------------------------------------------------
# Project:  NITRO-System - demos - g2d - bg - BgEx
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
# $Revision: 1.2 $
#----------------------------------------------------------------------------

SRCS		=	main.c loader.c
TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

MAKEROM_ROMROOT  = ./
MAKEROM_ROMFILES = data/*.*


G2D_TEST_RES_SRC			= $(wildcard data/src/Screen/*.nsc)
G2D_TEST_RES_SRC_NAMES		= $(notdir $(G2D_TEST_RES_SRC))
G2D_TEST_RES_TARGET			= $(addprefix data/,$(G2D_TEST_RES_SRC_NAMES:.nsc=.NSCR))
G2D_TEST_CVTR_OUTDIR 		= data

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

# 通常の変換
data/BG_normal.NSCR: data/src/Screen/BG_normal.nsc data/src/Character/BG_normal.ncg  data/src/ColorPalette/BG_normal.ncl
	$(NITROSYSTEM_ROOT)/tools/win/bin/g2dcvtr.exe $< -o/$(G2D_TEST_CVTR_OUTDIR)

# パレット未使用部分を含まない
data/BG_cmp.NSCR: data/src/Screen/BG_cmp.nsc data/src/Character/BG_cmp.ncg  data/src/ColorPalette/BG_cmp.ncl
	$(NITROSYSTEM_ROOT)/tools/win/bin/g2dcvtr.exe $< -o/$(G2D_TEST_CVTR_OUTDIR) -pcm

# キャラクタから矩形 (0, 12)-(31, 31) を抽出
data/BG_part.NSCR: data/src/Screen/BG_part.nsc data/src/Character/BG_part.ncg  data/src/ColorPalette/BG_part.ncl
	$(NITROSYSTEM_ROOT)/tools/win/bin/g2dcvtr.exe $< -o/$(G2D_TEST_CVTR_OUTDIR) -cr/0_12_32_20

# パレット未使用部分を含まない
# キャラクタから矩形 (0, 0)-(31, 11) を抽出
data/BG_cmp_part.NSCR: data/src/Screen/BG_cmp_part.nsc data/src/Character/BG_cmp_part.ncg  data/src/ColorPalette/BG_cmp_part.ncl
	$(NITROSYSTEM_ROOT)/tools/win/bin/g2dcvtr.exe $< -o/$(G2D_TEST_CVTR_OUTDIR) -pcm -cr/0_0_32_12


reconv: $(G2D_TEST_RES_TARGET)

do-build: reconv $(TARGETS)

include	$(NITROSYSTEM_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
