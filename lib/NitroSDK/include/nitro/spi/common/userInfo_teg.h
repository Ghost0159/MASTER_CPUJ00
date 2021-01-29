/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SPI - include
  File:     userInfo_teg.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: userInfo_teg.h,v $
  Revision 1.5  2006/01/18 02:12:38  kitase_hirotake
  do-indent

  Revision 1.4  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.3  2004/09/03 13:03:11  takano_makoto
  �X�y���~�X�C�� NitroConfigData -> NVRAMConfigData.

  Revision 1.2  2004/09/03 04:29:15  terui
  NVRAM�����[�U�[���Ɋւ���\���̒�`�����ɔ����C���B

  Revision 1.1  2004/08/25 09:20:43  terui
  Initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITRO_SPI_COMMON_USERINFO_TEG_H_
#define NITRO_SPI_COMMON_USERINFO_TEG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*/

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
// �R���t�B�O�ݒ�f�[�^�t�H�[�}�b�g�o�[�W����
#define NVRAM_CONFIG_DATA_VERSION           0

// �R���t�B�O���̊e��ݒ�l
#define NVRAM_CONFIG_BOOT_GAME_LOG_NUM      8   // �u�[�g�Q�[�������̕ۑ���
#define NVRAM_CONFIG_NICKNAME_LENGTH        8   // �j�b�N�l�[����

// ����ݒ�R�[�h
#define NVRAM_CONFIG_LANG_JAPANESE          0   // ���{��
#define NVRAM_CONFIG_LANG_ENGLISH           1   // �p��
#define NVRAM_CONFIG_LANG_FRENCH            2   // �t�����X��
#define NVRAM_CONFIG_LANG_GERMAN            3   // �h�C�c��
#define NVRAM_CONFIG_LANG_ITALIAN           4   // �C�^���A��
#define NVRAM_CONFIG_LANG_SPANISH           5   // �X�y�C����
#define NVRAM_CONFIG_LANG_CODE_MAX          6   // ����ݒ�R�[�h�̎�ސ�

/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/
// �j�b�N�l�[��(20byte)
typedef struct NVRAMConfigNickname
{
    u16     name[NVRAM_CONFIG_NICKNAME_LENGTH]; // �j�b�N�l�[���iUNICODE�ōő�8�����A�I�[�R�[�h�Ȃ��j
    u16     length;                    // ������
    u16     padding;

}
NVRAMConfigNickname;

// ���ʃR�[�h(4byte)
typedef enum NVRAMConfigSexCode
{
    NVRAM_CONFIG_SEX_MALE = 0,         // �j
    NVRAM_CONFIG_SEX_FEMALE,           // ��
    NVRAM_CONFIG_SEX_CODE_MAX
}
NVRAMConfigSexCode;

// ���t�^�R�[�h(4byte)
typedef enum NVRAMConfigBloodType
{
    NVRAM_CONFIG_BLOOD_A = 0,          // A �^
    NVRAM_CONFIG_BLOOD_B,              // B �^
    NVRAM_CONFIG_BLOOD_AB,             // AB�^
    NVRAM_CONFIG_BLOOD_O,              // O �^
    NVRAM_CONFIG_BLOOD_TYPE_MAX
}
NVRAMConfigBloodType;

// ���t�f�[�^(4byte)
typedef struct NVRAMConfigDate
{
    u16     year;                      //�N:1800�`2100
    u8      month;                     //��:01�`12
    u8      day;                       //��:01�`31

}
NVRAMConfigDate;

// �I�[�i�[���(32byte)
typedef struct NVRAMConfigOwnerInfo
{
    NVRAMConfigNickname nickname;      // �j�b�N�l�[��
    NVRAMConfigSexCode sex;            // ����
    NVRAMConfigBloodType bloodType;    // ���t�^
    NVRAMConfigDate birthday;          // ���N����

}
NVRAMConfigOwnerInfo;


// �N���Q�[�������f�[�^(36byte)
typedef struct NVRAMConfigBootGameLog
{
    u32     gameCode[NVRAM_CONFIG_BOOT_GAME_LOG_NUM];   // �ߋ��ɋN�������Q�[���̃Q�[���������X�g�i�����O�o�b�t�@�j
    u16     top;                       // �ŏI���O�̃C���f�b�N�X�ԍ�
    u16     num;                       // ���O�̌�

}
NVRAMConfigBootGameLog;                // 36byte

// �^�b�`�p�l�����W�␳�f�[�^(12byte)
typedef struct NVRAMConfigTpCData
{
    u16     calib_data[6];             // �L�����u���[�V�������

}
NVRAMConfigTpCData;


// �R���t�B�O�e��ݒ�f�[�^(96byte)
typedef struct NVRAMConfigData
{
    u8      version;                   // �t���b�V���i�[�f�[�^�t�H�[�}�b�g�̃o�[�W����
    u8      timezone;                  // �^�C���]�[��
    u16     agbLcd;                    // AGB���[�h�ŋN�����鎞�ɂǂ����LCD�ŋN�����邩�H�i0:TOP,1:BOTTOM�j
    u32     rtcOffset;                 // RTC�ݒ莞�̃I�t�Z�b�g�l�i���[�U�[��RTC�ݒ��ύX����x�ɂ��̒l�ɉ����đ������܂��B�j
    u32     language;                  // ����R�[�h
    NVRAMConfigOwnerInfo owner;        // �I�[�i�[���
    NVRAMConfigTpCData tp;             // �^�b�`�p�l���L�����u���[�V�����f�[�^
    NVRAMConfigBootGameLog bootGameLog; // �N���Q�[������

}
NVRAMConfigData;

// �R���t�B�O�e��ݒ�f�[�^��NVRAM�ۑ����t�H�[�}�b�g(100byte)
typedef struct NVRAMConfig
{
    NVRAMConfigData ncd;               // �R���t�B�O�e��ݒ�f�[�^
    u16     saveCount;                 // 0x00-0x7f�����[�v���ăJ�E���g���A�J�E���g�l���V�����f�[�^���L���B
    u16     crc16;                     // �R���t�B�O�e��ݒ�f�[�^��16bitCRC

}
NVRAMConfig;


/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* NITRO_SPI_COMMON_USERINFO_TEG_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
