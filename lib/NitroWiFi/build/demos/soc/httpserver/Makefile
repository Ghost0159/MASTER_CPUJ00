#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - soc -
# File:     Makefile
#
# Copyright 2006 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.1  2006/01/06 00:42:51  kitase_hirotake
# httpserver ‚Ì’Ç‰Á
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

SRCS		=	main.c server_thread.c
TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

NITRO_MAKEROM	=	TRUE
ROM_SPEC	=	main.rsf

#LCFILE_SPEC	=	# using default
#SRCDIR		=	# using default


#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/commondefs
include	$(NITROWIFI_ROOT)/build/buildtools/commondefs
include	$(NITROWIFI_ROOT)/build/demos/soc/netconnect/commondefs.add-ins.netconnect

do-build:		$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules
include	$(NITROWIFI_ROOT)/build/buildtools/modulerules


ifdef	NITRO_ELFTOBIN
$(TARGETS): $(ELFTOBIN_ARM7)

else	#NITRO_ELFTOBIN
$(TARGETS): $(MAKEROM_ARM7)

endif	#NITRO_ELFTOBIN


#===== End of Makefile =====
