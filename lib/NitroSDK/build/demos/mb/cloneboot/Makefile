#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - MB - demos - cloneboot
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
# Revision 1.4  2004/11/24 11:06:20  yosizaki
# add comment about parent-section.
# delete parent_section.c
#
# Revision 1.3  2004/11/24 10:31:11  yasu
# Change folder name holding libsyacall.a
#
# Revision 1.2  2004/11/24 10:05:14  yosizaki
# change LCFILE_TEMPLATE to use /specfiles.
#
# Revision 1.1  2004/11/15 06:20:48  yosizaki
# initial update.
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


# MBライブラリのサンプルはマルチブート機能を使うため、
# 同じ通信環境(無線あるいは有線)どうしの開発機材を複数必要とします。
# $NitroSDK/bin/ARM9-TS/Release/ ディレクトリにある
# mb_child.bin プログラムが最終実機におけるマルチブート子機と同等の
# 機能を提供するサンプルになっていますので、
# このバイナリをサンプルプログラムと同じ方法で他の機材に読み込み、
# 一緒に実行してください。

#----------------------------------------------------------------------------


#----------------------------------------------------------------------------
# このプログラムで使用するソースファイルは以下の通りです.
#----------------------------------------------------------------------------

SRCDIR		= ./src
INCDIR		= ./include

SRCS		= main.c		\
		  common.c		\
		  disp.c		\
		  font.c		\
		  child.c		\
		  parent.c		\
		  gmain.c

# サンプルの簡略化のため WH モジュールを使用します.

WH_DIR		=  $(ROOT)/build/demos/wireless_shared/wh
SRCDIR		+= $(WH_DIR)
INCDIR		+= $(WH_DIR)
SRCS		+= $(WH_DIR)/wh.c

# サンプルの簡略化のため MBP モジュールを使用します.

MBP_DIR		=  $(ROOT)/build/demos/wireless_shared/mbp
SRCDIR		+= $(MBP_DIR)
INCDIR		+= $(MBP_DIR)
SRCS		+= $(MBP_DIR)/mbp.c


#----------------------------------------------------------------------------
# アプリケーションの一般的な設定は以下の通りです.
#----------------------------------------------------------------------------

TARGET_NEF	= main.nef
TARGET_BIN	= main.srl
ROM_SPEC	= main.rsf
NITRO_MAKEROM	= TRUE

# MBのコールバック内で複雑な処理をする場合には、lcfファイルで
# IRQスタックサイズを少し大きめに設定しておいた方が安全です。
# 特にOS_Printf()は大量のスタックを消費するので、コールバック内では
# できるだけ軽量版のOS_TPrintf()を使用するようにしてください。
ifeq	($(NITRO_PLATFORM),TEG)
MAKELCF_FLAGS	+= -DADDRESS_MAIN=0x02004000
else
MAKELCF_FLAGS	+= -DADDRESS_MAIN=0x02000000
endif
# LCFILE_SPEC	= main.lsf

# プログラムの各セグメントを圧縮する場合はこのフラグを有効にします.
# 圧縮されたセグメントは CARD-ROM 上でのバイナリサイズが削減され
# CARD-ROM またはワイヤレス経由でのロード時間が短縮されるかわりに,
# 実行時は展開のためにいくらかの CPU 時間を消費します.
NITRO_COMPRESS	= TRUE

# ワイヤレスダウンロード子機としてオーバーレイを使用する場合,
# NITRO_DIGEST を有効にする必要があります.
NITRO_DIGEST	= TRUE


#----------------------------------------------------------------------------
# クローンブート親機としての特別な設定は以下の通りです.
#----------------------------------------------------------------------------

# セキュリティ目的の親機専用領域 .parent セクションを配置するため
# クローンブート用リンク設定テンプレートを指定します.
#
# .parent セクションの内容はDSダウンロードプレイ子機には送信されません.
# このセクションに親機専用かつ必須の関数を配置することによって,
# 傍受された受信内容から親機自身を再現することを不可能にします.
# コードの配置にあたっては, 以下の点に注意してください.
#   ・.text/.rodata セクション以外を含まないこと
#   ・親機のみが必要とする重要な処理を含むこと
# 
# .parent セクションは, 該当するコードについて以下のように配置します.
# 
#     #include <nitro/parent_begin.h>
#
#     void  ForParentOnly(void)
#     {
#         /* 何か親機固有の処理 */
#     }
#
#     #include <nitro/parent_end.h>
#
# このサンプルデモでは parent.c の全ての関数を .parent セクションに
# 配置しています. 詳細はソースコードを参照ください.

LCFILE_TEMPLATE	= $(NITRO_SPECDIR)/ARM9-TS-cloneboot-C.lcf.template

# 製品版アプリケーションのために各々へ配布された
# 正式なシステムコールライブラリをここで指定します.
# このサンプルでは暫定用として、あらかじめこのディレクトリに,
# SDK 付属版(lib/ARM9-TS/etc/libsyscall.a)のコピーを置いてあります．
# このデモを元にしてアプリケーションを作られる場合は、弊社サポートより
# 配布させていただいている libsyscal.a でこの暫定システムコールを
# 上書きしてください．

LIBSYSCALL		= ./etc/libsyscall.a
LIBSYSCALL_CHILD	= ./etc/libsyscall_child.bin


#----------------------------------------------------------------------------


include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------
# アプリケーションを製品版で実行させるための事前認証手続きは以下の通りです.
#----------------------------------------------------------------------------

# 事前認証手続きはおおよそ以下の流れになります.
# 詳細は「DSダウンロードプレイ解説」を参照ください.
# (/docs/TechnicalNotes/AboutMultiBoot.doc)
#
# (0) クローンブートなら emuchild コマンドで送信用プログラムを用意します.
#     そうでなければ main.srl をそのまま用意します.
#     この makefile では, 用意したものを bin/sign.srl とします.
#
# (1) 認証コード取得のため, (0) で用意した tmp/sign.srl をサーバへ送付します.
#     この作業はご使用のブラウザから手作業で行う必要があります.
#     この makefile では, 取得した認証コードを bin/sign.sgn とします.
#
# (2) attachsign コマンドを使用して, (1) で取得した bin/sign.sgn と
#     main.srl を結合した最終プログラムを生成します.
#     この makefile では, 生成物を main_with_sign.srl とします.
#

# クローンブートの場合は通常のDSダウンロードプレイプログラムと若干異なり,
# 自身のプログラムでなく emuchild で加工したプログラムをサーバへ送付します.
presign:
	$(EMUCHILD) \
		bin/$(NITRO_BUILDTYPE)/$(TARGET_BIN) \
		$(LIBSYSCALL_CHILD) \
		bin/sign.srl

# 得られた認証コードをバイナリに含める手続きはクローンブートも通常と同様です.
postsign:
	$(ATTACHSIGN) \
		bin/$(NITRO_BUILDTYPE)/$(TARGET_BIN) \
		bin/sign.sgn  \
		main_with_sign.srl

#----------------------------------------------------------------------------

do-build:	$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules

ifdef	NITRO_ELFTOBIN
$(TARGETS): $(ELFTOBIN_ARM7)

else	#NITRO_ELFTOBIN
$(TARGETS): $(MAKEROM_ARM7)

endif	#NITRO_ELFTOBIN


#===== End of Makefile =====
