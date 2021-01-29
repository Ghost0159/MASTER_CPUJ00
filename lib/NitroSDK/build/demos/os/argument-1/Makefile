#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - OS - demos - argument-1
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
# Revision 1.7  2005/08/31 05:55:54  yasu
# à¯êîïœçX
#
# Revision 1.6  2005/08/30 08:59:41  yasu
# OS_GetOpt í«â¡
#
# Revision 1.5  2005/07/22 09:16:49  yada
# support nlf file
#
# Revision 1.4  2005/07/22 04:20:05  yada
# fix echo line
#
# Revision 1.3  2005/07/21 12:30:51  yada
# consider for NITRO_FINALROM
#
# Revision 1.2  2005/07/21 11:01:39  yada
# add argument data to srl file
#
# Revision 1.1  2005/07/21 02:32:30  yada
# initial release
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		=	main.c
TARGET_BIN	=	main.srl

#SRCDIR		=	# using default
#LCFILE		=	# using default

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


BURYARG		=	$(ROOT)/tools/bin/buryarg.exe

#----------------------------------------------------------------------------

do-build:		$(TARGETS)
ifndef	NITRO_FINALROM
			@echo "--- replace argument data" $(NITRO_STDOUT)
			$(BURYARG) -q $(TARGETS)           -b test-string -s s-option "(C) NINTENDO" -t t-option 100
			$(BURYARG) -q $(TARGETS:.srl=.nlf) -b test-string -s s-option "(C) NINTENDO" -t t-option 100
			@echo --- done. try to execute $(TARGET_BIN). $(NITRO_STDOUT)
endif

include	$(NITROSDK_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
