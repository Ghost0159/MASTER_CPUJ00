#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - SND - demos - channel
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
# Revision 1.2  2005/04/13 14:10:25  yosizaki
# fix NITROSYSTEM_ROOT -> NITROSDK_ROOT
#
# Revision 1.1  2005/04/13 03:29:43  ida
# SNDをNITRO-SDKへ移設
#
# Revision 1.3  2005/04/13 01:15:26  kyuma_koichi
# 波形ファイルフォーマットの変更
#
# Revision 1.2  2005/03/08 07:45:24  kyuma_koichi
# 初版
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		=	main.c organ_det.g3.pcm16.c wihaho.pcm16.c
LINCLUDES	=	./include

TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl


#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
