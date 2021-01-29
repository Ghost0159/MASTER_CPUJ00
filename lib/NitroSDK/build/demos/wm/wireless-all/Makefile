#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - WM - demos - wireless-all
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
# Revision 1.1  2006/04/10 13:07:23  yosizaki
# initial upload.
#
# $NoKeywords: $
#----------------------------------------------------------------------------

SUBDIRS		=


#----------------------------------------------------------------------------
# このプログラムで使用するソースファイルは以下の通りです.
#----------------------------------------------------------------------------

SRCDIR		= ./src
INCDIR		= ./include

# 親機共通処理.
SRCS		=	main.c	\
				common.c	\
				test.c

# ワイヤレスドライバ処理. (WHの簡易ラッパー)
SRCS		+=	wh_measure.c	\
				wh_download.c	\
				wh_datasharing.c

# WH(ワイヤレスサンプルモジュール)を使用.
WH_DIR		=  $(ROOT)/build/demos/wireless_shared/wh
LINCLUDES	+= $(WH_DIR)
SRCS		+= $(WH_DIR)/wh.c

# MBP(DSダウンロードプレイサンプルモジュール)を使用.
MBP_DIR		=  $(ROOT)/build/demos/wireless_shared/mbp
LINCLUDES	+= $(MBP_DIR)
SRCS		+= $(MBP_DIR)/mbp.c

# WFS(ワイヤレスファイルシステムサンプルモジュール)を使用.
WFS_DIR		=  $(ROOT)/build/demos/wireless_shared/wfs
LINCLUDES	+= $(WFS_DIR)/include
SRCS		+= \
	$(WFS_DIR)/src/wfs_common.c	\
	$(WFS_DIR)/src/wfs_archive.c	\
	$(WFS_DIR)/src/wfs_thread.c


#----------------------------------------------------------------------------
# このプログラムでビルド時に使用するリソースは以下の通りです.
#----------------------------------------------------------------------------

# bin2obj.exe で .dat -> .o に変換すべきデータ群を格納するディレクトリ.
DATA_DIR	= ./data/image

# ワイヤレスリンク強度アイコン.
WLICON_DIR	= $(ROOT)/data/wl_icons
WLICON_BMP	= $(wildcard $(WLICON_DIR)/wireless_strength_level_*_b.bmp)
WLICON_OBJS	= $(patsubst $(WLICON_DIR)/wireless_strength_level_%_b.bmp,$(OBJDIR)/linkimg%.o,$(WLICON_BMP))
EXT_OBJS	+=	$(WLICON_OBJS)

# ASCII文字表示用BGキャラクタ.
BGCHAR_OBJS	= $(patsubst $(DATA_DIR)/%.dat,$(OBJDIR)/%.o,$(wildcard $(DATA_DIR)/*.dat))
EXT_OBJS	+=	$(BGCHAR_OBJS)


#----------------------------------------------------------------------------
# アプリケーションの一般的な設定は以下の通りです.
#----------------------------------------------------------------------------

TARGET_NEF	= main.nef
TARGET_BIN	= main.srl
NITRO_MAKEROM	= TRUE
MAKEROM_ROMROOT		= ./data/rom
MAKEROM_ROMFILES	= *

# プログラムの各セグメントを圧縮する場合はこのフラグを有効にします.
# 圧縮されたセグメントは CARD-ROM 上でのバイナリサイズが削減され
# CARD-ROM またはワイヤレス経由でのロード時間が短縮されるかわりに,
# 実行時は展開のためにいくらかの CPU 時間を消費します.

NITRO_COMPRESS	= TRUE

# ワイヤレスダウンロード子機としてオーバーレイを使用する場合,
# セキュリティのために必ず NITRO_DIGEST を有効にする必要があります.

NITRO_DIGEST	= TRUE


#----------------------------------------------------------------------------
# クローンブート親機としての特別な設定は以下の通りです.
#----------------------------------------------------------------------------

# セキュリティ目的の親機専用領域 .parent セクションを配置するため
# クローンブート用リンク設定テンプレートを指定します.

LCFILE_TEMPLATE	= $(NITRO_SPECDIR)/ARM9-TS-cloneboot-C.lcf.template

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
# このサンプルデモではDSダウンロードプレイ親機処理の全ての関数を
# .parent セクションに配置しています. 詳細はソースコードをご参照ください.


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
# アプリケーションを製品版で実行させるための署名手続きは以下の通りです.
#----------------------------------------------------------------------------

# 署名手続きはおおよそ以下の流れになります.
# 詳細は「DSダウンロードプレイ解説」を参照ください.
# (/docs/TechnicalNotes/AboutMultiBoot.pdf)
#
# (0) クローンブートなら emuchild コマンドで送信用プログラムを用意します.
#     そうでなければ main.srl をそのまま用意します.
#     この makefile では, 用意したものを bin/sign.srl とします.
#
# (1) 署名コード取得のため, (0) で用意した tmp/sign.srl をサーバへ送付します.
#     この作業はご使用のブラウザから手作業で行う必要があります.
#     この makefile では, 取得した署名コードを bin/sign.sgn とします.
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

# 得られた署名コードをバイナリに含める手続きはクローンブートも通常と同様です.
postsign:
	$(ATTACHSIGN) \
		bin/$(NITRO_BUILDTYPE)/$(TARGET_BIN) \
		bin/sign.sgn  \
		main_with_sign.srl

#----------------------------------------------------------------------------

do-build:	$(TARGETS)


include	$(NITROSDK_ROOT)/build/buildtools/modulerules

# リンク強度アイコンのビットマップをバイナリデータに変換します.
$(DATA_DIR)/linkimg%.dat : $(WLICON_DIR)/wireless_strength_level_%_b.bmp
	$(NTEXCONV) -no -bg -bgb -bgf palette16 $< >/dev/null && \
	cat	$(basename $<).nbfp $(basename $<).nbfc > $(basename $@).dat && \
	rm	$(basename $<).nbfp $(basename $<).nbfc $(basename $<).nbfs

# 全てのバイナリデータを定数配列としてリンクします.
$(OBJDIR)/%.o : $(DATA_DIR)/%.dat
	$(BIN2OBJ) --compatible $< $@ >/dev/null


#===== End of Makefile =====
