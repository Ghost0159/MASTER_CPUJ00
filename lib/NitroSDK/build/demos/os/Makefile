#! make -f
#----------------------------------------------------------------------------
# Project:  NitroSDK - OS - demos
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
# Revision 1.44  2006/10/19 01:33:20  okubata_ryoma
# サンプルデモforKorea-1の追加
#
# Revision 1.43  2005/09/09 05:19:13  yada
# add argument-2
#
# Revision 1.42  2005/07/27 07:44:29  seiki_masashi
# entropy-1 デモを追加
#
# Revision 1.41  2005/07/21 02:42:26  yada
# add argument-1
#
# Revision 1.40  2005/07/14 23:38:26  kitase_hirotake
# thread-10の追加
#
# Revision 1.39  2005/07/12 07:55:08  yada
# add thread-8 and thread-9 demos
#
# Revision 1.38  2005/07/08 12:03:52  yada
# add thread6 and thread7 demo
#
# Revision 1.37  2005/06/02 08:00:28  yosizaki
# add forChina-fs.
#
# Revision 1.36  2005/05/14 01:08:05  terui
# Add "forChina-1"
#
# Revision 1.35  2005/04/27 02:01:09  terui
# Update copyright.
#
# Revision 1.34  2005/04/26 05:52:39  terui
# remove china-1 from SUBDIRS
#
# Revision 1.33  2005/04/26 02:35:01  terui
# Fix comment
#
# Revision 1.32  2004/12/08 05:18:31  yada
# remove interrupt-key-1
#
# Revision 1.31  2004/10/08 00:38:36  yosizaki
# add favorite-color.
#
# Revision 1.30  2004/09/01 04:48:01  yada
# add reset-1
#
# Revision 1.29  2004/06/25 05:02:35  yada
# OS_JoinThread() and  OS_IsThreadTerminate() sample
#
# Revision 1.28  2004/06/03 07:15:12  yada
# sorry, update 1.27 is mistake. ignore comment of 1.27.
# now, add some demos because profile library separated.
#
# Revision 1.27  2004/06/03 06:59:41  yada
# fix indent processing
#
# Revision 1.26  2004/05/20 01:00:30  yada
# add waitIrq-1 and waitIrq-2
#
# Revision 1.25  2004/05/12 05:15:41  yada
# delete callTrace* and exceptionDisplay-3
#
# Revision 1.24  2004/05/10 12:00:31  yada
# only arrange tab
#
# Revision 1.23  2004/05/06 10:18:18  yasu
# fix typo
#
# Revision 1.22  2004/04/15 06:52:19  yada
# add exceptionDisplay-3, callTrace-1, callTrace-2
#
# Revision 1.21  2004/04/13 00:27:24  mita_kentaro
# add backslash at the spinwait-1 line
#
# Revision 1.20  2004/04/12 10:12:06  yasu
# add cplusplus-1 demo
#
# Revision 1.19  2004/04/05 01:53:10  yada
# add mutex-2 demo
#
# Revision 1.18  2004/03/17 08:18:04  yada
# exceptionDisplay-2 入れた
#
# Revision 1.17  2004/03/09 07:36:10  yada
# valarm-1, sleep-1, exceptionDisplay-1, exceptionDisplay2 を追加
#
# Revision 1.16  2004/03/08 09:18:47  yada
# fix top comment
#
# Revision 1.15  2004/02/27 01:55:51  yasu
# add demo for OS_SpinWait
#
# Revision 1.14  2004/02/25 11:35:02  yada
# systemClock→Tick に伴う変更
#
# Revision 1.13  2004/02/05 07:09:03  yasu
# change SDK prefix iris -> nitro
#
# Revision 1.12  2004/02/03 12:05:49  yada
# alarm-2 を追加
#
# Revision 1.11  2004/02/03 11:25:53  yada
# systemClock-1 と alarm-1 と追加
#
# Revision 1.10  2004/01/18 04:51:39  yada
# interrupt-dmaサンプルは MI に移した
#
# Revision 1.9  2004/01/17 12:30:25  yada
# interrupt-dma-1 追加
#
# Revision 1.8  2004/01/09 09:20:21  yada
# mutex-1 を追加
#
# Revision 1.7  2004/01/08 07:38:59  yada
# timer-1, interrupt-key-1 を追加
#
# Revision 1.6  2004/01/07 08:21:49  yada
# サブディレクトリ heap-1, heap2, heap3, arena-1 を追加した。
#
# Revision 1.5  2003/12/16 06:30:36  yasu
# thread-4 割り込みによる thread 切り替え処理のサンプル
#
# Revision 1.4  2003/11/30 09:28:08  yada
# interrupt-thread のサンプルを外した
#
# Revision 1.3  2003/11/29 12:39:41  yasu
# thread-3, interrupt-thread 追加
#
# Revision 1.2  2003/11/26 01:38:29  yasu
# thread-2 追加
#
# Revision 1.1  2003/11/25 09:52:58  yasu
# 新規追加
#
# $NoKeywords: $
#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/commondefs


#----------------------------------------------------------------------------

SUBDIRS =       thread-1 \
                thread-2 \
                thread-3 \
                thread-4 \
                thread-5 \
                thread-6 \
                thread-7 \
                thread-8 \
                thread-9 \
                thread-10 \
                mutex-1 \
                mutex-2 \
                arena-1 \
                heap-1 \
                heap-2 \
                heap-3 \
                tick-1 \
                timer-1 \
                alarm-1 \
                alarm-2 \
                valarm-1 \
                sleep-1 \
                exceptionDisplay-1 \
                exceptionDisplay-2 \
                spinwait-1 \
                cplusplus-1 \
                waitIrq-1 \
                waitIrq-2 \
                callTrace-1 \
                callTrace-2 \
                exceptionDisplay-3 \
                functionCost-1 \
                functionCost-2 \
                functionCost-3 \
                reset-1 \
                favorite-color \
                forChina-1 \
                forChina-fs \
                forKorea-1 \
                argument-1 \
                argument-2 \
                entropy-1

#----------------------------------------------------------------------------

include	$(NITROSDK_ROOT)/build/buildtools/modulerules


#===== End of Makefile =====
