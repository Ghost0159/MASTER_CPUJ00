#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - FS - demos - overlay-compressed
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
# Revision 1.1  2005/03/09 00:38:48  yosizaki
# initial upload.
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------

NITRO_COMPRESS	=	TRUE

REF_DIR		=	$(ROOT)/build/demos/fs/overlay
LINCLUDES	=	$(REF_DIR)/include
SRCDIR		=	../overlay/src

SRCS		=	main.c
SRCS_OVERLAY	=	func_1.c func_2.c func_3.c

TARGET_NEF	=	main.nef
TARGET_BIN	=	main.srl

NITRO_MAKEROM	=	TRUE
ROM_SPEC	=	$(REF_DIR)/main.rsf
LCFILE_SPEC	=	$(REF_DIR)/main.lsf

ifeq	($(NITRO_PLATFORM),TEG)
MAKELCF_FLAGS	+=	-DADDRESS_MAIN=0x02004000
else
MAKELCF_FLAGS	+=	-DADDRESS_MAIN=0x02000000
endif

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

do-build:	$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules

ifdef	NITRO_ELFTOBIN
$(TARGETS): $(ELFTOBIN_ARM7)

else	#NITRO_ELFTOBIN
$(TARGETS): $(MAKEROM_ARM7)

endif	#NITRO_ELFTOBIN

#===== End of Makefile =====
