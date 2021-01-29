#! make -f
#----------------------------------------------------------------------------
# Project:  NITRO-System - demos - g2d - Text
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

include	$(NITROSYSTEM_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

SUBDIRS	= 				\
	textdemolib			\
						\
	DoubleBuffering		\
	BGText				\
	BGAffine			\
						\
	OBJ1D				\
	OBJ2DRect			\
	CanvasCell			\
						\
	ShiftJISText		\
	UTF16Text			\
	UTF8Text			\
	CP1252Text			\
						\
	DrawLetter			\
	DrawLetters			\
	DrawTaggedText		\
						\
	MinimumCanvas		\
	SimpleFontView		\
						\
	PortraitHW			\
	PortraitSW			\

ifneq	($(shell find `cygpath -W`/Fonts/ -iname msgothic.ttc),)
SUBDIRS += DrawVertical
endif

#----------------------------------------------------------------------------

include	$(NITROSYSTEM_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
