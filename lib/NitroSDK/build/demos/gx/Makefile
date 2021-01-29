#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - GX - demos
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
# Revision 1.9  2004/08/27 00:31:55  yasu
# Hide UFO Demo temporarily
#
# Revision 1.8  2004/08/27 00:25:15  yasu
# Add New Sample UFO Demo
#
# Revision 1.7  2004/05/06 10:18:17  yasu
# fix typo
#
# Revision 1.6  2004/04/07 01:24:21  yada
# fix header comment
#
# Revision 1.5  2004/02/05 07:09:02  yasu
# change SDK prefix iris -> nitro
#
# Revision 1.4  2003/12/03 04:01:15  yasu
# _prototype 版のリンクをきる
#
# Revision 1.3  2003/11/30 10:43:49  yasu
# ディレクトリ名変更に伴なう修正
#
# Revision 1.2  2003/11/13 00:09:38  nishida_kenji
# SRDDemoのビルドをするようにした
#
# Revision 1.1  2003/11/04 10:17:51  yasu
# プロトタイプ版 SDK のサンプルコードを現在の SDK で make できるように修正
#
# $NoKeywords: $
#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

SUBDIRS		= UnitTours

ifdef	NITRO_WITH_ARM7
SUBDIRS		+= UFO_Demo
endif

#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
