/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SPI - demos
  File:     monkey.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: monkey.h,v $
  Revision 1.4  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.3  2005/02/28 05:26:27  yosizaki
  do-indent.

  Revision 1.2  2004/06/10 09:37:46  terui
  ���b�Z�[�WTYPE��ǉ��B�T���v�����O�����𒲐��B

  Revision 1.1  2004/06/02 09:53:00  terui
  Initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef	MONKEY_H_
#define	MONKEY_H_

#ifdef	__cplusplus
extern "C" {
#endif

/*===========================================================================*/

#include	<nitro/os.h>
#include	<nitro/spi.h>

/*---------------------------------------------------------------------------*
	�萔��`
 *---------------------------------------------------------------------------*/
#define		MONKEY_MESSAGE_ARRAY_MAX	8
#define		MONKEY_STACK_SIZE			1024
#define		MONKEY_THREAD_PRIORITY		15
#define		MONKEY_MIC_SPAN_TICK		( HW_SYSTEM_CLOCK / 64 / 60 / 263 )     //�� 1 ���C��
#define		MONKEY_MIC_ARRAY_MAX		256
#define		MONKEY_TP_ARRAY_MAX			8
#define		MONKEY_SAMPLING_SPAN_LINE	4       // Periodic�^�C�}�[�̃��C������

#define		MONKEY_MESSAGE_TYPE_MIC		1
#define		MONKEY_MESSAGE_TYPE_TP		2


/*---------------------------------------------------------------------------*
	�\���̒�`
 *---------------------------------------------------------------------------*/
typedef struct MonkeyWork
{
    OSMessageQueue msg_q;
    OSMessage msg_buf[MONKEY_MESSAGE_ARRAY_MAX];
    OSThread thread;
    u32     stack[MONKEY_STACK_SIZE / sizeof(u32)];
    OSAlarm alarm;
    u16     micBuf[MONKEY_MIC_ARRAY_MAX];
    u16     micIndex;
    TPData  tpBuf[MONKEY_TP_ARRAY_MAX];
    u16     tpIndex;
    u32     timerCount;

}
MonkeyWork;


/*---------------------------------------------------------------------------*
	�֐���`
 *---------------------------------------------------------------------------*/

// �������A�X���b�h�N��
void    MonkeyInit(void);

// �^�b�`�p�l�����͒l��z��Ɏ擾
void    MonkeyGetNewTpData(s32 num, TPData *array);

// �}�C�N���͒l��z��Ɏ擾
void    MonkeyGetNewMicData(s32 num, u16 *array);


/*===========================================================================*/

#ifdef	__cplusplus
}          /* extern "C" */
#endif

#endif // MONKEY_H_

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
