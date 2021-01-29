#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - tests -fs
# File:     Makefile
#
# Copyright 2003-2005 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.11  2005/06/28 02:12:19  yosizaki
# add file-3.
#
# Revision 1.10  2005/03/08 06:37:16  yosizaki
# add overlay-compressed.
#
# Revision 1.9  2005/01/19 07:45:51  yosizaki
# add overlay-staticinit.
#
# Revision 1.8  2004/09/29 03:10:48  yasu
# Add staticinit-1
#
# Revision 1.7  2004/07/12 13:20:09  yosizaki
# add arc-1, arc-2
#
# Revision 1.6  2004/05/20 09:59:52  yosizaki
# add demo async.
#
# Revision 1.5  2004/05/06 10:18:17  yasu
# fix typo
#
# Revision 1.4  2004/04/02 12:19:59  yosizaki
# add the demo 'overlay'
#
# Revision 1.3  2004/04/01 11:03:50  yosizaki
# add a demo file-2
#
# Revision 1.2  2004/02/05 07:09:02  yasu
# change SDK prefix iris -> nitro
#
# Revision 1.1  2004/01/15 10:50:35  yasu
# test dir for file system
#
# $NoKeywords: $
#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

SUBDIRS		= file-1 file-2 file-3 overlay async arc-1 arc-2 overlay-staticinit overlay-compressed

ifdef	NITRO_WITH_ARM7
SUBDIRS		+= staticinit-1
endif

#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
