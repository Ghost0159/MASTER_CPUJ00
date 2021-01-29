#! make -f
#----------------------------------------------------------------------------
# Project:  NITRO-System - demos - g2d - Text - data
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

FCNV_DIR	= ../../../../../../tools/win/fontcvtr
FONTCVTR	= $(FCNV_DIR)/fontcvtrc.exe
FONT_SRC	= .
FONT_OUT	= ..
FONT_XLOR	= $(FONT_SRC)/demofont.xlor
XLLT_1252	= $(FONT_SRC)/demo_cp1252.xllt
FONT_BPP	= 1
FONT_BMP_P	= $(FONT_SRC)/fontp.bmp
FONT_BMP_M	= $(FONT_SRC)/fontm.bmp

FONT_D		= fontd.NFTR
FONT_1252	= font1252.NFTR
FONT_S		= fonts.NFTR
FONT_U16	= fontu16.NFTR
FONT_U8		= fontu8.NFTR

FONT_U8_90	= fontu8_90.NFTR
FONT_U8_180	= fontu8_180.NFTR
FONT_U8_270	= fontu8_270.NFTR


FONT_BIN	= $(FONT_D) $(FONT_1252) $(FONT_S) $(FONT_U16) \
			  $(FONT_U8) $(FONT_U8_90) $(FONT_U8_180) $(FONT_U8_270)

FONT_TARGET = $(addprefix $(FONT_OUT)/,$(FONT_BIN))

#----------------------------------------------------------------------------
include	$(NITROSYSTEM_ROOT)/build/buildtools/commondefs

do-build:		$(TARGETS) $(FONT_TARGET)


$(FONT_OUT)/$(FONT_D):	$(FONT_BMP_M) $(FONT_XLOR) $(FONTCVTR) Makefile
	echo $< -\> $@
	$(FONTCVTR) -i bmp -if $< -io $(FONT_XLOR) -ib $(FONT_BPP) \
	            -o nitro -of $@ -oe utf8

$(FONT_OUT)/$(FONT_1252):	$(FONT_BMP_P) $(FONT_XLOR) $(XLLT_1252) $(FONTCVTR) Makefile
	echo $< -\> $@
	$(FONTCVTR) -i bmp -if $< -io $(FONT_XLOR) -ib $(FONT_BPP) \
	            -o nitro -of $@ -oe cp1252                     \
	            -f $(XLLT_1252)

$(FONT_OUT)/$(FONT_S):	$(FONT_BMP_P) $(FONT_XLOR) $(FONTCVTR) Makefile
	echo $< -\> $@
	$(FONTCVTR) -i bmp -if $< -io $(FONT_XLOR) -ib $(FONT_BPP) \
	            -o nitro -of $@ -oe sjis

$(FONT_OUT)/$(FONT_U16):	$(FONT_BMP_P) $(FONT_XLOR) $(FONTCVTR) Makefile
	echo $< -\> $@
	$(FONTCVTR) -i bmp -if $< -io $(FONT_XLOR) -ib $(FONT_BPP) \
	            -o nitro -of $@ -oe utf16

$(FONT_OUT)/$(FONT_U8):	$(FONT_BMP_P) $(FONT_XLOR) $(FONTCVTR) Makefile
	echo $< -\> $@
	$(FONTCVTR) -i bmp -if $< -io $(FONT_XLOR) -ib $(FONT_BPP) \
	            -o nitro -of $@ -oe utf8

$(FONT_OUT)/$(FONT_U8_90):	$(FONT_BMP_P) $(FONT_XLOR) $(FONTCVTR) Makefile
	echo $< -\> $@
	$(FONTCVTR) -i bmp -if $< -io $(FONT_XLOR) -ib $(FONT_BPP) \
	            -o nitro -of $@ -oe utf8 -oc 90

$(FONT_OUT)/$(FONT_U8_180):	$(FONT_BMP_P) $(FONT_XLOR) $(FONTCVTR) Makefile
	echo $< -\> $@
	$(FONTCVTR) -i bmp -if $< -io $(FONT_XLOR) -ib $(FONT_BPP) \
	            -o nitro -of $@ -oe utf8 -oc 180

$(FONT_OUT)/$(FONT_U8_270):	$(FONT_BMP_P) $(FONT_XLOR) $(FONTCVTR) Makefile
	echo $< -\> $@
	$(FONTCVTR) -i bmp -if $< -io $(FONT_XLOR) -ib $(FONT_BPP) \
	            -o nitro -of $@ -oe utf8 -oc 270


include	$(NITROSYSTEM_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
