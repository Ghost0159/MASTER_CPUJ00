/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SND - demos - seq
  File:     smfdefine.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: smfdefine.h,v $
  Revision 1.2  2006/01/18 02:12:27  kitase_hirotake
  do-indent

  Revision 1.1  2005/04/13 03:29:19  ida
  SND��NITRO-SDK�ֈڐ�

  Revision 1.2  2005/03/08 07:45:24  kyuma_koichi
  ����

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*                                                                  */
/* MIDI�f�[�^�\����`                                               */
/*                                                                  */
/*------------------------------------------------------------------*/

#ifndef SMFDEFINE_H_
#define SMFDEFINE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------*/
/* MIDI�K�i�ɂ������`                                               */
/*--------------------------------------------------------------------*/

/* ���4BIT��` */
#define MIDI_NOTEOFF            0x80   /* �m�[�g�I�t */
#define MIDI_NOTEON             0x90   /* �m�[�g�I�� */
#define MIDI_POLYPRESS          0xA0   /* �L�[�|���t�H�j�b�N�v���b�V���[ */
#define MIDI_CONTROLCHANGE      0xB0   /* �R���g���[���`�F���W */
#define MIDI_PROGRAMCHANGE      0xC0   /* �v���O�����`�F���W */
#define MIDI_CANNELPRESS        0xD0   /* �`�����l���v���b�V���[ */
#define MIDI_PITCHBEND          0xE0   /* �s�b�`�x���h */
#define MIDI_SYSTEMMESSAGE      0xF0   /* �V�X�e�����b�Z�[�W */

/* MIDI_SYSTEMMESSAGE */
#define MIDI_SYSX               0xF0   /* �G�N�X�N���[�V�u���b�Z�[�W */
/* COMMON MESSAGE */
#define MIDI_MTC                0xF1   /* MIDI�^�C���R�[�h */
#define MIDI_SONG_POSITION      0xF2   /* �\���O�|�W�V�����|�C���^ */
#define MIDI_SONG_SELECT        0xF3   /* �\���O�Z���N�g */
#define MIDI_UNDEFINED_F4       0xF4   /* ����` */
#define MIDI_UNDEFINED_F5       0xF5   /* ����` */
#define MIDI_TUNE_REQUEST       0xF6   /* �`���[�����N�G�X�g */
#define MIDI_END_OF_SYSX        0xF7   /* �G���h�I�u�G�N�X�N���[�V�u */
/* REALTIME MESSAGE */
#define MIDI_TIMING_CLOCK       0xF8   /* �^�C�~���O�N���b�N */
#define MIDI_UNDEFINED_F9       0xF9   /* ����` */
#define MIDI_START              0xFA   /* �X�^�[�g */
#define MIDI_CONTINUE           0xFB   /* �R���e�B�j���[ */
#define MIDI_STOP               0xFC   /* �X�g�b�v */
#define MIDI_UNDEFINED_FD       0xFD   /* ����` */
#define MIDI_ACTIVESENSING      0xFE   /* �A�N�e�B�u�Z���V���O */
#define MIDI_SYSTEMRESET        0xFF   /* �V�X�e�����Z�b�g */

/* Control Change */
/* continuos cc */
#define MIDI_CC_BANKSELECT_MSB  0x00   /*   0: �o���N�Z���N�g */
#define MIDI_CC_BANKSELECT_LSB  0x20   /*  32: �o���N�Z���N�g */
#define MIDI_CC_MODURATION      0x01   /*   1: ���W�����[�V�����z�C�[�� */
#define MIDI_CC_BREATH_CONTROL  0x02   /*   2: �u���X�R���g���[�� */
#define MIDI_CC_FOOT_CONTROL    0x04   /*   4: �t�b�g�R���g���[�� */
#define MIDI_CC_PORTAMENT_TIME  0x05   /*   5: �|���^�����g�^�C�� */
#define MIDI_CC_VOLUME          0x07   /*   7: �{�����[�� */
#define MIDI_CC_BALANCE         0x08   /*   8: �o�����X */
#define MIDI_CC_PAN             0x0A   /*  10: �p�� */
#define MIDI_CC_EXPRESSION      0x0B   /*  11: �G�N�X�v���b�V���� */
#define MIDI_CC_GENERAL1        0x10   /*  16: �ėp�P */
#define MIDI_CC_GENERAL2        0x11   /*  17: �ėp�Q */
#define MIDI_CC_GENERAL3        0x12   /*  18: �ėp�R */
#define MIDI_CC_GENERAL4        0x13   /*  19: �ėp�S */
/* switched cc */
#define MIDI_CC_HOLD1           0x40   /*  64: �z�[���h(�_���p�[) */
#define MIDI_CC_PORTAMENT       0x41   /*  65: �|���^�����g�X�C�b�` */
#define MIDI_CC_SOSTENUTO       0x42   /*  66: �\�X�e�k�[�g */
#define MIDI_CC_SOFT_PEDAL      0x43   /*  67: �\�t�g�y�_�� */
#define MIDI_CC_LEGATO          0x44   /*  68: ���K�[�g�t�b�g�X�C�b�` */
#define MIDI_CC_HOLD2           0x45   /*  69: �z�[���h(�t���[�Y) */
/* sound controler cc */
#define MIDI_CC_SOUND_CONTROL1  0x46   /*  70: �T�E���h�R���g���[���[�P */
#define MIDI_CC_SOUND_CONTROL2  0x47   /*  71: �T�E���h�R���g���[���[�Q */
#define MIDI_CC_SOUND_CONTROL3  0x48   /*  72: �T�E���h�R���g���[���[�R */
#define MIDI_CC_SOUND_CONTROL4  0x49   /*  73: �T�E���h�R���g���[���[�S */
#define MIDI_CC_SOUND_CONTROL5  0x4A   /*  74: �T�E���h�R���g���[���[�T */
#define MIDI_CC_SOUND_CONTROL6  0x4B   /*  75: �T�E���h�R���g���[���[�U */
#define MIDI_CC_SOUND_CONTROL7  0x4C   /*  76: �T�E���h�R���g���[���[�V */
#define MIDI_CC_SOUND_CONTROL8  0x4D   /*  77: �T�E���h�R���g���[���[�W */
#define MIDI_CC_SOUND_CONTROL9  0x4E   /*  78: �T�E���h�R���g���[���[�X */
#define MIDI_CC_SOUND_CONTROL10 0x4F   /*  79: �T�E���h�R���g���[���[10 */
#define MIDI_CC_SOUND_VARIATION 0x46   /*  70: �T�E���h�o���G�[�V���� */
#define MIDI_CC_RESONANCE       0x47   /*  71: ���]�i���X */
#define MIDI_CC_RELEASE_TIME    0x48   /*  72: �����[�X�^�C�� */
#define MIDI_CC_ATTACK_TIME     0x49   /*  73: �A�^�b�N�^�C�� */
#define MIDI_CC_BRIGHTNESS      0x4A   /*  74: �t�B���^�[�J�b�g�I�t */
#define MIDI_CC_DECAY_TIME      0x4B   /*  75: �f�B�P�C�^�C�� */
#define MIDI_CC_VIBRATO_RATE    0x4C   /*  76: �r�u���[�g���[�g(�X�s�[�h) */
#define MIDI_CC_VIBRATO_DEPTH   0x4D   /*  77: �r�u���[�g�f�v�X */
#define MIDI_CC_VIBRATO_DELAY   0x4E   /*  78: �r�u���[�g�f�B���C */
/* portament_control */
#define MIDI_CC_PORTAMENT_CTRL  0x54   /*  84: �|���^�����g�R���g���[�� */
/* effect controler cc */
#define MIDI_CC_EFFECT_CONTROL1 0x0C   /*  12: �G�t�F�N�g�R���g���[���P */
#define MIDI_CC_EFFECT_CONTROL2 0x0D   /*  13: �G�t�F�N�g�R���g���[���Q */
#define MIDI_CC_REVERB_SEND     0x5B   /*  91: ���o�[�u�Z���h���x�� */
#define MIDI_CC_TREMOLO_DEPTH   0x5C   /*  92: �g�������f�v�X */
#define MIDI_CC_CHORUS_SEND     0x5D   /*  93: �R�[���X�Z���h���x�� */
#define MIDI_CC_DELAY_SEND      0x5E   /*  94: �f�B���C�Z���h���x�� */
#define MIDI_CC_PHASER_DEPTH    0x5F   /*  95: �t�F�C�U�[�f�v�X */
/* parameter control */
#define MIDI_CC_DATA_ENTRY_MSB  0x06   /*   6: �f�[�^�G���g���[ */
#define MIDI_CC_DATA_ENTRY_LSB  0x26   /*  38: �f�[�^�G���g���[ */
#define MIDI_CC_DATA_INCREMENT  0x60   /*  96: �C���N�������g */
#define MIDI_CC_DATA_DECREMENT  0x61   /*  97: �f�N�������g */
#define MIDI_CC_RPN_MSB         0x65   /* 101: RPN MSB */
#define MIDI_CC_RPN_LSB         0x64   /* 100: RPN LSB */
#define MIDI_CC_NRPN_MSB        0x63   /*  99: NRPN MSB */
#define MIDI_CC_NRPN_LSB        0x62   /*  98: NRPN LSB */
/* mode messege */
#define MIDI_CC_ALL_SOUND_OFF   0x78   /* 120: �I�[���T�E���h�I�t */
#define MIDI_CC_RESET_ALL_CTRL  0x79   /* 121: ���Z�b�g�I�[���R���g���[�� */
#define MIDI_CC_ROCAL_CONTROL   0x7A   /* 122: ���[�J���R���g���[�� */
#define MIDI_CC_ALL_NOTE_OFF    0x7B   /* 123: �I�[���m�[�g�I�t */
#define MIDI_CC_OMNI_ON         0x7C   /* 124: �I���j���[�h�I�� */
#define MIDI_CC_OMNI_OFF        0x7D   /* 125: �I���j���[�h�I�t */
#define MIDI_CC_MONO_MODE       0x7E   /* 126: ���m���[�h�I�� */
#define MIDI_CC_POLY_MODE       0x7F   /* 127: �|�����[�h�I�� */
/* RPN */
#define MIDI_RPN_PITCHBEND_SENS 0x0000 /* RPN: �s�b�`�x���h�Z���V�e�B�r�e�B */
#define MIDI_RPN_FINE_TUNE      0x0001 /* RPN: �t�@�C���`���[�� */
#define MIDI_RPN_COASE_TUNE     0x0002 /* RPN: �R�[�X�`���[�� */
#define MIDI_RPN_TUNING_PROGRAM 0x0003 /* RPN: �`���[�j���O�v���O�����Z���N�g */
#define MIDI_RPN_TUNING_BANK    0x0004 /* RPN: �`���[�j���O�o���N�Z���N�g */
#define MIDI_RPN_MODURATION_SENS 0x0005 /* RPN: ���W�����[�V�����Z���V�e�B�r�e�B */



/*--------------------------------------------------------------------*/
/* Standard MIDI File �t�H�[�}�b�g�ɂ������`                        */
/*--------------------------------------------------------------------*/

#define SMF_FORMAT0             0      /* SMF format0 */
#define SMF_FORMAT1             1      /* SMF format1 */
#define SMF_FORMAT2             2      /* SMF format2 */

#define SMF_NOTEOFF             0x80   /* �m�[�g�I�t */
#define SMF_NOTEON              0x90   /* �m�[�g�I�� */
#define SMF_POLYPRESS           0xA0   /* �L�[�|���t�H�j�b�N�v���b�V���[ */
#define SMF_CONTROLCHANGE       0xB0   /* �R���g���[���`�F���W */
#define SMF_PROGRAMCHANGE       0xC0   /* �v���O�����`�F���W */
#define SMF_CANNELPRESS         0xD0   /* �`�����l���v���b�V���[ */
#define SMF_PITCHBEND           0xE0   /* �s�b�`�x���h */
#define SMF_SYSX                0xF0   /* �G�N�X�N���[�V�u���b�Z�[�W */
#define SMF_MTC                 0xF1   /* MIDI�^�C���R�[�h */
#define SMF_SONGPOSITION        0xF2   /* �\���O�|�W�V�����|�C���^ */
#define SMF_SONGSELECT          0xF3   /* �\���O�Z���N�g */
#define SMF_UNDEFINED_F4        0xF4   /* ����` */
#define SMF_UNDEFINED_F5        0xF5   /* ����` */
#define SMF_TUNEREQUEST         0xF6   /* �`���[�����N�G�X�g */
#define SMF_ENDOFSYSX           0xF7   /* �G���h�I�u�G�N�X�N���[�V�u */
#define SMF_TIMINGCLOCK         0xF8   /* �^�C�~���O�N���b�N */
#define SMF_UNDEFINED_F9        0xF9   /* ����` */
#define SMF_START               0xFA   /* �X�^�[�g */
#define SMF_CONTINUE            0xFB   /* �R���e�B�j���[ */
#define SMF_STOP                0xFC   /* �X�g�b�v */
#define SMF_UNDEFINED_FD        0xFD   /* ����` */
#define SMF_ACTIVESENSING       0xFE   /* �A�N�e�B�u�Z���V���O */
#define SMF_META                0xFF   /* ���^�C�x���g */

#define SMF_META_SEQ_NUM        0x00   /* �V�[�P���X�ԍ� */
#define SMF_META_TEXT           0x01   /* �e�L�X�g�C�x���g */
#define SMF_META_COPYRIGHT      0x02   /* ���쌠�\�� */
#define SMF_META_SEQ_NAME       0x03   /* �V�[�P���X���^�g���b�N�� */
#define SMF_META_INST_NAME      0x04   /* �y�햼 */
#define SMF_META_LYRIC          0x05   /* �̎� */
#define SMF_META_MARKER         0x06   /* �}�[�J�[ */
#define SMF_META_QUE            0x07   /* �L���[�|�C���g */
#define SMF_META_PREFIX         0x20   /* �`�����l���v���t�B�b�N�X */
#define SMF_META_ENDOFTRACK     0x2F   /* �G���h�I�u�g���b�N */
#define SMF_META_TEMPO          0x51   /* �Z�b�g�e���| */
#define SMF_META_SMPTE          0x54   /* SMPTE�I�t�Z�b�g */
#define SMF_META_BEAT           0x58   /* ���q�^���g���m�[���ݒ� */
#define SMF_META_KEY            0x59   /* �� */
#define SMF_META_OTHER          0x7F   /* �V�[�P���T�ŗL���^�C�x���g */


/*------------------------------------------------------------------*/
/* �\���̒�`                                                       */
/*------------------------------------------------------------------*/

/* SMF �̃`�����N�^�O�\���� */
typedef struct tSMFChunkTag
{
    char    chunkType[4];              /* �`�����N�^�C�v������(MThd, MTrk) */
    unsigned long length;              /* �`�����N�̃f�[�^�� */
}
SMFChunkTag;

/* �w�b�_�`�����N�\���� */
typedef struct tSMFHeaderChunk
{
    unsigned short format;             /* �t�@�C���t�H�[�}�b�g */
    unsigned short ntracks;            /* �g���b�N�� */
    unsigned short division;           /* 4����������\ */
}
SMFHeaderChunk;


#ifdef __cplusplus
}      /* extern "C" */
#endif

#endif // SMFDEFINE_H_
