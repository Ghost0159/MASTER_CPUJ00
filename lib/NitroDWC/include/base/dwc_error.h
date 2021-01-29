/*---------------------------------------------------------------------------*
  Project:  NitroDWC - base - dwc_error
  File:     dwc_error.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_error.h,v $
  Revision 1.38  2007/04/06 13:02:13  takayama
  �s��������`�F�b�N�@�\�̃G���[��`���������B

  Revision 1.37  2007/03/28 09:43:46  nakata
  DWC_ECODE_TYPE_PROF�Ɋւ���C���B

  Revision 1.36  2006/06/22 02:03:17  nakata
  �_�E�����[�h���C�u�����̃o�b�t�@�s���G���[����菜�����B(DWCError�̎�ނ𑝂₵�����Ȃ�����)

  Revision 1.35  2006/06/22 00:49:17  nakata
  �_�E�����[�h���C�u�����̃o�b�t�@�s���G���[��ǉ�

  Revision 1.34  2006/06/01 04:48:49  takayama
  �R���p�C�����Ƀ��[�j���O���o������C���B

  Revision 1.33  2006/05/29 01:00:42  nakata
  DWC_Nd�֘A�ŕK�v�̂Ȃ��G���[�R�[�h���폜

  Revision 1.32  2006/05/25 09:06:53  nakata
  DWC_ERROR_SVL_HTTP��ǉ�

  Revision 1.31  2006/05/25 09:05:36  takayama
  GHTTP���C�u�����̃G���[��FriendsMatch���C�u�����̃G���[�ƕ����āA�Ɨ�����
  ���߂��B

  Revision 1.30  2006/05/25 05:09:50  nakata
  DWC_Nd/DWC_SVL��DWC�G���[��Ԃ��@�\��ǉ��������Ƃɔ����ύX

  Revision 1.29  2006/05/17 00:50:34  nakata
  DWC_Nd�p�G���[�萔���̕ύX

  Revision 1.28  2006/04/28 12:25:13  nakata
  �萔���ύX�ɔ����C��

  Revision 1.27  2006/04/28 08:16:51  nakata
  Nd�T�[�o�G���[�p�ԍ����폜(�K�v���Ȃ��Ȃ���)

  Revision 1.26  2006/04/27 07:33:04  nakata
  Nd���C�u��������UNKNOWN�G���[�R�[�h��ύX

  Revision 1.25  2006/04/27 05:15:25  nakata
  Nd���C�u�����p�̃G���[�R�[�h��ǉ�

  Revision 1.24  2006/04/26 10:52:17  nakata
  �g���@�\�p�̃G���[�R�[�h��ǉ�

  Revision 1.23  2006/01/23 04:50:43  arakit
  ����p��ɍ��킹�ăR�����g���C��

  Revision 1.22  2005/12/26 23:59:55  arakit
  �R�����g�̊ԈႢ���C��

  Revision 1.21  2005/10/24 12:14:49  arakit
  �T�[�o�N���C�A���g�^�}�b�`���O�ŁA�T�[�o������I�[�o�[�ł���Ƃ����G���[
  DWC_ERROR_SERVER_FULL��ǉ������B

  Revision 1.20  2005/10/20 09:59:45  sasakit
  GT2�֌W�̃G���[��ǉ��B

  Revision 1.19  2005/10/14 07:43:23  onozawa
  ghttp�p�̃G���[�R�[�h��ǉ�

  Revision 1.18  2005/10/14 06:58:31  arakit
  ghttp�p�̃G���[�R�[�h������ɒǉ�

  Revision 1.17  2005/10/14 06:41:37  arakit
  ghttp�p�̃G���[�R�[�h��ǉ�

  Revision 1.16  2005/10/11 04:08:18  arakit
  �G���[�����^�C�v���擾�ł���֐�DWC_GetLastErrorEx()��ǉ������B

  Revision 1.15  2005/10/05 08:29:43  arakit
  �T�[�o�N���C�A���g�}�b�`���O���ɁA�}�b�`���O������A�}�b�`���O���u���b�N���A�u���b�N���ꂽ
  �N���C�A���g�ɁA��p�̃G���[��Ԃ��}�b�`���O�I�v�V������ǉ������B

  Revision 1.14  2005/10/03 05:06:49  arakit
  DWC_ECODE_SEQ_TRANSPORT��DWC_ECODE_SEQ_ETC�ɕύX�����B

  Revision 1.13  2005/09/30 02:49:04  arakit
  �T�[�o�N���C�A���g�^�̐ڑ����s�G���[�R�[�h��ǉ������B

  Revision 1.12  2005/09/28 12:40:54  arakit
  NAT�l�S�V�G�[�V�����K��񐔎��s�̃G���[��ǉ������B

  Revision 1.11  2005/09/24 12:57:10  arakit
  �R�l�N�V�������I�[�o�̃G���[��ǉ������B

  Revision 1.10  2005/09/13 07:35:19  sasakit
  �G���[�X�e�[�g��DWC_ERROR_DISCONNECTED��ǉ��B

  Revision 1.9  2005/09/06 09:12:38  sasakit
  ConnectInet���̃G���[�̏�����ǉ��B

  Revision 1.8  2005/09/01 12:15:16  arakit
  �G���[�̃R�����g���C��

  Revision 1.7  2005/08/29 06:34:51  arakit
  gt2��socketErrorCallback�֐����쐬�����B

  Revision 1.6  2005/08/26 08:15:22  arakit
  �EASSERT�ƃ��W���[������ϐ��̃`�F�b�N�𐮗������B
  �E�O���֐�DWC_ShutdownFriendsMatch()�̈������Ƃ�Ȃ��悤�ɕύX�����B
  �܂��A���֐�����e���W���[���̐���ϐ���NULL�N���A����悤�ɂ����B
  �E�G���[�����֐���ύX���A�G���[�Z�b�g�A�G���[�`�F�b�N��K�v�ȉӏ��ɓ��ꂽ�B

  Revision 1.5  2005/08/20 07:01:19  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_ERROR_H_
#define DWC_ERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// enum
//----------------------------------------------------------------------------
// �G���[�񋓎q
typedef enum {
    DWC_ERROR_NONE = 0,            // �G���[�Ȃ�
    DWC_ERROR_DS_MEMORY_ANY,       // �{�̃Z�[�u�f�[�^�Ɋւ���G���[
    DWC_ERROR_AUTH_ANY,            // �F�؂Ɋւ���G���[
    DWC_ERROR_AUTH_OUT_OF_SERVICE, // �T�[�r�X�I��
    DWC_ERROR_AUTH_STOP_SERVICE,   // �T�[�r�X�ꎞ���f
    DWC_ERROR_AC_ANY,              // �����ڑ��Ɋւ���G���[
    DWC_ERROR_NETWORK,             // ���̑��̃l�b�g���[�N�G���[
    DWC_ERROR_GHTTP_ANY,           // GHTTP(HTTP�ʐM)�Ɋւ���G���[
    DWC_ERROR_DISCONNECTED,        // �ؒf���ꂽ
    DWC_ERROR_FATAL,               // �v���I�ȃG���[

    // [arakit] main 051011
    // �V�[�P���X��̌y�x�̃G���[�iDWCErrorType = DWC_ETYPE_LIGHT�j
    DWC_ERROR_FRIENDS_SHORTAGE,    // �w��̐l���̗L���ȗF�B�����Ȃ��̂ɗF�B�w��s�A�}�b�`���C�N���n�߂悤�Ƃ���
    DWC_ERROR_NOT_FRIEND_SERVER,   // �T�[�o�N���C�A���g�}�b�`���C�N�ŁA�T�[�o�Ɏw�肵���F�B�����݂ɗF�B�ł͂Ȃ��A�������̓T�[�o�𗧂ĂĂ��Ȃ�
    DWC_ERROR_MO_SC_CONNECT_BLOCK, // �T�[�o�N���C�A���g�}�b�`���C�N�̃}�b�`���C�N�I�v�V�����ŁA�T�[�o����ڑ������ۂ��ꂽ
    // [arakit] main 051011
    // [arakit] main 051024
    DWC_ERROR_SERVER_FULL,         // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o������I�[�o�[
	
	// [nakata] Nd���C�u�����p�̃G���[��ʒǉ�
	DWC_ERROR_ND_ANY,
	DWC_ERROR_ND_HTTP,
	
	// [nakata] SVL���C�u�����p�̃G���[��ʒǉ�
	DWC_ERROR_SVL_ANY,
	DWC_ERROR_SVL_HTTP,
    
    // [nakata] �s��������`�F�b�N�p�̃G���[��ʒǉ�
    DWC_ERROR_PROF_ANY,
    DWC_ERROR_PROF_HTTP,
        
    DWC_ERROR_NUM
} DWCError;

// [arakit] main 051010
// �G���[���o���ɕK�v�ȏ����������G���[�^�C�v
typedef enum {
    DWC_ETYPE_NO_ERROR = 0,   // �G���[�Ȃ�
    DWC_ETYPE_LIGHT,          // �Q�[���ŗL�̕\���݂̂ŁA�G���[�R�[�h�\���͕K�v�Ȃ�
    DWC_ETYPE_SHOW_ERROR,     // �G���[�R�[�h�\���݂̂��s��
    DWC_ETYPE_SHUTDOWN_FM,    // DWC_ShutdownFriendsMatch()���Ăяo���āAFriendsMatch���C�u�������I������K�v������B
                              // �G���[�R�[�h�̕\�����K�v�B
    DWC_ETYPE_SHUTDOWN_GHTTP, // DWC_ShutdownGHTTP()���Ăяo���āAGHTTP���C�u�������I������K�v������B
                              // �G���[�R�[�h�̕\�����K�v�B
    DWC_ETYPE_SHUTDOWN_ND,    // DWC_NdCleanupAsync()���Ăяo����DWC_Nd���C�u�������I������K�v������B
                              // �G���[�R�[�h�̕\�����K�v
    DWC_ETYPE_DISCONNECT,     // FriendsMatch�������Ȃ�DWC_ShutdownFriendsMatch()���Ăяo���A�X��DWC_CleanupInet()�ŒʐM�̐ؒf���s���K�v������B
                              //�G���[�R�[�h�̕\�����K�v�B
    DWC_ETYPE_FATAL,          // Fatal Error �����Ȃ̂ŁA�d��OFF�𑣂��K�v������B
                              // �G���[�R�[�h�̕\�����K�v�B
    DWC_ETYPE_NUM
} DWCErrorType;
// [arakit] main 051010

// �f�o�b�O�pdwc_base�G���[�R�[�h�e�v�f
enum {
    // �ǂ̃V�[�P���X���ɋN�������G���[��
    DWC_ECODE_SEQ_LOGIN          = (-60000),  // ���O�C�������ł̃G���[
    DWC_ECODE_SEQ_FRIEND         = (-70000),  // �F�B�Ǘ������ł̃G���[
    DWC_ECODE_SEQ_MATCH          = (-80000),  // �}�b�`���C�N�����ł̃G���[
    DWC_ECODE_SEQ_ETC            = (-90000),  // ��L�ȊO�̏����ł̃G���[

    // GameSpy�̃G���[�̏ꍇ�A�ǂ�SDK���o�����G���[��
    DWC_ECODE_GS_GP              =  (-1000),  // GameSpy GP �̃G���[
    DWC_ECODE_GS_PERS            =  (-2000),  // GameSpy Persistent �̃G���[
    DWC_ECODE_GS_STATS           =  (-3000),  // GameSpy Stats �̃G���[
    DWC_ECODE_GS_QR2             =  (-4000),  // GameSpy QR2 �̃G���[
    DWC_ECODE_GS_SB              =  (-5000),  // GameSpy Server Browsing �̃G���[
    DWC_ECODE_GS_NN              =  (-6000),  // GameSpy Nat Negotiation �̃G���[
    DWC_ECODE_GS_GT2             =  (-7000),  // GameSpy gt2 �̃G���[
    DWC_ECODE_GS_HTTP            =  (-8000),  // GameSpy HTTP �̃G���[
    DWC_ECODE_GS_ETC             =  (-9000),  // GameSpy ���̑���SDK�̃G���[

    // �ʏ�G���[���
    DWC_ECODE_TYPE_NETWORK       =   ( -10),  // �l�b�g���[�N��Q
    DWC_ECODE_TYPE_SERVER        =   ( -20),  // GameSpy�T�[�o��Q
    DWC_ECODE_TYPE_DNS           =   ( -30),  // DNS��Q
    DWC_ECODE_TYPE_DATA          =   ( -40),  // �s���ȃf�[�^����M
    DWC_ECODE_TYPE_SOCKET        =   ( -50),  // �\�P�b�g�ʐM�G���[
    DWC_ECODE_TYPE_BIND          =   ( -60),  // �\�P�b�g�̃o�C���h�G���[
    DWC_ECODE_TYPE_TIMEOUT       =   ( -70),  // �^�C���A�E�g����
    DWC_ECODE_TYPE_PEER          =   ( -80),  // �P�΂P�̒ʐM�ŕs�
    DWC_ECODE_TYPE_CONN_OVER     =   (-100),  // �R�l�N�V�������I�[�o
    DWC_ECODE_TYPE_STATS_AUTH    =   (-200),  // STATS�T�[�o���O�C���G���[
    DWC_ECODE_TYPE_STATS_LOAD    =   (-210),  // STATS�T�[�o�f�[�^���[�h�G���[
    DWC_ECODE_TYPE_STATS_SAVE    =   (-220),  // STATS�T�[�o�f�[�^�Z�[�u�G���[
    DWC_ECODE_TYPE_NOT_FRIEND    =   (-400),  // �w�肳�ꂽ���肪���݂ɓo�^���ꂽ�F�B�ł͂Ȃ�
    DWC_ECODE_TYPE_OTHER         =   (-410),  // ���肪�ʐM�ؒf�����i��ɃT�[�o�N���C�A���g�}�b�`���C�N���j
    DWC_ECODE_TYPE_MUCH_FAILURE  =   (-420),  // NAT�l�S�V�G�[�V�����ɋK��񐔈ȏ㎸�s����
    DWC_ECODE_TYPE_SC_CL_FAIL    =   (-430),  // �T�[�o�N���C�A���g�}�b�`���C�N�E�N���C�A���g�̐ڑ����s
    DWC_ECODE_TYPE_CLOSE         =   (-600),  // �R�l�N�V�����N���[�Y���̃G���[
    DWC_ECODE_TYPE_TRANS_HEADER  =   (-610),  // ���肦�Ȃ���M�X�e�[�g��Reliable�f�[�^����M����
    DWC_ECODE_TYPE_TRANS_BODY    =   (-620),  // ��M�o�b�t�@�I�[�o�[�t���[
    DWC_ECODE_TYPE_AC_FATAL      =   (-700),  // AC��������AC�ł̃G���[�O���[�v�B���̏ꍇ���ʌ��͐�p��state�l�B
    DWC_ECODE_TYPE_OPEN_FILE     =   (-800),  // GHTTP�t�@�C���I�[�v�����s
    DWC_ECODE_TYPE_INVALID_POST  =   (-810),  // GHTTP�����ȑ��M
    DWC_ECODE_TYPE_REQ_INVALID   =   (-820),  // GHTTP�t�@�C����������
    DWC_ECODE_TYPE_UNSPECIFIED   =   (-830),  // GHTTP�ڍוs���̃G���[
    DWC_ECODE_TYPE_BUFF_OVER     =   (-840),  // GHTTP�o�b�t�@�I�[�o�[�t���[
    DWC_ECODE_TYPE_PARSE_URL     =   (-850),  // GHTTP URL�̉�̓G���[
    DWC_ECODE_TYPE_BAD_RESPONSE  =   (-860),  // GHTTP�T�[�o����̃��X�|���X��̓G���[
    DWC_ECODE_TYPE_REJECTED      =   (-870),  // GHTTP�T�[�o����̃��N�G�X�g������
    DWC_ECODE_TYPE_FILE_RW       =   (-880),  // GHTTP���[�J���t�@�C��Read/Write�G���[
    DWC_ECODE_TYPE_INCOMPLETE    =   (-890),  // GHTTP�_�E�����[�h���f
    DWC_ECODE_TYPE_TO_BIG        =   (-900),  // GHTTP�t�@�C���T�C�Y���傫�����ă_�E�����[�h�s��
    DWC_ECODE_TYPE_ENCRYPTION    =   (-910),  // GHTTP�G���N���v�V�����G���[
	
    // Fatal Error��p���
    DWC_ECODE_TYPE_ALLOC         =     (-1),  // �������m�ێ��s
    DWC_ECODE_TYPE_PARAM         =     (-2),  // �p�����[�^�G���[
    DWC_ECODE_TYPE_SO_SOCKET     =     (-3),  // SO�̃G���[��������GameSpy gt2�̃\�P�b�g�G���[
    DWC_ECODE_TYPE_NOT_INIT      =     (-4),  // ���C�u����������������Ă��Ȃ��B
    DWC_ECODE_TYPE_DUP_INIT      =     (-5),  // ���C�u��������x���������ꂽ�B
    DWC_ECODE_TYPE_WM_INIT       =     (-6),  // WM�̏������Ɏ��s�����B
    DWC_ECODE_TYPE_UNEXPECTED    =     (-9),  // �\�����Ȃ���ԁA��������Unkwon��GameSpy�G���[����

	// 30000��G���[�ԍ��̒�`(�g���@�\�p)
	DWC_ECODE_SEQ_ADDINS        =   (-30000), // �g���@�\�G���[
	
	DWC_ECODE_FUNC_ND            =   (-1000), // DWC_Nd�p�̈�
    DWC_ECODE_FUNC_PROF          =   (-3000), // �s��������`�F�b�N�p�̈�
	
	DWC_ECODE_TYPE_ND_ALLOC      =   ( -1), // �������m�ێ��s
	DWC_ECODE_TYPE_ND_FATAL      =   ( -9), // �v���I�G���[
	DWC_ECODE_TYPE_ND_BUSY       =   (-10), // �_�E�����[�h���ɐV�K���N�G�X�g��v������
	DWC_ECODE_TYPE_ND_HTTP       =   (-20), // HTTP�ʐM�Ɏ��s
	DWC_ECODE_TYPE_ND_BUFFULL    =   (-30), // HTTP�ʐM�o�b�t�@���s�\��
	DWC_ECODE_TYPE_ND_SERVER     =   (  0), // �T�[�o���G���[�R�[�h��Ԃ��Ă���

	DWC_ECODE_TYPE_PROF_CONN     =   ( -10),// �s��������`�F�b�N�ŃT�[�o�Ƃ̐ڑ��ɖ�肪������
	DWC_ECODE_TYPE_PROF_PARSE    =   ( -20),// �s��������`�F�b�N�ŃT�[�o�̉������������Ȃ�����
	DWC_ECODE_TYPE_PROF_OTHER    =   ( -30) // �s��������`�F�b�N�ł��̑��̃G���[����������
};


//----------------------------------------------------------------------------
// function - external
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  �G���[�擾�֐�
  �����@�FerrorCode �G���[�R�[�h�i�[��|�C���^�B�K�v�Ȃ����NULL��n���Ηǂ��B
  �߂�l�F�G���[���
  �p�r�@�F�Ō�̒ʐM�G���[��ʁA�G���[�R�[�h���擾����
 *---------------------------------------------------------------------------*/
extern DWCError DWC_GetLastError(int* errorCode);


// [arakit] main 051011
/*---------------------------------------------------------------------------*
  �g���ŃG���[�擾�֐�
  �����@�FerrorCode �G���[�R�[�h�i�[��|�C���^�B�K�v�Ȃ����NULL��n���Ηǂ��B
          errorType �G���[�����^�C�v�i�[��|�C���^�B�K�v�Ȃ����NULL��n���Ηǂ��B
  �߂�l�F�G���[���
  �p�r�@�F�Ō�̒ʐM�G���[��ʁA�G���[�R�[�h�A�G���[�����^�C�v���擾����
 *---------------------------------------------------------------------------*/
extern DWCError DWC_GetLastErrorEx(int* errorCode, DWCErrorType* errorType);
// [arakit] main 051011


/*---------------------------------------------------------------------------*
  �G���[�N���A�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�ʐM�G���[�̋L�^���N���A����BFatal Error�̓N���A�ł��Ȃ��B
 *---------------------------------------------------------------------------*/
extern void DWC_ClearError(void);


//----------------------------------------------------------------------------
// function - internal
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  �G���[�`�F�b�N�֐�
  �����@�F�Ȃ�
  �߂�l�FTRUE:�G���[����AFALSE:�G���[�Ȃ�
  �p�r�@�F�ʐM�G���[�������������ǂ������`�F�b�N����
 *---------------------------------------------------------------------------*/
extern BOOL DWCi_IsError(void);


/*---------------------------------------------------------------------------*
  �G���[�Z�b�g�֐�
  �����@�Ferror     �G���[���
          errorCode �G���[�̏ڍׂ������G���[�R�[�h
  �߂�l�F�Ȃ�
  �p�r�@�F�ʐM�G���[�̔������L�^����BFatal Error�͏㏑���ł��Ȃ��B
 *---------------------------------------------------------------------------*/
extern void DWCi_SetError(DWCError error, int errorCode);


#ifdef __cplusplus
}
#endif


#endif // DWC_ERROR_H_
