/*
  Project:  NitroSDK - wireless_shared - demos - wh
  File:     wh.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wh.c,v $
  Revision 1.70  2007/10/19 05:12:41  yosizaki
  fix about auto-reconnecting of WC.
  fix WcFinish().

  Revision 1.69  2007/10/10 00:22:13  yosizaki
  fix about DataSharing-child.

  Revision 1.68  2007/10/09 08:56:40  seiki_masashi
  sBssDesc �Ɏ��̂��������A�L���b�V���Z�[�t�ȗ̈�Ƃ��Ďg�p����悤�ɏC��

  Revision 1.67  2007/10/04 11:35:24  okubata_ryoma
  small fix

  Revision 1.66  2007/10/04 03:07:11  seiki_masashi
  WM_StartScanEx ���g�p����悤�ɕύX

  Revision 1.65  2007/10/04 01:51:29  seiki_masashi
  macAddr ������ NULL �����e����悤�ɕύX
  WC ���g�p�����ۂɎ����ڑ��֌W�ŏ�ԕs�������N�����Ă����̂��C��

  Revision 1.64  2007/10/03 05:38:37  yosizaki
  add WC compatible layer.

  Revision 1.63  2007/10/02 09:06:05  yosizaki
  small fix about const-cast.

  Revision 1.62  2007/04/16 02:14:45  seiki_masashi
  �R�����g�̌�L�̏C��

  Revision 1.61  2007/02/20 00:28:08  kitase_hirotake
  indent source

  Revision 1.60  2006/10/26 06:56:01  kitase_hirotake
  WH_SetIndCallback �̒ǉ�

  Revision 1.59  2006/10/25 02:03:19  kitase_hirotake
  WH_Initialize ���d�����ČĂяo���Ȃ��悤�C��

  Revision 1.58  2006/08/29 03:39:56  yosizaki
  fix WH_StateOutStartChildMP

  Revision 1.57  2006/08/17 10:46:43  okubata_ryoma
  DataSharing�̐ؒf��������̏C��

  Revision 1.56  2006/08/14 12:27:38  seiki_masashi
  WH_ChildConnectAuto �� WH_EndScan �ŃL�����Z�������ꍇ�̕s����C��

  Revision 1.55  2006/07/19 09:07:33  yosizaki
  add WH_SetSsid.

  Revision 1.54  2006/07/19 04:08:33  yosizaki
  fix comment.

  Revision 1.53  2006/07/06 11:58:02  okubata_ryoma
  undo

  Revision 1.51  2006/06/14 01:42:20  okubata_ryoma
  Reset����WM_EndDataSharing�֐����Ăԏ�����ǉ�

  Revision 1.50  2006/04/10 13:19:21  yosizaki
  support WH_MP_FREQUENCY.

  Revision 1.49  2006/03/13 07:11:28  yosizaki
  fix assertion code.

  Revision 1.48  2006/02/20 08:05:00  seiki_masashi
  WM_IsValidGameInfo �֐����g�p����悤�ɕύX

  Revision 1.47  2006/02/20 02:37:17  seiki_masashi
  WMGameInfo.version ��p�~���AmagicNumber �� ver ��ǉ�

  Revision 1.46  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.45  2006/01/10 02:51:04  adachi_hiroaki
  16�i�\�L�̏o�͂ɂ͑S�Ė����I��0x�������B/([^xX])(%\d*[xX][^hH])/ -> \10x\2

  Revision 1.44  2005/11/28 05:00:46  seiki_masashi
  WM_SetMPParaneter �֐��̒ǉ��ɑΉ�

  Revision 1.43  2005/11/01 00:37:18  adachi_hiroaki
  WM_STATECODE�̒ǉ�����\���e�[�u���ɂ��ǉ�

  Revision 1.42  2005/10/28 11:25:35  seiki_masashi
  WM_STATECODE_DISCONNECTED_FROM_MYSELF �̒ǉ��ɑΉ�
  WM_STATECODE_PORT_INIT �̒ǉ��ɑΉ�

  Revision 1.41  2005/07/29 03:44:26  seiki_masashi
  WM_STATECODE_INFORMATION �̒ǉ�

  Revision 1.40  2005/06/27 11:10:11  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.39  2005/06/07 05:45:48  seiki_masashi
  Key Sharing �Ɋւ�����ʏ�����ጸ���邽�߂̕ύX

  Revision 1.38  2005/04/26 02:35:02  terui
  Fix comment

  Revision 1.37  2005/03/25 03:05:23  seiki_masashi
  WEPKey �ݒ�֐���ݒ肷�� WH_Set{Parent,Child}WEPKeyGenerator ��ǉ�

  Revision 1.36  2005/03/04 09:26:10  takano_makoto
  �e�@�r�[�R����MB�t���O�̃`�F�b�N��ǉ�

  Revision 1.35  2005/03/04 09:18:56  takano_makoto
  WH_SetUserGameInfo��ǉ�

  Revision 1.34  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.33  2005/02/18 10:38:04  yosizaki
  fix around hidden warnings.

  Revision 1.32  2005/02/08 01:22:38  takano_makoto
  WH_SetReceiver�̐��۔���̕s��C��

  Revision 1.31  2005/01/11 07:48:59  takano_makoto
  fix copyright header.

  Revision 1.30  2005/01/07 12:04:47  terui
  �����s�\�ȃG���[�ʒm����M�����ۂ̏�����ǉ��B

  Revision 1.29  2005/01/07 06:15:26  takano_makoto
  WM_StartConnect�̃R�[���o�b�N�G���[������ǉ�

  Revision 1.28  2004/12/22 02:48:43  terui
  �s�N�g�`���b�g�T�[�`�ɑΉ�

  Revision 1.27  2004/12/20 08:31:27  takano_makoto
  WH_Initialize���g�p���ď�����������Z�k����悤�ɕύX�B

  Revision 1.26  2004/12/20 07:17:20  takano_makoto
  WHReceiver��WHReceiverFunc�ɕύX�A������OS_Alloc���g�p����̂�r���A�A�v�����ɐݒ肷��p�����[�^��wh_config.h�Ƃ��ĕ���

  Revision 1.25  2004/12/09 08:14:00  takano_makoto
  �d�g�g�p���̍ł��Ⴂ�`�����l���������������ꍇ�ɁA�������g�p����悤�ɏC��

  Revision 1.24  2004/11/26 02:38:09  takano_makoto
  �ڑ����s���ɂ�WM_SYSSTATE_CONNECT_FAIL�ɑJ�ڂ���悤�ɏC��

  Revision 1.23  2004/11/16 03:01:11  takano_makoto
  WH_GetCurrentAid�֐���ǉ�

  Revision 1.22  2004/11/11 03:50:58  seiki_masashi
  �q�@���� WM_StartDataSharing �� aidBitmap �������C��

  Revision 1.21  2004/11/10 06:13:38  takano_makoto
  WM_EndKeySharing�̐��������WM_ERRCODE_OPERATING�ɏC��

  Revision 1.20  2004/11/08 02:46:17  takano_makoto
  WM_EndScan�̑��d�Ăіh�~

  Revision 1.19  2004/11/05 04:27:40  sasakis
  ���r�[��ʁA�e�@�I����ʂ̒ǉ��ƁA����ɕK�v�ȉ����iscan�֘A�Ȃǁj�B

  Revision 1.18  2004/11/02 19:36:19  takano_makoto
  WH_StartMeasureChannel, WH_EndScan�̕Ԃ�l��BOOL�ɕύX

  Revision 1.17  2004/11/02 10:08:05  takano_makoto
  WH_Finalize�̏�ԕύX�^�C�~���O��ύX

  Revision 1.16  2004/11/02 10:03:10  seiki_masashi
  �X�L�������ɒ��f�����ۂ̕s����C��

  Revision 1.15  2004/11/01 09:32:12  takano_makoto
  �f�o�b�O�o�͂�ύX

  Revision 1.14  2004/11/01 09:19:57  takano_makoto
  WH��ԑJ�ڂ̌�����

  Revision 1.13  2004/11/01 06:28:10  seiki_masashi
  �R�����g�̒���

  Revision 1.12  2004/11/01 02:48:56  takano_makoto
  WH_StateInInitialize���ł̏�ԕύX�ʒu���ړ��BWH_StateInParentMP��WM_EndMP���Ƃ΂��V�[�P���X���폜�B

  Revision 1.11  2004/10/29 07:27:56  takano_makoto
  ������ԕύX�ʒu�ύX

  Revision 1.10  2004/10/29 02:35:30  takano_makoto
  �f�o�b�O�o�͏C��
  
  Revision 1.9  2004/10/29 02:16:34  takano_makoto
  WH_StartScan�̈����ύX
  
  Revision 1.8  2004/10/29 02:05:28  takano_makoto
  �e�@���X�g���쐬�ł���悤�ɁAScan�֐����O���痘�p�\�Ȃ悤�ɕύX
  
  Revision 1.7  2004/10/28 10:38:31  terui
  �e�@�Ƃ��Ă̏I�������������C���B
  WM_StartConnect�ւ̃R�[���o�b�N�����ł̃G���[�`�F�b�N�������C���B

  Revision 1.6  2004/10/28 07:11:20  takano_makoto
  WH_Connect�̊֐�����ύX

  Revision 1.5  2004/10/27 02:31:31  takano_makoto
  ����������MeasureChannel�n�֐��̓���
  WM_ERRCODE_OPERATING�ő҂ׂ�������WM_ERRCODE_SUCCESS�Ŕ��肵�Ă����s��C��
  ���̑��s��̏C��

  Revision 1.4  2004/10/25 05:43:33  seiki_masashi
  WM_APIID_* �̕�����ϊ��֐����ŐV�� wm.h �ɍ����悤�X�V
  WM_GetAllowedChannel �� 0x8000 ��Ԃ����ꍇ�̏����̒ǉ�
  �R�����g�̕ύX

  Revision 1.3  2004/10/25 02:18:17  seiki_masashi
  ��M�o�b�t�@�T�C�Y�̎Z�o�̕s��C��
  �e�q�ő呗�M�T�C�Y�� WH_{PARENT,CHILD}_MAX_SIZE �ɓ���
  WM_StartDataSharing �� aidBitmap �� WH_CHILD_MAX �𔽉f

  Revision 1.2  2004/10/22 07:36:55  sasakis
  �G���[�\���p�̕����񂪑���Ȃ������̂Œǉ��B

  Revision 1.1  2004/10/21 00:41:50  yosizaki
  Initial upload.

  $NoKeywords$
 */

#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/cht.h>

#include "wh.h"

/*
  wh.c : Wireless Manager �֘A Wrapper

  �E ���[�U�����������

  WM API �Ŏ�������Ă���̂́A���ꉻ�̒��x���Ⴂ����

  - ��ʓI�� MP �ʐM
  - �f�[�^�V�F�A�����O
  - �L�[�V�F�A�����O

  ��3�ʂ�̒ʐM�l���ł��B
  �����͊e�X�ʂ̂��̂Ƃ����킯�ł͂Ȃ��A�Ⴆ�΃L�[�V�F�A�����O��
  �f�[�^�V�F�A�����O�𗘗p������@�̈�ŁA�f�[�^�V�F�A�����O��
  �P�� MP �ʐM���g�����f�[�^���L�̎d�g�݂̂��Ƃł����A�s���ケ��
  3�ɋ�ʂ��ĉ�����Ă��܂��B

  �u��ʓI MP �ʐM�v�Ƃ����̂� Multi Poll �v���g�R�����̂܂܂̃��x���ł�
  �ʐM���[�h�̂��Ƃ��w���A�ȒP�ɂ܂Ƃ߂��

  (1) �e�@����S�Ă̎q�@�փf�[�^�̃u���[�h�L���X�g���s��
  (2) �e�q�@���甽����Ԃ�
  (3) �e�@���ʐM�T�C�N���̏I����u���[�h�L���X�g�ō��m

  �Ƃ����X�e�b�v����̃T�C�N���Ƃ��ĒʐM���s���A�Ƃ������̂ł��B

  �f�[�^�V�F�A�����O�Ƃ����̂́A�e�@�������ɐڑ�����Ă���q�@�S�Ă���
  �f�[�^���z�o���i��Ō���(2)�ł̏����j�A�����̕����܂߈�ɂ܂Ƃ߂�
  ���L�f�[�^�Ƃ��Ďq�@�S�Ăɔz��i��Ō���(1)�ł̏����j�A�Ƃ������[�h��
  ���Ƃ������܂��B

  �L�[�V�F�A�����O�Ƃ����̂́A�f�[�^�V�F�A�����O�ŋ��L����f�[�^��
  �e�@�̃L�[�f�[�^�̔z��ɂȂ��Ă���A�Ƃ������̂ł��B


  1) �ʐM�����̊T�v

  Wireless LAN �Ŏ�������Ă���@�\�ɂ��āA�����̋�̓I�ȍ�Ƃ�
  ���ۂɍs���Ă���̂́A ARM7�v���Z�b�T�ł��B
  �]���� ARM9 ������̑���M�v���Ȃǂ͑S�� ARM7 ��ʂ��K�v������A����
  ���߂� ARM9 ���̂قƂ�ǂ� WM �֌W API �͔񓯊��ɂȂ��Ă��āA���悻

  1. ARM7 �ւ̗v���� FIFO �ɏ����o���֐����Ă�
  2. ARM7 ���Ԃ������ʂ� FIFO ����󂯎�����V�X�e�����i1.�Ŏw�肵���j
  �R�[���o�b�N�֐����Ă�

  �Ƃ��������ꂪ��{�I�ȏ����ƂȂ�܂��B

  ���̃T���v���̎����ł́A���̏����ɕK�v�ȂQ�̊֐����P�Z�b�g�Ƃ���
  Wireless LAN �@�\�́u��ԁv��\�����Ă�����̂ƍl���A���́u��ԁv��
  �A������邱�ƂŒʐM�������s���Ă��܂��i�Ȍ�A��������ɋ�ʂ���
  �����Ō��� Wireless LAN �́u��ԁv���w�������ꍇ�A�uWH��ԁv�Ƃ���
  ���t���g�����Ƃɂ��܂��j�B

  ���̃\�[�X�ŊY������֐��̖��O�͊eWH��ԂɑΉ����Ă���A

  WH_StateInXXXXXX   - WH��� XXXXXX �̊J�n�֐�
  WH_StateOutXXXXXX  - WH��� XXXXXX �̏I���ʒm���󂯎��R�[���o�b�N�֐�

  �̂悤�ɂȂ��Ă��܂��B


  2) WH��ԊԂ̑J��

  �����I�Ɏ���WH��ԂɑJ�ڂ���ׂ�WH��Ԃ̃R�[���o�b�N�֐��ł́A������
  �Ō�Ŏ��Ɉڍs���ׂ�WH��Ԃ̊J�n�֐����Ăяo���悤�ɂ��Ă��܂�
  �i��{�I�ɏ�ԑJ�ڂ̐i�s�̓\�[�X���ŏォ�牺�ւƐi�ނ悤�ɂȂ��Ă��܂��j�B

  �S�Ă�WH��Ԗ��Ƃ���瑊�݂̃V�[�P���X���ł̑J�ڊ֌W�́A�ȉ��̂悤��
  �Ȃ�܂��B

  - �������V�[�P���X
  �iWH_Initialize �֐��j
  ��
  Initialize

  - �e�@�ʐM�����V�[�P���X
  �iWH_Connect �֐��j
  ��
  SetParentParam
  ��
  StartParent
  ��
  StartParentMP
  ��
  (**)
  ��
  StartParentKeyShare

  ���F WH_Connect �� MP �ڑ����[�h�i�܂��� DS �ڑ����[�h�j���w�肵��
  ������A(**) �ŏI������B

  - �e�@�I���V�[�P���X
  �iWH_Finalize �֐��j
  ��
  �iEndParentKeyShare�j
  ��
  EndParentMP
  ��
  EndParent

  - �q�@�ʐM�����V�[�P���X
  �iWH_Connect �֐��j
  ��
  StartScan
  ��
  EndScan
  ��
  StartChild
  ��
  StartChildMP
  ��
  (**)
  ��
  StartChildKeyShare

  ���F WH_Connect �� MP �ڑ����[�h�i�܂��� DS �ڑ����[�h�j���w�肵��
  ������A(**) �ŏI������B

  - �q�@�I���V�[�P���X
  �iWH_Finalize �֐��j
  ��
  �iEndChildKeyShare�j
  ��
  EndChildMP
  ��
  EndChild

  �����Œ��ӂ��K�v�Ȃ̂́A�eWH��Ԃ̃R�[���o�b�N�֐��́A����WH��Ԃ̏I������
  �Ă΂�邾���Ƃ͌���Ȃ��A�Ƃ������Ƃł��B

  �Ⴆ�΁A WH��� StartParentMP �̃R�[���o�b�N�ł��� WH_StateOutStartParentMP
  �Ƃ����֐��́A StartMP �̏������I�������������łȂ��A MP �V�[�P���X����ʂ�
  �����������Ȃǂɂ��Ă΂�܂��i�֐��{�̂̃R�����g���Q�Ƃ��ĉ������j�B


  3) ���ۂ̑���M���

  ���̃\�[�X�ł́A��ʓI�� MP �ʐM�A�f�[�^�V�F�A�����O�A�L�[�V�F�A�����O��
  3�ʂ�̒ʐM�l�����������Ă��܂����A���̂��������ꂪ�s���邩�ŁA�f�[�^��
  ���M�E��M�̕��@���S������Ă��܂��B

  ��ʓI�� MP �ʐM�̏ꍇ�A���������Ɉȉ��̊֐��ŃR�[���o�b�N��ݒ肷��K�v��
  ����܂��B

  WH_SetReceiver(WHReceiverFunc proc);

  �f�[�^����M����ƁA��M�̃^�C�~���O�Ŏ󂯎�������e���Z�b�g����
  �R�[���o�b�N�֐��ɓn����܂��B
  �f�[�^�̑��M�� WH_SendData �ōs���܂��B
  WH_SendData �Őݒ肵�� callback ���Ă΂��܂ł�
  ���M�f�[�^�̃��������㏑�����Ȃ��悤�ɂ��Ă��������B

  �f�[�^�V�F�A�����O�ł́A WH_StepDS �֐����Ă�Ńf�[�^��n���Ɠ�����
  �����̍X�V�������s���A WH_GetSharedDataAdr �Ńf�[�^���擾���܂��B

  �L�[�V�F�A�����O�ʐM�ł́A WH_GetKeySet �֐��ŃL�[�f�[�^���擾���܂�
  �iWH_StepDS �ɑ������鏈���������ōs���܂��B���M���ׂ��L�[�f�[�^��
  ���C�u����������ɑ��M���Ă����̂ŁA���ɉ�������K�v�͂���܂���j�B


  4) �G���[�ƕ��A�ɂ���

  ���̎����ł̕��j�́A

  - ��ʂ̊֐��ł̃G���[����͕Ԓl�ōs���BTRUE �i�����j�A FALSE �i���s�j��
    ���f����B
  - ���ڍׂȃG���[�R�[�h�́A sErrCode �Ɋi�[���Ă����B
  - ���s�s�\�ȏ�ԁi�����K�v�Ƃ����ԂȂǁj�ɂȂ������� sSysState ��
    WH_SYSSTATE_ERROR �ɕω�����̂ŁA��������m����B

  �Ƃ������̂ł��B
  �A�v���P�[�V�����ɂ���ẮA�G���[�̕񍐂ɃR�[���o�b�N�֐���
  OS_SendMessage ���g���K�v������Ǝv���܂����A���̎����ł͓��ɑΉ�����
  ���܂���i�\�[�X�𗬗p����Ȃ�A WH_SetError �֐�������������̂��ǂ�
  �ł��傤�j�B

  �܂��A�ؒf�i���A�j�����Ɋւ��Ắu�s�V�̗ǂ��v�ؒf�̂��߂ɁA�Ȃ�ׂ���Ԃ�
  �I���������s���Ă��疾���I�� WM_Disconnect �Ȃǂ��ĂԂ��Ƃ����������
  ���܂��B���̎����ł́A���́u�s�V�̗ǂ��I���v���s���ׂ� WH_Finalize �֐���
  �p�ӂ���Ă��܂��B���̊֐��͌��݂̏�Ԃɂ���āi���Ȃ��Ƃ����ꂪ�\�Ȏ��́j
  �e�I�������ւ̕��������悤�ɂȂ��Ă��܂��B


  5) �K�C�h���C�������ɂ���

  �K�C�h���C���ɏ��������ŕK�v�ȃR�[�h�ɂ́A���̎|�R�����g�����Ă��܂�
  �i�K�C�h���C���̋K��ɑΉ����邽�߂̏����͂��̃t�@�C���ȊO�̏ꏊ�ɂ�����
  �̂Œ��ӂ��ĉ������B�Y�����ɂ� `GUIDELINE' �̕�����t�����R�����g��
  �Y�����Ă��܂��̂ŁA���Q�l���������j�B
 */

/* ======================================================================
   static contents
   ====================================================================== */

static WMParentParam sParentParam ATTRIBUTE_ALIGN(32);

/*  ���L�̒l�̓R�[�h���Őݒ肳��Ă��܂��B
static WMParentParam sParentParam ATTRIBUTE_ALIGN(32) =
{
    // beacon �Ƀ��[�U��`�̃f�[�^���ڂ���ꍇ�͂����Ɏw�肵�܂��B
    // �q�@�̐e�@�I����ʂŐe�@�� Nickname ��\�����������ꍇ�Ȃǂ�
    // �����ɏ����Z�b�g���Đe�@����q�@�ɓ`���邱�ƂɂȂ�܂��B
    NULL,                         // userGameInfo
    0,                            // userGameInfoLength
    0,                            // padding
    // ggid �́A�Q�[���O���[�v�̎��ʂɎg�p���܂��B
    // �����ɂ́A�Q�[���ɂ���Ċ��蓖�Ă�ꂽ�l���g�p���܂��B
    0,
    // tgid �́A�e���|�����O���[�v�̎��ʂɎg�p���܂��B
    // �e�@�Ƃ��Ă̊������J�n���邽�тɈႤ�l�ł���K�v������܂��B
    // ���̃\�[�X�ł́A WH_Initialize �֐��̒��Őݒ肵�Ă��܂��B
    0x0001,                       // tgid
    // �q�@����̐V�K�ڑ����󂯓���邩�ǂ������w�肵�܂��B
    1,                            // entryFlag
    // �ڑ��\�Ȏq�@�̍ő吔���w�肵�܂��B
    WH_CHILD_MAX,                 // maxEntry
    // �}���`�u�[�g���Ȃ��ꍇ�� 0 �ɂ��Ă����܂��B
    0,                            // multiBootFlag
    // KS_Flag �� 1 �ɐݒ肷��ƁA�ڑ����Ɏ����I�ɃL�[�V�F�A�����O�p�̕␳��
    // parentMaxSize �� childMaxSize �ɓ���悤�ɂȂ�܂��B
    // �L�[�V�F�A�����O���g�p����ꍇ�ł��A������ƌv�Z����
    // parentMaxSize �� childMaxSize ���w�肷��ꍇ�ɂ� 0 �ō\���܂���B
    0,                            // KS_Flag
    // �ߋ��Ƃ̌݊����̂��߂ɗp�ӂ��ꂽ�t�B�[���h�ŁA���݂͖�������܂��B
    0,                            // CS_Flag
    // �e�@�������� beacon �̊Ԋu�ł��B[ms]
    // ���ۂ̋N�����ɂ� WM_GetDispersionBeaconPeriod() �ɂ����
    // �K�x�Ƀo�������l���̗p���܂����A200[ms] �O��ł��B
    200,                          // beaconPeriod (millisec)
    // �\��̈�ł��B
    {0},                           // rsv1
    {0},                           // rsv2
    // �e�@���q�@��҂��󂯂閳���`�����l���ł��B
    WH_DEFAULT_CHANNEL ,          // channel
    // �ȉ��̃����o�ɂ͍ő�l�Ƃ��ēK�؂Ȓl��ݒ肵�Ă��܂��B
    // parentMaxSize - �e�@������f�[�^�̍ő�T�C�Y
    WH_PARENT_MAX_SIZE,
    // childMaxSize - �q�@������f�[�^�̍ő�T�C�Y
    WH_CHILD_MAX_SIZE,
};
*/

// wh �̏�ԁiWH��ԂƂ͕ʃ��m�ł��B�O����͎Q�Ƃ̂݁j
static int sSysState = WH_SYSSTATE_STOP;

// �ڑ����[�h�i�O����ݒ肵�A���̃\�[�X���ł͐G��܂���j
static int sConnectMode;

// MP �ʐM���[�h�ł̃f�[�^��M���[�U�֐�
static WHReceiverFunc sReceiverFunc;

// �ڑ�������p���[�U�֐�
static WHJudgeAcceptFunc sJudgeAcceptFunc;

// ������ aid ������܂��i�q�@�͐ؒf�E�Đڑ����ɕω�����\������j
static u16 sMyAid;

// �ڑ���Ԃ����� bitmap ���i�[����܂�
static u16 sConnectBitmap;

// �G���[�R�[�h�i�[�p
static int sErrCode;

// �����p
static u32 sRand;

/* �ʐM���p������p (WH_StartMeasureChannel, WH_GetMeasureChannel �Ŏg�p) */
static u16 sChannel;
static u16 sChannelBusyRatio;
static u16 sChannelBitmap;

/* WM �p�V�X�e�����[�N�̈�o�b�t�@ */
static u8 sWmBuffer[WM_SYSTEM_BUF_SIZE] ATTRIBUTE_ALIGN(32);
// �����g�p���鑗��M�o�b�t�@�Ƃ��̃T�C�Y
static u8 sSendBuffer[MATH_MAX(WH_PARENT_SEND_BUFFER_SIZE, WH_CHILD_SEND_BUFFER_SIZE)]
ATTRIBUTE_ALIGN(32);
static u8 sRecvBuffer[MATH_MAX(WH_PARENT_RECV_BUFFER_SIZE, WH_CHILD_RECV_BUFFER_SIZE)]
ATTRIBUTE_ALIGN(32);
static s32 sSendBufferSize;
static s32 sRecvBufferSize;


/* �e�@�����p */
static WMBssDesc sBssDesc ATTRIBUTE_ALIGN(32);
static u8 sScanBuf[WM_SIZE_SCAN_EX_BUF] ATTRIBUTE_ALIGN(32);
static WMScanExParam sScanExParam ATTRIBUTE_ALIGN(32);
static WHStartScanCallbackFunc sScanCallback;
static u16 sChannelIndex;
static u16 sAutoConnectFlag;

static BOOL sPictoCatchFlag = FALSE;

/* �e�@�ڑ����Ɏg�p����ݒ� */
static u8 sConnectionSsid[WM_SIZE_CHILD_SSID];

/* WEP �ݒ�p */
static WHParentWEPKeyGeneratorFunc sParentWEPKeyGenerator = NULL;
static WHChildWEPKeyGeneratorFunc sChildWEPKeyGenerator = NULL;
static u16 sWEPKey[20 / sizeof(u16)] ATTRIBUTE_ALIGN(32);

/* �f�[�^�V�F�A�����O�p */
static WMDataSharingInfo sDSInfo ATTRIBUTE_ALIGN(32);
static WMDataSet sDataSet ATTRIBUTE_ALIGN(32);
static WMKeySetBuf sWMKeySetBuf ATTRIBUTE_ALIGN(32);

/* WH �������t���O */
static u16 whInitialized = 0;

/* WM �� Indication �ʒm�p�R�[���o�b�N */
static WMCallbackFunc sIndicationCallback = NULL;

/* �f�o�b�O�o�̓t�b�N�֐� */
static void (*wh_trace) (const char *, ...) =
#if !defined(SDK_FINALROM)
        OS_TPrintf;
#else
        NULL;
#endif


// WC�Ƃ̌݊����̂��߂Ɋg�����ꂽWH�̃����o
static void (*sStateCallback)(int state, int prev) = NULL;
static BOOL sDataSharingDoubleMode = TRUE;
static WMCallbackFunc sPortExtraCallback = NULL;

#if defined(WM_DEMO_WIRELESS_SHARED_WC_WH_CONFIG_H_)
// WC�Ɠ����ȓ����ێ����邽�߂̃����o

typedef struct WHCompatibleInfoForWC
{
    WMParentParam               parentParam ATTRIBUTE_ALIGN(32);
    OSThreadQueue               queue[1];
    WCPictoCatchCallbackFunc    pictoCatchCallback;
    BOOL                        useDS;
    WCStatus                    wcStatus;
    WCStatus                    wcTarget;
    u16                         framePeriod;
    u16                         targetChannel;
    u16                         tmpTgid;
    u8                          padding[2];
}
WHCompatibleInfoForWC;

static WHCompatibleInfoForWC infoForWC[1] ATTRIBUTE_ALIGN(32);

static const int    parentRecvBufferSize = WH_PARENT_RECV_BUFFER_SIZE;
static const int    parentSendBufferSize = WH_PARENT_SEND_BUFFER_SIZE;
static const int    childRecvBufferSize = WH_CHILD_RECV_BUFFER_SIZE;
static const int    childSendBufferSize = WH_CHILD_SEND_BUFFER_SIZE;
#undef  WH_PARENT_RECV_BUFFER_SIZE
#define WH_PARENT_RECV_BUFFER_SIZE  parentRecvBufferSize
#undef  WH_PARENT_SEND_BUFFER_SIZE
#define WH_PARENT_SEND_BUFFER_SIZE  parentSendBufferSize
#undef  WH_CHILD_RECV_BUFFER_SIZE
#define WH_CHILD_RECV_BUFFER_SIZE   childRecvBufferSize
#undef  WH_CHILD_SEND_BUFFER_SIZE
#define WH_CHILD_SEND_BUFFER_SIZE   childSendBufferSize
#undef  WH_PLAYER_MAX
#define WH_PLAYER_MAX             (infoForWC->parentParam.maxEntry + 1)
#undef  WH_CHILD_MAX
#define WH_CHILD_MAX              infoForWC->parentParam.maxEntry
#undef  WH_DS_DATA_SIZE
#define WH_DS_DATA_SIZE           infoForWC->parentParam.childMaxSize
#undef  WH_PARENT_MAX_SIZE
#define WH_PARENT_MAX_SIZE        infoForWC->parentParam.parentMaxSize
#undef  WH_CHILD_MAX_SIZE
#define WH_CHILD_MAX_SIZE         infoForWC->parentParam.childMaxSize
#undef  WH_MP_FREQUENCY
#define WH_MP_FREQUENCY           (u16)(infoForWC->parentParam.CS_Flag ? 0 : 1)

#endif // WM_DEMO_WIRELESS_SHARED_WC_WH_CONFIG_H_


/* ======================================================================
   state functions
   ====================================================================== */

// �����}�N��
#define RAND()  ( sRand = sRand * 69069UL + 12345 )
#define RAND_INIT(x) ( sRand = (u32)(x) )

#define WH_MAX(a, b) ( ((a) > (b)) ? (a) : (b) )

/* �s����Ȓʒm���󂯎��֐� */
static void WH_IndicateHandler(void *arg);

/* (stateless) -> READY -> STOP -> IDLE */
static BOOL WH_StateInInitialize(void);

#ifndef WH_USE_DETAILED_INITIALIZE
static void WH_StateOutInitialize(void *arg);
#else
static BOOL WH_StateInEnable(void);
static void WH_StateOutEnable(void *arg);
static BOOL WH_StateInPowerOn(void);
static void WH_StateOutPowerOn(void *arg);
#endif

/* IDLE -> MEASURECHANNEL -> IDLE */
static u16 WH_StateInMeasureChannel(u16 channel);
static void WH_StateOutMeasureChannel(void *arg);
static WMErrCode WHi_MeasureChannel(WMCallbackFunc func, u16 channel);
static s16 SelectChannel(u16 bitmap);


/* IDLE -> PARENT -> MP-PARENT */
static BOOL WH_StateInSetParentParam(void);
static void WH_StateOutSetParentParam(void *arg);
static BOOL WH_StateInSetParentWEPKey(void);
static void WH_StateOutSetParentWEPKey(void *arg);
static BOOL WH_StateInStartParent(void);
static void WH_StateOutStartParent(void *arg);
static BOOL WH_StateInStartParentMP(void);
static void WH_StateOutStartParentMP(void *arg);

/* IDLE -> SCAN */
static BOOL WH_StateInStartScan(void);
static void WH_StateOutStartScan(void *arg);

/* SCAN -> IDLE */
static BOOL WH_StateInEndScan(void);
static void WH_StateOutEndScan(void *arg);

/* IDLE -> CHILD -> MP-CHILD */
static BOOL WH_StateInSetChildWEPKey(void);
static void WH_StateOutSetChildWEPKey(void *arg);
static BOOL WH_StateInStartChild(void);
static void WH_StateOutStartChild(void *arg);
static BOOL WH_StateInStartChildMP(void);
static void WH_StateOutStartChildMP(void *arg);

/* MP + key sharing */
static BOOL WH_StateInStartParentKeyShare(void);
static BOOL WH_StateInEndParentKeyShare(void);
static BOOL WH_StateInStartChildKeyShare(void);
static BOOL WH_StateInEndChildKeyShare(void);

/* MP + SetMPData */
static BOOL WH_StateInSetMPData(const void *data, u16 datasize, WHSendCallbackFunc callback);
static void WH_StateOutSetMPData(void *arg);
static void WH_PortReceiveCallback(void *arg);

/* MP-PARENT -> IDLE */
static BOOL WH_StateInEndParentMP(void);
static void WH_StateOutEndParentMP(void *arg);
static BOOL WH_StateInEndParent(void);
static void WH_StateOutEndParent(void *arg);
static BOOL WH_StateInDisconnectChildren(u16 bitmap);
static void WH_StateOutDisconnectChildren(void *arg);

/* MP-CHILD -> IDLE */
static BOOL WH_StateInEndChildMP(void);
static void WH_StateOutEndChildMP(void *arg);
static BOOL WH_StateInEndChild(void);
static void WH_StateOutEndChild(void *arg);

/* IDLE -> STOP -> READY -> (stateless) */
static BOOL WH_StateInPowerOff(void);
static void WH_StateOutPowerOff(void *arg);
static BOOL WH_StateInDisable(void);
static void WH_StateOutDisable(void *arg);
static void WH_StateOutEnd(void *arg);

/* X -> IDLE */
static BOOL WH_StateInReset(void);
static void WH_StateOutReset(void *arg);


/* ======================================================================
   debug codes
   ====================================================================== */
#define WMHIGH_DEBUG

#if defined(WMHIGH_DEBUG)

#define WH_TRACE    if(wh_trace) wh_trace

#define WH_TRACE_STATE OS_TPrintf("%s sSysState = %d\n", __func__, sSysState)

#define WH_REPORT_FAILURE(result)                \
    do{ OS_TPrintf("Failed in %s, %s = %s\n",      \
                  __func__,                      \
                  #result,                       \
                  WH_GetWMErrCodeName(result));  \
        WH_SetError(result); }while(0)

#define WH_ASSERT(exp) \
    (void) ((exp) || (OSi_Panic(__FILE__, __LINE__, "Failed assertion " #exp), 0))

#else  /* defined(WMHIGH_DEBUG) */

#define WH_TRACE_STATE                 /* */
#define WH_TRACE(...)               ((void)0)
#define WH_REPORT_FAILURE(result)   WH_SetError(result)
#define WH_ASSERT(exp)              ((void) 0)

#endif



#if defined(WMHIGH_DEBUG)
static const char *sStateNames[] = {
    "WM_STATE_READY",
    "WM_STATE_STOP",
    "WM_STATE_IDLE",
    "WM_STATE_CLASS1",
    "WM_STATE_TESTMODE",
    "WM_STATE_SCAN",
    "WM_STATE_CONNECT",
    "WM_STATE_PARENT",
    "WM_STATE_CHILD",
    "WM_STATE_MP_PARENT",
    "WM_STATE_MP_CHILD",
    "WM_STATE_DCF_CHILD",
};

static const char *WH_GetWMErrCodeName(int result)
{
    static const char *errnames[] = {
        "WM_ERRCODE_SUCCESS",
        "WM_ERRCODE_FAILED",
        "WM_ERRCODE_OPERATING",
        "WM_ERRCODE_ILLEGAL_STATE",
        "WM_ERRCODE_WM_DISABLE",
        "WM_ERRCODE_NO_DATASET",
        "WM_ERRCODE_INVALID_PARAM",
        "WM_ERRCODE_NO_CHILD",
        "WM_ERRCODE_FIFO_ERROR",
        "WM_ERRCODE_TIMEOUT",
        "WM_ERRCODE_SEND_QUEUE_FULL",
        "WM_ERRCODE_NO_ENTRY",
        "WM_ERRCODE_OVER_MAX_ENTRY",
        "WM_ERRCODE_INVALID_POLLBITMAP",
        "WM_ERRCODE_NO_DATA",
        "WM_ERRCODE_SEND_FAILED",
        "WM_ERRCODE_DCF_TEST",
        "WM_ERRCODE_WL_INVALID_PARAM",
        "WM_ERRCODE_WL_LENGTH_ERR",
        "WM_ERRCODE_FLASH_ERROR",

        "WH_ERRCODE_DISCONNECTED",
        "WH_ERRCODE_PARENT_NOT_FOUND",
        "WH_ERRCODE_NO_RADIO",
    };

    if (0 <= result && result < sizeof(errnames) / sizeof(char *))
    {
        return errnames[result];
    }
    else
    {
        return "N/A";
    }
}

static const char *WH_GetWMStateCodeName(u16 code)
{
    static const char *statenames[] = {
        "WM_STATECODE_PARENT_START",
        "N/A",
        "WM_STATECODE_BEACON_SENT",
        "WM_STATECODE_SCAN_START",
        "WM_STATECODE_PARENT_NOT_FOUND",
        "WM_STATECODE_PARENT_FOUND",
        "WM_STATECODE_CONNECT_START",
        "WM_STATECODE_CONNECTED",
        "WM_STATECODE_BEACON_LOST",
        "WM_STATECODE_DISCONNECTED",
        "WM_STATECODE_MP_START",
        "WM_STATECODE_MPEND_IND",
        "WM_STATECODE_MP_IND",
        "WM_STATECODE_MPACK_IND",
        "WM_STATECODE_DCF_START",
        "WM_STATECODE_DCF_IND",
        "WM_STATECODE_BEACON_RECV",
        "WM_STATECODE_DISASSOCIATE",
        "WM_STATECODE_REASSOCIATE",
        "WM_STATECODE_AUTHENTICATE",
        "WM_STATECODE_PORT_SEND",
        "WM_STATECODE_PORT_RECV",
        "WM_STATECODE_FIFO_ERROR",
        "WM_STATECODE_INFORMATION",
        "WM_STATECODE_UNKNOWN",
        "WM_STATECODE_PORT_INIT",
        "WM_STATECODE_DISCONNECTED_FROM_MYSELF",
    };

    if (0 <= code && code < sizeof(statenames) / sizeof(char *))
    {
        return statenames[code];
    }
    else
    {
        return "N/A";
    }
}

static const char *WH_GetWMApiidName(u16 apiid)
{
    static const char *apinames[] = {
        "WM_APIID_INITIALIZE",
        "WM_APIID_RESET",
        "WM_APIID_END",

        "WM_APIID_ENABLE",
        "WM_APIID_DISABLE",
        "WM_APIID_POWER_ON",
        "WM_APIID_POWER_OFF",

        "WM_APIID_SET_P_PARAM",
        "WM_APIID_START_PARENT",
        "WM_APIID_END_PARENT",
        "WM_APIID_START_SCAN",
        "WM_APIID_END_SCAN",
        "WM_APIID_START_CONNECT",
        "WM_APIID_DISCONNECT",
        "WM_APIID_START_MP",
        "WM_APIID_SET_MP_DATA",
        "WM_APIID_END_MP",
        "WM_APIID_START_DCF",
        "WM_APIID_SET_DCF_DATA",
        "WM_APIID_END_DCF",
        "WM_APIID_SET_WEPKEY",
        "WM_APIID_START_KS",
        "WM_APIID_END_KS",
        "WM_APIID_GET_KEYSET",
        "WM_APIID_SET_GAMEINFO",
        "WM_APIID_SET_BEACON_IND",
        "WM_APIID_START_TESTMODE",
        "WM_APIID_STOP_TESTMODE",
        "WM_APIID_VALARM_MP",
        "WM_APIID_SET_LIFETIME",
        "WM_APIID_MEASURE_CHANNEL",
        "WM_APIID_INIT_W_COUNTER",
        "WM_APIID_GET_W_COUNTER",
        "WM_APIID_SET_ENTRY",
        "WM_APIID_AUTO_DEAUTH",
        "WM_APIID_SET_MP_PARAMETER",
        "WM_APIID_SET_BEACON_PERIOD",
        "WM_APIID_AUTO_DISCONNECT",

        "WM_APIID_KICK_MP_PARENT",
        "WM_APIID_KICK_MP_CHILD",
        "WM_APIID_KICK_MP_RESUME",
    };
    static const char *apinames_indicates[] = {
        "WM_APIID_INDICATION",
        "WM_APIID_PORT_SEND",
        "WM_APIID_PORT_RECV",
        "WM_APIID_READ_STATUS"
    };
    static const char *apiname_unknown = "WM_APIID_UNKNOWN";

    if (0 <= apiid && apiid < sizeof(apinames) / sizeof(char *))
    {
        return apinames[apiid];
    }
    else if (WM_APIID_INDICATION <= apiid &&
             apiid < WM_APIID_INDICATION + sizeof(apinames_indicates) / sizeof(char *))
    {
        return apinames_indicates[apiid - WM_APIID_INDICATION];
    }
    else if (apiid == WM_APIID_UNKNOWN)
    {
        return apiname_unknown;
    }
    else
    {
        return "N/A";
    }
}

static void WH_OutputWMState(void)
{
    WMStatus s;

    if (WM_ReadStatus(&s) == WM_ERRCODE_SUCCESS)
    {
        WH_TRACE("state = %s\n", WH_GetWMStateCodeName(s.state));
        WH_TRACE("apiid = %s\n", WH_GetWMApiidName(s.BusyApiid));
    }
    else
    {
        WH_TRACE("WM not Initialized\n");
    }
}

static void WH_OutputBitmap(u16 bitmap)
{
    u16     i;
    for (i = 15; i >= 0; --i)
    {
        if ((bitmap >> i) & 0x01)
        {
            OS_TPrintf("o");
        }
        else
        {
            OS_TPrintf("-");
        }
    }

    OS_TPrintf("\n");
}

static void WH_ChangeSysState(int state)
{
    static const char *statenames[] = {
        "WH_SYSSTATE_STOP",
        "WH_SYSSTATE_IDLE",
        "WH_SYSSTATE_SCANNING",
        "WH_SYSSTATE_BUSY",
        "WH_SYSSTATE_CONNECTED",
        "WH_SYSSTATE_DATASHARING",
        "WH_SYSSTATE_KEYSHARING",
        "WH_SYSSTATE_MEASURECHANNEL",
        "WH_SYSSTATE_CONNECT_FAIL",
        "WH_SYSSTATE_ERROR",
    };
    int     prev = sSysState;
    WH_TRACE("%s -> ", statenames[sSysState]);
    sSysState = state;
    WH_TRACE("%s\n", statenames[sSysState]);
    if (sStateCallback)
    {
        (*sStateCallback)(sSysState, prev);
    }
}

#else

#define WH_GetWMErrCodeName(result)    ("")
#define WH_GetWMStateCodeName(result)  ("")
#define WH_GetWMApiidName(result)      ("")

static void WH_OutputWMState(void)
{;
}
static void WH_OutputBitmap(void)
{;
}

static void WH_ChangeSysState(int state)
{
    sSysState = state;
}

#endif

static void WH_SetError(int code)
{
    // ���ɃV�X�e���� ERROR ��ԂɂȂ��Ă���ꍇ�́A�㏑�����Ȃ��B
    if (sSysState == WH_SYSSTATE_ERROR || sSysState == WH_SYSSTATE_FATAL)
    {
        return;
    }

    sErrCode = code;
}



/* ======================================================================
   state functions
   ====================================================================== */

/* ----------------------------------------------------------------------
   state : setparentparam
  ---------------------------------------------------------------------- */
static BOOL WH_StateInSetParentParam(void)
{
    // ���̏�Ԃł́A�e�@�̎����Ă���Q�[������ ARM7 �ɓn���Ă��܂��B
    WMErrCode result;
    WH_TRACE_STATE;

    WH_ChangeSysState(WH_SYSSTATE_BUSY);

    result = WM_SetParentParameter(WH_StateOutSetParentParam, &sParentParam);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return FALSE;
    }

    return TRUE;
}

static void WH_StateOutSetParentParam(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return;
    }

    if (sParentWEPKeyGenerator != NULL)
    {
        // WEP Key Generator ���ݒ肳��Ă���΁AWEP Key �̐ݒ��
        if (!WH_StateInSetParentWEPKey())
        {
            WH_ChangeSysState(WH_SYSSTATE_ERROR);
        }
    }
    else
    {
        // ����ɐi�s���Ă���Ύ��� StartParent ��ԂցB
        if (!WH_StateInStartParent())
        {
            WH_ChangeSysState(WH_SYSSTATE_ERROR);
        }
    }
}

static BOOL WH_StateInSetParentWEPKey(void)
{
    u16     wepmode;
    WMErrCode result;
    WH_TRACE_STATE;

    WH_ChangeSysState(WH_SYSSTATE_BUSY);

    wepmode = (*sParentWEPKeyGenerator) (sWEPKey, &sParentParam);
    result = WM_SetWEPKey(WH_StateOutSetParentWEPKey, wepmode, sWEPKey);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return FALSE;
    }

    return TRUE;
}

static void WH_StateOutSetParentWEPKey(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return;
    }

    // ����ɐi�s���Ă���Ύ��� StartParent ��ԂցB
    if (!WH_StateInStartParent())
    {
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
    }
}


/* ----------------------------------------------------------------------
   state : StartParent
  ---------------------------------------------------------------------- */
static BOOL WH_StateInStartParent(void)
{
    // ���̏�Ԃł� StartParent �֐����ĂсA�e�@�Ƃ��Ă̐ݒ���J�n���܂��B

    WMErrCode result;
    WH_TRACE_STATE;

    if ((sSysState == WH_SYSSTATE_CONNECTED)
        || (sSysState == WH_SYSSTATE_KEYSHARING) || (sSysState == WH_SYSSTATE_DATASHARING))
    {
        // �ȏ�̏ꍇ�ɂ͊��ɐe�Ƃ��Ă̐ݒ�͍ς�ł���͂��B
        return TRUE;
    }

    result = WM_StartParent(WH_StateOutStartParent);

    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    sMyAid = 0;
    sConnectBitmap = WH_BITMAP_EMPTY;

    return TRUE;
}

static void WH_StateOutStartParent(void *arg)
{
    // StartParent �Őݒ肵���R�[���o�b�N�֐��́A
    // 1) �r�[�R�������M���ꂽ
    // 2) �V�����q�@���ڑ�����
    // 3) StartParent ���I������
    // 4) �q�@�̐ؒf�����m
    // �ƁA���ʂ肩�̃P�[�X�ŌĂ΂��̂ŁA��ʂ���K�v������܂��B

    WMStartParentCallback *cb = (WMStartParentCallback *)arg;
    const u16 target_bitmap = (u16)(1 << cb->aid);

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return;
    }

    switch (cb->state)
    {
        //-----------------------------------
        // �r�[�R�����M�ʒm
    case WM_STATECODE_BEACON_SENT:
        // �r�[�R�����M�ʒm�͖������܂��B
        break;

        //-----------------------------------
        // �q�@�̐ڑ���ʒm
    case WM_STATECODE_CONNECTED:
        {
            // cb->macAddress �ɐڑ����Ă����q�@�� MAC �A�h���X�������Ă��܂��B
            // cb->ssid �͎q�@�� WM_StartConnect �� ssid �����ɃZ�b�g�����f�[�^�ł��B
            WH_TRACE("StartParent - new child (aid 0x%x) connected\n", cb->aid);
            // �ڑ����Ă����q�@���ڑ��������𖞂����Ă��邩�ǂ������`�F�b�N
            if (sJudgeAcceptFunc != NULL)
            {
                if (!sJudgeAcceptFunc(cb))
                {
                    WMErrCode result;
                    // �ڑ���ؒf���܂��B
                    result = WM_Disconnect(NULL, cb->aid);
                    if (result != WM_ERRCODE_OPERATING)
                    {
                        WH_REPORT_FAILURE(result);
                        WH_ChangeSysState(WH_SYSSTATE_ERROR);
                    }
                    break;
                }
            }
            sConnectBitmap |= target_bitmap;
            if (sPortExtraCallback)
            {
                (*sPortExtraCallback)(arg);
            }
        }
        break;

        //-----------------------------------
        // �q�@�̐ؒf��ʒm
    case WM_STATECODE_DISCONNECTED:
        {
            WH_TRACE("StartParent - child (aid 0x%x) disconnected\n", cb->aid);
            // cb->macAddress �ɂ�, �ؒf���ꂽ�q�@�� MAC �A�h���X�������Ă��܂��B
            sConnectBitmap &= ~target_bitmap;
            if (sPortExtraCallback)
            {
                (*sPortExtraCallback)(arg);
            }
        }
        break;

        //-----------------------------------
        // ����q�@��ؒf����
    case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
        {
            WH_TRACE("StartParent - child (aid 0x%x) disconnected from myself\n", cb->aid);
            // ����ؒf�����ꍇ�͏������s���܂���
            // cb->macAddress �ɂ�, �ؒf���ꂽ�q�@�� MAC �A�h���X�������Ă��܂��B
        }
        break;

        //-----------------------------------
        // StartParent�̏������I��
    case WM_STATECODE_PARENT_START:
        {
            // MP �ʐM��ԂɈڍs���܂��B
            if (!WH_StateInStartParentMP())
            {
                WH_ChangeSysState(WH_SYSSTATE_ERROR);
            }
        }
        break;

        //-----------------------------------
    default:
        WH_TRACE("unknown indicate, state = %d\n", cb->state);
    }
}

/* ----------------------------------------------------------------------
   state : StartParentMP
  ---------------------------------------------------------------------- */
static BOOL WH_StateInStartParentMP(void)
{
    // WM_Start �֐����ĂсA MP �ʐM�v���g�R���ɂ��ڑ����J�n���܂��B

    WMErrCode result;
    WH_TRACE_STATE;

    if ((sSysState == WH_SYSSTATE_CONNECTED)
        || (sSysState == WH_SYSSTATE_KEYSHARING) || (sSysState == WH_SYSSTATE_DATASHARING))
    {
        return TRUE;
    }

    WH_ChangeSysState(WH_SYSSTATE_CONNECTED);
    result = WM_StartMP(WH_StateOutStartParentMP,
                        (u16 *)sRecvBuffer,
                        (u16)sRecvBufferSize, (u16 *)sSendBuffer, (u16)sSendBufferSize,
                        WH_MP_FREQUENCY);

    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    return TRUE;
}

static void WH_StateOutStartParentMP(void *arg)
{
    // StartMP �Ŏw�肵���R�[���o�b�N�֐��́A
    // 1) StartMP �ɂ�郂�[�h�J�n��
    // 2) MP �V�[�P���X�����i�e�@�̂݁j
    // 3) MP ��M�i�q�@�̂݁j
    // 4) MP �V�[�P���X�����ʒm(ACK��M)�����o�i�q�@�̂݁j
    // �̂S�ʂ�̃P�[�X�ŌĂ΂�邽�߁A��ʂ���K�v������܂��B

    WMStartMPCallback *cb = (WMStartMPCallback *)arg;
    // WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return;
    }

    switch (cb->state)
    {
    case WM_STATECODE_MP_START:
        // StartMP ����I���̒ʒm�B
        // ����ȍ~�A����M�\�ɂȂ�܂��B
        if (sConnectMode == WH_CONNECTMODE_KS_PARENT)
        {
            // �L�[�V�F�A�����O�w�肾�����ꍇ�́A�X�� StartParentKeyShare ��
            // �ڍs���܂��B
            if (sSysState == WH_SYSSTATE_CONNECTED)
            {
                // �ʏ�� MP �ڑ��B
                if (!WH_StateInStartParentKeyShare())
                {
                    WH_TRACE("WH_StateInStartParentKeyShare failed\n");
                    WH_ChangeSysState(WH_SYSSTATE_ERROR);
                }
                return;

            }
            else if (sSysState == WH_SYSSTATE_KEYSHARING)
            {
                // ���ɃL�[�V�F�A�����O��ԂɂȂ��Ă���͗l�B
                return;
            }
        }
        else if (sConnectMode == WH_CONNECTMODE_DS_PARENT)
        {
            // �f�[�^�V�F�A�����O�w��̏ꍇ�́AStartDataSharing ���Ăт܂��B
            // ���̊֐��͓�������֐��Ȃ̂ŁAWH��Ԃ̑J�ڂ͂��܂���B
            WMErrCode result;
            u16     aidBitmap;

            aidBitmap = (u16)((1 << (WH_CHILD_MAX + 1)) - 1);   // ���� WH_CHILD_MAX+1 �r�b�g��1�� bitmap
            result = WM_StartDataSharing(&sDSInfo, WH_DS_PORT, aidBitmap, WH_DS_DATA_SIZE, sDataSharingDoubleMode);

            if (result != WM_ERRCODE_SUCCESS)
            {
                WH_REPORT_FAILURE(result);
                WH_ChangeSysState(WH_SYSSTATE_ERROR);
                return;
            }
            // WH_TRACE("WM_StartDataSharing OK\n");
            WH_ChangeSysState(WH_SYSSTATE_DATASHARING);
            if (sPortExtraCallback)
            {
                (*sPortExtraCallback)(arg);
            }
            return;
        }

        WH_ChangeSysState(WH_SYSSTATE_CONNECTED);
        if (sPortExtraCallback)
        {
            (*sPortExtraCallback)(arg);
        }
        break;

    case WM_STATECODE_MPEND_IND:
        // �e�@�̎�M�����ʒm�B

        // cb->recvBuf �Ŏq�@�����M�����f�[�^�t���[���̓��e���󂯎��܂����A
        // �ʏ�̃f�[�^��M�� WM_SetPortCallback �ŃR�[���o�b�N��ݒ肵�Ă��������B
        // �܂��A�f�[�^�V�F�A�����O�E�L�[�V�F�A�����O���g���ꍇ�́A
        // ��M�����͓����I�ɍs���܂��̂ŁAWM_SetPortCallback ���g���K�v������܂���B
        break;

    case WM_STATECODE_MP_IND:
    case WM_STATECODE_MPACK_IND:
        // �e�@�Ȃ炱���ւ͗��Ȃ��͂��ł��B

    default:
        WH_TRACE("unknown indicate, state = %d\n", cb->state);
        break;
    }
}

/* ----------------------------------------------------------------------
  state : StartParentKeyShare
  ---------------------------------------------------------------------- */
static BOOL WH_StateInStartParentKeyShare(void)
{
    WMErrCode result;
    WH_TRACE_STATE;
    WH_ChangeSysState(WH_SYSSTATE_KEYSHARING);

    // WM_StartKeySharing �֐����ĂсA�L�[�V�F�A�����O���J�n���܂��B
    result = WM_StartKeySharing(&sWMKeySetBuf, WH_DS_PORT);

    if (result != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    return TRUE;
}

/* ----------------------------------------------------------------------
   state : EndParentKeyShare
  ---------------------------------------------------------------------- */
static BOOL WH_StateInEndParentKeyShare(void)
{
    // WM_EndKeySharing ���ĂсA�L�[�V�F�A�����O���I�����܂��B
    WMErrCode result;
    WH_TRACE_STATE;

    result = WM_EndKeySharing(&sWMKeySetBuf);

    if (result != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    // �����I�ɁA MP �ʐM�̏I���������J�n���܂��B
    if (!WH_StateInEndParentMP())
    {
        WH_TRACE("WH_StateInEndParentMP failed\n");
        WH_Reset();
        return FALSE;
    }

    return TRUE;
}

/* ----------------------------------------------------------------------
   state : EndParentMP
  ---------------------------------------------------------------------- */
static BOOL WH_StateInEndParentMP(void)
{
    WMErrCode result;
    WH_TRACE_STATE;

    // ����ȍ~�A����M�s�\�ɂȂ�܂��B
    WH_ChangeSysState(WH_SYSSTATE_BUSY);

    result = WM_EndMP(WH_StateOutEndParentMP);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    return TRUE;
}

static void WH_StateOutEndParentMP(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        WH_Reset();
        return;
    }

    // �����I�ɁA�I���������J�n���܂��B
    if (!WH_StateInEndParent())
    {
        WH_TRACE("WH_StateInEndParent failed\n");
        WH_Reset();
        return;
    }
}

/* ----------------------------------------------------------------------
   state : EndParent
   ---------------------------------------------------------------------- */
static BOOL WH_StateInEndParent(void)
{
    WMErrCode result;
    WH_TRACE_STATE;

    // �����ŁA�e�@�Ƃ��Ă̓�����I�����܂��B
    // �ڑ����̎q�@������ꍇ�́A�ʂɔF�؂�ؒf������
    // �e�@�Ƃ��Ă̊�������~����܂��B
    result = WM_EndParent(WH_StateOutEndParent);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    return TRUE;
}

static void WH_StateOutEndParent(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        return;
    }

    // �����ŁA�e�@�Ƃ��Ă̐ؒf�����͊����� �A�C�h�����O�i�ҋ@���j��Ԃɖ߂�܂��B
    WH_ChangeSysState(WH_SYSSTATE_IDLE);
}

/* ----------------------------------------------------------------------
   Name:        WH_ChildConnectAuto
   Description: �q�@�ڑ��V�[�P���X���J�n���܂��B
                �������AWH_ParentConnect �� WH_ChildConnect �Ŏw�肷��
                �e��ݒ������̎��������ɔC���܂��B
   Arguments:   mode    - WH_CONNECTMODE_MP_CHILD �Ȃ�Ύq�@�Ƃ���MP�J�n�B
                          WH_CONNECTMODE_DS_CHILD �Ȃ�Ύq�@�Ƃ���DataSharing�J�n�B
                          WH_CONNECTMODE_KS_CHILD �Ȃ�Ύq�@�Ƃ���KeySharing�J�n�B

                macAddr - �ڑ�����e�@��MAC�A�h���X���w��
                          NULL �������� 0xFFFFFFFFFFFF�Ȃ�΂��ׂĂ̐e�@����������B
                          
                channel - �e����������`�����l�����w��
                          0�Ȃ�΂��ׂẴ`�����l������������B
                          
   ---------------------------------------------------------------------- */
BOOL WH_ChildConnectAuto(int mode, const u8 *macAddr, u16 channel)
{
    WH_TRACE_STATE;

    // WM_StartMP() �p�̑���M�o�b�t�@�T�C�Y�v�Z
    // ���O�ɐÓI�Ƀo�b�t�@���m�ۂ������ꍇ�� WM_SIZE_MP_* �֐��}�N�����A
    // ���I�Ɋm�ۂ��č\��Ȃ��ꍇ�́A�e�q�ڑ���� WM_StartMP() ���Ăяo�����O��
    // WM_GetReceiveBufferSize() API ��p���܂��B
    // ���l�Ɏ��O�ɐÓI�Ƀo�b�t�@���m�ۂ������ꍇ�� WM_SIZE_MP_* �֐��}�N�����A
    // ���I�Ɋm�ۂ��č\��Ȃ��ꍇ�́A�e�q�ڑ���� WM_StartMP() ���Ăяo�����O��
    // WM_GetSendBufferSize() API ��p���܂��B
    sRecvBufferSize = WH_CHILD_RECV_BUFFER_SIZE;
    sSendBufferSize = WH_CHILD_SEND_BUFFER_SIZE;

    WH_TRACE("recv buffer size = %d\n", sRecvBufferSize);
    WH_TRACE("send buffer size = %d\n", sSendBufferSize);

    WH_ChangeSysState(WH_SYSSTATE_SCANNING);

    // �q�@���[�h�Ō����J�n�B
    if (macAddr != NULL)
    {
        *(u16 *)(&sScanExParam.bssid[4]) = *(u16 *)(macAddr + 4);
        *(u16 *)(&sScanExParam.bssid[2]) = *(u16 *)(macAddr + 2);
        *(u16 *)(&sScanExParam.bssid[0]) = *(u16 *)(macAddr + 0);
    }
    else
    {
        MI_CpuFill8(&sScanExParam.bssid, 0xff, WM_SIZE_BSSID);
    }

    sConnectMode = mode;

    sScanCallback = NULL;
    sChannelIndex = channel;
    sScanExParam.channelList = 1;
    sAutoConnectFlag = TRUE;

    if (!WH_StateInStartScan())
    {
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return FALSE;
    }

    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         WH_TurnOnPictoCatch

  Description:  �s�N�g�`���b�g�T�[�`�@�\��L���ɂ���B
                WH_StartScan�ɂăX�L�������ɁA�s�N�g�`���b�g�𔭌������ꍇ�ɂ�
                �R�[���o�b�N�֐����Ă΂��悤�ɂȂ�B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WH_TurnOnPictoCatch(void)
{
    sPictoCatchFlag = TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         WH_TurnOffPictoCatch

  Description:  �s�N�g�`���b�g�T�[�`�@�\�𖳌��ɂ���B
                WH_StartScan�ɂăX�L�������ɁA�s�N�g�`���b�g�𔭌������ꍇ�ł�
                ���������悤�ɂȂ�B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WH_TurnOffPictoCatch(void)
{
    sPictoCatchFlag = FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         WH_StartScan

  Description:  �e�@�̃r�[�R�����擾����֐�

  Arguments:    callback - �e�@�������ɕԂ��R�[���o�b�N��ݒ肷��B
                
                macAddr  - �ڑ�����e�@��MAC�A�h���X���w��
                           NULL �������� 0xFFFFFFFFFFFF�Ȃ�΂��ׂĂ̐e�@����������B
                           
                channel  - �e����������`�����l�����w��
                           0�Ȃ�΂��ׂẴ`�����l������������B

  Returns:      None.
 *---------------------------------------------------------------------------*/
BOOL WH_StartScan(WHStartScanCallbackFunc callback, const u8 *macAddr, u16 channel)
{
    WH_TRACE_STATE;
    WH_ASSERT(sSysState == WH_SYSSTATE_IDLE);

    WH_ChangeSysState(WH_SYSSTATE_SCANNING);

    sScanCallback = callback;
    sChannelIndex = channel;
    sScanExParam.channelList = 1;
    sAutoConnectFlag = FALSE;          // �����ڑ��͂��Ȃ�

    // ��������MAC�A�h���X�̏�����ݒ�
    if (macAddr != NULL)
    {
        *(u16 *)(&sScanExParam.bssid[4]) = *(u16 *)(macAddr + 4);
        *(u16 *)(&sScanExParam.bssid[2]) = *(u16 *)(macAddr + 2);
        *(u16 *)(&sScanExParam.bssid[0]) = *(u16 *)(macAddr);
    }
    else
    {
        MI_CpuFill8(&sScanExParam.bssid, 0xff, WM_SIZE_BSSID);
    }

    if (!WH_StateInStartScan())
    {
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return FALSE;
    }

    return TRUE;
}

/* ----------------------------------------------------------------------
  state : StartScan
  ---------------------------------------------------------------------- */
static BOOL WH_StateInStartScan(void)
{
    // ���̏�Ԃ̎��A�e�@��T�����܂��B
    WMErrCode result;
    u16     chanpat;

    WH_ASSERT(sSysState == WH_SYSSTATE_SCANNING);

    chanpat = WM_GetAllowedChannel();

    // �������g�p�\���ǂ������`�F�b�N����
    if (chanpat == 0x8000)
    {
        // 0x8000 ���Ԃ��Ă����ꍇ�́A����������������Ă��Ȃ��Ȃ�
        // �������C�u�����̏�Ԉُ��\���Ă���̂ŃG���[�ɂ��܂��B
        WH_REPORT_FAILURE(WM_ERRCODE_ILLEGAL_STATE);
        return FALSE;
    }
    if (chanpat == 0)
    {
        // �������g���Ȃ���ԁB
        WH_REPORT_FAILURE(WH_ERRCODE_NO_RADIO);
        return FALSE;
    }

    if (sChannelIndex == 0)
    {
        /* ���݂̎w�肩�珸���ɁA�\�ȃ`�����l�����������܂� */
        u32 chan = MATH_CountTrailingZeros(sScanExParam.channelList) + 1;
        while (TRUE)
        {
            chan++;
            if (chan > 16)
            {
                chan = 1;
            }

            if (chanpat & (0x0001 << (chan - 1)))
            {
                break;
            }
        }
        sScanExParam.channelList = (u16)(1 << (chan - 1));
    }
    else
    {
        sScanExParam.channelList = (u16)(1 << (sChannelIndex - 1));
    }

    sScanExParam.maxChannelTime = WM_GetDispersionScanPeriod();
    sScanExParam.scanBuf = (WMbssDesc *)sScanBuf;
    sScanExParam.scanBufSize = WM_SIZE_SCAN_EX_BUF;
    sScanExParam.scanType = WM_SCANTYPE_PASSIVE;
    sScanExParam.ssidLength = 0;
    MI_CpuFill8(sScanExParam.ssid, 0xFF, sizeof(sScanExParam.ssid));

    result = WM_StartScanEx(WH_StateOutStartScan, &sScanExParam);

    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }
    return TRUE;
}


static void WH_StateOutStartScan(void *arg)
{
    WMStartScanExCallback *cb = (WMStartScanExCallback *)arg;
    int i;
    BOOL found;

    // �X�L�����R�}���h�Ɏ��s�����ꍇ
    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return;
    }

    if (sSysState != WH_SYSSTATE_SCANNING)
    {
        // ��Ԃ��ύX����Ă���΃X�L�����I��
        sAutoConnectFlag = FALSE;      // �����ڑ����L�����Z��
        if (!WH_StateInEndScan())
        {
            WH_ChangeSysState(WH_SYSSTATE_ERROR);
        }
        return;
    }

    switch (cb->state)
    {
    case WM_STATECODE_PARENT_NOT_FOUND:
        break;

    case WM_STATECODE_PARENT_FOUND:
        // �e�@�����������ꍇ

        WH_TRACE("WH_StateOutStartScan : bssDescCount=%d\n", cb->bssDescCount);

        if ( cb->bssDescCount > 0 )
        {
            // BssDesc�̏��ARM7�����珑�����܂�Ă��邽��
            // �o�b�t�@�ɐݒ肳�ꂽBssDesc�̃L���b�V����j��
            DC_InvalidateRange(&sScanBuf, sizeof(sScanBuf));
        }

        found = FALSE;
        for ( i = 0; i < cb->bssDescCount; i++ )
        {
            WMBssDesc* bd = cb->bssDesc[i];

            // GUIDELINE : �K�C�h���C�������|�C���g(6.3.5)
            // ggid ���r���A����Ă����玸�s�Ƃ��܂��B
            // �܂��AWMBssDesc.gameInfoLength ���m�F���A
            // ggid �ɗL���Ȓl�������Ă��邱�Ƃ��璲�ׂ�K�v������܂��B
            WH_TRACE(" parent: MAC=%02x%02x%02x%02x%02x%02x ",
                     bd->bssid[0], bd->bssid[1], bd->bssid[2],
                     bd->bssid[3], bd->bssid[4], bd->bssid[5]);

            // �e�@���s�N�g�`���b�g���ǂ�������
            if (sPictoCatchFlag)
            {
                if (CHT_IsPictochatParent(bd))
                {
                    // �e�@���s�N�g�`���b�g�ł���ꍇ
                    WH_TRACE("pictochat parent find\n");
                    if (sScanCallback != NULL)
                    {
                        sScanCallback(bd);
                    }
                    continue;
                }
            }

            if ((!WM_IsValidGameInfo(&bd->gameInfo, bd->gameInfoLength))
                || bd->gameInfo.ggid != sParentParam.ggid)
            {
                // GGID������Ă���Ζ�������
                WH_TRACE("not my parent ggid \n");
                continue;
            }

            // �G���g���[�t���O�������Ă��Ȃ���Ύq�@����t���łȂ��̂Ŗ�������
            // �܂��}���`�u�[�g�t���O�������Ă���ꍇ�́ADS�_�E�����[�h�e�@�ł���̂Ŗ�������B
            if ((bd->gameInfo.gameNameCount_attribute & (WM_ATTR_FLAG_ENTRY | WM_ATTR_FLAG_MB))
                != WM_ATTR_FLAG_ENTRY)
            {
                WH_TRACE("not recieve entry\n");
                continue;
            }

            WH_TRACE("parent find\n");
            MI_CpuCopy8(bd, &sBssDesc, sizeof(WMBssDesc)); // �L���b�V���Z�[�t�ȃo�b�t�@�փR�s�[
            found = TRUE;
            break;
        }

        if (found)
        {
            // �R�[���o�b�N���K�v�Ȃ�ΌĂяo��
            if (sScanCallback != NULL)
            {
                sScanCallback(&sBssDesc);
            }

            // ���������e�@�Ɏ����ڑ��̂��߃X�L�����I��
            if (sAutoConnectFlag)
            {
                if (!WH_StateInEndScan())
                {
                    WH_ChangeSysState(WH_SYSSTATE_ERROR);
                }
                return;
            }
        }
        break;
    }

    // �`�����l����ύX���čăX�L�������J�n���܂��B
    if (!WH_StateInStartScan())
    {
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
    }
}

/* ----------------------------------------------------------------------
   state : EndScan
  ---------------------------------------------------------------------- */

/*---------------------------------------------------------------------------*
  Name:         WH_EndScan

  Description:  �e�@�̃r�[�R�����擾����֐�

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
BOOL WH_EndScan(void)
{
    if (sSysState != WH_SYSSTATE_SCANNING)
    {
        return FALSE;
    }

    sAutoConnectFlag = FALSE;
    WH_ChangeSysState(WH_SYSSTATE_BUSY);
    return TRUE;
}


static BOOL WH_StateInEndScan(void)
{
    WMErrCode result;
    WH_TRACE_STATE;

    // ���̏�Ԃł́A�X�L�����̏I���������s���܂��B
    result = WM_EndScan(WH_StateOutEndScan);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    return TRUE;
}

static void WH_StateOutEndScan(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        return;
    }

    WH_ChangeSysState(WH_SYSSTATE_IDLE);

    if (!sAutoConnectFlag)
    {
        // �X�L�����݂̂��w������Ă����ꍇ�͂����ŏ����͊������܂��B
        return;
    }

    // �����ڑ��̐ڑ��悪�m�肵���̂ŁA�����ڑ����[�h���������܂��B
    sAutoConnectFlag = FALSE;

    if (sChildWEPKeyGenerator != NULL)
    {
        // WEP Key Generator ���ݒ肳��Ă���΁AWEP Key �̐ݒ��
        if (!WH_StateInSetChildWEPKey())
        {
            WH_ChangeSysState(WH_SYSSTATE_ERROR);
        }
    }
    else
    {
        // �X�L�����I���������I�������̂ŁA���̂܂܎q�@�Ƃ��Ă̊�����
        // �J�n���܂��B
        if (!WH_StateInStartChild())
        {
            WH_TRACE("WH_StateOutEndScan : startchild failed\n");
            WH_ChangeSysState(WH_SYSSTATE_ERROR);
        }
    }
}

static BOOL WH_StateInSetChildWEPKey(void)
{
    u16     wepmode;
    WMErrCode result;
    WH_TRACE_STATE;

    WH_ChangeSysState(WH_SYSSTATE_BUSY);

    wepmode = (*sChildWEPKeyGenerator) (sWEPKey, &sBssDesc);
    result = WM_SetWEPKey(WH_StateOutSetChildWEPKey, wepmode, sWEPKey);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return FALSE;
    }

    return TRUE;
}

static void WH_StateOutSetChildWEPKey(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return;
    }

    // �q�@�Ƃ��Đe�@�ɐڑ����܂�
    if (!WH_StateInStartChild())
    {
        WH_TRACE("WH_StateOutSetChildWEPKey : startchild failed\n");
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
    }
}


/* ----------------------------------------------------------------------
   state : StartChild
  ---------------------------------------------------------------------- */
static BOOL WH_StateInStartChild(void)
{
    WMErrCode result;
    WH_TRACE_STATE;

    if ((sSysState == WH_SYSSTATE_CONNECTED)
        || (sSysState == WH_SYSSTATE_KEYSHARING) || (sSysState == WH_SYSSTATE_DATASHARING))
    {
        // ���ɐڑ��ς݁B
        WH_TRACE("WH_StateInStartChild : already connected?\n");
        return TRUE;
    }

    WH_ChangeSysState(WH_SYSSTATE_BUSY);

    result = WM_StartConnectEx(WH_StateOutStartChild, &sBssDesc, sConnectionSsid, TRUE,
                               (u16)((sChildWEPKeyGenerator !=
                                      NULL) ? WM_AUTHMODE_SHARED_KEY : WM_AUTHMODE_OPEN_SYSTEM));
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    return TRUE;
}

static void WH_StateOutStartChild(void *arg)
{
    // StartConnect �Őݒ肳�ꂽ�R�[���o�b�N�́A
    // 1) �ڑ������̊J�n��
    // 2) �F�؏I����
    // 3) �ڑ�������A�e�@������ؒf���ꂽ��
    // 4) �ő�䐔�ȏ�̐ڑ������悤�Ƃ�����
    // �Ƃ��������̃P�[�X�ŌĂ΂��̂ŁA�e�X��ʂ���K�v������܂��B
    // ���̊֐��̏ꍇ�A���֐i��ŗǂ��̂� 2) �̂Ƃ��݂̂ł��B

    WMStartConnectCallback *cb = (WMStartConnectCallback *)arg;
    WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);

        if (cb->errcode == WM_ERRCODE_OVER_MAX_ENTRY)
        {
            // GUIDELINE : �K�C�h���C�������|�C���g(6.3.7)
            // �����i�q�@�j���e�@�̑Ή��䐔���炠�ӂ�Ă��܂��Ă���ꍇ�B
            // �����ł͒v���I�G���[�Ƃ��܂��B
            // ���̎��́A���C�����ŉ����\������K�v������܂��B
            // �G���[�R�[�h�� WM_ERRCODE_OVER_MAX_ENTRY ���������ǂ�����
            // WH_GetLastError �֐��Ń`�F�b�N�o���܂��B
            WH_ChangeSysState(WH_SYSSTATE_ERROR);
            return;
        }
        else if (cb->errcode == WM_ERRCODE_NO_ENTRY)
        {
            // �����i�q�@�j���ڑ����悤�Ǝ��݂��e�@��
            // �G���g���[���󂯕t���Ă��Ȃ��ꍇ�B
            // �����ł͒v���I�G���[�Ƃ��܂��B
            // ���̎��́A���C�����ŉ����\������K�v������܂��B
            // �G���[�R�[�h�� WM_ERRCODE_NO_ENTRY ���������ǂ�����
            // WH_GetLastError �֐��Ń`�F�b�N�o���܂��B
            WH_ChangeSysState(WH_SYSSTATE_ERROR);
            return;
        }
        else if (cb->errcode == WM_ERRCODE_FAILED)
        {
            // �����i�q�@�j���ڑ����悤�Ǝ��݂��e�@�����Ȃ��Ȃ��Ă��܂�������
            // ���R�ŁA�ڑ��v�����^�C���A�E�g�����ꍇ�B
            // �����ł͒v���I�G���[�Ƃ��܂��B
            // ���̎��́A���C�����Ń��Z�b�g������Őڑ������g���C���邩�A
            // �������͉����\������K�v������܂��B
            // �G���[�R�[�h�� WM_ERRCODE_FAILED ���������ǂ�����
            // WH_GetLastError �֐��Ń`�F�b�N�o���܂��B
            WH_ChangeSysState(WH_SYSSTATE_CONNECT_FAIL);
            return;
        }
        else
        {
            // �e�@�̃`�����l���ݒ肪�s���ȏꍇ�ɂ�WM_ERRCODE_INVALID_PARAM��
            // �Ԃ�\��������B
            WH_ChangeSysState(WH_SYSSTATE_ERROR);
        }
        return;
    }

    if (cb->state == WM_STATECODE_BEACON_LOST)
    {
        // �ڑ����̐e�@����̃r�[�R���� 16 ��A���Ŏ�M���s���܂����B
        // �r�[�R�����������ƁAV �u�����N����������Ă��܂��\���ƁA
        // �e�@�����̃Z�b�V�������J�n���Ă���(TGID ���ς����)���Ƃ�
        // �C�Â��Ȃ��\��������܂��B

        // ���̃f���ł͓��ɉ����s���܂���B
        return;
    }

    if (cb->state == WM_STATECODE_CONNECTED)
    {
        // �F�؏I�����B
        // cb->aid �Ɏ����Ɋ���U��ꂽ AID �������Ă��܂��B
        WH_TRACE("Connect to Parent\n");
        WH_ChangeSysState(WH_SYSSTATE_CONNECTED);
        if (!WH_StateInStartChildMP())
        {
            /* FIXME : ������ BUSY �̂܂ܒu���Ă����ėǂ��̂�? */
            WH_TRACE("WH_StateInStartChildMP failed\n");
            WH_ChangeSysState(WH_SYSSTATE_BUSY);
            return;
        }

        // ������ aid ��ۑ����Ă����B
        sMyAid = cb->aid;
        return;

    }
    else if (cb->state == WM_STATECODE_CONNECT_START)
    {
        // �ڑ������̊J�n���B
        // �e�@�����Ȃ��Ȃ��Ă����ꍇ�Ȃǂ� WM_ERRCODE_FAILED ���A
        // �e�@���G���g���[���󂯕t���Ă��Ȃ��ꍇ�� WM_ERRCODE_NO_ENTRY ���A
        // �e�@�̐ڑ����������ς��̏ꍇ�� WM_ERRCODE_OVER_MAX_ENTRY ���A
        // ���ꂼ�� cb->errcode �ɕԂ��Ă��܂��B
        // �����ł͉����s�킸�A�F�؂̏I����҂��܂��B
        return;

    }
    else if (cb->state == WM_STATECODE_DISCONNECTED)
    {
        // GUIDELINE : �K�C�h���C�������|�C���g(6.3.1)
        // �e�@����ؒf����Ă��܂����ꍇ�B
        // ��(6.3.7)�Ɠ��l�̏����i�����\������j���K�v�ł��B
        // ���̎��̃G���[�R�[�h�́A WH_ERRCODE_DISCONNECTED �ł�
        // �i`WM_'�ł͂Ȃ����ɒ��Ӂj�B
        // �����Ȃ�ꍇ�i�e����ؒf�j�́A���ʂ̃Q�[�����ɂ����肦��
        // �ł��傤���A�����ł͂Ƃ肠�����G���[�̈��Ƃ��Ă�����
        // ���C�����őΉ����܂��B

        WH_TRACE("Disconnected from Parent\n");
        WH_SetError(WH_ERRCODE_DISCONNECTED);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return;
    }
    else if (cb->state == WM_STATECODE_DISCONNECTED_FROM_MYSELF)
    {
        // ����ؒf�����ꍇ�͏������s���܂���
        return;
    }

    WH_TRACE("unknown state %d, %s\n", cb->state, WH_GetWMStateCodeName(cb->state));
    WH_ChangeSysState(WH_SYSSTATE_ERROR);
}

/* ----------------------------------------------------------------------
   state : StartChildMP
   ---------------------------------------------------------------------- */
static BOOL WH_StateInStartChildMP(void)
{
    WMErrCode result;
    WH_TRACE_STATE;

    result = WM_StartMP(WH_StateOutStartChildMP,
                        (u16 *)sRecvBuffer,
                        (u16)sRecvBufferSize, (u16 *)sSendBuffer, (u16)sSendBufferSize,
                        WH_MP_FREQUENCY);

    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    return TRUE;
}

static void WH_StateOutStartChildMP(void *arg)
{
    WMStartMPCallback *cb = (WMStartMPCallback *)arg;
    // WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {

        if (cb->errcode == WM_ERRCODE_SEND_FAILED)
        {
            // WM_STATECODE_MPACK_IND �ɂ�����
            // MPACK �t���[���ɂ��e�@�̎�M�G���[���ʒm���ꂽ�ꍇ�B
            // �đ������Ȃǂ͕ʂ̏ꏊ�ōs���Ă��邽�߁A
            // �����ł͓��ɉ�������K�v�͂���܂���B�f�o�b�O�p�ł��B
            return;

        }
        else if (cb->errcode == WM_ERRCODE_TIMEOUT)
        {
            // MP �t���[���̎�M��A��莞�Ԍo���Ă� MPACK �t���[����
            // ��M�ł��Ȃ������ꍇ�B(cb->state == WM_STATECODE_MPACK_IND)
            // �đ������Ȃǂ͕ʂ̏ꏊ�ōs���Ă��邽�߁A
            // �����ł͓��ɉ�������K�v�͂���܂���B�f�o�b�O�p�ł��B
            return;

        }
        else if (cb->errcode == WM_ERRCODE_INVALID_POLLBITMAP)
        {
            // WM_STATECODE_MP_IND, WM_STATECODE_MPACK_IND �ɂ�����
            // �������ĂłȂ��t���[�����󂯎�����ꍇ�B
            // 3 ��ȏ�̒ʐM�ł��΂��Δ������邽�߁A
            // �v���I�G���[�ɂ��Ă͂����܂���B
            // �����ł͓��ɉ�������K�v�͂���܂���B�f�o�b�O�p�ł��B
            return;
        }

        WH_REPORT_FAILURE(cb->errcode);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return;
    }

    switch (cb->state)
    {
    case WM_STATECODE_MP_START:
        // StartMP ������I�������ʒm�B
        // ����ȍ~�A����M�\�ƂȂ�܂��B

        if (sConnectMode == WH_CONNECTMODE_KS_CHILD)
        {
            // �L�[�V�F�A�����O�w�肾�����ꍇ�B
            if (sSysState == WH_SYSSTATE_KEYSHARING)
            {
                // ���ɃL�[�V�F�A�����O��Ԃɂ���̂ŁA�������܂���B
                return;
            }

            if (sSysState == WH_SYSSTATE_CONNECTED)
            {
                // �X�� StartChildKeyShare �ֈڍs���܂��B
                if (!WH_StateInStartChildKeyShare())
                {
                    WH_TRACE("WH_StateInStartChildKeyShare failed\n");
                    (void)WH_Finalize();
                }
                return;
            }

        }
        else if (sConnectMode == WH_CONNECTMODE_DS_CHILD)
        {
            // �f�[�^�V�F�A�����O�w�肾�����ꍇ�́A WM_StartDataSharing ��
            // �Ăт܂��B���̊֐��͓����֐��Ȃ̂ŁAWH��Ԃ̑J�ڂ͂��Ă��܂���B
            WMErrCode result;
            u16     aidBitmap;

            aidBitmap = (u16)((1 << (WH_CHILD_MAX + 1)) - 1);   // ���� WH_CHILD_MAX+1 �r�b�g��1�� bitmap
            result = WM_StartDataSharing(&sDSInfo, WH_DS_PORT, aidBitmap, WH_DS_DATA_SIZE, sDataSharingDoubleMode);
            if (result != WM_ERRCODE_SUCCESS)
            {
                WH_REPORT_FAILURE(result);
                (void)WH_Finalize();
                return;
            }

            WH_TRACE("WH_StateOutStartChildMP : WM_StartDataSharing OK\n");
            WH_ChangeSysState(WH_SYSSTATE_DATASHARING);
            if (sPortExtraCallback)
            {
                (*sPortExtraCallback)(arg);
            }
            return;
        }

        WH_ChangeSysState(WH_SYSSTATE_CONNECTED);
        if (sPortExtraCallback)
        {
            (*sPortExtraCallback)(arg);
        }
        break;

    case WM_STATECODE_MP_IND:
        // �q�@�̎�M�����B

        // cb->recvBuf �Őe�@�����M�����f�[�^�t���[���̓��e���󂯎��܂����A
        // �ʏ�̃f�[�^��M�� WM_SetPortCallback �ŃR�[���o�b�N��ݒ肵�Ă��������B
        // �܂��A�f�[�^�V�F�A�����O�E�L�[�V�F�A�����O���g���ꍇ�́A
        // ��M�����͓����I�ɍs���܂��̂ŁAWM_SetPortCallback ���g���K�v������܂���B

        break;

    case WM_STATECODE_MPACK_IND:
        // MPACK �t���[���̎�M�ʒm�B�f�o�b�O�p�ł��B
        break;

    case WM_STATECODE_MPEND_IND:
        // �q�@�Ȃ炱���ւ͗��Ȃ��͂��ł��B

    default:
        WH_TRACE("unknown indicate, state = %d\n", cb->state);
        break;
    }
}

/* ----------------------------------------------------------------------
   state : StartChildKeyShare
  ---------------------------------------------------------------------- */
static BOOL WH_StateInStartChildKeyShare(void)
{
    WMErrCode result;
    WH_TRACE_STATE;

    if (sSysState == WH_SYSSTATE_KEYSHARING)
    {
        // ���ɃL�[�V�F�A�����O���Ă��܂��B
        return TRUE;
    }

    if (sSysState != WH_SYSSTATE_CONNECTED)
    {
        // �ڑ����Ă��܂���B
        return FALSE;
    }

    WH_ChangeSysState(WH_SYSSTATE_KEYSHARING);
    result = WM_StartKeySharing(&sWMKeySetBuf, WH_DS_PORT);

    if (result != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    return TRUE;
}

/* ----------------------------------------------------------------------
   state : EndChildKeyShare
   ---------------------------------------------------------------------- */
static BOOL WH_StateInEndChildKeyShare(void)
{
    // �L�[�V�F�A�����O���I�����܂��B
    WMErrCode result;
    WH_TRACE_STATE;

    if (sSysState != WH_SYSSTATE_KEYSHARING)
    {
        return FALSE;
    }

    WH_ChangeSysState(WH_SYSSTATE_BUSY);
    result = WM_EndKeySharing(&sWMKeySetBuf);

    if (result != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    if (!WH_StateInEndChildMP())
    {
        return FALSE;
    }

    return TRUE;
}

/* ----------------------------------------------------------------------
   state : EndChildMP
  ---------------------------------------------------------------------- */
static BOOL WH_StateInEndChildMP(void)
{
    // MP �ʐM���I�����܂��B
    WMErrCode result;
    WH_TRACE_STATE;

    WH_ChangeSysState(WH_SYSSTATE_BUSY);

    result = WM_EndMP(WH_StateOutEndChildMP);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }
    return TRUE;
}

static void WH_StateOutEndChildMP(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        (void)WH_Finalize();
        return;
    }

    if (!WH_StateInEndChild())
    {
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
    }
}

/* ----------------------------------------------------------------------
  state : EndChild
  ---------------------------------------------------------------------- */
static BOOL WH_StateInEndChild(void)
{
    WMErrCode result;
    WH_TRACE_STATE;

    WH_ChangeSysState(WH_SYSSTATE_BUSY);

    // �e�@�Ƃ̐ڑ���ؒf���܂��B
    result = WM_Disconnect(WH_StateOutEndChild, 0);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        WH_Reset();
        return FALSE;
    }

    return TRUE;
}

static void WH_StateOutEndChild(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;
    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        return;
    }
    // �����ŁA�q�@�Ƃ��Ă̐ؒf�����͊����� �A�C�h�����O�i�ҋ@���j��Ԃɖ߂�܂��B
    WH_ChangeSysState(WH_SYSSTATE_IDLE);
}

/* ----------------------------------------------------------------------
  state : Reset
  ---------------------------------------------------------------------- */
static BOOL WH_StateInReset(void)
{
    // ���̏�Ԃ́A�e�@�q�@���ʂł��B
    // �V�X�e����������Ԃɖ߂��܂��B
    WMErrCode result;
    WH_TRACE_STATE;

    WH_ChangeSysState(WH_SYSSTATE_BUSY);
    result = WM_Reset(WH_StateOutReset);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }
    return TRUE;
}

static void WH_StateOutReset(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;
    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        WH_REPORT_FAILURE(cb->errcode);
        return;
    }
    if (sPortExtraCallback)
    {
        (*sPortExtraCallback)(arg);
    }
    // Reset �͎��̏�Ԃ��J�n�����A�A�C�h�����O�i�ҋ@���j��Ԃɂ��܂��B
    WH_ChangeSysState(WH_SYSSTATE_IDLE);
}

/* ----------------------------------------------------------------------
   disconnect
  ---------------------------------------------------------------------- */
static BOOL WH_StateInDisconnectChildren(u16 bitmap)
{
    // ���̏�Ԃł́A�����Ŏw�肵���q�@�Ƃ̐ڑ���ؒf���܂��B
    WMErrCode result;
    WH_TRACE_STATE;

    result = WM_DisconnectChildren(WH_StateOutDisconnectChildren, bitmap);

    if (result == WM_ERRCODE_NO_CHILD)
    {
        return FALSE;
    }

    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    {
        OSIntrMode enabled = OS_DisableInterrupts();
        sConnectBitmap &= ~bitmap;
        (void)OS_RestoreInterrupts(enabled);
    }
    return TRUE;
}

static void WH_StateOutDisconnectChildren(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;
    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        return;
    }
}

/* ----------------------------------------------------------------------
   power off
  ---------------------------------------------------------------------- */
static BOOL WH_StateInPowerOff(void)
{
    // �����n�[�h�E�F�A�ւ̓d�͋������I�����܂��B
    // ���̏�Ԃ́A�e�@�q�@���ʂł��B
    WMErrCode result;
    WH_TRACE_STATE;

    result = WM_PowerOff(WH_StateOutPowerOff);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }
    return TRUE;
}

static void WH_StateOutPowerOff(void *arg)
{
    // �d���ؒf���Ԃł��B
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        return;
    }

    if (!WH_StateInDisable())
    {
        WH_ChangeSysState(WH_SYSSTATE_FATAL);
    }
}

/* ----------------------------------------------------------------------
   disable
  ---------------------------------------------------------------------- */
static BOOL WH_StateInDisable(void)
{
    // �����n�[�h�E�F�A�̎g�p�I����ʒm���܂��B
    // ���̏�Ԃ́A�e�@�q�@���ʂł��B
    WMErrCode result;
    WH_TRACE_STATE;

    result = WM_Disable(WH_StateOutDisable);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }
    return TRUE;
}

static void WH_StateOutDisable(void *arg)
{
    // �S�ďI�����܂����B
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
    }
}

/* ----------------------------------------------------------------------
  state : SetMPData
  ---------------------------------------------------------------------- */
static BOOL WH_StateInSetMPData(const void *data, u16 datasize, WHSendCallbackFunc callback)
{
    // ���̏�Ԃ́A�e�@�q�@���ʂł��B
    // �f�[�^���Z�b�g���A���M���܂��B
    WMErrCode result;
    // WH_TRACE_STATE;

    DC_FlushRange(sSendBuffer, (u32)sSendBufferSize);
    /* PXI�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    result = WM_SetMPDataToPortEx(WH_StateOutSetMPData,
                                  (void *)callback,
                                  data, datasize, 0xffff, WH_DATA_PORT, WH_DATA_PRIO);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_TRACE("WH_StateInSetMPData failed - %s\n", WH_GetWMErrCodeName(result));
        return FALSE;
    }
    return TRUE;
}

static void WH_StateOutSetMPData(void *arg)
{
    WMPortSendCallback *cb = (WMPortSendCallback *)arg;
    // WH_TRACE_STATE;

    // ���� callback ���Ă΂��܂ł́ASetMPDataToPort ��
    // �ݒ肵�����M�f�[�^�̃������̈���㏑�����Ă͂����܂���B

    // 0�`7�� port ���g�����ꍇ�́A���M�Ɏ��s�����ꍇ��
    // WM_ERRCODE_SEND_FAILED ���Ԃ�܂��B
    // �܂��A���M�L���[����t�������ꍇ�ɂ�
    // WM_ERRCODE_SEND_QUEUE_FULL ���Ԃ�܂��B

    if (sPortExtraCallback)
    {
        (*sPortExtraCallback)(arg);
    }
    if (cb->errcode != WM_ERRCODE_SUCCESS && cb->errcode != WM_ERRCODE_SEND_FAILED)
    {
        WH_REPORT_FAILURE(cb->errcode);
        return;
    }

    if (cb->arg != NULL)
    {
        WHSendCallbackFunc callback = (WHSendCallbackFunc) cb->arg;
        // �K�v�ɉ����āAWHSendCallbackFunc �^��ύX���A
        // cb->aid �� cb->data �Ȃǂ��󂯎��悤�ɂ��Ă��������B
        (*callback) ((cb->errcode == WM_ERRCODE_SUCCESS));
    }
}

static void WH_PortReceiveCallback(void *arg)
{
    WMPortRecvCallback *cb = (WMPortRecvCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
    }
    else if (sReceiverFunc != NULL)
    {
        if (cb->state == WM_STATECODE_PORT_INIT)
        {
            // ���������ɂ͉������܂���B
            // cb->aidBitmap �ɏ��������_�Őڑ����Ă��鑊��� aid ��
            // �ݒ肳��Ă��܂��B
        }
        if (cb->state == WM_STATECODE_PORT_RECV)
        {
            // �f�[�^����M�����̂ŁA�R�[���o�b�N���Ăт܂��B
            (*sReceiverFunc) (cb->aid, cb->data, cb->length);
        }
        else if (cb->state == WM_STATECODE_DISCONNECTED)
        {
            // �ؒf���ꂽ�|�� NULL ���M�Œʒm���܂��B
            (*sReceiverFunc) (cb->aid, NULL, 0);
        }
        else if (cb->state == WM_STATECODE_DISCONNECTED_FROM_MYSELF)
        {
            // �ߋ��Ƃ̌݊��̂��߁A����ؒf�����ꍇ�͒ʒm���܂���B
        }
        else if (cb->state == WM_STATECODE_CONNECTED)
        {
            // �ڑ����ꂽ�ꍇ�͉������܂���B
            // cb->aid �ɐڑ����Ă�������� aid ���A
            // cb->macAddress �ɑ���� MAC �A�h���X���A
            // �ݒ肳��Ă��܂��B
        }
    }
}


/* ----------------------------------------------------------------------
  state : End
  ---------------------------------------------------------------------- */

/* ----------------------------------------------------------------------
  state : WM_End
  ---------------------------------------------------------------------- */
static void WH_StateOutEnd(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_ChangeSysState(WH_SYSSTATE_FATAL);
        return;
    }
    WH_ChangeSysState(WH_SYSSTATE_STOP);
}


/* ======================================================================
   Public Interfaces
   ====================================================================== */


/**************************************************************************
 * �ȉ��́AWH �̊e��ݒ�l��ύX����֐��ł��B
 **************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         WH_SetGgid

  Description:  �Q�[���O���[�vID��ݒ肵�܂��B
                �e�@�̐ڑ��O�ɌĂяo���܂��B

  Arguments:    ggid    �ݒ肷��Q�[���O���[�vID.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WH_SetGgid(u32 ggid)
{
    sParentParam.ggid = ggid;
}

/*---------------------------------------------------------------------------*
  Name:         WH_SetSsid

  Description:  �q�@�ڑ����Ɏw�肷��SSID��ݒ肵�܂��B
                �q�@����̐ڑ��O�ɌĂяo���܂��B

  Arguments:    ssid    �ݒ肷��SSID���i�[���ꂽ�o�b�t�@�B
                length  �ݒ肷��SSID�̃f�[�^���B
                        WM_SIZE_CHILD_SSID(24�o�C�g)�ɖ����Ȃ��ꍇ��
                        �㑱����]����0�Ŗ��߁A����ꍇ�ɂ͐؂�̂Ă܂��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WH_SetSsid(const void *ssid, u32 length)
{
    length = (u32)MATH_MIN(length, WM_SIZE_CHILD_SSID);
    MI_CpuCopy8(ssid, sConnectionSsid, length);
    MI_CpuClear8(sConnectionSsid + length, (u32)(WM_SIZE_CHILD_SSID - length));
}

/*---------------------------------------------------------------------------*
  Name:         WH_SetUserGameInfo

  Description:  ���[�U��`�̐e�@����ݒ肵�܂��B
                �e�@�̐ڑ��O�ɌĂяo���܂��B

  Arguments:    userGameInfo  ���[�U��`�̐e�@���ւ̃|�C���^
                length        ���[�U��`�̐e�@���̃T�C�Y

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WH_SetUserGameInfo(u16 *userGameInfo, u16 length)
{
    SDK_ASSERT(length <= WM_SIZE_USER_GAMEINFO);
    SDK_ASSERT((userGameInfo != NULL) || (length > 0));
    SDK_ASSERT(sSysState == WH_SYSSTATE_IDLE);

    // beacon �Ƀ��[�U��`�̃f�[�^���ڂ���ꍇ�͂����Ɏw�肵�܂��B
    // �q�@�̐e�@�I����ʂŐe�@�� Nickname ��\�����������ꍇ�Ȃǂ�
    // �����ɏ����Z�b�g���Đe�@����q�@�ɓ`���邱�ƂɂȂ�܂��B
    sParentParam.userGameInfo = userGameInfo;
    sParentParam.userGameInfoLength = length;
}

/*---------------------------------------------------------------------------*
  Name:         WH_SetDebugOutput

  Description:  �f�o�b�O������o�͗p�̊֐���ݒ肵�܂��B

  Arguments:    func    �ݒ肷��f�o�b�O������o�͗p�̊֐�.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WH_SetDebugOutput(void (*func) (const char *, ...))
{
    OSIntrMode enabled = OS_DisableInterrupts();
    wh_trace = func;
    (void)OS_RestoreInterrupts(enabled);
}


/**************************************************************************
 * �ȉ��́AWM ���C�u�����̏�Ԃ��擾���郉�b�p�[�֐��ł��B
 **************************************************************************/

/* ----------------------------------------------------------------------
  Name:        WH_GetLinkLevel
  Description: �d�g�̎�M���x���擾���܂��B
  Arguments:   none.
  Returns:     WMLinkLevel �̐��l��Ԃ��܂��B
  ---------------------------------------------------------------------- */
int WH_GetLinkLevel(void)
{
    return (int)WM_GetLinkLevel();
}

/* ----------------------------------------------------------------------
   Name:        WH_GetAllowedChannel
   Description: �ڑ��Ɏg�p�o����`�����l���̃r�b�g�p�^�[�����擾���܂��B
   Arguments:   none.
   Returns:     channel pattern
   ---------------------------------------------------------------------- */
u16 WH_GetAllowedChannel(void)
{
    return WM_GetAllowedChannel();
}


/**************************************************************************
 * �ȉ��́AWH �̏�Ԃ��擾����֐��ł��B
 **************************************************************************/

/* ----------------------------------------------------------------------
   Name:        WH_GetBitmap
   Description: �ڑ���Ԃ������r�b�g�p�^�[�����擾���܂��B
   Arguments:   none.
   Returns:     bitmap pattern
   ---------------------------------------------------------------------- */
u16 WH_GetBitmap(void)
{
    return sConnectBitmap;
}

/* ----------------------------------------------------------------------
   Name:        WH_GetSystemState
   Description: WH �̓�����Ԃ��擾���܂��B
   Arguments:   none.
   Returns:     ������ԁiWH_SYSSTATE_XXXX�j�B
   ---------------------------------------------------------------------- */
int WH_GetSystemState(void)
{
    return sSysState;
}

/* ----------------------------------------------------------------------
   Name:        WH_GetConnectMode
   Description: �ڑ������擾���܂��B
   Arguments:   none.
   Returns:     �ڑ����iWH_CONNECTMODE_XX_XXXX�j�B
   ---------------------------------------------------------------------- */
int WH_GetConnectMode(void)
{
    return sConnectMode;
}

/* ----------------------------------------------------------------------
   Name:        WH_GetLastError
   Description: �ł��ŋ߂ɋN�����G���[�̃R�[�h���擾���܂��B
   Arguments:   none.
   Returns:     �G���[�R�[�h�B
   ---------------------------------------------------------------------- */
int WH_GetLastError(void)
{
    return sErrCode;
}

/*---------------------------------------------------------------------------*
  Name:         WH_PrintBssDesc

  Description:  WMBssDesc �\���̂̃����o���f�o�b�O�o�͂���B

  Arguments:    info    �f�o�b�O�o�͂�����BssDesc�ւ̃|�C���^.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WH_PrintBssDesc(WMBssDesc *info)
{
#pragma unused( info )
    u16     i;

    OS_TPrintf("length = 0x%04x\n", info->length);
    OS_TPrintf("rssi   = 0x%04x\n", info->rssi);
    OS_TPrintf("bssid = %02x%02x%02x%02x%02x%02x\n", info->bssid[0], info->bssid[1], info->bssid[2],
               info->bssid[3], info->bssid[4], info->bssid[5]);
    OS_TPrintf("ssidLength = 0x%04x\n", info->ssidLength);
    OS_TPrintf("ssid = ");
    for (i = 0; i < 32; i++)
    {
        OS_TPrintf("0x%02x", info->ssid[i]);
    }
    OS_TPrintf("\n");
    OS_TPrintf("capaInfo        = 0x%04x\n", info->capaInfo);
    OS_TPrintf("rateSet.basic   = 0x%04x\n", info->rateSet.basic);
    OS_TPrintf("rateSet.support = 0x%04x\n", info->rateSet.support);
    OS_TPrintf("beaconPeriod    = 0x%04x\n", info->beaconPeriod);
    OS_TPrintf("dtimPeriod      = 0x%04x\n", info->dtimPeriod);
    OS_TPrintf("channel         = 0x%04x\n", info->channel);
    OS_TPrintf("cfpPeriod       = 0x%04x\n", info->cfpPeriod);
    OS_TPrintf("cfpMaxDuration  = 0x%04x\n", info->cfpMaxDuration);
    OS_TPrintf("gameInfoLength  = 0x%04x\n", info->gameInfoLength);
    OS_TPrintf("gameInfo.magicNumber = 0x%04x\n", info->gameInfo.magicNumber);
    OS_TPrintf("gameInfo.ver    = 0x%02x\n", info->gameInfo.ver);
    OS_TPrintf("gameInfo.ggid   = 0x%08x\n", info->gameInfo.ggid);
    OS_TPrintf("gameInfo.tgid   = 0x%04x\n", info->gameInfo.tgid);
    OS_TPrintf("gameInfo.userGameInfoLength = 0x%02x\n", info->gameInfo.userGameInfoLength);
    OS_TPrintf("gameInfo.gameNameCount_attribute = 0x%02x\n",
               info->gameInfo.gameNameCount_attribute);
    OS_TPrintf("gameInfo.parentMaxSize   = 0x%04x\n", info->gameInfo.parentMaxSize);
    OS_TPrintf("gameInfo.childMaxSize    = 0x%04x\n", info->gameInfo.childMaxSize);
}


/**************************************************************************
 * �ȉ��́A�`�����l���Ɋւ��鏈�����s���֐��ł��B
 **************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         WH_StartMeasureChannel

  Description:  �`�����l���d�g�g�p���̒������J�n

  Arguments:    None.

  Returns:     �V�[�P���X�J�n�ɐ�������ΐ^�B
 *---------------------------------------------------------------------------*/
BOOL WH_StartMeasureChannel(void)
{
#define MAX_RATIO 100                  // �`�����l���g�p����0�`100�͈̔�
    u16     result;
    u8      macAddr[6];

    OS_GetMacAddress(macAddr);
    RAND_INIT(OS_GetVBlankCount() + *(u16 *)&macAddr[0] + *(u16 *)&macAddr[2] + *(u16 *)&macAddr[4]);   // ����������
    RAND();

    sChannel = 0;
    sChannelBusyRatio = MAX_RATIO + 1;

    WH_ChangeSysState(WH_SYSSTATE_BUSY);

    result = WH_StateInMeasureChannel(1);

    if (result == WH_ERRCODE_NOMORE_CHANNEL)
    {
        // �g�p�ł���`�����l����1���Ȃ�
        // ���������d�g���g���Ȃ�����ȏ󋵂�
        // ����Ƃ������ƂȂ̂ŁA�����ł̓G���[�ŕԂ��Ă��܂�
        // �i�{���͂����ŉ����\������K�v������܂��j�B
        WH_REPORT_FAILURE(WH_ERRCODE_NOMORE_CHANNEL);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return FALSE;
    }

    if (result != WM_ERRCODE_OPERATING)
    {
        // �G���[�I��
        WH_REPORT_FAILURE(result);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         WH_StateInMeasureChannel

  Arguments:    channel     �������J�n����`�����l���ԍ�

  Returns:      WM_ERRCODE_SUCCESS        - ������
                WM_ERRCODE_NOMORE_CHANNEL - ������������`�����l�����Ȃ�
                WM_ERRCODE_API_ERR        - WM_MeasureChannel��API�Ăяo�����s
 *---------------------------------------------------------------------------*/
static u16 WH_StateInMeasureChannel(u16 channel)
{
    u16     allowedChannel;
    u16     result;

    allowedChannel = WM_GetAllowedChannel();

    if (allowedChannel == 0x8000)
    {
        // 0x8000 ���Ԃ��Ă����ꍇ�́A����������������Ă��Ȃ��Ȃ�
        // �������C�u�����̏�Ԉُ��\���Ă���̂ŃG���[�ɂ��܂��B
        WH_REPORT_FAILURE(WM_ERRCODE_ILLEGAL_STATE);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    if (allowedChannel == 0)
    {
        // 0���Ԃ��Ă����ꍇ�A���������d�g���g���Ȃ�����ȏ󋵂�
        // ����Ƃ������ƂȂ̂ŁA�g�p�ł���`�����l�����Ȃ�����Ԃ��܂��B
        WH_REPORT_FAILURE(WH_ERRCODE_NO_RADIO);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return WH_ERRCODE_NOMORE_CHANNEL;
    }

    while (((1 << (channel - 1)) & allowedChannel) == 0)
    {
        channel++;
        if (channel > 16)
        {
            /* �����ꂽ�`�����l�������ׂĒ��׏I������ꍇ */
            return WH_ERRCODE_NOMORE_CHANNEL;
        }
    }

    result = WHi_MeasureChannel(WH_StateOutMeasureChannel, channel);
    if (result != WM_ERRCODE_OPERATING)
    {
        return result;
    }
    return result;
}

/*---------------------------------------------------------------------------*
  Name:         WH_StateOutMeasureChannel

  Arguments:    arg     ���������ʂ�ʒm����WMMeasureChannelCallback�\����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WH_StateOutMeasureChannel(void *arg)
{
    u16     result;
    u16     channel;
    WMMeasureChannelCallback *cb = (WMMeasureChannelCallback *)arg;
    WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        // �����s�s�����������ꍇ�B
        // MeasureChannel �Ŏ��s����悤�Ȃ�ǂ݂̂��d�g���g���Ȃ��A��
        // �l������̂ŁA�G���[��Ԃɂ��܂��B
        WH_REPORT_FAILURE(cb->errcode);
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return;
    }

    WH_TRACE("channel %d bratio = 0x%x\n", cb->channel, cb->ccaBusyRatio);

    channel = cb->channel;

    /* ���g�p���̒Ⴂ�`�����l�����擾 (�����l 101% �Ȃ̂Ő擪�͕K���I��) */
    if (sChannelBusyRatio > cb->ccaBusyRatio)
    {
        sChannelBusyRatio = cb->ccaBusyRatio;
        sChannelBitmap = (u16)(1 << (channel - 1));
    }
    else if (sChannelBusyRatio == cb->ccaBusyRatio)
    {
        sChannelBitmap |= 1 << (channel - 1);
    }

    result = WH_StateInMeasureChannel(++channel);

    if (result == WH_ERRCODE_NOMORE_CHANNEL)
    {
        // �`�����l�������I��
        WH_ChangeSysState(WH_SYSSTATE_MEASURECHANNEL);
        return;
    }

    if (result != WM_ERRCODE_OPERATING)
    {
        // �G���[�I��
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        return;
    }
}

/* ----------------------------------------------------------------------
  �d�g�g�p���̃`�F�b�N
  ---------------------------------------------------------------------- */
static WMErrCode WHi_MeasureChannel(WMCallbackFunc func, u16 channel)
{
#define WH_MEASURE_TIME         30     // 1�t���[���Ɉ��ʐM���Ă���d�g���E���邾���̊Ԋu(ms)
#define WH_MEASURE_CS_OR_ED     3      // �L�����A�Z���X��ED�l�̘_���a
#define WH_MEASURE_ED_THRESHOLD 17     // �����f�[�^�ɂ��o���I�ɗL���Ǝv���邨����ED臒l

    /*
     * �d�g�g�p���擾�p�����[�^�Ƃ��āA
     * �����ɂ��o���I�ɗL���Ǝv����l�����Ă��܂��B
     */
    return WM_MeasureChannel(func,     /* �R�[���o�b�N�ݒ� */
                             WH_MEASURE_CS_OR_ED,       /* CS or ED */
                             WH_MEASURE_ED_THRESHOLD,   /* ��2�������L�����A�Z���X�݂̂̏ꍇ�͖��� */
                             channel,  /* ����̌����`�����l�� */
                             WH_MEASURE_TIME);  /*�P�`�����l��������̒�������[ms] */
}


/*---------------------------------------------------------------------------*
  Name:         WH_GetMeasureChannel

  Description:  ���p�\�Ȓ������Ԏg�p���̒Ⴂ�`�����l����Ԃ��܂��B
                WH_MeasureChannel�̓��삪������WH_SYSSTATE_MEASURECHANNEL���
                �ɂȂ��Ă���K�v������܂��B
                ���̊֐����R�[�������ƈ�Ԏg�p���̒Ⴂ�`�����l����Ԃ�
                WH_SYSSTATE_IDLE��ԂɑJ�ڂ��܂��B
                
  Arguments:    None.

  Returns:      �����Ƃ��g�p���̒Ⴂ���p�\�ȃ`�����l���ԍ�.
 *---------------------------------------------------------------------------*/
u16 WH_GetMeasureChannel(void)
{
    WH_ASSERT(sSysState == WH_SYSSTATE_MEASURECHANNEL);

    WH_ChangeSysState(WH_SYSSTATE_IDLE);
    sChannel = (u16)SelectChannel(sChannelBitmap);
    WH_TRACE("decided channel = %d\n", sChannel);
    return sChannel;
}


/*---------------------------------------------------------------------------*
  Name:         SelectChannel

  Description:  �ł��d�g�g�p���̒Ⴉ�����`�����l�����擾���܂��B
                �ł��d�g�g�p���̒Ⴂ�`�����l���������������ꍇ�ɂ́A
                �������g�p����B
                
  Arguments:    �`�����l���r�b�g�}�b�v.

  Returns:      �����Ƃ��g�p���̒Ⴂ���p�\�ȃ`�����l���ԍ�.
 *---------------------------------------------------------------------------*/
static s16 SelectChannel(u16 bitmap)
{
    s16     i;
    s16     channel = 0;
    u16     num = 0;
    u16     select;

    for (i = 0; i < 16; i++)
    {
        if (bitmap & (1 << i))
        {
            channel = (s16)(i + 1);
            num++;
        }
    }

    if (num <= 1)
    {
        return channel;
    }

    // �����d�g�g�p���̃`�����l�����������݂����ꍇ
    select = (u16)(((RAND() & 0xFF) * num) / 0x100);

    channel = 1;

    for (i = 0; i < 16; i++)
    {
        if (bitmap & 1)
        {
            if (select == 0)
            {
                return (s16)(i + 1);
            }
            select--;
        }
        bitmap >>= 1;
    }

    return 0;
}


/**************************************************************************
 * �ȉ��́A���������������ĒʐM�\��Ԃ܂őJ�ڂ���֐��ł��B
 **************************************************************************/

/* ----------------------------------------------------------------------
   Name:        WH_Initialize
   Description: ��������Ƃ��s���A�������V�[�P���X���J�n���܂��B
   Arguments:   None.
   Returns:     �V�[�P���X�J�n�ɐ�������ΐ^�B
   ---------------------------------------------------------------------- */
BOOL WH_Initialize(void)
{
    if (whInitialized)
    {
        return FALSE;
    }

    if (sIndicationCallback == NULL)
    {
        sIndicationCallback = WH_IndicateHandler;
    }

    sRecvBufferSize = 0;
    sSendBufferSize = 0;

    sReceiverFunc = NULL;
    sMyAid = 0;
    sConnectBitmap = WH_BITMAP_EMPTY;
    sErrCode = WM_ERRCODE_SUCCESS;

    sParentParam.userGameInfo = NULL;
    sParentParam.userGameInfoLength = 0;

    MI_CpuClear8(sScanBuf, sizeof(sScanBuf));
    MI_CpuClear8(&sBssDesc, sizeof(sBssDesc));

    MI_CpuClear8(sConnectionSsid, sizeof(sConnectionSsid));
    // �ڑ��q�@�̃��[�U����֐���NULL (multiboot)
    sJudgeAcceptFunc = NULL;

    // �������V�[�P���X�J�n�B
    if (!WH_StateInInitialize())
    {
        return FALSE;
    }

    whInitialized = 1;

    return TRUE;
}

/* ----------------------------------------------------------------------
   Indicate handler
   ---------------------------------------------------------------------- */
static void WH_IndicateHandler(void *arg)
{
    WMindCallback *cb = (WMindCallback *)arg;

    if (cb->errcode == WM_ERRCODE_FIFO_ERROR)
    {
        // �����s�\�ȃG���[�����������ꍇ�ł��B
        // �v���C���[�ɂ��̎|��ʒm�����ʂȂǂ�\�����A
        // ���C�����X�ʐM�������~�����ĉ������B
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        OS_TPanic("Wireless FATAL error occured.\n");
    }
}

/* ----------------------------------------------------------------------
   state : Initialize
   ---------------------------------------------------------------------- */
static BOOL WH_StateInInitialize(void)
{
    // �������V�[�P���X���J�n���܂��B
    WMErrCode result;
    WH_TRACE_STATE;

#ifndef WH_USE_DETAILED_INITIALIZE
    WH_ChangeSysState(WH_SYSSTATE_BUSY);
    result = WM_Initialize(&sWmBuffer, WH_StateOutInitialize, WH_DMA_NO);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        WH_ChangeSysState(WH_SYSSTATE_FATAL);
        return FALSE;
    }

#else
    // WM_Init �͓����֐��ł��B
    result = WM_Init(&sWmBuffer, WH_DMA_NO);
    if (result != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    WH_ChangeSysState(WH_SYSSTATE_BUSY);

    if (!WH_StateInEnable())
    {
        WH_ChangeSysState(WH_SYSSTATE_FATAL);
        return FALSE;
    }
#endif

    return TRUE;
}


#ifndef WH_USE_DETAILED_INITIALIZE

static void WH_StateOutInitialize(void *arg)
{
    // �d���������Ԃł��B
    WMErrCode result;
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        WH_ChangeSysState(WH_SYSSTATE_FATAL);
        return;
    }
    // �s����ɔ�������ʒm���󂯎��R�[���o�b�N�֐���ݒ肵�܂��B
    result = WM_SetIndCallback(sIndicationCallback);
    if (result != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(result);
        WH_ChangeSysState(WH_SYSSTATE_FATAL);
        return;
    }

    // �V�X�e����Ԃ��A�C�h�����O�i�ҋ@���j�ɕύX�B
    WH_ChangeSysState(WH_SYSSTATE_IDLE);

    // ���̏�Ԃ��Z�b�g���Ȃ��̂ŁA�����ŃV�[�P���X�͂�������I���ł��B
    // ���̏�Ԃ� WH_Connect ���Ă΂��Ɛڑ���ƂɈڍs���܂��B
}

#else

/* ----------------------------------------------------------------------
   enable
  ---------------------------------------------------------------------- */
static BOOL WH_StateInEnable(void)
{
    // �n�[�h�E�F�A���g�p�\�ɂ��܂��i�g�p���𓾂܂��j�B
    WMErrCode result;
    WH_TRACE_STATE;

    result = WM_Enable(WH_StateOutEnable);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        WH_ChangeSysState(WH_SYSSTATE_FATAL);
        return FALSE;
    }
    return TRUE;
}

static void WH_StateOutEnable(void *arg)
{
    // �n�[�h�E�F�A�̎g�p�������ꂽ��A�d��������Ԃֈڍs���܂��B
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        WH_ChangeSysState(WH_SYSSTATE_FATAL);
        return;
    }

    if (!WH_StateInPowerOn())
    {
        WH_ChangeSysState(WH_SYSSTATE_FATAL);
        return;
    }
}

/* ----------------------------------------------------------------------
   power on
  ---------------------------------------------------------------------- */
static BOOL WH_StateInPowerOn(void)
{
    // �����n�[�h�E�F�A���g�p�\�ɂȂ����̂ŁA�d�͋������J�n���܂��B
    WMErrCode result;
    WH_TRACE_STATE;

    result = WM_PowerOn(WH_StateOutPowerOn);
    if (result != WM_ERRCODE_OPERATING)
    {
        WH_REPORT_FAILURE(result);
        WH_ChangeSysState(WH_SYSSTATE_FATAL);
        return FALSE;
    }

    return TRUE;
}

static void WH_StateOutPowerOn(void *arg)
{
    // �d���������Ԃł��B
    WMErrCode result;
    WMCallback *cb = (WMCallback *)arg;
    WH_TRACE_STATE;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(cb->errcode);
        WH_ChangeSysState(WH_SYSSTATE_FATAL);
        return;
    }

    // �s����ɔ�������ʒm���󂯎��R�[���o�b�N�֐���ݒ肵�܂��B
    result = WM_SetIndCallback(sIndicationCallback);
    if (result != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(result);
        WH_ChangeSysState(WH_SYSSTATE_FATAL);
        return;
    }

    // �V�X�e����Ԃ��A�C�h�����O�i�ҋ@���j�ɕύX�B
    WH_ChangeSysState(WH_SYSSTATE_IDLE);

    // ���̏�Ԃ��Z�b�g���Ȃ��̂ŁA�����ŃV�[�P���X�͂�������I���ł��B
    // ���̏�Ԃ� WH_Connect ���Ă΂��Ɛڑ���ƂɈڍs���܂��B
}

#endif // #ifdef WH_USE_DETAILED_INITIALIZE


/* ----------------------------------------------------------------------
  Name:        WH_ParentConnect
  Description: �ڑ��V�[�P���X���J�n���܂��B
  Arguments:   mode    - WH_CONNECTMODE_MP_PARENT �Ȃ�ΐe�@�Ƃ���MP�J�n�B
                         WH_CONNECTMODE_DS_PARENT �Ȃ�ΐe�@�Ƃ���DataSharing�J�n�B
                         WH_CONNECTMODE_KS_PARENT �Ȃ�ΐe�@�Ƃ���KeySharing�J�n�B
               tgid    - �e�@�ʐMtgid
               channel - �e�@�ʐMchannel
  Returns:     �ڑ��V�[�P���X�J�n�ɐ�������ΐ^�B
  ---------------------------------------------------------------------- */
BOOL WH_ParentConnect(int mode, u16 tgid, u16 channel)
{
    // �ҋ@��ԂɂȂ���ΐڑ��V�[�P���X���J�n�o���܂���B
    WH_ASSERT(sSysState == WH_SYSSTATE_IDLE);

    // WM_StartMP() �p�̑���M�o�b�t�@�T�C�Y�v�Z
    // ���O�ɐÓI�Ƀo�b�t�@���m�ۂ������ꍇ�� WM_SIZE_MP_* �֐��}�N�����A
    // ���I�Ɋm�ۂ��č\��Ȃ��ꍇ�́A�e�q�ڑ���� WM_StartMP() ���Ăяo�����O��
    // WM_GetReceiveBufferSize() API ��p���܂��B
    // ���l�Ɏ��O�ɐÓI�Ƀo�b�t�@���m�ۂ������ꍇ�� WM_SIZE_MP_* �֐��}�N�����A
    // ���I�Ɋm�ۂ��č\��Ȃ��ꍇ�́A�e�q�ڑ���� WM_StartMP() ���Ăяo�����O��
    // WM_GetSendBufferSize() API ��p���܂��B
    sRecvBufferSize = WH_PARENT_RECV_BUFFER_SIZE;
    sSendBufferSize = WH_PARENT_SEND_BUFFER_SIZE;

    WH_TRACE("recv buffer size = %d\n", sRecvBufferSize);
    WH_TRACE("send buffer size = %d\n", sSendBufferSize);

    sConnectMode = mode;
    WH_ChangeSysState(WH_SYSSTATE_BUSY);

    sParentParam.tgid = tgid;
    sParentParam.channel = channel;
    sParentParam.beaconPeriod = WM_GetDispersionBeaconPeriod();
    sParentParam.parentMaxSize = WH_PARENT_MAX_SIZE;
    sParentParam.childMaxSize = WH_CHILD_MAX_SIZE;
    sParentParam.maxEntry = WH_CHILD_MAX;
    sParentParam.CS_Flag = 0;
    sParentParam.multiBootFlag = 0;
    sParentParam.entryFlag = 1;
    sParentParam.KS_Flag = (u16)((mode == WH_CONNECTMODE_KS_PARENT) ? 1 : 0);

    switch (mode)
    {
    case WH_CONNECTMODE_MP_PARENT:
    case WH_CONNECTMODE_KS_PARENT:
    case WH_CONNECTMODE_DS_PARENT:
        // �e�@���[�h�Őڑ��J�n�B
        return WH_StateInSetParentParam();
    default:
        break;
    }

    WH_TRACE("unknown connect mode %d\n", mode);
    return FALSE;
}


/* ----------------------------------------------------------------------
  Name:        WH_ChildConnect
  Description: �ڑ��V�[�P���X���J�n���܂��B
  Arguments:   mode -    WH_CONNECTMODE_MP_CHILD �Ȃ�Ύq�@�Ƃ���MP�J�n�B
                         WH_CONNECTMODE_DS_CHILD �Ȃ�Ύq�@�Ƃ���DataSharing�J�n�B
                         WH_CONNECTMODE_KS_CHILD �Ȃ�Ύq�@�Ƃ���KeySharing�J�n�B
               bssDesc - �ڑ�����e�@��bssDesc
                
  Returns:     �ڑ��V�[�P���X�J�n�ɐ�������ΐ^�B
  ---------------------------------------------------------------------- */
BOOL WH_ChildConnect(int mode, WMBssDesc *bssDesc)
{
    // �ҋ@��ԂɂȂ���ΐڑ��V�[�P���X���J�n�o���܂���B
    WH_ASSERT(sSysState == WH_SYSSTATE_IDLE);

    // WM_StartMP() �p�̑���M�o�b�t�@�T�C�Y�v�Z
    // ���O�ɐÓI�Ƀo�b�t�@���m�ۂ������ꍇ�� WM_SIZE_MP_* �֐��}�N�����A
    // ���I�Ɋm�ۂ��č\��Ȃ��ꍇ�́A�e�q�ڑ���� WM_StartMP() ���Ăяo�����O��
    // WM_GetReceiveBufferSize() API ��p���܂��B
    // ���l�Ɏ��O�ɐÓI�Ƀo�b�t�@���m�ۂ������ꍇ�� WM_SIZE_MP_* �֐��}�N�����A
    // ���I�Ɋm�ۂ��č\��Ȃ��ꍇ�́A�e�q�ڑ���� WM_StartMP() ���Ăяo�����O��
    // WM_GetSendBufferSize() API ��p���܂��B
    sRecvBufferSize = WH_CHILD_RECV_BUFFER_SIZE;
    sSendBufferSize = WH_CHILD_SEND_BUFFER_SIZE;

    WH_TRACE("recv buffer size = %d\n", sRecvBufferSize);
    WH_TRACE("send buffer size = %d\n", sSendBufferSize);

    sConnectMode = mode;
    WH_ChangeSysState(WH_SYSSTATE_BUSY);

    switch (mode)
    {
    case WH_CONNECTMODE_MP_CHILD:
    case WH_CONNECTMODE_KS_CHILD:
    case WH_CONNECTMODE_DS_CHILD:
        // �q�@���[�h�Őڑ��J�n�B
        // �ۑ�����Ă����e�@��BssDesc���g�p���ăX�L���������Őڑ�����B
        MI_CpuCopy8(bssDesc, &sBssDesc, sizeof(WMBssDesc));
        if (sChildWEPKeyGenerator != NULL)
        {
            // WEP Key Generator ���ݒ肳��Ă���΁AWEP Key �̐ݒ��
            return WH_StateInSetChildWEPKey();
        }
        else
        {
            return WH_StateInStartChild();
        }
    default:
        break;
    }

    WH_TRACE("unknown connect mode %d\n", mode);
    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         WH_SetJudgeAcceptFunc

  Description:  �q�@�̐ڑ��󂯓���𔻒肷�邽�߂̊֐����Z�b�g���܂��B

  Arguments:    �q�@�̐ڑ�����֐���ݒ�.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WH_SetJudgeAcceptFunc(WHJudgeAcceptFunc func)
{
    sJudgeAcceptFunc = func;
}


/**************************************************************************
 * �ȉ��́AWH_DATA_PORT �|�[�g���g�p���钼�ړI�� MP �ʐM�̊֐��ł��B
 **************************************************************************/

/* ----------------------------------------------------------------------
   Name:        WH_SetReceiver
   Description: WH_DATA_PORT �|�[�g�Ƀf�[�^��M�R�[���o�b�N��ݒ肵�܂��B
   Arguments:   proc - �f�[�^��M�R�[���o�b�N
   Returns:     none.
   ---------------------------------------------------------------------- */
void WH_SetReceiver(WHReceiverFunc proc)
{
    sReceiverFunc = proc;
    if (WM_SetPortCallback(WH_DATA_PORT, WH_PortReceiveCallback, NULL) != WM_ERRCODE_SUCCESS)
    {
        WH_ChangeSysState(WH_SYSSTATE_ERROR);
        WH_TRACE("WM not Initialized\n");
    }
}

/* ----------------------------------------------------------------------
   Name:        WH_SendData
   Description: WH_DATA_PORT �|�[�g�Ƀf�[�^���M���J�n���܂��B
               �iMP�ʐM�p�B�f�[�^�V�F�A�����O���ȂǂɌĂԕK�v�͂���܂���j
   Arguments:   size - �f�[�^�T�C�Y
   Returns:     ���M�J�n�ɐ�������ΐ^�B
   ---------------------------------------------------------------------- */
BOOL WH_SendData(const void *data, u16 size, WHSendCallbackFunc callback)
{
    return WH_StateInSetMPData(data, size, callback);
}


/**************************************************************************
 * �ȉ��́A�f�[�^�V�F�A�����O�ʐM�𐧌䂷��֐��ł��B
 **************************************************************************/

/* ----------------------------------------------------------------------
   Name:        WH_GetKeySet
   Description: ���L�L�[�f�[�^��ǂݏo���܂��B
   Arguments:   keyset - �f�[�^�i�[��w��
   Returns:     ��������ΐ^�B
   ---------------------------------------------------------------------- */
BOOL WH_GetKeySet(WMKeySet *keyset)
{
    WMErrCode result;

    if (sSysState != WH_SYSSTATE_KEYSHARING)
    {
        WH_TRACE("WH_GetKeySet failed (invalid system state)\n");
        return FALSE;
    }

    if ((sConnectMode != WH_CONNECTMODE_KS_CHILD) && (sConnectMode != WH_CONNECTMODE_KS_PARENT))
    {
        WH_TRACE("WH_GetKeySet failed (invalid connect mode)\n");
        return FALSE;
    }

    result = WM_GetKeySet(&sWMKeySetBuf, keyset);
    if (result != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    return TRUE;
}

/* ----------------------------------------------------------------------
   Name:        WH_GetSharedDataAdr
  Description: �w��� aid �����}�V�����瓾���f�[�^�̃A�h���X��
                ���L�f�[�^�̃A�h���X����v�Z���擾���܂��B
   Arguments:   aid - �}�V���̎w��
   Returns:     ���s���� NULL �B
   ---------------------------------------------------------------------- */
u16    *WH_GetSharedDataAdr(u16 aid)
{
    return WM_GetSharedDataAddress(&sDSInfo, &sDataSet, aid);
}

/* ----------------------------------------------------------------------
   Name:        WH_StepDS
   Description: �f�[�^�V�F�A�����O�̓�����1�i�߂܂��B
                ���t���[���ʐM����Ȃ�A���̊֐������t���[���ĂԕK�v��
                ����܂��B
   Arguments:   data - ���M����f�[�^
   Returns:     ��������ΐ^�B
   ---------------------------------------------------------------------- */
BOOL WH_StepDS(const void *data)
{
    WMErrCode result;

    result = WM_StepDataSharing(&sDSInfo, (const u16 *)data, &sDataSet);

    if (result == WM_ERRCODE_NO_CHILD)
    {
        // �e�@�Ȃ̂Ɏq�@�����Ȃ��i������G���[�Ƃ��邩�͎��R�j
        return TRUE;
    }

    if (result == WM_ERRCODE_NO_DATASET)
    {
        WH_TRACE("WH_StepDataSharing - Warning No DataSet\n");
        WH_SetError(result);
        return FALSE;
    }

    if (result != WM_ERRCODE_SUCCESS)
    {
        WH_REPORT_FAILURE(result);
        return FALSE;
    }

    return TRUE;
}


/**************************************************************************
 * �ȉ��́A�ʐM���I�����ď�������Ԃ܂őJ�ڂ�����֐��ł��B
 **************************************************************************/

/* ----------------------------------------------------------------------
   Name:        WH_Reset
   Description: ���Z�b�g�V�[�P���X���J�n���܂��B
                ���̊֐����ĂԂƁA���̏�Ԃɍ\�킸���Z�b�g���܂��B
        �G���[����̋������A�p�ł��B
   Arguments:   none.
   Returns:     none.
   ---------------------------------------------------------------------- */
void WH_Reset(void)
{
    WMErrCode result;

    if (sSysState == WH_SYSSTATE_DATASHARING)
    {
        result = WM_EndDataSharing(&sDSInfo);
        if (result != WM_ERRCODE_SUCCESS)
        {
            WH_REPORT_FAILURE(result);
        }
    }

    if (!WH_StateInReset())
    {
        WH_ChangeSysState(WH_SYSSTATE_FATAL);
    }
}

/* ----------------------------------------------------------------------
   Name:        WH_Finalize
   Description: �㏈���E�I���V�[�P���X���J�n���܂��B
                ���̊֐����ĂԂƁA���̏�Ԃ����ēK�؂ȏI���V�[�P���X��
                ���s���܂��B
                �ʏ�̏I�������ɂ́iWH_Reset�ł͂Ȃ��j���̊֐����g�p���܂��B
   Arguments:   None.
   Returns:     None.
   ---------------------------------------------------------------------- */
void WH_Finalize(void)
{
    if (sSysState == WH_SYSSTATE_IDLE)
    {
        WH_TRACE("already WH_SYSSTATE_IDLE\n");
        return;
    }

    WH_TRACE("WH_Finalize, state = %d\n", sSysState);

    if (sSysState == WH_SYSSTATE_SCANNING)
    {
        if (!WH_EndScan())
        {
            WH_Reset();
        }
        return;
    }


    if ((sSysState != WH_SYSSTATE_KEYSHARING) &&
        (sSysState != WH_SYSSTATE_DATASHARING) && (sSysState != WH_SYSSTATE_CONNECTED))
    {
        // �ڑ����Ă��Ȃ��E�G���[��ԂȂǂ̏ꍇ�̓��Z�b�g���Ă����B
        WH_ChangeSysState(WH_SYSSTATE_BUSY);
        WH_Reset();
        return;
    }

    WH_ChangeSysState(WH_SYSSTATE_BUSY);

    switch (sConnectMode)
    {
    case WH_CONNECTMODE_KS_CHILD:
        if (!WH_StateInEndChildKeyShare())
        {
            WH_Reset();
        }
        break;

    case WH_CONNECTMODE_DS_CHILD:
        if (WM_EndDataSharing(&sDSInfo) != WM_ERRCODE_SUCCESS)
        {
            WH_Reset();
            break;
        }
        // don't break;

    case WH_CONNECTMODE_MP_CHILD:
        if (!WH_StateInEndChildMP())
        {
            WH_Reset();
        }
        break;

    case WH_CONNECTMODE_KS_PARENT:
        if (!WH_StateInEndParentKeyShare())
        {
            WH_Reset();
        }
        break;

    case WH_CONNECTMODE_DS_PARENT:
        if (WM_EndDataSharing(&sDSInfo) != WM_ERRCODE_SUCCESS)
        {
            WH_Reset();
            break;
        }
        // don't break;

    case WH_CONNECTMODE_MP_PARENT:
        if (!WH_StateInEndParentMP())
        {
            WH_Reset();
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         WH_End

  Description:  �����ʐM���I������B

  Arguments:    None.

  Returns:      ��������ΐ^�B
 *---------------------------------------------------------------------------*/
BOOL WH_End(void)
{
    WH_ASSERT(sSysState == WH_SYSSTATE_IDLE);

    WH_ChangeSysState(WH_SYSSTATE_BUSY);
    if (WM_End(WH_StateOutEnd) != WM_ERRCODE_OPERATING)
    {
        WH_ChangeSysState(WH_SYSSTATE_ERROR);

        return FALSE;
    }

    whInitialized = 0;

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         WH_GetCurrentAid

  Description:  ���݂̎�����AID���擾���܂��B
                �q�@�͐ڑ��E�ؒf���ɕω�����\��������܂��B

  Arguments:    None.

  Returns:      AID�̒l
 *---------------------------------------------------------------------------*/
u16 WH_GetCurrentAid(void)
{
    return sMyAid;
}

/*---------------------------------------------------------------------------*
  Name:         WH_SetParentWEPKeyGenerator

  Description:  WEP Key �𐶐�����֐���ݒ肵�܂��B
                ���̊֐����Ăяo���ƁA�ڑ����̔F�؂� WEP ���g���܂��B
                �Q�[���A�v���P�[�V�������Ƃ̃A���S���Y����
                �ڑ��O�ɐe�q�œ���̒l��ݒ肵�܂��B
                ���̊֐��͐e�@�p�ł��B

  Arguments:    func    WEP Key �𐶐�����֐��ւ̃|�C���^
                        NULL ���w�肷��� WEP Key ���g�p���Ȃ�

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WH_SetParentWEPKeyGenerator(WHParentWEPKeyGeneratorFunc func)
{
    sParentWEPKeyGenerator = func;
}

/*---------------------------------------------------------------------------*
  Name:         WH_SetChildWEPKeyGenerator

  Description:  WEP Key �𐶐�����֐���ݒ肵�܂��B
                ���̊֐����Ăяo���ƁA�ڑ����̔F�؂� WEP ���g���܂��B
                �Q�[���A�v���P�[�V�������Ƃ̃A���S���Y����
                �ڑ��O�ɐe�q�œ���̒l��ݒ肵�܂��B
                ���̊֐��͎q�@�p�ł��B

  Arguments:    func    WEP Key �𐶐�����֐��ւ̃|�C���^
                        NULL ���w�肷��� WEP Key ���g�p���Ȃ�

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WH_SetChildWEPKeyGenerator(WHChildWEPKeyGeneratorFunc func)
{
    sChildWEPKeyGenerator = func;
}

/*---------------------------------------------------------------------------*
  Name:         WH_SetIndCallback

  Description:  WH_Initialize �֐��ŌĂяo���Ă��� WM_SetIndCallback �֐��Ɏw�肷��
                �R�[���o�b�N�֐����w�肵�܂��B
                ���̊֐��� WH_Initialize �֐��̑O�ɌĂԂ悤�ɂ��Ă��������B
                ���̊֐��ŃR�[���o�b�N�֐����w�肵�Ă��Ȃ��ꍇ�̓f�t�H���g��
                WH_IndicateHandler ���R�[���o�b�N�ɐݒ肳��܂��B

  Arguments:    callback    WM_SetIndCallback �Ŏw�肷�� Indication �ʒm�p�R�[���o�b�N�ł��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WH_SetIndCallback(WMCallbackFunc callback)
{
    sIndicationCallback = callback;
}


#if defined(WM_DEMO_WIRELESS_SHARED_WC_WH_CONFIG_H_)
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// ������WC�T���v�����W���[���Ɠ����̋@�\��WH�Ŏ������郉�b�p�[�w�B
// wc.h�Ƃ��Č��J����Ă�������̃C���^�t�F�[�X�����̂܂ܑΉ�����B

// WC��WH�̎�ȑ���_�͈ȉ��̒ʂ�B
//
//   - WC�͏���������WMParentParam�����̂܂܎w�肷���������
//     WH�͏�ԑJ�ڂ̂��тɊ֐��̈����Ƃ��ď������Ɏw�肷��B
//
//   - �]����WC�ł�WcInit()�ɗ^�����|�C���^�̓��e�����[�U���ύX�����
//     �h���C�o�̓�����Ԃ��s����ɂȂ肤��\���ł��������߁A
//     ����͂��̂悤�ȋ���������ē����ŕ�������悤�ύX����B
//
//   - WH�ł�WH_SYSSTATE_IDLE(IDLE�X�e�[�g)�����C�����X�̈����Ԃ���
//     WC�ł�WC_STATUS_READY(READY)�X�e�[�g)�������ԂƂ��Ă���B
//     �����WC�ł�IDLE�X�e�[�g�������ԂƂ���B
//

/*---------------------------------------------------------------------------*
  Name:         WcIndicateCallback

  Description:  WH����̃C���W�P�[�V�����R�[���o�b�N

  Arguments:    arg         WMCallback

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcIndicateCallback(void *arg)
{
    WHCompatibleInfoForWC  *info = infoForWC;
    WMindCallback *cb = (WMindCallback *)arg;
    if (cb->errcode == WM_ERRCODE_FIFO_ERROR)
    {
        info->wcTarget = WC_STATUS_ERROR;
        info->wcStatus = WC_STATUS_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcStateScheduler

  Description:  �ڕW�ƂȂ�X�e�[�g�ɑ΂��Č��݉\�ȃA�N�V���������s

  Arguments:    target      �ŏI�I�ȏ�ԑJ�ڐ�

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcStateScheduler(WCStatus target)
{
    WHCompatibleInfoForWC  *info = infoForWC;
    // �G���[�łȂ���ΑJ�ڐ��ύX
    if (info->wcStatus != WC_STATUS_ERROR)
    {
        info->wcTarget = target;
        // �\�Ȃ炱�̏�ŃA�N�V����
        if ((info->wcStatus != WC_STATUS_BUSY) &&
            (info->wcStatus != target))
        {
            // MP�e�@�ւ̏�ԑJ��
            if (target == WC_STATUS_PARENT)
            {
                // ���݂�READY�Ȃ�MP�J�n�\ 
                if (info->wcStatus == WC_STATUS_READY)
                {
                    info->wcStatus = WC_STATUS_BUSY;
                    (void)WH_ParentConnect(info->useDS ? WH_CONNECTMODE_DS_PARENT : WH_CONNECTMODE_MP_PARENT,
                                           ++info->tmpTgid, info->targetChannel);
                }
                // MP�q�@�Ƃ��ċN�����Ă���΂�������READY��
                else if (info->wcStatus == WC_STATUS_CHILD)
                {
                    info->wcStatus = WC_STATUS_BUSY;
                    WH_Reset();
                }
            }
            // MP�q�@�ւ̏�ԑJ��
            else if (target == WC_STATUS_CHILD)
            {
                // ���݂�READY�Ȃ�MP�J�n�\ 
                if (info->wcStatus == WC_STATUS_READY)
                {
                    info->wcStatus = WC_STATUS_BUSY;
                    {
                        static const u8 any[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,};
                        // �P�Ȃ�s�N�g�L���b�`����
                        if (info->pictoCatchCallback)
                        {
                            (void)WH_StartScan((WHStartScanCallbackFunc)info->pictoCatchCallback,
                                               any, 0);
                        }
                        // ���ۂ�MP�q�@
                        else
                        {
                            (void)WH_ChildConnectAuto(info->useDS ? WH_CONNECTMODE_DS_CHILD : WH_CONNECTMODE_MP_CHILD,
                                                      any, info->parentParam.channel);
                        }
                    }
                }
                // MP�e�@�Ƃ��ċN�����Ă���΂�������READY��
                else if (info->wcStatus == WC_STATUS_PARENT)
                {
                    info->wcStatus = WC_STATUS_BUSY;
                    WH_Reset();
                }
            }
            // READY�ւ̏�ԑJ��
            else if (target == WC_STATUS_READY)
            {
                // MP�ʐM���s���Ă���΂Ƃɂ����I��
                if (info->wcStatus == WC_STATUS_PARENT)
                {
                    info->wcStatus = WC_STATUS_BUSY;
                    WH_Reset();
                }
                else if (info->wcStatus == WC_STATUS_CHILD)
                {
                    info->wcStatus = WC_STATUS_BUSY;
                    WH_Reset();
                }
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcStateCallback

  Description:  WH�̏�Ԃ��X�V����邲�ƂɌĂяo�����R�[���o�b�N

  Arguments:    state       ���݂̏��
                prev        �ȑO�̏��

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcStateCallback(int state, int prev)
{
    WHCompatibleInfoForWC  *info = infoForWC;
    BOOL                    needToNotify = TRUE;
    // WM�̈�ʓI�ȃG���[�B
    if (state == WH_SYSSTATE_ERROR)
    {
        // �ʐM���̎q�@���ؒf���ꂽ�烏�C�����X���I��
        if ((info->wcTarget == WC_STATUS_CHILD) &&
            (WH_GetLastError() == WH_ERRCODE_DISCONNECTED))
        {
            info->wcTarget = WC_STATUS_READY;
            if (info->wcStatus != WC_STATUS_BUSY)
            {
                WH_Reset();
            }
        }
        // ����ȊO�� WH_Reset() ���͂���ōĎ��s����]�n������B
        else
        {
            WH_Reset();
        }
    }
    // CONNECT�����MP�X�e�[�g�ւ̑J�ڊ����B
    // 2��ڂ�MP�ʒm�ł���Ƃ����_�ɒ��ӂ���B
    // WC�̏�ԑJ�ړ_�B
    else if ((state == WH_SYSSTATE_CONNECTED) &&
             (prev == WH_SYSSTATE_CONNECTED))
    {
        info->wcStatus = (info->wcTarget == WC_STATUS_PARENT) ?
                          WC_STATUS_PARENT : WC_STATUS_CHILD;
        WcStateScheduler(info->wcTarget);
    }
    // WM_StartDataSharing()�����B
    // WC�̏�ԑJ�ړ_�B
    else if (state == WH_SYSSTATE_DATASHARING)
    {
        info->wcStatus = (info->wcTarget == WC_STATUS_PARENT) ?
                          WC_STATUS_PARENT : WC_STATUS_CHILD;
        WcStateScheduler(info->wcTarget);
    }
    // IDLE�X�e�[�g���B�B
    // WC�̏�ԑJ�ړ_�B
    else if (state == WH_SYSSTATE_IDLE)
    {
        if (!sAutoConnectFlag)
        {
            info->wcStatus = WC_STATUS_READY;
            WcStateScheduler(info->wcTarget);
        }
    }
    // WM_StartConnect()���s�B
    // CAUTION!:
    //     �{���̓��g���C�񐔂̏����Ȃǂ�����K�v������B
    // WH_Reset() ���͂���ōĎ��s�B
    else if (state == WH_SYSSTATE_CONNECT_FAIL)
    {
        WH_Reset();
    }
    // WM�̏������E�������уC���W�P�[�g�ݒ�֐�
    // �����ꂩ�̎��s�ɂ�鏈���p������G���[�B
    // WC_STATUS_ERROR �����B
    else if (state == WH_SYSSTATE_FATAL)
    {
        info->wcStatus = WC_STATUS_ERROR;
        WcStateScheduler(info->wcTarget);
    }
    // �I�������̊����B
    else if (state == WH_SYSSTATE_STOP)
    {
        info->wcStatus = WC_STATUS_READY;
    }
    // MeasureChannel�����B
    // �l�����ǂݏo���Ă�������IDLE�֑J�ڂ�����B
    // ���̃R�[���o�b�N�͍ċA�I�ɒʒm�����B
    else if (state == WH_SYSSTATE_MEASURECHANNEL)
    {
        info->targetChannel = WH_GetMeasureChannel();
        needToNotify = FALSE;
    }
    // ����ȊO�͓��ɂ��ׂ����Ƃ̖����ʒm�B
    else
    {
        // - WH_SYSSTATE_BUSY
        // - WH_SYSSTATE_SCANNING
        // - WH_SYSSTATE_KEYSHARING
        needToNotify = FALSE;
    }
    // WcWaitForState() �őҋ@���Ă���\��������̂Œʒm���Ă���
    if (needToNotify)
    {
        OS_WakeupThread(info->queue);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcWaitForState

  Description:  �w��̏�Ԃ܂őJ�ڂ��邩���s����܂Ńu���b�L���O����B
                WcFinish()�ł̂ݎg�p����B

  Arguments:    target      �J�ڂ��ׂ����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static BOOL WcWaitForState(int target)
{
    BOOL    succeeded = FALSE;
    OSIntrMode  bak_cpsr = OS_DisableInterrupts();
    for (;;)
    {
        int     state = WH_GetSystemState();
        if (state == target)
        {
            succeeded = TRUE;
            break;
        }
        // StartConnect�̎��s�B
        // WH_Reset() ���͂���ōĎ��s�B
        // �����Ƀ��g���C�I�[�o�[��ǉ����ׂ��B
        else if (state == WH_SYSSTATE_CONNECT_FAIL)
        {
            break;
        }
        // WM_StartParent�ȂǁAWM API�G���[�S�ʁB
        // WH_Reset() ���͂���ōĎ��s�B
        else if (state == WH_SYSSTATE_ERROR)
        {
            break;
        }
        // �����������A�I�������A�C���W�P�[�g�ݒ�Ɏ��s�B
        // WC_STATUS_ERROR �����B
        else if (state == WH_SYSSTATE_FATAL)
        {
            break;
        }
        OS_SleepThread(infoForWC->queue);
    }
    (void)OS_RestoreInterrupts(bak_cpsr);
    return succeeded;
}

SDK_WEAK_SYMBOL void WcInit(const WMParentParam *pp, BOOL dsFlag, BOOL btFlag, u16 framePeriod)
{
    WHCompatibleInfoForWC  *info = infoForWC;
    // WC�̏�ԑJ�ڂŎg�p������������ŕۑ�
    info->parentParam = *pp;
    info->useDS = dsFlag;
    (void)btFlag;   // �Q�Ƃ���Ȃ�
    info->targetChannel = info->parentParam.channel;
    OS_InitThreadQueue(info->queue);
    // WC�̂��߂Ɋg�����ꂽWH�̃����o��ݒ�
    sStateCallback = WcStateCallback;
    sDataSharingDoubleMode = (framePeriod <= 1);
    sPortExtraCallback = NULL;
    WH_SetGgid(info->parentParam.ggid);
    WH_SetIndCallback(WcIndicateCallback);
    // �K�v�Ȃ�IDLE�X�e�[�g�֑J�ڊJ�n
    info->wcTarget = WC_STATUS_READY;
    if (whInitialized)
    {
        info->wcStatus = WC_STATUS_READY;
    }
    else
    {
        info->wcStatus = WC_STATUS_BUSY;
    }
    (void)WH_Initialize();
}

SDK_WEAK_SYMBOL void WcFinish(void)
{
    WHCompatibleInfoForWC  *info = infoForWC;
    {
        OSIntrMode  bak_cpsr = OS_DisableInterrupts();
        // WH�̊��S�ȏI����҂�
        if (info->wcStatus == WC_STATUS_BUSY)
        {
            WH_Reset();
        }
        WcEnd();
        (void)WcWaitForState(WH_SYSSTATE_IDLE);
        info->wcStatus = WC_STATUS_BUSY;
        (void)WH_End();
        (void)WcWaitForState(WH_SYSSTATE_STOP);
        // WC�̂��߂Ɋg�����ꂽWH�̃����o�����ɖ߂�
        sStateCallback = NULL;
        sDataSharingDoubleMode = TRUE;
        sPortExtraCallback = NULL;
        sAutoConnectFlag = FALSE;
        (void)OS_RestoreInterrupts(bak_cpsr);
    }
}

SDK_WEAK_SYMBOL void WcStartParent(void)
{
    WcStateScheduler(WC_STATUS_PARENT);
}

SDK_WEAK_SYMBOL void WcStartChild(void)
{
    WcStateScheduler(WC_STATUS_CHILD);
}

SDK_WEAK_SYMBOL void WcEnd(void)
{
    WcStateScheduler(WC_STATUS_READY);
}

SDK_WEAK_SYMBOL WCStatus WcGetStatus(void)
{
    return infoForWC->wcStatus;
}

SDK_WEAK_SYMBOL u16 WcGetAid(void)
{
    return WH_GetCurrentAid();
}

SDK_WEAK_SYMBOL void WcSetMpReceiveCallback(WCCallbackFunc cb)
{
    WH_SetReceiver((WHReceiverFunc)cb);
}

SDK_WEAK_SYMBOL void WcSetBlockTransferCallback(WCBlockTransferCallbackFunc cb)
{
    sPortExtraCallback = (WMCallbackFunc)cb;
}

SDK_WEAK_SYMBOL void WcSetParentData(const void *buf, u16 size)
{
    (void)WH_SendData(buf, size, NULL);
}

SDK_WEAK_SYMBOL void WcSetChildData(const void *buf, u16 size)
{
    (void)WH_SendData(buf, size, NULL);
}

SDK_WEAK_SYMBOL BOOL WcStepDataSharing(const void *send, u16 *bitmap)
{
    BOOL    succeeded = WH_StepDS(send);
    if (succeeded && bitmap)
    {
        *bitmap = sDataSet.receivedBitmap;
    }
    return succeeded;
}

SDK_WEAK_SYMBOL u8 *WcGetSharedDataAddress(u16 aid)
{
    return (u8 *)WH_GetSharedDataAdr(aid);
}

SDK_WEAK_SYMBOL void WcTurnOnPictoCatch(WCPictoCatchCallbackFunc func)
{
    infoForWC->pictoCatchCallback = func;
    WH_TurnOnPictoCatch();
}

SDK_WEAK_SYMBOL void WcTurnOffPictoCatch(void)
{
    WH_TurnOffPictoCatch();
}

#endif // WM_DEMO_WIRELESS_SHARED_WC_WH_CONFIG_H_
