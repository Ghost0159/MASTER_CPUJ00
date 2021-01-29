#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - WM - demos
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
# $Log: makefile,v $
# Revision 1.16  2005/11/22 01:06:16  yosizaki
# copyright �̔N���X�V.
#
# Revision 1.15  2005/11/21 00:41:10  adachi_hiroaki
# �e�@�I����ʂɎQ���l���ƃj�b�N�l�[����\������悤�ɂ���
#
# Revision 1.14  2004/11/05 04:27:40  sasakis
# ���r�[��ʁA�e�@�I����ʂ̒ǉ��ƁA����ɕK�v�ȉ����iscan�֘A�Ȃǁj�B
#
# Revision 1.13  2004/11/02 07:15:01  sasakis
# makefile �̏C���Y��B
#
# Revision 1.12  2004/11/02 07:12:55  sasakis
# �R�[�h�����B
#
# Revision 1.11  2004/11/01 04:11:44  sasakis
# ���������΍�R�[�h�Ȃǂ̒ǉ��i��ƒ��R�[�h���܂ގb��Łj�B
#
# Revision 1.10  2004/10/22 07:35:18  sasakis
# ���L������ wh �ɑΉ��B
#
# Revision 1.9  2004/10/21 00:43:23  yosizaki
# add SHARED_WH switch.
#
# Revision 1.8  2004/10/06 05:11:35  sasakis
# �O���t�\���̒ǉ��ȂǁB
#
# Revision 1.7  2004/09/30 00:00:29  takano_makoto
# ���[�J���ݒ���폜
#
# Revision 1.6  2004/09/22 09:39:33  sasakis
# �f�o�O��ʂ�ǉ��B
#
# Revision 1.5  2004/09/17 09:13:30  sasakis
# �e�@���AMeasureChannel���g�p���āA�ł����G���Ă��Ȃ��`�����l����
# �I������悤�ɉ��������i�������������܂����肵�Ă��Ȃ��j�B
#
# Revision 1.4  2004/09/15 10:23:40  sasakis
# �d�g��M���x�A�C�R���̒ǉ��ƁA�f�[�^�T�C�Y�ω��̂��߂̉����ȂǁB
#
# Revision 1.3  2004/09/10 08:44:52  sasakis
# �K�C�h���C�����������̒ǉ��A�G���[�֌W�̌������ȂǁB
#
# Revision 1.2  2004/09/08 01:52:52  sasakis
# �G���[�E�ؒf�֌W���ǁB
#
# Revision 1.1  2004/09/07 04:59:42  sasakis
# ���œo�^�B
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS         =

#----------------------------------------------------------------------------

INCDIR			=	./include
SRCDIR			=	./src
SRCS            = \
                main.c          \
                print.c         \
		key.c		\
		graphics.c	\
                font.c          \
		menu.c		\
		unicode.c	\
                icon.c

WH_DIR		=  $(ROOT)/build/demos/wireless_shared/wh
SRCDIR		+= $(WH_DIR)
INCDIR		+= $(WH_DIR)
SRCS		+= $(WH_DIR)/wh.c
MACRO_FLAGS	+= -DSHARED_WH

TARGET_BIN      = main.srl

include $(NITROSDK_ROOT)/build/buildtools/commondefs

#----------------------------------------------------------------------------

do-build:       $(TARGETS)

include $(NITROSDK_ROOT)/build/buildtools/modulerules

#===== End of Makefile =====