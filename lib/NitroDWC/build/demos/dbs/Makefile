#! make -f
#----------------------------------------------------------------------------
# Project:  NitroDWC - demos - debug string
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
# Revision 1.1  2005/09/03 07:13:00  sasakit
# Add the Debug Console library dbs for samples.
#
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS     =

#----------------------------------------------------------------------------

SRCS    = dbs_print.c dbs_console.c dbs_demo.c

TARGET_LIB	= libdwcdbs$(NITRO_LIBSUFFIX).a

CCFLAGS_MSGSTYLE = -msgstyle gcc

include $(NITROSDK_ROOT)/build/buildtools/commondefs

#----------------------------------------------------------------------------

do-build:       $(TARGETS)

include $(NITROSDK_ROOT)/build/buildtools/modulerules
