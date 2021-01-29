/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SPI - include
  File:     userInfo_ts_300.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: userInfo_ts_300.h,v $
  Revision 1.3  2006/10/17 05:28:26  okubata_ryoma
  �؍���IPL�ɑΉ�

  Revision 1.2  2006/01/18 02:12:38  kitase_hirotake
  do-indent

  Revision 1.1  2005/04/12 10:49:57  terui
  ������Ή��ׂ̈ɐV�K�ǉ��B

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITRO_SPI_COMMON_USERINFO_TS_300_H_
#define NITRO_SPI_COMMON_USERINFO_TS_300_H_

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*/

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
// �R���t�B�O�ݒ�f�[�^�t�H�[�}�b�g�o�[�W����
#define NVRAM_CONFIG_DATA_VERSION           5
#define NVRAM_CONFIG_DATA_EX_VERSION        1

// �R���t�B�O���̊e��ݒ�l
#define NVRAM_CONFIG_NICKNAME_LENGTH        10  // �j�b�N�l�[����
#define NVRAM_CONFIG_COMMENT_LENGTH         26  // �R�����g��
#define NVRAM_CONFIG_FAVORITE_COLOR_MAX     16  // �D���ȐF�̍ő吔

// IPL2 �^�C�v
#define NVRAM_CONFIG_IPL2_TYPE_ADDRESS      0x001d
#define NVRAM_CONFIG_IPL2_TYPE_SIZE         1
#define NVRAM_CONFIG_IPL2_TYPE_NORMAL       0xff
#define NVRAM_CONFIG_IPL2_TYPE_EX_MASK      0x50

// ����ݒ�R�[�h
#define NVRAM_CONFIG_LANG_JAPANESE          0   // ���{��
#define NVRAM_CONFIG_LANG_ENGLISH           1   // �p��
#define NVRAM_CONFIG_LANG_FRENCH            2   // �t�����X��
#define NVRAM_CONFIG_LANG_GERMAN            3   // �h�C�c��
#define NVRAM_CONFIG_LANG_ITALIAN           4   // �C�^���A��
#define NVRAM_CONFIG_LANG_SPANISH           5   // �X�y�C����
#define NVRAM_CONFIG_LANG_CHINESE           6   // ������
#define NVRAM_CONFIG_LANG_HANGUL            7   // �؍���
#define NVRAM_CONFIG_LANG_CODE_MAX          8   // ����ݒ�R�[�h�̎�ސ�

// �ʏ�łł̑Ή�����r�b�g�}�b�v
#define NVRAM_CONFIG_LANG_BITMAP_NORMAL     ( (0x0001 << NVRAM_CONFIG_LANG_JAPANESE ) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_ENGLISH) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_FRENCH) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_GERMAN) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_ITALIAN) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_SPANISH) )

// �����łł̑Ή�����r�b�g�}�b�v
#define NVRAM_CONFIG_LANG_BITMAP_CHINESE    ( (0x0001 << NVRAM_CONFIG_LANG_ENGLISH ) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_FRENCH) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_GERMAN) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_ITALIAN) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_SPANISH) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_CHINESE) )


// �؍��łł̑Ή�����r�b�g�}�b�v
#define NVRAM_CONFIG_LANG_BITMAP_HANGUL     ( (0x0001 << NVRAM_CONFIG_LANG_JAPANESE ) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_ENGLISH) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_FRENCH) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_GERMAN) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_SPANISH) \
                                            | (0x0001 << NVRAM_CONFIG_LANG_HANGUL) )

/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/
// �a�����f�[�^(2byte)
typedef struct NVRAMConfigDate
{
    u8      month;                     // ��:01�`12
    u8      day;                       // ��:01�`31

}
NVRAMConfigDate;

// �j�b�N�l�[��(22byte)
typedef struct NVRAMConfigNickname
{
    u16     str[NVRAM_CONFIG_NICKNAME_LENGTH];  // �j�b�N�l�[���iUnicode(UTF16)�ōő�10�����A�I�[�R�[�h�Ȃ��j
    u8      length;                    // ������
    u8      rsv;

}
NVRAMConfigNickname;

// �R�����g(54byte)
typedef struct NVRAMConfigComment
{
    u16     str[NVRAM_CONFIG_COMMENT_LENGTH];   //�R�����g�iUnicode(UTF16)�ōő�26�����A�I�[�R�[�h�Ȃ��j
    u8      length;                    // ������
    u8      rsv;

}
NVRAMConfigComment;

// �I�[�i�[���(80byte)
typedef struct NVRAMConfigOwnerInfo
{
    u8      favoriteColor:4;           // �D���ȐF
    u8      rsv:4;                     // �\��
    NVRAMConfigDate birthday;          // �a����
    u8      pad;
    NVRAMConfigNickname nickname;      // �j�b�N�l�[��
    NVRAMConfigComment comment;        // �R�����g

}
NVRAMConfigOwnerInfo;


// IPL�p�ڊo�܂����v�f�[�^(6byte)
typedef struct NVRAMConfigAlarm
{
    u8      hour;                      // �A���[����:00�`23
    u8      minute;                    // �A���[����:00�`59
    u8      second;                    // �A���[���b:00�`59
    u8      pad;
    u16     enableWeek:7;              // �ǂ̗j���ɃA���[�����L�����̃t���O�ibit0:�� bit1:��..."1"���L���j
    u16     alarmOn:1;                 // �A���[��ON,OFF�i0:OFF, 1:ON�j
    u16     rsv:8;                     // �\��

}
NVRAMConfigAlarm;

// �^�b�`�p�l�����W�␳�f�[�^(12byte)
typedef struct NVRAMConfigTpCalibData
{
    u16     raw_x1;                    // ��P�L�����u���[�V�����_��TP�擾�lX
    u16     raw_y1;                    // ��P�L�����u���[�V�����_��TP�擾�lY
    u8      dx1;                       // ��P�L�����u���[�V�����_��LCD���W X
    u8      dy1;                       // ��P�L�����u���[�V�����_��LCD���W Y
    u16     raw_x2;                    // ��Q�L�����u���[�V�����_��TP�擾�lX
    u16     raw_y2;                    // ��Q�L�����u���[�V�����_��TP�擾�lY
    u8      dx2;                       // ��Q�L�����u���[�V�����_��LCD���W X
    u8      dy2;                       // ��Q�L�����u���[�V�����_��LCD���W Y

}
NVRAMConfigTpCalibData;

// �I�v�V�������(12byte)
typedef struct NVRAMConfigOption
{
    u16     language:3;                // ����R�[�h
    u16     agbLcd:1;                  // AGB���[�h�ŋN�����鎞�ɂǂ����LCD�ŋN�����邩�H�i0:TOP,1:BOTTOM�j
    u16     detectPullOutCardFlag:1;   // �J�[�h�����������ꂽ���Ƃ������t���O�B
    u16     detectPullOutCtrdgFlag:1;  // �J�[�g���b�W�����������ꂽ���Ƃ������t���O�B
    u16     autoBootFlag:1;            // �N���V�[�P���X�ŁA���j���[��~�Ȃ��Ŏ����N�����邩�ǂ����H
    u16     rsv:4;                     // �\��B
    u16     input_favoriteColor:1;     // �D���ȐF�����͂��ꂽ���H
    u16     input_tp:1;                // �^�b�`�p�l�����L�����u���[�V�������ꂽ���H�i  �V  )
    u16     input_language:1;          // ������͂����ꂽ���H�@       (0:���ݒ�, 1:�ݒ�ς�)
    u16     input_rtc:1;               // RTC�ݒ肪���ꂽ���H          (       �V�@�@�@�@�@ )
    u16     input_nickname:1;          // �j�b�N�l�[�������͂��ꂽ���H (       �V�@�@�@�@�@ )
    u8      timezone;                  // �^�C���]�[���i���݂̓��U�[�u�j
    u8      rtcClockAdjust;            // RTC�N���b�N�����l
    s64     rtcOffset;                 // RTC�ݒ莞�̃I�t�Z�b�g�l�i���[�U�[��RTC�ݒ��ύX����x�ɂ��̒l�ɉ����đ������܂��B�j

}
NVRAMConfigOption;


// �R���t�B�O�e��ݒ�f�[�^(112byte)
typedef struct NVRAMConfigData
{
    u8      version;                   // �t���b�V���i�[�f�[�^�t�H�[�}�b�g�̃o�[�W����
    u8      pad;
    NVRAMConfigOwnerInfo owner;        // �I�[�i�[���
    NVRAMConfigAlarm alarm;            // IPL�p�ڊo�܂����v�f�[�^
    NVRAMConfigTpCalibData tp;         // �^�b�`�p�l���L�����u���[�V�����f�[�^
    NVRAMConfigOption option;          // �I�v�V����

}
NVRAMConfigData;

// �R���t�B�O�e��ݒ�f�[�^��NVRAM�ۑ����t�H�[�}�b�g(116byte)
typedef struct NVRAMConfig
{
    NVRAMConfigData ncd;               // �R���t�B�O�e��ݒ�f�[�^
    u16     saveCount;                 // 0x00-0x7f �����[�v���ăJ�E���g���A�J�E���g�l���V�����f�[�^���L���B
    u16     crc16;                     // �R���t�B�O�e��ݒ�f�[�^�� 16bit CRC

}
NVRAMConfig;

// �R���t�B�O�g���e��ݒ�f�[�^(138byte)
typedef struct NVRAMConfigDataEx
{
    u8      version;                   // �g���R���t�B�O���̌^�o�[�W����
    u8      language;                  // �g������R�[�h( NVRAM_CONFIG_LANG_CHINESE �ȍ~�Ɋg�����ꂽ�l���i�[����� )
    u16     valid_language_bitmap;     // �L���Ȍ���R�[�h���������r�b�g�}�b�v
    u8      padding[256 - sizeof(NVRAMConfigData) - 4 - 4 - 2];
    // NVRAMConfigEx �̃T�C�Y�� NVRAM 1�y�[�W(256byte)�ɂȂ�悤�Ƀp�f�B���O
}
NVRAMConfigDataEx;

// �g�����ꂽ�R���t�B�O�e��ݒ�f�[�^��NVRAM�ۑ����t�H�[�}�b�g(256byte)
typedef struct NVRAMConfigEx
{
    NVRAMConfigData ncd;               // �R���t�B�O�e��ݒ�f�[�^
    u16     saveCount;                 // 0x00-0x7f �����[�v���ăJ�E���g���A�J�E���g�l���V�����f�[�^���L���B
    u16     crc16;                     // �R���t�B�O�e��ݒ�f�[�^�� 16bit CRC
    NVRAMConfigDataEx ncd_ex;          // �g���R���t�B�O�e��ݒ�f�[�^
    u16     crc16_ex;                  // �g���R���t�B�O�e��ݒ�f�[�^�� 16bit CRC

}
NVRAMConfigEx;


/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* NITRO_SPI_COMMON_USERINFO_TS_300_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
