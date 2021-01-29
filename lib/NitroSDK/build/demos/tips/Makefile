#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - tips
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
# Revision 1.9  2005/07/29 00:50:54  seiki_masashi
# copyright ÇÃèCê≥
#
# Revision 1.8  2005/07/27 05:11:49  yasu
# make clean/clobber ÇÃëŒâû
#
# Revision 1.7  2004/12/27 04:48:00  yasu
# Add MyBuildMessages
#
# Revision 1.6  2004/09/21 02:34:58  yasu
# Add BuildFinalRom
#
# Revision 1.5  2004/08/10 09:06:35  yasu
# Hide ShowWarning-1
#
# Revision 1.4  2004/08/09 09:39:57  yasu
# Add ShowWarning-1
#
# Revision 1.3  2004/06/25 11:42:38  yasu
# add MultiLink
#
# Revision 1.2  2004/05/06 10:18:18  yasu
# fix typo
#
# Revision 1.1  2004/03/31 13:56:29  takano_makoto
# First Upload
#
# $NoKeywords: $
#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

SUBDIRS 	= SwapCrossOver MultiLink MyBuildMessages

# if (FINALROM or clean or clobber), go into BuildFinalRom
ifneq		(,$(NITRO_FINALROM)$(findstring $(MAKECMDGOALS),clean clobber))
SUBDIRS		+= BuildFinalRom
endif

ifdef	NITRO_WITH_ARM7
SUBDIRS 	+= ShowWarning-1
endif

#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
