#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - PAD - demos - padRead
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
# Revision 1.7  2004/07/01 02:25:48  yasu
# .bin -> .srl
#
# Revision 1.6  2004/05/06 10:18:18  yasu
# fix typo
#
# Revision 1.5  2004/04/06 11:29:57  yada
# fix header comment
#
# Revision 1.4  2004/03/08 09:18:47  yada
# fix top comment
#
# Revision 1.3  2004/02/05 07:09:03  yasu
# change SDK prefix iris -> nitro
#
# Revision 1.2  2004/01/18 06:00:22  yada
# ターゲットを axf→bin にした
#
# Revision 1.1  2003/11/14 10:55:19  yada
# 初版
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		=	main.c data.c

TARGET_BIN	=	padRead.srl

#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:		$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
