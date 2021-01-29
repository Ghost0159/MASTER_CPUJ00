/*===========================================================================*
  Project:  Pokemon Global Data Station Network Library
  File:     poke_net_common.h

  $NoKeywords: $
 *===========================================================================*/
/*! @file
	@brief	Pokemon GDS WiFi ���C�u����

*/
//===========================================================================
#ifndef ___POKE_NET_GDS_COMMON___
#define ___POKE_NET_GDS_COMMON___

//===============================================
//                ���ԃp�b�N
//===============================================
#define POKE_NET_TIMETOU64(Y,M,D,h,m,s)	(u64)((((u64)Y) << 40) | (((u64)M) << 32) | (((u64)D) << 24) | (((u64)h) << 16) | (((u64)m) << 8) | (((u64)s) << 0))

//===============================================
//!          �f�c�r���N�G�X�g�R�[�h
//===============================================
enum POKE_NET_GDS_REQCODE {
	POKE_NET_GDS_REQCODE_DEBUG_START = 0 ,				// ���ȉ��f�o�b�O�Ŏg�p���郊�N�G�X�g

	// ------------------------- �ȉ��f�o�b�O���N�G�X�g ----------------------
	POKE_NET_GDS_REQCODE_DEBUG_MESSAGE = 0 ,			//!< �f�o�b�O���b�Z�[�W

	POKE_NET_GDS_REQCODE_DEBUG_END   = 9999 ,			// ��9999�܂ł̓f�o�b�O�Ŏd�l���郊�N�G�X�g
	POKE_NET_GDS_REQCODE_ADMIN_START = 10000 ,			// ��10000�ȍ~�͊Ǘ��҂��g�p�ł��郊�N�G�X�g

#ifndef ___POKE_NET_BUILD_DS___
	// -------------------------- �ȉ��Ǘ��҃��N�G�X�g -----------------------
	POKE_NET_GDS_REQCODE_ADMIN_SVR_STATUS = 10000 ,			// �T�[�o�[�X�e�[�^�X�擾
	POKE_NET_GDS_REQCODE_ADMIN_SVR_CLEARSTATUS ,			// �T�[�o�[���N���A
	POKE_NET_GDS_REQCODE_ADMIN_SVR_COMMAND ,				// �T�[�o�[�ւ̖���
	POKE_NET_GDS_REQCODE_ADMIN_SVR_DBINITIALIZE ,			// �f�[�^�x�[�X������

	POKE_NET_GDS_REQCODE_ADMIN_DRESSUPSHOT_STATUS = 11000 ,	// �h���X�A�b�v�V���b�g��Ԏ擾
	POKE_NET_GDS_REQCODE_ADMIN_DRESSUPSHOT_DELETE ,			// �h���X�A�b�v�V���b�g�폜
	POKE_NET_GDS_REQCODE_ADMIN_DRESSUPSHOT_CLEAR ,			// �h���X�A�b�v�V���b�g�N���A

	POKE_NET_GDS_REQCODE_ADMIN_BOXSHOT_STATUS = 12000 ,		// �{�b�N�X�V���b�g��Ԏ擾
	POKE_NET_GDS_REQCODE_ADMIN_BOXSHOT_DELETE ,				// �{�b�N�X�V���b�g�폜
	POKE_NET_GDS_REQCODE_ADMIN_BOXSHOT_CLEAR ,				// �{�b�N�X�V���b�g�N���A

	POKE_NET_GDS_REQCODE_ADMIN_BATTLEDATA_STATUS = 13000 ,	// �o�g���f�[�^��Ԏ擾
	POKE_NET_GDS_REQCODE_ADMIN_BATTLEDATA_DELETE ,			// �o�g���f�[�^�폜
	POKE_NET_GDS_REQCODE_ADMIN_BATTLEDATA_RANKHISTORY ,		// �o�g���f�[�^�����L���O�����擾
	POKE_NET_GDS_REQCODE_ADMIN_BATTLEDATA_EVENTFLAG ,		// �o�g���f�[�^�C�x���g�o�^�v��
	POKE_NET_GDS_REQCODE_ADMIN_BATTLEDATA_CLEAR ,			// �o�g���f�[�^�N���A
	POKE_NET_GDS_REQCODE_ADMIN_BATTLEDATA_GETPARAM ,		// �p�����[�^�擾
	POKE_NET_GDS_REQCODE_ADMIN_BATTLEDATA_SETPARAM ,		// �p�����[�^�ݒ�
	POKE_NET_GDS_REQCODE_ADMIN_BATTLEDATA_EXECDECISION ,	// �m�菈�����s(����)

	POKE_NET_GDS_REQCODE_ADMIN_RANKING_STATUS = 14000 ,		// �����L���O��Ԏ擾
	POKE_NET_GDS_REQCODE_ADMIN_RANKING_HISTORY ,			// �����L���O�����擾
	POKE_NET_GDS_REQCODE_ADMIN_RANKING_GET ,				// ���[�U�[�̃����L���O���擾
	POKE_NET_GDS_REQCODE_ADMIN_RANKING_SET ,				// ���[�U�[�̃����L���O���ݒ�
	POKE_NET_GDS_REQCODE_ADMIN_RANKING_CLEAR ,				// �����L���O���N���A

	POKE_NET_GDS_REQCODE_ADMIN_RANKING_GETSCHEDULE ,		// �����L���O�X�P�W���[���擾
	POKE_NET_GDS_REQCODE_ADMIN_RANKING_SETSCHEDULE ,		// �����L���O�X�P�W���[���ݒ�
	POKE_NET_GDS_REQCODE_ADMIN_RANKING_EXECDECISION ,		// �m�菈�����s(����)

	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_POKEMONTABLE_GET = 15000 ,	// �s���`�F�b�N:�|�P�������̐ݒ�擾
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_POKEMONTABLE_SET ,			// �s���`�F�b�N:�|�P�������̐ݒ�Z�b�g
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_NGWORD_GET ,				// �s���`�F�b�N:�m�f���[�h�擾
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_NGWORD_SET ,				// �s���`�F�b�N:�m�f���[�h�Z�b�g
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_LANGCODE_GET ,				// �s���`�F�b�N:����R�[�h�擾
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_LANGCODE_SET ,				// �s���`�F�b�N:����R�[�h�Z�b�g
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_ITEM_GET ,					// �s���`�F�b�N:�ǂ����擾
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_ITEM_SET ,					// �s���`�F�b�N:�ǂ����Z�b�g
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_MEETPLACE_GET ,				// �s���`�F�b�N:�o������ꏊ�擾
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_MEETPLACE_SET ,				// �s���`�F�b�N:�o������ꏊ�Z�b�g
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_WAZATABLE_GET ,				// �s���`�F�b�N:�Z�e�[�u��
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_WAZATABLE_SET ,				// �s���`�F�b�N:�Z�e�[�u��
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_ACCESSORY_GET ,				// �s���`�F�b�N:�m�f�A�N�Z�T���擾
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_ACCESSORY_SET ,				// �s���`�F�b�N:�m�f�A�N�Z�T���ݒ�
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_DRESSUPBGID_GET ,			// �s���`�F�b�N:�m�f�h���X�A�b�v�V���b�g�w�i�h�c
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_DRESSUPBGID_SET ,			// �s���`�F�b�N:�m�f�h���X�A�b�v�V���b�g�w�i�h�c
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_REGULATION_GET ,			// �s���`�F�b�N:�L���ȃ��M�����[�V����
	POKE_NET_GDS_REQCODE_ADMIN_ILLEGALCHECK_REGULATION_SET ,			// �s���`�F�b�N:�L���ȃ��M�����[�V����

	POKE_NET_GDS_REQCODE_ADMIN_DHOCHECKLOG_GET ,						// �s���`�F�b�N���O:�擾
	POKE_NET_GDS_REQCODE_ADMIN_DHOCHECKLOG_CLEAR ,						// �s���`�F�b�N���O:�N���A

#endif
	POKE_NET_GDS_REQCODE_ADMIN_END  = 19999 ,			// ��19999�܂ł͊Ǘ��҂��g�p�ł��郊�N�G�X�g
	POKE_NET_GDS_REQCODE_USER_START = 20000 ,			// ��20000�ȍ~�͈�ʃ��[�U�[���g���郊�N�G�X�g

	// ------------------------- �ȉ����[�U�[���N�G�X�g ----------------------
	POKE_NET_GDS_REQCODE_DRESSUPSHOT_REGIST = 20000 ,	//!< �h���X�A�b�v�V���b�g�o�^
	POKE_NET_GDS_REQCODE_DRESSUPSHOT_GET ,				//!< �h���X�A�b�v�V���b�g�擾
	POKE_NET_GDS_REQCODE_BOXSHOT_REGIST = 21000,		//!< �{�b�N�X�V���b�g�o�^
	POKE_NET_GDS_REQCODE_BOXSHOT_GET ,					//!< �{�b�N�X�V���b�g�擾
	POKE_NET_GDS_REQCODE_RANKING_GETTYPE = 22000,		//!< ���݂̃����L���O�^�C�v�擾
	POKE_NET_GDS_REQCODE_RANKING_UPDATE ,				//!< ���݂̃����L���O���̍X�V�Ǝ擾
	POKE_NET_GDS_REQCODE_BATTLEDATA_REGIST = 23000,		//!< �o�g���f�[�^�o�^
	POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH ,			//!< �o�g���f�[�^����
	POKE_NET_GDS_REQCODE_BATTLEDATA_GET ,				//!< �o�g���f�[�^�擾
	POKE_NET_GDS_REQCODE_BATTLEDATA_FAVORITE ,			//!< �o�g���f�[�^���C�ɓ���o�^

	POKE_NET_GDS_REQCODE_LAST
};

#define	POKE_NET_GDS_REQUESTCOMMON_AUTH_SVLTOKEN_LENGTH		(304)
//===============================================
//! �|�P����WiFi���C�u���� ���N�G�X�g�F�؏��
//===============================================
typedef struct {
	s32		PID;								//!< �v���t�@�C���h�c
	u8		ROMCode;							//!< �o�[�W�����R�[�h
	u8		LangCode;							//!< ����R�[�h
	char	SvlToken[POKE_NET_GDS_REQUESTCOMMON_AUTH_SVLTOKEN_LENGTH];	// �T�[�r�X���P�[�^�g�[�N��
	u16		Dummy;
} POKE_NET_REQUESTCOMMON_AUTH;

//===============================================
//!  �|�P����WiFi���C�u���� ���N�G�X�g�w�b�_
//===============================================
typedef struct {
	unsigned short ReqCode;						//!< ���N�G�X�g�R�[�h(POKE_NET_GDS_REQUEST_REQCODE_xxxx)
	unsigned short Option;						//!< ���N�G�X�g�I�v�V����
	POKE_NET_REQUESTCOMMON_AUTH Auth;			//!< ���[�U�[�F�؏��
	unsigned char Param[0];						//!< �p�����[�^(�e���N�G�X�g�̍\����)
} POKE_NET_REQUEST;

//===============================================
//!  �|�P����WiFi���C�u���� ���X�|���X�w�b�_
//===============================================
typedef struct {
	unsigned short ReqCode;						//!< ���N�G�X�g�R�[�h(POKE_NET_GDS_REQUEST_REQCODE_xxxx)
	unsigned short Result;						//!< ���X�|���X����(POKE_NET_GDS_RESPONSE_RESULT_xxxx)
	unsigned char Param[0];						//!< �p�����[�^(�e���X�|���X�̍\����)
} POKE_NET_RESPONSE;

//===============================================
//       ���N�G�X�g/���X�|���X�\����
//===============================================
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//      �f�o�b�O���N�G�X�g/���X�|���X
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------------------
//         �f�o�b�O���b�Z�[�W
//------------------------------------
//! �f�c�r�f�o�b�O���b�Z�[�W����
enum POKE_NET_GDS_RESPONSE_RESULT_DEBUG_MESSAGE {
	POKE_NET_GDS_RESPONSE_RESULT_DEBUG_MESSAGE_SUCCESS ,				//!< �o�^����
};

//! �f�c�r�f�o�b�O���b�Z�[�W���N�G�X�g
typedef struct {
	char Message[1024];						//!< �f�o�b�O���b�Z�[�W
} POKE_NET_GDS_REQUEST_DEBUG_MESSAGE;

//! �f�c�r�f�o�b�O���b�Z�[�W���X�|���X
typedef struct {
	char ResultMessage[1024];				//!< �f�o�b�O���b�Z�[�W
} POKE_NET_GDS_RESPONSE_DEBUG_MESSAGE;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//        ���[�U�[���N�G�X�g/���X�|���X
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------------------
//     �h���X�A�b�v�V���b�g�o�^
//------------------------------------
//! �f�c�r�h���X�A�b�v�V���b�g�o�^����
enum POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST {
	POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_SUCCESS ,				//!< �o�^����
	POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_AUTH ,			//!< ���[�U�[�F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_ALREADY ,			//!< ���łɓo�^����Ă���
	POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_ILLEGAL ,			//!< �s���ȃf�[�^
	POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_ILLEGALPROFILE ,	//!< �s���ȃ��[�U�[�v���t�B�[��
	POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_UNKNOWN			//!< ���̑��G���[
};

//! �f�c�r�h���X�A�b�v�V���b�g�o�^���N�G�X�g
typedef struct {
	GT_GDS_DRESS_SEND Data;							//!< �h���X�A�b�v�V���b�g���M�f�[�^
} POKE_NET_GDS_REQUEST_DRESSUPSHOT_REGIST;

//! �f�c�r�h���X�A�b�v�V���b�g�o�^���X�|���X
typedef struct {
	u64 Code;										//!< �o�^���ꂽ�h���X�A�b�v�V���b�g�̃R�[�h
} POKE_NET_GDS_RESPONSE_DRESSUPSHOT_REGIST;

//------------------------------------
//     �h���X�A�b�v�V���b�g�擾
//------------------------------------
//! �f�c�r�h���X�A�b�v�V���b�g�擾����
enum POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_GET {
	POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_GET_SUCCESS ,				//!< �擾����
	POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_GET_ERROR_AUTH ,			//!< ���[�U�[�F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_GET_ERROR_ILLEGALPOKEMON ,	//!< �|�P�����ԍ��G���[
	POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_GET_ERROR_UNKNOWN			//!< ���̑��G���[
};

//! �f�c�r�h���X�A�b�v�V���b�g�擾�^�C�v
enum POKE_NET_GDS_REQUEST_DRESSUPSHOT_GETTYPE {
	POKE_NET_GDS_REQUEST_DRESSUPSHOT_GETTYPE_POKEMON ,				//!< �|�P�����ԍ��ōŐV�擾(SearchParam�Ƀ|�P�����ԍ�)
	POKE_NET_GDS_REQUEST_DRESSUPSHOT_GETTYPE_CODE					//!< �R�[�h���ڎw��Ŏ擾
};

//! �f�c�r�h���X�A�b�v�V���b�g�擾���N�G�X�g
typedef struct {
	unsigned short SearchType;										//!< �����^�C�v(POKE_NET_GDS_REQUEST_DRESSUPSHOT_GETTYPE_xxxx)
	unsigned short SearchOpt;										//!< �����I�v�V����(���ݖ��g�p)
	u64 SearchParam;												//!< �����p�����[�^
} POKE_NET_GDS_REQUEST_DRESSUPSHOT_GET;

//! �f�c�r�h���X�A�b�v�V���b�g�擾���X�|���X���e
typedef struct {
	long PID;														//!< �A�b�v���[�h�҂̃v���t�@�C���h�c
	u64 Code;														//!< �h���X�A�b�v�V���b�g�R�[�h
	GT_GDS_DRESS_RECV Data;											//!< �h���X�A�b�v�V���b�g��M�f�[�^
} POKE_NET_GDS_RESPONSE_DRESSUPSHOT_RECVDATA;

#define	POKE_NET_GDS_RESPONSE_DRESSUPSHOT_GET_MAX	(10)			// �N���C�A���g���󂯎��h���X�A�b�v�V���b�g�ő吔

//! �f�c�r�h���X�A�b�v�V���b�g�擾���X�|���X
typedef struct {
	long HitCount;													//!< �q�b�g����
	POKE_NET_GDS_RESPONSE_DRESSUPSHOT_RECVDATA Data[0];				//!< �h���X�A�b�v�V���b�g��M�f�[�^
} POKE_NET_GDS_RESPONSE_DRESSUPSHOT_GET;

//------------------------------------
//        �{�b�N�X�V���b�g�o�^
//------------------------------------
//! �f�c�r�{�b�N�X�V���b�g�o�^����
enum POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST {
	POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_SUCCESS ,				//!< �o�^����
	POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_AUTH ,			//!< ���[�U�[�F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_ALREADY ,			//!< ���łɓo�^����Ă���
	POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_ILLEGAL ,			//!< �s���ȃf�[�^
	POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_ILLEGALPROFILE ,	//!< �s���ȃ��[�U�[�v���t�B�[��
	POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_ILLEGALGROUP ,	//!< �s���ȃO���[�v�ԍ�
	POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_UNKNOWN			//!< ���̑��G���[
};

//! �f�c�r�{�b�N�X�V���b�g�o�^���N�G�X�g
typedef struct {
	long GroupNo;													//!< �o�^��O���[�v�ԍ�
	GT_BOX_SHOT_SEND Data;											//!< �{�b�N�X�V���b�g���M�f�[�^
} POKE_NET_GDS_REQUEST_BOXSHOT_REGIST;

//! �f�c�r�{�b�N�X�V���b�g�o�^���X�|���X
typedef struct {
	u64 Code;														//!< �o�^���ꂽ�{�b�N�X�V���b�g�̃R�[�h
} POKE_NET_GDS_RESPONSE_BOXSHOT_REGIST;

//------------------------------------
//        �{�b�N�X�V���b�g�擾
//------------------------------------
//! �f�c�r�{�b�N�X�V���b�g�擾����
enum POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_GET {
	POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_GET_SUCCESS ,				//!< �擾����
	POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_GET_ERROR_AUTH ,			//!< ���[�U�[�F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_GET_ERROR_ILLEGALGROUP ,	//!< �O���[�v�ԍ��G���[
	POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_GET_ERROR_UNKNOWN			//!< ���̑��G���[
};

//! �f�c�r�{�b�N�X�V���b�g�擾�^�C�v
enum POKE_NET_GDS_REQUEST_BOXSHOT_GETTYPE {
	POKE_NET_GDS_REQUEST_BOXSHOT_GETTYPE_GROUP ,					//!< �O���[�v�ԍ��ōŐV�擾(SearchParam�ɃO���[�v�ԍ�)
	POKE_NET_GDS_REQUEST_BOXSHOT_GETTYPE_CODE						//!< �h�c�w��Ŏ擾(SearchParam�ɃR�[�h�ԍ�)
};

//! �f�c�r�{�b�N�X�V���b�g�擾���N�G�X�g
typedef struct {
	unsigned short SearchType;										//!< �����^�C�v(POKE_NET_GDS_REQUEST_BOXSHOT_GETTYPE_xxxx)
	unsigned short SearchOpt;										//!< �����I�v�V����(���ݖ��g�p)
	u64 SearchParam;												//!< �����p�����[�^
} POKE_NET_GDS_REQUEST_BOXSHOT_GET;

//! �f�c�r�{�b�N�X�V���b�g�擾���X�|���X���e
typedef struct {
	long PID;														//!< �A�b�v���[�h�҂̃v���t�@�C���h�c
	long GroupNo;													//!< �O���[�v�ԍ�
	u64 Code;														//!< �{�b�N�X�V���b�g�R�[�h
	GT_BOX_SHOT_RECV Data;											//!< �{�b�N�X�V���b�g��M�f�[�^
} POKE_NET_GDS_RESPONSE_BOXSHOT_RECVDATA;

#define	POKE_NET_GDS_RESPONSE_BOXSHOT_GET_MAXNUM	(20)			// �N���C�A���g���󂯎��{�b�N�X�V���b�g�ő吔

//! �f�c�r�{�b�N�X�V���b�g�擾���X�|���X
typedef struct {
	long HitCount;													//!< �q�b�g����
	POKE_NET_GDS_RESPONSE_BOXSHOT_RECVDATA Data[0];					//!< �{�b�N�X�V���b�g��M�f�[�^
} POKE_NET_GDS_RESPONSE_BOXSHOT_GET;


//------------------------------------
//      �����L���O�^�C�v�擾
//------------------------------------
/*
//! �f�c�r�����L���O�^�C�v�擾���N�G�X�g
typedef struct {
} POKE_NET_GDS_REQUEST_RANKING_GETTYPE;
*/

//! �f�c�r�����L���O�^�C�v�擾����
enum POKE_NET_GDS_RESPONSE_RESULT_RANKING_GETTYPE {
	POKE_NET_GDS_RESPONSE_RESULT_RANKING_GETTYPE_SUCCESS ,				//!< �擾����
	POKE_NET_GDS_RESPONSE_RESULT_RANKING_GETTYPE_ERROR_AUTH ,			//!< ���[�U�[�F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_RANKING_GETTYPE_ERROR_UNKNOWN			//!< ���̑��G���[
};

//! �f�c�r�����L���O�擾���X�|���X���e
typedef struct {
	GT_RANKING_TYPE_RECV Data;					//!< �����L���O�^�C�v�f�[�^
} POKE_NET_GDS_RESPONSE_RANKING_GETTYPE;

//------------------------------------
//        �����L���O�X�V
//------------------------------------
//! �f�c�r�����L���O�X�V����
enum POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE {
	POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_SUCCESS ,				//!< �X�V����
	POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_AUTH ,			//!< ���[�U�[�F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_ILLEGALTYPE	 ,	//!< �����L���O�^�C�v�G���[
	POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_ILLEGALDATA	 ,	//!< �s���ȃf�[�^�G���[
	POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_ILLEGALPROFILE ,	//!< �s���ȃv���t�B�[��
	POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_UNKNOWN			//!< ���̑��G���[
};

//! �f�c�r�����L���O�X�V���N�G�X�g
typedef struct {
	GT_RANKING_MYDATA_SEND Data;					//!< �����L���O�X�V�f�[�^
} POKE_NET_GDS_REQUEST_RANKING_UPDATE;

//! �f�c�r�����L���O�X�V���X�|���X
typedef struct {
	GT_LAST_WEEK_RANKING_ALL_RECV LastWeek;			//!< ��T�̃����L���O�f�[�^
	GT_THIS_WEEK_RANKING_DATA_ALL_RECV ThisWeek;	//!< ���T�̃����L���O�f�[�^
} POKE_NET_GDS_RESPONSE_RANKING_UPDATE;

//------------------------------------
//       �o�g���f�[�^�o�^
//------------------------------------
//! �f�c�r�o�g���f�[�^�o�^����
enum POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST {
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_SUCCESS ,				//!< �o�^����
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_AUTH ,				//!< ���[�U�[�F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ALREADY ,			//!< ���łɓo�^����Ă���
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ILLEGAL ,			//!< �s���ȃf�[�^
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ILLEGALPROFILE ,	//!< �s���ȃ��[�U�[�v���t�B�[��
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_UNKNOWN ,			//!< ���̑��G���[
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ILLEGALPOKEMON_MINE ,	//!< �����̃|�P�����ŃG���[
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ILLEGALPOKEMON_OTHER ,	//!< ����̃|�P�����ŃG��
};

//! �f�c�r�o�g���f�[�^�o�^���N�G�X�g
typedef struct {
	GT_BATTLE_REC_SEND Data;												//!< �o�g���f�[�^���M�f�[�^
} POKE_NET_GDS_REQUEST_BATTLEDATA_REGIST;

//! �f�c�r�o�g���f�[�^�o�^���X�|���X
typedef struct {
	u64 Code;																//!< �o�^���ꂽ�o�g���f�[�^�̃R�[�h
} POKE_NET_GDS_RESPONSE_BATTLEDATA_REGIST;


//------------------------------------
//      �o�g���f�[�^���X�g�擾
//------------------------------------
//! �f�c�r�o�g���f�[�^���X�g�擾����
enum POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_SEARCH {
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_SEARCH_SUCCESS ,			//!< �擾����
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_SEARCH_ERROR_AUTH ,			//!< ���[�U�[�F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_SEARCH_ERROR_ILLEGALPARAM ,	//!< �����p�����[�^�G���[
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_SEARCH_ERROR_UNKNOWN		//!< ���̑��G���[
};

//! �f�c�r�o�g���f�[�^���X�g�擾�^�C�v
enum POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCHTYPE {
	POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCHTYPE_CONDITION ,				//!< �����w��Ŏ擾
	POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCHTYPE_RANKING ,				//!< ���T�̒ʏ탉���L���O
	POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCHTYPE_EXRANKING ,				//!< ���T�̊g�������L���O
	POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCHTYPE_PID ,					// PID�ɂ��擾(�Ǘ��p)
};

//! �f�c�r�o�g���f�[�^���X�g�擾���N�G�X�g
typedef struct {
	unsigned short SearchType;											//!< �����^�C�v(POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCHTYPE_xxxx)
	unsigned short SearchOpt;											//!< �����I�v�V����(���ݖ��g�p)
	GT_BATTLE_REC_SEARCH_SEND ConditionParam;							//!< ���������p�����[�^
	u64 SearchParam;													//!< �����p�����[�^
} POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCH;

#define	POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH_MAXNUM	(30)			// �N���C�A���g���󂯎��o�g���f�[�^�w�b�_�ő吔

//! �f�c�r�o�g���f�[�^���X�g�擾���X�|���X���e
typedef struct {
	long PID;															//!< �A�b�v���[�h�҂̃v���t�@�C���h�c
	u64 Code;															//!< �o�g���f�[�^�R�[�h
	GT_BATTLE_REC_OUTLINE_RECV Data;									//!< �o�g���f�[�^�w�b�_
} POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH_RECVDATA;

//! �f�c�r�o�g���f�[�^���X�g�擾���X�|���X
typedef struct {
	long HitCount;														//!< �q�b�g����
	POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH_RECVDATA Data[0];			//!< �o�g���f�[�^���X�g
} POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH;

//------------------------------------
//      �o�g���f�[�^�擾
//------------------------------------
//! �f�c�r�o�g���f�[�^�擾����
enum POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_GET {
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_GET_SUCCESS ,				//!< �擾����
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_GET_ERROR_AUTH ,			//!< ���[�U�[�F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_GET_ERROR_ILLEGALCODE ,		//!< �R�[�h�G���[
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_GET_ERROR_UNKNOWN			//!< ���̑��G���[
};

//! �f�c�r�o�g���f�[�^�擾���N�G�X�g
typedef struct {
	u64 Code;															//!< �o�g���f�[�^�R�[�h
	u32 ServerVersion;													//!< �擾�Ώۃo�g���f�[�^�̃T�[�o�[�o�[�W����
} POKE_NET_GDS_REQUEST_BATTLEDATA_GET;

//! �f�c�r�o�g���f�[�^�擾���X�|���X
typedef struct {
	long PID;															//!< �A�b�v���[�h�҂̃v���t�@�C���h�c
	u64 Code;															//!< �o�g���f�[�^�R�[�h
	GT_BATTLE_REC_RECV Data;											//!< �o�g���f�[�^
} POKE_NET_GDS_RESPONSE_BATTLEDATA_GET;

//------------------------------------
//      �o�g���f�[�^���C�ɓ���o�^
//------------------------------------
//! �f�c�r�o�g���f�[�^���C�ɓ���o�^����
enum POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_FAVORITE {
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_FAVORITE_SUCCESS ,				//!< ���C�ɓ���o�^����
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_FAVORITE_ERROR_AUTH ,			//!< ���[�U�[�F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_FAVORITE_ERROR_ILLEGALCODE ,	//!< �R�[�h�G���[
	POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_FAVORITE_ERROR_UNKNOWN			//!< ���̑��G���[
};

//! �f�c�r�o�g���f�[�^���C�ɓ���o�^���N�G�X�g
typedef struct {
	u64 Code;															//!< �o�g���f�[�^�R�[�h
} POKE_NET_GDS_REQUEST_BATTLEDATA_FAVORITE;

/*
//! �f�c�r�o�g���f�[�^���C�ɓ���o�^���X�|���X
typedef struct {
} POKE_NET_GDS_RESPONSE_BATTLEDATA_FAVORITE;
*/

#ifndef ___POKE_NET_BUILD_DS___
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//        �Ǘ��҃��N�G�X�g/���X�|���X
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------------------
//        �T�[�o�[��Ԏ擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_STATUS {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_STATUS_SUCCESS ,				// �擾����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_STATUS_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_STATUS_ERROR_UNKNOWN			// ���̑��G���[
};

enum {
	POKE_NET_GDS_RESPONSE_ADMIN_SVR_STATUSDATA_STATUS_STOPPED ,			// ��~���
	POKE_NET_GDS_RESPONSE_ADMIN_SVR_STATUSDATA_STATUS_ACTIVE ,			// �ғ����
	POKE_NET_GDS_RESPONSE_ADMIN_SVR_STATUSDATA_STATUS_SHUTDOWN ,		// �V���b�g�_�E��
	POKE_NET_GDS_RESPONSE_ADMIN_SVR_STATUSDATA_STATUS_UNKNOWN ,			// �s�����(�����Ȃ�)
};

#define POKE_NET_GDS_RESPONSE_ADMIN_SVR_MAXNUM	(256)			// �ő�T�[�o�[��

enum {
	POKE_NET_GDS_RESPONSE_ADMIN_SVR_STATUSDATA_SVRTYPE_ADMIN ,	// �Ǘ��T�[�o�[
	POKE_NET_GDS_RESPONSE_ADMIN_SVR_STATUSDATA_SVRTYPE_SLAVE ,	// �ʏ�T�[�o�[
};

typedef struct {
	long Type;												// �T�[�o�[��(POKE_NET_GDS_RESPONSE_ADMIN_SVR_STATUSDATA_SVRTYPE_xxxx)
	long Status;											// �X�e�[�^�X(POKE_NET_GDS_RESPONSE_ADMIN_SVR_STATUSDATA_STATUS_xxxxx)
	long MaxConnectCount;									// �ő�R�l�N�V�����\��
	long ConnectCount;										// �R�l�N�V������
	u64 LastUpdateTime;										// �ŏI�X�e�[�^�X�X�V����(0xYYYYMMDDHHMMSS)
	unsigned long IP;										// �T�[�o�[IP�A�h���X
	long Request;											// �T�[�o�[�֑����Ă��郊�N�G�X�g�R�[�h
} POKE_NET_GDS_RESPONSE_ADMIN_SVR_STATUSDATA;

typedef struct {
	long ServerCount;										// �T�[�o�[��
	POKE_NET_GDS_RESPONSE_ADMIN_SVR_STATUSDATA Status[0];	// �e�T�[�o�[�̃X�e�[�^�X
} POKE_NET_GDS_RESPONSE_ADMIN_SVR_STATUS;

//------------------------------------
//        �T�[�o�[���N���A
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_CLEARSTATUS {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_CLEARSTATUS_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_CLEARSTATUS_ERROR_AUTH ,		// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_CLEARSTATUS_ERROR_UNKNOWN	// ���̑��G���[
};

//------------------------------------
//        �T�[�o�[�ւ̖���
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_COMMAND {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_COMMAND_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_COMMAND_ERROR_AUTH ,				// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_COMMAND_ERROR_ILLEGALIP ,		// �s���ȃT�[�o�[�h�o�A�h���X
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_COMMAND_ERROR_ILLEGALTYPE ,		// �ʏ�T�[�o�[�ȊO�ɂ͖��߂ł��Ȃ�
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_COMMAND_ERROR_ILLEGALSTATUS ,	// ���߂ł��Ȃ��X�e�[�^�X���
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_COMMAND_ERROR_ALREADYREQUEST ,	// ���łɗv���������Ă���T�[�o�[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_COMMAND_ERROR_UNKNOWN			// ���̑��G���[
};

enum POKE_NET_GDS_REQUEST_ADMIN_SVR_COMMAND_REQCODE {
	POKE_NET_GDS_REQUEST_ADMIN_SVR_COMMAND_NOREQ ,						// �v���Ȃ�
	POKE_NET_GDS_REQUEST_ADMIN_SVR_COMMAND_START ,						// �J�n�v��
	POKE_NET_GDS_REQUEST_ADMIN_SVR_COMMAND_STOP ,						// ��~�v��
	POKE_NET_GDS_REQUEST_ADMIN_SVR_COMMAND_SHUTDOWN ,					// �V���b�g�_�E���v��
};

typedef struct {
	short Command;														// �R�}���h�ԍ�(POKE_NET_GDS_REQUEST_ADMIN_SVR_COMMAND_xxx)
	short Option;														// �I�v�V����(���ݖ��g�p)
	unsigned long IP;													// �J�n����T�[�o�[��IP�A�h���X
} POKE_NET_GDS_REQUEST_ADMIN_SVR_COMMAND;

//------------------------------------
// �����c�a�ݒ�
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_DBINITIALIZE {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_DBINITIALIZE_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_DBINITIALIZE_ERROR_AUTH ,	// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_SVR_DBINITIALIZE_ERROR_UNKNOWN	// ���̑��G���[
};

//------------------------------------
//  �h���X�A�b�v�V���b�g�F�S�̏��
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DRESSUPSHOT_STATUS {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DRESSUPSHOT_STATUS_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DRESSUPSHOT_STATUS_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DRESSUPSHOT_STATUS_ERROR_UNKNOWN			// ���̑��G���[
};

/* ���N�G�X�g�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_REQUEST_ADMIN_DRESSUPSHOT_STATUS;
*/

typedef struct {
	long TotalCount;										// �o�^����
	// ����ǉ��\��
} POKE_NET_GDS_RESPONSE_ADMIN_DRESSUPSHOT_STATUS;

//------------------------------------
//  �h���X�A�b�v�V���b�g�F�폜
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DRESSUPSHOT_DELETE {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DRESSUPSHOT_DELETE_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DRESSUPSHOT_DELETE_ERROR_ILLEGALCODE ,	// �R�[�h�G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DRESSUPSHOT_DELETE_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DRESSUPSHOT_DELETE_ERROR_UNKNOWN			// ���̑��G���[
};

typedef struct {
	u64 Code;												// �폜�������R�[�h
} POKE_NET_GDS_REQUEST_ADMIN_DRESSUPSHOT_DELETE;

/* ���X�|���X�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_RESPONSE_ADMIN_DRESSUPSHOT_DELETE;
*/

//------------------------------------
//  �h���X�A�b�v�V���b�g�F�N���A
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DRESSUPSHOT_CLEAR {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DRESSUPSHOT_CLEAR_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DRESSUPSHOT_CLEAR_ERROR_PERMISSION ,		// �F�߂��Ă��Ȃ�
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DRESSUPSHOT_CLEAR_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DRESSUPSHOT_CLEAR_ERROR_UNKNOWN			// ���̑��G���[
};

/* ���N�G�X�g�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_REQUEST_ADMIN_DRESSUPSHOT_CLEAR;
*/

/* ���X�|���X�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_RESPONSE_ADMIN_DRESSUPSHOT_CLEAR;
*/

//------------------------------------
//  �{�b�N�X�V���b�g�F�S�̏��
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BOXSHOT_STATUS {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BOXSHOT_STATUS_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BOXSHOT_STATUS_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BOXSHOT_STATUS_ERROR_UNKNOWN			// ���̑��G���[
};

/* ���N�G�X�g�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_REQUEST_ADMIN_BOXSHOT_STATUS;
*/

typedef struct {
	long TotalCount;										// �o�^����
	// ����ǉ��\��
} POKE_NET_GDS_RESPONSE_ADMIN_BOXSHOT_STATUS;

//------------------------------------
//  �{�b�N�X�V���b�g�F�폜
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BOXSHOT_DELETE {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BOXSHOT_DELETE_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BOXSHOT_DELETE_ERROR_ILLEGALCODE ,	// �R�[�h�G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BOXSHOT_DELETE_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BOXSHOT_DELETE_ERROR_UNKNOWN			// ���̑��G���[
};

typedef struct {
	u64 Code;												// �폜�������R�[�h
} POKE_NET_GDS_REQUEST_ADMIN_BOXSHOT_DELETE;

/* ���X�|���X�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_RESPONSE_ADMIN_BOXSHOT_DELETE;
*/

//------------------------------------
//  �{�b�N�X�V���b�g�F�N���A
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BOXSHOT_CLEAR {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BOXSHOT_CLEAR_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BOXSHOT_CLEAR_ERROR_PERMISSION ,		// �F�߂��Ă��Ȃ�
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BOXSHOT_CLEAR_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BOXSHOT_CLEAR_ERROR_UNKNOWN			// ���̑��G���[
};

/* ���N�G�X�g�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_REQUEST_ADMIN_BOXSHOT_CLEAR;
*/

/* ���X�|���X�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_RESPONSE_ADMIN_BOXSHOT_CLEAR;
*/

//------------------------------------
//  �o�g���f�[�^�F�S�̏��
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_STATUS {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_STATUS_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_STATUS_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_STATUS_ERROR_UNKNOWN			// ���̑��G���[
};

/* ���N�G�X�g�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_STATUS;
*/

typedef struct {
	long TotalRankingCount;									// ���݂܂ł̃����L���O�m���
	long TotalRegistCount;									// �o�^����
	long TotalEventCount;									// �C�x���g�o�^����
	// ����ǉ��\��
} POKE_NET_GDS_RESPONSE_ADMIN_BATTLEDATA_STATUS;

//------------------------------------
//  �o�g���f�[�^�F�폜
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_DELETE {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_DELETE_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_DELETE_ERROR_ILLEGALCODE ,	// �R�[�h�G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_DELETE_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_DELETE_ERROR_UNKNOWN			// ���̑��G���[
};

enum POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_DELETE_AREA {
	POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_DELETE_AREA_ALL ,					// �S��
	POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_DELETE_AREA_REGIST ,				// �o�^�G���A
	POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_DELETE_AREA_TODAYRANKING ,		// �����̃����L���O
	POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_DELETE_AREA_YESTERDAYRANKING ,	// �m�肳�ꂽ��������L���O�G���A
	POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_DELETE_AREA_TODAYEXRANKING ,		// �����̊g�������L���O
	POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_DELETE_AREA_YESTERDAYEXRANKING ,	// �m�肳�ꂽ����g�������L���O�G���A
	POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_DELETE_AREA_EVENT ,				// �C�x���g�G���A
};

typedef struct {
	long TargetArea;										// �^�[�Q�b�g�G���A(POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_DELETE_AREA_xxxx)
	u64 Code;												// �폜�������R�[�h
} POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_DELETE;

/* ���X�|���X�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_RESPONSE_ADMIN_BATTLEDATA_DELETE;
*/

//------------------------------------
//  �o�g���f�[�^�F�N���A
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_CLEAR {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_CLEAR_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_CLEAR_ERROR_PERMISSION ,		// �F�߂��Ă��Ȃ�
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_CLEAR_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_CLEAR_ERROR_UNKNOWN			// ���̑��G���[
};

/*���N�G�X�g�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_CLEAR;
*/

/* ���X�|���X�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_RESPONSE_ADMIN_BATTLEDATA_CLEAR;
*/


//------------------------------------
//  �o�g���f�[�^�F�����L���O�����擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_RANKHISTORY {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_RANKHISTORY_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_RANKHISTORY_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_RANKHISTORY_ERROR_UNKNOWN			// ���̑��G���[
};

enum POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_RANKHISTORY_TYPE {
	POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_RANKHISTORY_TYPE_RANKING ,			// �ʏ탉���L���O
	POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_RANKHISTORY_TYPE_EXRANKING ,			// �g�������L���O
	POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_RANKHISTORY_TYPE_NEW ,				// �ŐV
};

typedef struct {											// - �����L���O�������N�G�X�g -
	long Type;												// �^�C�v(POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_RANKHISTORY_TYPE_xxxx)
	long StartBefore;										// �擾�J�n�ߋ�����(���݂���StartBefore���O�̃����L���O����擾)
	long Count;												// �擾���郉���L���O����
} POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_RANKHISTORY;

typedef struct {											// - �����L���O���f�[�^��� -
	s32 PID;												// �v���t�@�C���h�c
	u64 Code;												// �o�g���f�[�^�R�[�h
	u64 Point;												// �l���|�C���g
	u64 RegistTime;											// �o�^����(0xYYYYMMDDHHMMSS)
	u32 FavoriteCount;										// ���C�ɓ���o�^��
	GT_GDS_PROFILE Profile;									// �v���t�B�[��
	GT_BATTLE_REC_OUTLINE_RECV Data;						// �o�g���f�[�^�w�b�_
} POKE_NET_GDS_RESPONSE_ADMIN_BATTLEDATA_RANKHISTORY_PARAM;

typedef struct {														// - �����L���O��� -
	long Count;															// �q�b�g��
	POKE_NET_GDS_RESPONSE_ADMIN_BATTLEDATA_RANKHISTORY_PARAM List[0];	// �e�f�[�^
} POKE_NET_GDS_RESPONSE_ADMIN_BATTLEDATA_RANKHISTORY_RANKING;

typedef struct {											// - �����L���O�������X�|���X -
	long HitCount;											// �q�b�g���������L���O��
	char RankingList[0];									// �����L���O���X�g
															// (sizeof(POKE_NET_GDS_RESPONSE_ADMIN_BATTLEDATA_RANKHISTORY_RANKING) +
															//	sizeof(POKE_NET_GDS_RESPONSE_ADMIN_BATTLEDATA_RANKHISTORY_PARAM) * ���ꂼ��̃����L���O����Count) * HitCount
} POKE_NET_GDS_RESPONSE_ADMIN_BATTLEDATA_RANKHISTORY;

//------------------------------------
//  �o�g���f�[�^�F�C�x���g�o�^�v��
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_EVENTFLAG {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_EVENTFLAG_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_EVENTFLAG_ERROR_ILLEGALCODE ,		// �R�[�h�G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_EVENTFLAG_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_EVENTFLAG_ERROR_UNKNOWN			// ���̑��G���[
};

typedef struct {
	u64 Code;												// �C�x���g�̈�ֈړ����������f�[�^�̃R�[�h
} POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_EVENTFLAG;

/*
typedef struct {
} POKE_NET_GDS_RESPONSE_ADMIN_BATTLEDATA_EVENTFLAG;
*/

//------------------------------------
//  �o�g���f�[�^�F�Ǘ��p�����[�^�擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_GETPARAM {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_GETPARAM_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_GETPARAM_ERROR_AUTH ,				// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_GETPARAM_ERROR_UNKNOWN ,			// �G���[
};

#define	POKE_NET_GDS_BATTLEDATA_GETPARAM_WEIGHT_MAXNUM	(30+2)			// ���ʂɂ��p�����[�^��(���ʐ�+���̑�/�ŐV)
#define	POKE_NET_GDS_BATTLEDATA_GETPARAM_SCALING_MAXNUM	(10+1)			// ���ʂɂ��p�����[�^��

/*
typedef struct {
} POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_GETPARAM;
*/

typedef struct {
	long Weight[POKE_NET_GDS_BATTLEDATA_GETPARAM_WEIGHT_MAXNUM];		// �|�C���g�d�݂Â�
	long Scaling[POKE_NET_GDS_BATTLEDATA_GETPARAM_SCALING_MAXNUM];		// ���ʂɂ��|�C���g�{��
	s64 InitPoint;														// �����|�C���g�����l
	s64 Bias;															// ���R��臒l
} POKE_NET_GDS_RESPONSE_ADMIN_BATTLEDATA_GETPARAM;

//------------------------------------
//  �o�g���f�[�^�F�Ǘ��p�����[�^�ݒ�
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_SETPARAM {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_SETPARAM_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_SETPARAM_ERROR_AUTH ,				// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_SETPARAM_ERROR_PARAMS ,			// �p�����[�^�G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_SETPARAM_ERROR_UNKNOWN ,			// �G���[
};

#define	POKE_NET_GDS_BATTLEDATA_SETPARAM_WEIGHT_MAXNUM	(30+2)			// ���ʂɂ��p�����[�^��(���ʐ�+���̑�/�ŐV)
#define	POKE_NET_GDS_BATTLEDATA_SETPARAM_SCALING_MAXNUM	(10+1)			// ���ʂɂ��p�����[�^��

typedef struct {
	long Weight[POKE_NET_GDS_BATTLEDATA_SETPARAM_WEIGHT_MAXNUM];		// �|�C���g�d�݂Â�
	long Scaling[POKE_NET_GDS_BATTLEDATA_SETPARAM_SCALING_MAXNUM];		// ���ʂɂ��|�C���g�{��
	s64 InitPoint;														// �����|�C���g�����l
	s64 Bias;															// ���R��臒l
} POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_SETPARAM;

/*
typedef struct {
} POKE_NET_GDS_RESPONSE_ADMIN_BATTLEDATA_SETPARAM;
*/

//------------------------------------
//  �o�g���f�[�^�F�����m�菈��
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_EXECDECISION {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_EXECDECISION_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_EXECDECISION_ERROR_AUTH ,				// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_BATTLEDATA_EXECDECISION_ERROR_UNKNOWN ,			// �G���[
};

/*
typedef struct {
} POKE_NET_GDS_REQUEST_ADMIN_BATTLEDATA_EXECDECISION;
*/

/*
typedef struct {
} POKE_NET_GDS_RESPONSE_ADMIN_BATTLEDATA_EXECDECISION;
*/

//------------------------------------
//  �����L���O�F�S�̏��
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_STATUS {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_STATUS_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_STATUS_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_STATUS_ERROR_UNKNOWN			// ���̑��G���[
};

/* ���N�G�X�g�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_REQUEST_ADMIN_RANKING_STATUS;
*/

typedef struct {
	long TotalRankingCount;									// ���݂܂ł̃����L���O��
	long ThisWeekRankingNo;									// ���݂̃����L���O�̊J�Ôԍ�
	// ����ǉ��\��
} POKE_NET_GDS_RESPONSE_ADMIN_RANKING_STATUS;


//------------------------------------
//  �����L���O�F�����L���O�����擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_HISTORY {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_HISTORY_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_HISTORY_ERROR_AUTH ,				// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_HISTORY_ERROR_UNKNOWN			// ���̑��G���[
};

typedef struct {											// - �����L���O�������N�G�X�g -
	long Type;												// �����L���O�^�C�v(-1:�w��Ȃ�)
	long StartBefore;										// �擾�J�n�ߋ��J�É�(���݂���StartBefore��O�̃����L���O����擾)
	long Count;												// �擾���郉���L���O��
} POKE_NET_GDS_REQUEST_ADMIN_RANKING_HISTORY;


typedef struct {											// - �����L���O�������X�|���X -
	long Type;												// �擾���������L���O�^�C�v
	long HitCount;											// �q�b�g���������L���O��
	GT_LAST_WEEK_RANKING_DATA Ranking[0];					// �����L���O�f�[�^(Type:-1�̎���GT_RANKING_WEEK_NUM�łP�����L���O)
} POKE_NET_GDS_RESPONSE_ADMIN_RANKING_HISTORY;


//------------------------------------
//  �����L���O�F���[�U�[�p�����[�^�擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_GET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_GET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_GET_ERROR_AUTH ,		// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_GET_ERROR_UNKNOWN	// ���̑��G���[
};

#define POKE_NET_GDS_ADMIN_RANKING_USER_STATUS_INEFFECTIVE (0)				// ����
#define POKE_NET_GDS_ADMIN_RANKING_USER_STATUS_EFFECTIVE   (1)			// �L��

#define POKE_NET_GDS_ADMIN_RANKING_USER_MAXCOUNT    (50)									// �p�����[�^���Ԃ��Ă���ő�v���C���[��
#define POKE_NET_GDS_ADMIN_RANKING_USER_PARAMCOUNT  (GT_RANKING_MYDATA_RANKING_TYPE_MAX)	// �p�����[�^���Ԃ��Ă���ő�v���C���[��

typedef struct {													// - �����L���O �p�����[�^�擾���N�G�X�g -
	long PID;														// ���[�U�[�̃v���t�@�C���h�c(0:�S�������[�U�[)
	long PageNo;													// �y�[�W�ԍ�(POKE_NET_GDS_ADMIN_RANKING_USER_MAXCOUNT = �P�y�[�W)
} POKE_NET_GDS_REQUEST_ADMIN_RANKING_GET;

typedef struct {													// - �����L���O ���[�U�[�p�����[�^�擾 -
	long PID;														// PID
	long Status;													// ���݂̃X�e�[�^�X(POKE_NET_GDS_ADMIN_RANKING_USER_STATUS_xxxx)
	u64 Params[POKE_NET_GDS_ADMIN_RANKING_USER_PARAMCOUNT];			// �e�p�����[�^(�T�[�o�[�ɕێ����Ă��郆�[�U�[���̓��_)
} POKE_NET_GDS_RESPONSE_ADMIN_RANKING_GETPARAM;

typedef struct {													// - �����L���O ���[�U�[�p�����[�^�擾 -
	long Count;														// ���[�U�[��
	POKE_NET_GDS_RESPONSE_ADMIN_RANKING_GETPARAM List[0];			// ���[�U�[�p�����[�^
																	// �ő�� * POKE_NET_GDS_ADMIN_RANKING_USER_MAXCOUNT
} POKE_NET_GDS_RESPONSE_ADMIN_RANKING_GET;


//------------------------------------
//  �����L���O�F���[�U�[�p�����[�^�ݒ�
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_SET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_SET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_SET_ERROR_AUTH ,		// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_SET_ERROR_UNKNOWN	// ���̑��G���[
};

typedef struct {													// - �����L���O ���[�U�[�p�����[�^�ݒ� -
	long PID;														// PID(0:����)
	long Status;													// ���݂̃X�e�[�^�X(POKE_NET_GDS_ADMIN_RANKING_USER_STATUS_xxxx)
	u64 Params[POKE_NET_GDS_ADMIN_RANKING_USER_PARAMCOUNT];			// �e�p�����[�^(�T�[�o�[�ɕێ����Ă��郆�[�U�[���̓��_)
} POKE_NET_GDS_REQUEST_ADMIN_RANKING_SETPARAM;

typedef struct {																						// - �����L���O ���[�U�[�p�����[�^�ݒ� -
	long Count;																							// ���[�U�[��
	POKE_NET_GDS_REQUEST_ADMIN_RANKING_SETPARAM List[POKE_NET_GDS_ADMIN_RANKING_USER_MAXCOUNT];			// ���[�U�[�p�����[�^
} POKE_NET_GDS_REQUEST_ADMIN_RANKING_SET;

/* ���X�|���X�̓R�[�h�̂�
typedef struct {													// - ���[�U�[�p�����[�^�ݒ背�X�|���X -
} POKE_NET_GDS_RESPONSE_ADMIN_RANKING_SET;
*/

//------------------------------------
//  �����L���O�F�N���A
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_CLEAR {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_CLEAR_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_CLEAR_ERROR_PERMISSION ,		// �F�߂��Ă��Ȃ�
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_CLEAR_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_CLEAR_ERROR_UNKNOWN			// ���̑��G���[
};

/* ���N�G�X�g�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_REQUEST_ADMIN_RANKING_CLEAR;
*/

/* ���X�|���X�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_RESPONSE_ADMIN_RANKING_CLEAR;
*/

//------------------------------------
//  �����L���O�F�X�P�W���[���擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_GETSCHEDULE {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_GETSCHEDULE_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_GETSCHEDULE_ERROR_AUTH ,				// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_GETSCHEDULE_ERROR_UNKNOWN			// ���̑��G���[
};

/* ���N�G�X�g�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_REQUEST_ADMIN_RANKING_GETSCHEDULE;
*/

#define	POKE_NET_GDS_RESPONSE_ADMIN_RANKING_SCHEDULE_CATEGORYCOUNT	(3)
#define	POKE_NET_GDS_RESPONSE_ADMIN_RANKING_SCHEDULE_ITEMCOUNT		(256)

// �X�P�W���[�����X�g
typedef struct {
	long CurrentWeekNo;
	long CurrentNo[POKE_NET_GDS_RESPONSE_ADMIN_RANKING_SCHEDULE_CATEGORYCOUNT];
	unsigned char Kind[POKE_NET_GDS_RESPONSE_ADMIN_RANKING_SCHEDULE_CATEGORYCOUNT][POKE_NET_GDS_RESPONSE_ADMIN_RANKING_SCHEDULE_ITEMCOUNT];
} POKE_NET_GDS_RESPONSE_ADMIN_RANKING_GETSCHEDULE;

//------------------------------------
//  �����L���O�F�X�P�W���[���ݒ�
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_SETSCHEDULE {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_SETSCHEDULE_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_SETSCHEDULE_ERROR_AUTH ,				// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_SETSCHEDULE_ERROR_UNKNOWN			// ���̑��G���[
};

#define	POKE_NET_GDS_RESPONSE_ADMIN_RANKING_SCHEDULE_CATEGORYCOUNT	(3)
#define	POKE_NET_GDS_RESPONSE_ADMIN_RANKING_SCHEDULE_ITEMCOUNT		(256)

// �X�P�W���[�����X�g
typedef struct {
	unsigned char Kind[POKE_NET_GDS_RESPONSE_ADMIN_RANKING_SCHEDULE_CATEGORYCOUNT][POKE_NET_GDS_RESPONSE_ADMIN_RANKING_SCHEDULE_ITEMCOUNT];
} POKE_NET_GDS_REQUEST_ADMIN_RANKING_SETSCHEDULE;

/* ���X�|���X�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_RESPONSE_ADMIN_RANKING_SETSCHEDULE;
*/

//------------------------------------
//  �����L���O�F�����m�菈��
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_EXECDECISION {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_EXECDECISION_SUCCESS ,				// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_EXECDECISION_ERROR_AUTH ,			// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_RANKING_EXECDECISION_ERROR_UNKNOWN ,			// �G���[
};

/*
typedef struct {
} POKE_NET_GDS_REQUEST_ADMIN_RANKING_EXECDECISION;
*/

/*
typedef struct {
} POKE_NET_GDS_RESPONSE_ADMIN_RANKING_EXECDECISION;
*/

//------------------------------------
// �s���`�F�b�N:�|�P�������̐ݒ�擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_POKEMONTABLE_GET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_POKEMONTABLE_GET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_POKEMONTABLE_GET_ERROR_AUTH ,	// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_POKEMONTABLE_GET_ERROR_UNKNOWN	// ���̑��G���[
};

/*
typedef struct {													// - �|�P�������ݒ�擾���N�G�X�g -
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_POKEMONTABLE_GET;
*/

#define POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_MONSCOUNT	(507)	// �|�P�������\�̓t�H�����g�|�P�������܂�500��

enum {
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_EFFECTIVE ,	// �L��/����
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_RAREFLAG ,		// ���A�t���O(�����Ă����烌�A�L��)
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_DISTRIBUTE ,	// �z�z(�����Ă�����z�z�t���O���`�F�b�N����)
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_LVMIN ,		// �Œ჌�x��
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_FORMMAX ,		// �t�H�����ԍ��ő�
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_CFORM ,		// �v�Z�ς݃t�H�����l
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_ABSEVO ,		// �����I�ɐi���O�Ƃ��Ĉ������ۂ�
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_LEGEND ,		// �`���|�P�������ۂ�
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_ABILITY0 ,		// �Ƃ�����0
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_ABILITY1 ,		// �Ƃ�����1
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_HP ,			// HP
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_ATTACK ,		// ��������
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_DEFENSE ,		// �ڂ�����
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_AGILITY ,		// ���΂₳
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_SATTACK ,		// �Ƃ�����
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_SDEFENSE ,		// �Ƃ��ڂ�
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_TYPE1 ,		// �^�C�v1
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_TYPE2 ,		// �^�C�v2
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_CAPTUREPER ,	// �ߊl��
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_EXP ,			// �o���l
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_EHP ,			// �w�͒lHP
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_EATTACK ,		// �w�͒l��������
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_EDEFENSE ,		// �w�͒l�ڂ�����
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_EAGILITY ,		// �w�͒l���΂₳
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_ESATTACK ,		// �w�͒l�Ƃ�����
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_ESDEFENSE ,	// �w�͒l�Ƃ��ڂ�
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_ITEM0 ,		// �ǂ���0
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_ITEM1 ,		// �ǂ���1
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_SEX ,			// ���ʃx�N�g��
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_BORNWALK ,		// �z������
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_ATTACH ,		// �����Ȃ��x
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_GROWTH ,		// �����Ȑ�
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_EGG0 ,			// �^�}�S�O���[�v0
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_EGG1 ,			// �^�}�S�O���[�v2
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_ESCAPEPER ,	// ������
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_COLOR ,		// �F
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_REV ,			// ���]�s��
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_WAZAFLAG0 ,	// �킴�}�V���t���O0
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_WAZAFLAG1 ,	// �킴�}�V���t���O1
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_WAZAFLAG2 ,	// �킴�}�V���t���O2
	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_WAZAFLAG3 ,	// �킴�}�V���t���O3

	POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_COUNT			// ��
};

typedef struct {													// - �|�P�������ݒ�擾���X�|���X -
	unsigned long Table[POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_MONSCOUNT][POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_COUNT];
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_POKEMONTABLE_GET;

//------------------------------------
// �s���`�F�b�N:�|�P�������̐ݒ�Z�b�g
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_POKEMONTABLE_SET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_POKEMONTABLE_SET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_POKEMONTABLE_SET_ERROR_AUTH ,	// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_POKEMONTABLE_SET_ERROR_UNKNOWN	// ���̑��G���[
};

typedef struct {													// - �|�P�������ݒ胊�N�G�X�g -
	unsigned long Table[POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_MONSCOUNT][POKE_NET_GDS_ADMIN_ILLEGALCHECK_POKEMONTABLE_COUNT];
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_POKEMONTABLE_SET;

/*
typedef struct {													// - �|�P�������ݒ背�X�|���X -
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_POKEMONTABLE_SET;
*/

//------------------------------------
// �s���`�F�b�N:�m�f���[�h�̐ݒ�擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_NGWORD_GET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_NGWORD_GET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_NGWORD_GET_ERROR_AUTH ,		// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_NGWORD_GET_ERROR_UNKNOWN	// ���̑��G���[
};

/*
typedef struct {													// - �m�f���[�h�ݒ�擾���N�G�X�g -
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_NGWORD_GET;
*/

#define POKE_NET_GDS_ADMIN_ILLEGALCHECK_NGWORD_MAXNUM (100)

typedef struct {													// - �m�f���[�h�ݒ�擾���X�|���X -
	long Count;														// �m�f���[�h�R�[�h��
	long Codes[0];													// �m�f���[�h�R�[�h
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_NGWORD_GET;

//------------------------------------
// �s���`�F�b�N:�m�f���[�h�̐ݒ�Z�b�g
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_NGWORD_SET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_NGWORD_SET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_NGWORD_SET_ERROR_AUTH ,		// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_NGWORD_SET_ERROR_UNKNOWN	// ���̑��G���[
};

typedef struct {													// - �m�f���[�h�ݒ胊�N�G�X�g -
	long Count;														// �m�f���[�h�R�[�h��
	long Codes[POKE_NET_GDS_ADMIN_ILLEGALCHECK_NGWORD_MAXNUM];		// �m�f���[�h�R�[�h
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_NGWORD_SET;

/*
typedef struct {													// - �m�f���[�h�ݒ背�X�|���X -
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_NGWORD_SET;
*/

//------------------------------------
// �s���`�F�b�N:����R�[�h�̐ݒ�擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_LANGCODE_GET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_LANGCODE_GET_SUCCESS ,			// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_LANGCODE_GET_ERROR_AUTH ,		// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_LANGCODE_GET_ERROR_UNKNOWN		// ���̑��G���[
};

/*
typedef struct {													// - ����R�[�h�ݒ�擾���N�G�X�g -
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_LANGCODE_GET;
*/

#define POKE_NET_GDS_ADMIN_ILLEGALCHECK_LANGCODE_MAXNUM (100)

typedef struct {													// - ����R�[�h�ݒ�擾���X�|���X -
	long Count;														// ����R�[�h�R�[�h��
	long Codes[0];													// ����R�[�h�R�[�h
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_LANGCODE_GET;

//------------------------------------
// �s���`�F�b�N:����R�[�h�̐ݒ�Z�b�g
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_LANGCODE_SET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_LANGCODE_SET_SUCCESS ,			// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_LANGCODE_SET_ERROR_AUTH ,		// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_LANGCODE_SET_ERROR_UNKNOWN		// ���̑��G���[
};

typedef struct {													// - ����R�[�h�ݒ胊�N�G�X�g -
	long Count;														// ����R�[�h��
	long Codes[POKE_NET_GDS_ADMIN_ILLEGALCHECK_LANGCODE_MAXNUM];	// ����R�[�h
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_LANGCODE_SET;

/*
typedef struct {													// - ����R�[�h�ݒ背�X�|���X -
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_LANGCODE_SET;
*/

//------------------------------------
// �s���`�F�b�N:�ǂ����̐ݒ�擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ITEM_GET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ITEM_GET_SUCCESS ,			// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ITEM_GET_ERROR_AUTH ,		// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ITEM_GET_ERROR_UNKNOWN		// ���̑��G���[
};


/*
typedef struct {													// - �ǂ����ݒ�擾���N�G�X�g -
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_ITEM_GET;
*/

#define POKE_NET_GDS_ADMIN_ILLEGALCHECK_ITEM_MAXNUM (100)

typedef struct {													// - �ǂ����ݒ�擾���X�|���X -
	long Count;														// �ǂ�����
	long Codes[0];													// �ǂ����R�[�h
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_ITEM_GET;

//------------------------------------
// �s���`�F�b�N:�ǂ����̐ݒ�Z�b�g
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ITEM_SET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ITEM_SET_SUCCESS ,			// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ITEM_SET_ERROR_AUTH ,		// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ITEM_SET_ERROR_UNKNOWN		// ���̑��G���[
};

typedef struct {													// - �ǂ����ݒ胊�N�G�X�g -
	long Count;														// �ǂ�����
	long Codes[POKE_NET_GDS_ADMIN_ILLEGALCHECK_ITEM_MAXNUM];		// �ǂ����R�[�h
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_ITEM_SET;

/*
typedef struct {													// - �ǂ����ݒ背�X�|���X -
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_ITEM_SET;
*/

//------------------------------------
// �s���`�F�b�N:�o������ꏊ�̐ݒ�擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_MEETPLACE_GET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_MEETPLACE_GET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_MEETPLACE_GET_ERROR_AUTH ,	// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_MEETPLACE_GET_ERROR_UNKNOWN	// ���̑��G���[
};

/*
typedef struct {													// - �o������ꏊ�ݒ�擾���N�G�X�g -
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_MEETPLACE_GET;
*/

#define POKE_NET_GDS_ADMIN_ILLEGALCHECK_MEETPLACE_MAXPLACE		(1+32)
#define POKE_NET_GDS_ADMIN_ILLEGALCHECK_MEETPLACE_MAXPOKEMON	(GT_BOXSHOT_MONSNO_MAX)

typedef struct {													// - �o������ꏊ�ݒ�擾���X�|���X -
	long Count;														// �ݒ肳��Ă���|�P������
	s16 Table[0];													// sizeof(s16)*POKE_NET_GDS_ADMIN_ILLEGALCHECK_MEETPLACE_MAXPLACE * HitCount
																	// [0]�̓|�P�����ԍ�
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_MEETPLACE_GET;

//------------------------------------
// �s���`�F�b�N:�o������ꏊ�̐ݒ�Z�b�g
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_MEETPLACE_SET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_MEETPLACE_SET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_MEETPLACE_SET_ERROR_AUTH ,	// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_MEETPLACE_SET_ERROR_UNKNOWN	// ���̑��G���[
};

typedef struct {													// - �o������ꏊ�ݒ胊�N�G�X�g -
	long Count;
	s16 Table[POKE_NET_GDS_ADMIN_ILLEGALCHECK_MEETPLACE_MAXPOKEMON*POKE_NET_GDS_ADMIN_ILLEGALCHECK_MEETPLACE_MAXPLACE];	// sizeof(s16)*POKE_NET_GDS_ADMIN_ILLEGALCHECK_MEETPLACE_MAXPLACE * HitCount
																														// [0]�̓|�P�����ԍ�
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_MEETPLACE_SET;

/*
typedef struct {													// - �o������ꏊ�ݒ背�X�|���X -
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_MEETPLACE_SET;
*/


//------------------------------------
// �s���`�F�b�N:�Z�\�̐ݒ�擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_WAZATABLE_GET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_WAZATABLE_GET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_WAZATABLE_GET_ERROR_AUTH ,	// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_WAZATABLE_GET_ERROR_UNKNOWN	// ���̑��G���[
};

#define POKE_NET_GDS_ADMIN_ILLEGALCHECK_WAZATABLE_MONSCOUNT	(507)	// �Z�\�̓t�H�����g�|�P�������܂�507��

/*
typedef struct {													// - �Z�\�ݒ�擾���N�G�X�g -
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_WAZATABLE_GET;
*/

typedef struct {													// - �Z�\�ݒ�擾���X�|���X -
	long Size;														// �T�C�Y
	char Binary[0];													// �o�C�i���f�[�^
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_WAZATABLE_GET;

//------------------------------------
// �s���`�F�b�N:�Z�\�̐ݒ�Z�b�g
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_WAZATABLE_SET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_WAZATABLE_SET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_WAZATABLE_SET_ERROR_AUTH ,	// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_WAZATABLE_SET_ERROR_UNKNOWN	// ���̑��G���[
};

typedef struct {													// - �Z�\�ݒ胊�N�G�X�g -
	long Size;														// �T�C�Y
	char Binary[0];													// �o�C�i���f�[�^
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_WAZATABLE_SET;

/*
typedef struct {													// - �Z�\�ݒ背�X�|���X -
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_WAZATABLE_SET;
*/


//------------------------------------
// �s���`�F�b�N:�A�N�Z�T���̐ݒ�擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ACCESSORY_GET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ACCESSORY_GET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ACCESSORY_GET_ERROR_AUTH ,	// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ACCESSORY_GET_ERROR_UNKNOWN	// ���̑��G���[
};

/*
typedef struct {													// - �A�N�Z�T���ݒ�擾���N�G�X�g -
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_ACCESSORY_GET;
*/

#define POKE_NET_GDS_ADMIN_ILLEGALCHECK_ACCESSORY_MAXNUM (100)

typedef struct {													// - �A�N�Z�T���ݒ�擾���X�|���X -
	long Count;														// �A�N�Z�T���R�[�h��
	long Codes[0];													// �A�N�Z�T���R�[�h
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_ACCESSORY_GET;

//------------------------------------
// �s���`�F�b�N:�A�N�Z�T���̐ݒ�Z�b�g
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ACCESSORY_SET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ACCESSORY_SET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ACCESSORY_SET_ERROR_AUTH ,	// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_ACCESSORY_SET_ERROR_UNKNOWN	// ���̑��G���[
};

typedef struct {														// - �A�N�Z�T���ݒ胊�N�G�X�g -
	long Count;															// �A�N�Z�T���R�[�h��
	long Codes[POKE_NET_GDS_ADMIN_ILLEGALCHECK_ACCESSORY_MAXNUM];		// �A�N�Z�T���R�[�h
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_ACCESSORY_SET;

/*
typedef struct {													// - �A�N�Z�T���ݒ背�X�|���X -
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_ACCESSORY_SET;
*/

//------------------------------------
// �s���`�F�b�N:�h���X�A�b�vBGID�̐ݒ�擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_DRESSUPBGID_GET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_DRESSUPBGID_GET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_DRESSUPBGID_GET_ERROR_AUTH ,	// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_DRESSUPBGID_GET_ERROR_UNKNOWN	// ���̑��G���[
};

/*
typedef struct {													// - �h���X�A�b�vBGID�ݒ�擾���N�G�X�g -
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_DRESSUPBGID_GET;
*/

#define POKE_NET_GDS_ADMIN_ILLEGALCHECK_DRESSUPBGID_MAXNUM (18)

typedef struct {													// - �h���X�A�b�vBGID�ݒ�擾���X�|���X -
	long Count;														// �h���X�A�b�vBGID�R�[�h��
	long Codes[0];													// �h���X�A�b�vBGID�R�[�h
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_DRESSUPBGID_GET;

//------------------------------------
// �s���`�F�b�N:�h���X�A�b�vBGID�̐ݒ�Z�b�g
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_DRESSUPBGID_SET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_DRESSUPBGID_SET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_DRESSUPBGID_SET_ERROR_AUTH ,	// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_DRESSUPBGID_SET_ERROR_UNKNOWN	// ���̑��G���[
};

typedef struct {														// - �h���X�A�b�vBGID�ݒ胊�N�G�X�g -
	long Count;															// �h���X�A�b�vBGID�R�[�h��
	long Codes[POKE_NET_GDS_ADMIN_ILLEGALCHECK_DRESSUPBGID_MAXNUM];		// �h���X�A�b�vBGID�R�[�h
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_DRESSUPBGID_SET;

/*
typedef struct {													// - �h���X�A�b�vBGID�ݒ背�X�|���X -
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_DRESSUPBGID_SET;
*/


//------------------------------------
// �s���`�F�b�N:���M�����[�V�����̐ݒ�擾
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_REGULATION_GET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_REGULATION_GET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_REGULATION_GET_ERROR_AUTH ,		// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_REGULATION_GET_ERROR_UNKNOWN	// ���̑��G���[
};

/*
typedef struct {																	// - ���M�����[�V�����ݒ�擾���N�G�X�g -
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_REGULATION_GET;
*/

#define POKE_NET_GDS_ADMIN_ILLEGALCHECK_REGULATION_MAXNUM (100)

typedef struct {																	// - ���M�����[�V�����ݒ�擾���X�|���X -
	long Count;																		// ���M�����[�V������
	GT_REGULATION Regulation[0];													// ���M�����[�V����
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_REGULATION_GET;

//------------------------------------
// �s���`�F�b�N:���M�����[�V�����̐ݒ�Z�b�g
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_REGULATION_SET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_REGULATION_SET_SUCCESS ,		// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_REGULATION_SET_ERROR_AUTH ,		// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_ILLEGALCHECK_REGULATION_SET_ERROR_UNKNOWN	// ���̑��G���[
};

typedef struct {																	// - ���M�����[�V�����ݒ胊�N�G�X�g -
	long Count;																		// ���M�����[�V�����R�[�h��
	GT_REGULATION Regulation[POKE_NET_GDS_ADMIN_ILLEGALCHECK_REGULATION_MAXNUM];	// ���M�����[�V������
} POKE_NET_GDS_REQUEST_ADMIN_ILLEGALCHECK_REGULATION_SET;

/*
typedef struct {																	// - ���M�����[�V�����ݒ背�X�|���X -
} POKE_NET_GDS_RESPONSE_ADMIN_ILLEGALCHECK_REGULATION_SET;
*/

//------------------------------------
// �s���`�F�b�N���O:�擾
//------------------------------------
#define POKE_NET_GDS_RESPONSE_ADMIN_DHOCHECKLOG_GET_TIMECAPA	(20)	// ����������̑傫��
#define POKE_NET_GDS_RESPONSE_ADMIN_DHOCHECKLOG_GET_CONTENTCAPA	(256)	// ���e������̑傫��
#define POKE_NET_GDS_RESPONSE_ADMIN_DHOCHECKLOG_GET_MAXRESPONSE	(30)	// �ő剽���܂ł̃f�[�^��ԓ����邩

enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DHOCHECKLOG_GET {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DHOCHECKLOG_GET_SUCCESS ,
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DHOCHECKLOG_GET_ERROR_UNKNOWN
};

typedef struct {
	long	PageNo;		// �y�[�W�ԍ�
} POKE_NET_GDS_REQUEST_ADMIN_DHOCHECKLOG_GET;

typedef struct {
	char	Time[POKE_NET_GDS_RESPONSE_ADMIN_DHOCHECKLOG_GET_TIMECAPA];
	char	Content[POKE_NET_GDS_RESPONSE_ADMIN_DHOCHECKLOG_GET_CONTENTCAPA];
} POKE_NET_GDS_RESPONSE_ADMIN_DHOCHECKLOG_GET_PARAM;

typedef struct {
	long	HitCount;
	POKE_NET_GDS_RESPONSE_ADMIN_DHOCHECKLOG_GET_PARAM	List[0];
} POKE_NET_GDS_RESPONSE_ADMIN_DHOCHECKLOG_GET;

//------------------------------------
// �s���`�F�b�N���O:�N���A
//------------------------------------
enum POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DHOCHECKLOG_CLEAR {
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DHOCHECKLOG_CLEAR_SUCCESS ,			// ����
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DHOCHECKLOG_CLEAR_ERROR_PERMISSION ,	// �F�߂��Ă��Ȃ�
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DHOCHECKLOG_CLEAR_ERROR_AUTH ,		// �F�؃G���[
	POKE_NET_GDS_RESPONSE_RESULT_ADMIN_DHOCHECKLOG_CLEAR_ERROR_UNKNOWN		// ���̑��G���[
};

/*	 ���N�G�X�g�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_REQUEST_ADMIN_DHOCHECKLOG_CLEAR;
*/

/*	���X�|���X�̓R�[�h�̂�
typedef struct {
} POKE_NET_GDS_RESPONSE_ADMIN_DHOCHECKLOG_CLEAR;
*/

#endif

#endif //___POKE_NET_GDS_COMMON___
