#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - GX - demos - UnitTours
# File:	    Makefile
#
# Copyright 2003,2004,2007 Nintendo.  All rights reserved.
#
# These coded instructions, statements, and computer programs contain
# proprietary information of Nintendo of America Inc. and/or Nintendo
# Company Ltd., and are protected by Federal copyright law.  They may
# not be disclosed to third parties or copied or duplicated in any form,
# in whole or in part, without the prior written consent of Nintendo.
#
# $Log: Makefile,v $
# Revision 1.30  2007/01/29 02:12:28  okubata_ryoma
# VCountƒfƒ‚‚ð”ñŒöŠJ‚É
#
# Revision 1.29  2004/11/25 11:44:58  takano_makoto
# 2D_CharBg_BankEx‚ð’Ç‰Á
#
# Revision 1.28  2004/10/07 08:15:06  yasu
# Add ClearDepthOnly
#
# Revision 1.27	 2004/08/27 09:29:48  yasu
# Support NITRO_WITHOUT_GXDEMOS
#
# Revision 1.26	 2004/08/27 09:27:56  yasu
# Support NITRO_WITHOUT_GXDEMO
#
# Revision 1.25	 2004/05/06 10:18:18  yasu
# fix typo
#
# Revision 1.24	 2004/04/06 12:48:08  yada
# fix header comment
#
# Revision 1.23	 2004/03/01 11:25:48  terui
# add "Capture" demos
#
# Revision 1.21	 2004/03/01 09:11:16  takano_makoto
# Add new demo program.
#
# Revision 1.20	 2004/02/13 11:15:58  nishida_kenji
# Add 3D_Pol_Split.
#
# Revision 1.19	 2004/02/05 07:09:02  yasu
# change SDK prefix iris -> nitro
#
# Revision 1.18	 2004/02/04 11:12:05  nishida_kenji
# add Sub_Double3D.
#
# Revision 1.17	 2004/02/02 06:44:12  nishida_kenji
# Add samples for the sub display.
#
# Revision 1.16	 2004/01/13 07:45:16  nishida_kenji
# 3D_Pol_MakeDL
#
# Revision 1.15	 2004/01/10 07:50:14  nishida_kenji
# add 3D_Pol_TexSRT
#
# Revision 1.14	 2004/01/07 09:38:05  nishida_kenji
# revises comments
#
# $NoKeywords: $
#----------------------------------------------------------------------------

include $(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------


SUBDIRS		= DEMOLib

ifndef	NITRO_WITHOUT_GXDEMOS
SUBDIRS		+= 2D_CharBg_1			\
		   2D_CharBg_2			\
		   2D_CharBg_3			\
		   2D_CharBg_4			\
		   2D_CharBg_5			\
		   2D_CharBg_6			\
		   2D_CharBg_7			\
		   2D_CharBg_8			\
		   2D_CharBg_9			\
		   2D_Oam_1			\
		   2D_Oam_2			\
		   2D_Oam_3			\
		   2D_Oam_4			\
		   2D_Oam_5			\
		   2D_BmpBg_Vram		\
		   2D_BmpBg_MainRam		\
		   3D_Pol_OneTri		\
		   3D_Pol_Simple		\
		   3D_Pol_Simple2		\
		   3D_Pol_Simple3		\
		   3D_Pol_Light			\
		   3D_Pol_Color			\
		   3D_Pol_LightColor		\
		   3D_Pol_Translucent		\
		   3D_Pol_Transparent		\
		   3D_Edge_Marking		\
		   3D_Fog			\
		   3D_Alpha_Test		\
		   3D_Shininess			\
		   3D_Command_Pack		\
		   3D_Pol_Tex4_Plett		\
		   3D_Pol_Tex16_Plett		\
		   3D_Pol_Tex256_Plett		\
		   3D_Pol_Tex32768		\
		   3D_Pol_Tex4x4		\
		   3D_Pol_Tex_Alpha		\
		   3D_Pol_Tex_Decal		\
		   3D_Pol_Toon			\
		   3D_Pol_High_Light		\
		   3D_Pol_Env_Mapping		\
		   3D_Pol_TexSRT		\
		   3D_Pol_MakeDL		\
		   3D_Pol_Split			\
		   Sub_CharBg_1			\
		   Sub_CharBg_2			\
		   Sub_CharBg_3			\
		   Sub_CharBg_4			\
		   Sub_CharBg_5			\
		   Sub_CharBg_6			\
		   Sub_CharBg_7			\
		   Sub_CharBg_8			\
		   Sub_CharBg_9			\
		   Sub_Oam_1			\
		   Sub_Oam_2			\
		   Sub_Oam_3			\
		   Sub_Oam_4			\
		   Sub_Oam_5			\
		   Sub_Double3D			\
		   2D_CharBg_256BMP		\
		   2D_CharBg_256_16		\
		   2D_CharBg_Direct		\
		   2D_CharBg_BankEx		\
		   2D_Oam_256_16		\
		   2D_Oam_Bmp1D			\
		   2D_Oam_Char1D		\
		   2D_Oam_Direct		\
		   2D_Oam_OBJWindow		\
		   2D_Oam_Translucent		\
		   3D_BoxTest			\
		   3D_PolAttr_1Dot		\
		   3D_PolAttr_DpthTest		\
		   3D_PolAttr_DpthUpdate	\
		   3D_PolAttr_FARClip		\
		   3D_Pol_Vertex_Source		\
		   3D_Shadow_Pol		\
		   AntiAlias			\
		   Capture			\
		   ClearColor			\
		   ClearDepthOnly		\
		   ClearImage			\
		   CurrentMtx			\
		   FlipRepeat			\
		   LinesOver			\
		   Master_Bright		\
		   PosVecTest			\
		   RamOverFlow			\
		   ViewPort			\
		   Window			\
		   Window_HDMA
endif

#----------------------------------------------------------------------------

include $(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
