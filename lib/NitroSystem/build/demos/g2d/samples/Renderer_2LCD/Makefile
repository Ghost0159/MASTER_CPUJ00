#! make -f
#----------------------------------------------------------------------------
# Project:  NITRO-System - demos - g2d - samples - Renderer_2LCD
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
# $Revision: 1.5 $
#----------------------------------------------------------------------------

G2D_DEMOLIB		=	$(NITROSYSTEM_ROOT)/build/demos/g2d/demolib

LINCLUDES       =   $(G2D_DEMOLIB)/include
LLIBRARY_DIRS   =   $(G2D_DEMOLIB)/lib/$(NITRO_BUILDTYPE)
LLIBRARIES      =   libg2d_demo.a

#----------------------------------------------------------------------------

SRCS		=	main.c
TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

MAKEROM_ROMROOT  = ./
MAKEROM_ROMFILES = data/*.*

#
#再コンバートを行うリソース名
# G2D_TEST_CVTR_SRCDIR ルートとして指定する
#
G2D_TEST_RES_SRC = Renderer_2LCD.nce

G2D_TEST_CVTR_OUTDIR = data/
G2D_TEST_CVTR_SRCDIR = data/src/

G2D_TEST_RES_SRC := $(G2D_TEST_CVTR_SRCDIR)$(G2D_TEST_RES_SRC)


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

reconv: $(G2D_TEST_RES_SRC)
	$(NITROSYSTEM_ROOT)/tools/win/bin/g2dcvtr.exe $(G2D_TEST_RES_SRC) -o/$(G2D_TEST_CVTR_OUTDIR) $(G2D_TEST_CVTR_OPTS)
	$(NITROSYSTEM_ROOT)/tools/win/bin/g2dcvtr.exe $(G2D_TEST_RES_SRC) -o/$(G2D_TEST_CVTR_OUTDIR) -bmp $(G2D_TEST_CVTR_OPTS)

do-build: reconv $(TARGETS)




include	$(NITROSYSTEM_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
