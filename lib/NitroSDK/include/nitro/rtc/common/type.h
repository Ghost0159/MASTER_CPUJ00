/*---------------------------------------------------------------------------*
  Project:  NitroSDK - RTC - include
  File:     type.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: type.h,v $
  Revision 1.3  2006/01/18 02:12:38  kitase_hirotake
  do-indent

  Revision 1.2  2005/02/28 05:26:09  yosizaki
  do-indent.

  Revision 1.1  2004/05/12 02:27:35  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef	NITRO_RTC_COMMON_TYPE_H_
#define	NITRO_RTC_COMMON_TYPE_H_

#ifdef	__cplusplus
extern "C" {
#endif

/*===========================================================================*/

#include	<nitro/types.h>

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
// RTC�����݊֘A��`
#define		RTC_INTERRUPT_MODE_NONE				0x0     // �����݂Ȃ�
#define		RTC_INTERRUPT_MODE_32kHz			0x8     // 32kHz���g�o��
#define		RTC_INTERRUPT_MODE_PULSE			0x1     // �I�����g����튄����
#define		RTC_INTERRUPT_MODE_MINUTE_EDGE		0x2     // ���P�ʃG�b�W������
#define		RTC_INTERRUPT_MODE_MINUTE_PULSE1	0x3     // ���P�ʒ�튄���݂P(�f���[�e�B�[ 50%)
#define		RTC_INTERRUPT_MODE_ALARM			0x4     // �A���[��������
#define		RTC_INTERRUPT_MODE_MINUTE_PULSE2	0x7     // ���P�ʒ�튄���݂Q

#define		RTC_INTERRUPT_MASK_32kHz			0x8     // 32kHz���g�o�͎��̈Ӗ������r�b�g�}�X�N
#define		RTC_INTERRUPT_MASK_PULSE			0xb     // �I�����g����튄���ݎ��̈Ӗ������r�b�g�}�X�N
#define		RTC_INTERRUPT_MASK_MINUTE_EDGE		0xb     // ���P�ʃG�b�W�����ݎ��̈Ӗ������r�b�g�}�X�N

// ���g���f���[�e�B�[��`
#define		RTC_PULSE_DUTY_1HZ					0x01
#define		RTC_PULSE_DUTY_2HZ					0x02
#define		RTC_PULSE_DUTY_4HZ					0x04
#define		RTC_PULSE_DUTY_8HZ					0x08
#define		RTC_PULSE_DUTY_16HZ					0x10
/* ������ݒ肵���ꍇ�A�ݒ肵���p���X���S�ăn�C�ɂȂ����Ƃ������ݐM������
   �n�C���o�͂����B�ǂꂩ�ЂƂł����[�������ꍇ�̓��[�o�͂ƂȂ�B*/


/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/
// ���t�\����
typedef struct RTCRawDate
{
    u32     year:8;                    // �N( 00 ~ 99 )
    u32     month:5;                   // ��( 01 ~ 12 )
    u32     dummy0:3;
    u32     day:6;                     // ��( 01 ~ 31 ) ��/�[�N�ɂ�����͕ϓ�
    u32     dummy1:2;
    u32     week:3;                    // �j��( 00 ~ 06 )
    u32     dummy2:5;

}
RTCRawDate;

// ���ԍ\����
typedef struct RTCRawTime
{
    u32     hour:6;                    // ����( 00 ~ 23 �܂��� 00 ~ 11 )
    u32     afternoon:1;               // 12���ԕ\�L�̏ꍇ��p.m.�t���O
    u32     dummy0:1;
    u32     minute:7;                  // ��( 00 ~ 59 )
    u32     dummy1:1;
    u32     second:7;                  // �b( 00 ~ 59 )
    u32     dummy2:9;

}
RTCRawTime;

// �X�e�[�^�X�P�\����
typedef struct RTCRawStatus1
{
    u16     reset:1;                   // W   ���Z�b�g�t���O
    u16     format:1;                  // R/W �̎��ԕ\�L�t���O( 0: 12���ԕ\�L�A1: 24���ԕ\�L )
    u16     dummy0:2;
    u16     intr1:1;                   // R   �����݂P�����t���O( �����ݐM�����ɂ̓��[���o�͂���� )
    u16     intr2:1;                   // R   �����݂Q�����t���O( �����ݐM�����ɂ̓��[���o�͂���� )
    u16     bld:1;                     // R   �d���d���~�����o�t���O
    u16     poc:1;                     // R   �d���������o�t���O
    u16     dummy1:8;

}
RTCRawStatus1;

// �X�e�[�^�X�Q�\����
typedef struct RTCRawStatus2
{
    u16     intr_mode:4;               // R/W �����݂P�I��( RTC_INTERRUPT_MODE_* )
    u16     dummy0:2;
    u16     intr2_mode:1;              // R/W �����݂Q���t���O
    u16     test:1;                    // R/W �e�X�g���[�h�t���O
    u16     dummy1:8;

}
RTCRawStatus2;

// �A���[���\����
typedef struct RTCRawAlarm
{
    u32     week:3;                    // �j��( 00 ~ 06 )
    u32     dummy0:4;
    u32     we:1;                      // �j���ݒ�L���t���O
    u32     hour:6;                    // ����( 00 ~ 23 �܂��� 00 ~ 11 ) ���ԕ\�L�̓X�e�[�^�X�P�Ɉˑ�
    u32     afternoon:1;               // 12���ԕ\�L�̏ꍇ��p.m.�t���O
    u32     he:1;                      // ���Ԑݒ�L���t���O
    u32     minute:7;                  // ��( 00 ~ 59 )
    u32     me:1;                      // ���ݒ�L���t���O
    u32     dummy2:8;

}
RTCRawAlarm;

// �p���X�\����
typedef struct RTCRawPulse
{
    u32     pulse:5;                   // ���g���f���[�e�B�[�t���O( RTC_PULSE_DUTY_* )
    u32     dummy:27;

}
RTCRawPulse;

// �N���b�N�������W�X�^�\����
typedef struct RTCRawAdjust
{
    u32     adjust:8;                  // �N���b�N�����p���W�X�^( �ݒ�ɂ��Ă͕ʎ����Q�� )
    u32     dummy:24;

}
RTCRawAdjust;

// �t���[���W�X�^�\����
typedef struct RTCRawFree
{
    u32     free:8;                    // �t���[���W�X�^
    u32     dummy:24;

}
RTCRawFree;

// ARM9 <-> ARM7 �󂯓n���f�[�^�\����( 8�o�C�g���p�� )
typedef union RTCRawData
{
    struct
    {
        RTCRawDate date;               // ���t
        RTCRawTime time;               // ����
    }
    t;

    struct
    {
        RTCRawStatus1 status1;         // �X�e�[�^�X�P���W�X�^
        RTCRawStatus2 status2;         // �X�e�[�^�X�Q���W�X�^
        union
        {
            RTCRawPulse pulse;         // ���g����튄���݃��W�X�^�ݒ�
            RTCRawAlarm alarm;         // �A���[��( 1 or 2 )���W�X�^�ݒ�
            RTCRawAdjust adjust;       // �N���b�N�����p���W�X�^�ݒ�
            RTCRawFree free;           // �t���[���W�X�^�ݒ�
        };
    }
    a;

    u32     words[2];                  // 4�o�C�g�A�N�Z�X�p

    u16     halfs[4];                  // 2�o�C�g�A�N�Z�X�p

    u8      bytes[8];                  // �o�C�g�A�N�Z�X�p

}
RTCRawData;


/*===========================================================================*/

#ifdef	__cplusplus
}          /* extern "C" */
#endif

#endif /* NITRO_RTC_COMMON_TYPE_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
