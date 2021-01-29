#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - demos
# File:     Makefile
#
# Copyright 2003-2006 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.52  2006/07/05 07:44:46  kitase_hirotake
# std に sjis <-> unicode 変換ライブラリの追加
#
# Revision 1.51  2006/03/28 04:42:01  okubata_ryoma
# undo
#
# Revision 1.50  2006/03/27 11:42:34  okubata_ryoma
# test
#
# Revision 1.49  2006/03/02 10:04:43  okubata_ryoma
# copyright修正
#
# Revision 1.48  2006/03/02 10:03:34  okubata_ryoma
# vibを追加
#
# Revision 1.47  2005/09/05 01:22:46  yada
# add env
#
# Revision 1.46  2005/07/07 07:45:30  yasu
# utest をベータ扱いにする
#
# Revision 1.45  2005/06/24 02:18:20  yasu
# utest 追加
#
# Revision 1.44  2005/04/13 13:59:59  terui
# SUBDIRSにsndが多重登録されていた件を修正。
#
# Revision 1.43  2005/04/13 03:30:31  ida
# snd を追加
#
# Revision 1.42  2005/02/16 10:38:32  terui
# wvr が常時ビルドされるよう修正。
#
# Revision 1.41  2005/01/14 06:22:06  terui
# SUBDIRSにwvrを追加。
#
# Revision 1.40  2004/12/22 02:49:43  terui
# Add cht in SUBDIRS
#
# Revision 1.39  2004/12/14 13:19:39  takano_makoto
# mathを追加
#
# Revision 1.38  2004/09/13 11:46:52  yada
# add ctrdg
#
# Revision 1.37  2004/09/13 06:20:06  yosizaki
# add wbt.
#
# Revision 1.36  2004/09/09 12:48:39  yosizaki
# add card demos.
#
# Revision 1.35  2004/08/10 07:31:18  yosizaki
# add mb demo.
#
# Revision 1.34  2004/07/24 05:43:11  yasu
# Link to wm/
#
# Revision 1.33  2004/07/13 12:31:55  yasu
# delete libcxx workaround
#
# Revision 1.32  2004/07/12 09:10:13  yasu
# Support e3s libraires on TEG only.
#
# Revision 1.31  2004/06/29 00:58:54  yasu
# build libcxx again for workaround against lcf error
#
# Revision 1.30  2004/06/23 12:02:11  seiki_masashi
# add 'prc/characterRecognition-1'
#
# Revision 1.29  2004/06/10 01:42:06  yasu
# fix checklist gen
#
# Revision 1.28  2004/06/09 11:21:55  yasu
# Makefile Picks up only public demos
#
# Revision 1.27  2004/06/09 10:56:46  yasu
# Add demo check list generator
#
# Revision 1.26  2004/05/31 03:24:12  yasu
# merge e3s gpio package
#
# Revision 1.25  2004/05/24 07:02:41  yasu
# Hide ide sample, because MW provides offical ide samples
#
# Revision 1.24  2004/05/17 05:27:36  takano_makoto
# Add CP demos.
#
# Revision 1.23  2004/05/12 05:36:20  terui
# Add spi , rtc in SUBDIRS.
#
# Revision 1.22  2004/05/06 10:18:18  yasu
# fix typo
#
# Revision 1.21  2004/03/31 13:55:44  takano_makoto
# Add tips directory
#
# Revision 1.20  2004/03/05 04:22:57  yasu
# add PXI
#
# Revision 1.19  2004/02/27 05:36:37  yasu
# template_sp into private
#
# Revision 1.18  2004/02/23 06:41:51  ida
# hide snd library
#
# Revision 1.17  2004/02/23 06:33:47  yasu
# NITRO_PRIVATES -> NITRO_PRIVATE
#
# Revision 1.16  2004/02/18 06:31:54  ida
# changed snd demo to Nindendo confidential
#
# Revision 1.15  2004/02/06 08:17:48  yasu
# small fix
#
# Revision 1.14  2004/02/05 07:09:02  yasu
# change SDK prefix iris -> nitro
#
# Revision 1.13  2004/01/20 07:02:19  yasu
# ide の追加
#
# Revision 1.12  2004/01/18 05:15:31  yada
# MIディレクトリ追加
#
# Revision 1.11  2004/01/15 13:22:29  yasu
# Makefile chains to fs/
#
# Revision 1.10  2003/12/18 09:49:37  nishida_kenji
# add fx
#
# Revision 1.9  2003/12/16 08:09:08  yasu
# 一時的に template_sp のリンクを切る
#
# Revision 1.8  2003/12/16 07:39:25  yasu
# templateSP の追加
#
# Revision 1.7  2003/12/11 07:57:44  yasu
# _template/ -> template/ の修正
#
# Revision 1.6  2003/11/25 09:52:39  yasu
# os の追加
#
# Revision 1.5  2003/11/14 10:55:48  yada
# pad ディレクトリを追加
#
# Revision 1.4  2003/11/05 04:46:31  yasu
# _template の追加
#
# Revision 1.3  2003/11/04 10:16:06  yasu
# gx へのチェインの追加
#
# Revision 1.2  2003/10/30 05:51:52  yasu
# build: ターゲットの削除
#
# Revision 1.1  2003/10/29 13:27:27  yasu
# make の伝播のフレームワーク完成
# all/install/clean/clobber が使える
#
# $NoKeywords: $
#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/commondefs
include	$(NITROSDK_ROOT)/build/buildtools/commondefs.cctype.CW

#----------------------------------------------------------------------------

SUBDIRS		= template		\
		  gx 			\
		  pad			\
		  os			\
		  fx			\
		  card			\
		  fs			\
		  mi			\
		  tips			\
		  spi			\
		  rtc			\
		  cp			\
		  prc			\
		  wm			\
		  wbt			\
		  mb			\
		  ctrdg			\
		  math			\
		  cht			\
		  wvr			\
		  snd			\
		  env			\
		  vib           \
		  std

ifdef	NITRO_WITH_ARM7
SUBDIRS		+= pxi			\
		   utest		\
		   template_sp
ifeq	($(NITRO_PLATFORM),TEG)
SUBDIRS		+= e3s
endif
endif

#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/modulerules

XMLFILE		=	_checklist.xml
XMLGROUP	=	^gx\\/UnitTours\\/2/GX_2D		\
			^gx\\/UnitTours\\/3/GX_3D		\
			^gx\\/UnitTours\\/[^23]/GX_MISC		\
			^[mo]/OS				\
			^[a-fh-lnp-z]/MISC

make_list:
	# Make a list of public demos
	DEMODIRS="`/bin/find . -name src -exec dirname {} \; | sed '/\/_/d' | sed 's/^..//g'`"; \
	for i in $$DEMODIRS; do									\
		if [ ! \( -e $$i'/_' -o -e $$i'/@' -o -e $$i'/../_' -o -e $$i'/../@' \) ]; then	\
			echo $$i;								\
		fi;										\
	done > $(XMLFILE).tmp
	# Make XML files
	for i in $(XMLGROUP); do \
		REGEX=`dirname $$i`; FNAME='_checklist_'`basename $$i`'.xml' ;\
		echo '<?xml version="1.0" encoding="Shift_JIS"?>'                             >  $$FNAME; \
		echo '<?xml-stylesheet type="text/xsl" href="_checklist.xsl"?>'               >> $$FNAME; \
		echo '<ALL>'                                                       	      >> $$FNAME; \
		gawk '/'$$REGEX'/{ print "<DEMO><NAME>"$$1"</NAME></DEMO>"; }' $(XMLFILE).tmp >> $$FNAME; \
		echo '</ALL>'                                                  	              >> $$FNAME; \
	done
	rm $(XMLFILE).tmp

#===== End of Makefile =====
