#! make -f
#----------------------------------------------------------------------------
# Project:  NitroDWC - build - demos
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
# $Revision: 1.27 $ 
# $Date: 2007/03/16 10:36:09 $
# $NoKeywords: $
#----------------------------------------------------------------------------

include	$(NITRODWC_ROOT)/build/buildtools/commondefs

#----------------------------------------------------------------------------

SUBDIRS	= dbs dwc_connect_inet_sample dwc_naslogin_sample dwc_nd_sample dwc_prof_sample
#SUBDIRS	= dbs dwc_connect_inet_sample dwc_naslogin_sample dwc_svl_sample dwc_nd_sample

ifndef NITRODWC_NOGS
SUBDIRS	+= \
	dwc_userdata_sample \
	dwc_frienddata_sample \
	dwc_anybody_match_sample \
	dwc_friend_match_sample \
	dwc_eval_callback_sample \
	dwc_sc_match_sample \
	ghttp_sample \
	dwc_sample \
	friend_match \
	rank_sample \
	lobby_sample
endif

-include @privates

#----------------------------------------------------------------------------

include	$(NITRODWC_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====
