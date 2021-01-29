#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - FX - demos - test
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
# Revision 1.10  2004/07/01 02:25:30  yasu
# .bin -> .srl
#
# Revision 1.9  2004/05/06 10:18:17  yasu
# fix typo
#
# Revision 1.8  2004/04/07 01:27:57  yada
# fix header comment
#
# Revision 1.7  2004/02/05 07:09:02  yasu
# change SDK prefix iris -> nitro
#
# Revision 1.6  2004/01/29 01:35:09  kitani_toshikazu
# Fix some bugs for debug build, restore appended test cases.
#
# Revision 1.5  2004/01/27 04:41:47  kitani_toshikazu
# Removed new tests for a while
#
# Revision 1.4  2004/01/27 01:35:40  kitani_toshikazu
# Appended some test cases
#
# Revision 1.3  2004/01/16 10:41:45  nishida_kenji
# tests the sin/cos table
#
# Revision 1.2  2003/12/18 09:49:29  nishida_kenji
# revise comments
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS     =

#----------------------------------------------------------------------------

SRCS        =   main.c table.c fx_test_common.c fx_append_vec.c fx_append_mtx.c 


TARGET_BIN  =   main.srl
LINCLUDES   =   ../DEMOLib/include
LLIBRARY_DIRS   =   ../DEMOLib/lib/$(NITRO_BUILDTYPE)
LLIBRARIES  =   

#SRCDIR     =   # using default
#LCFILE     =   # using default

include $(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:       $(TARGETS)


include $(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
