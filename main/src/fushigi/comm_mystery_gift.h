//============================================================================================
/**
 * @file	comm_fushigi_present.h
 * @bfief	�ӂ����ȁ@���������
 * @author	Satoshi Mitsuhara
 * @date	06.05.08
 *
 * $Id: comm_mystery_gift.h,v 1.25 2006/08/12 06:29:26 mitsuhara Exp $
 */
//============================================================================================
#ifndef __COMM_MYSTERY_GIFT_H__
#define __COMM_MYSTERY_GIFT_H__

#include "common.h"
#include "system/procsys.h"
#include "system/heapdefine.h"
#include "system/bmp_list.h"
#include "system/wordset.h"
#include "system/wipe.h"
#include "system/clact_tool.h"

#include "savedata/savedata_def.h"
#include "savedata/fushigi_data.h"



enum {
  MYSTERYGIFT_WIN_MAIN = 0,		/* ���C�����j���[�I���E�B���h�E */
  MYSTERYGIFT_WIN_COMM_TYPE,		/* �ʐM�����I���E�B���h�E */
  MYSTERYGIFT_WIN_COMM_DIRECT_YESNO,	/* �ʐM�����Ń_�C���N�g�ʐM���g���܂����u�͂��^�������v�E�B���h�E */
  MYSTERYGIFT_WIN_MAX
};

#define MYSTERYGIFT_TITLE_MAX		(36)
#define MYSTERYGIFT_EXPLANATION_MAX	(250)
// �e�@�Ǝq�@�����������邽�߂̎��ʃR�[�h
#define MYSTERYGIFT_SYNC_CODE		(0xAB)
#define MYSTERYGIFT_SYNC_CODE2		(0x93)

// �ʐM��������߂鎞��
#define MYSTERYGIFT_TIMEOUT		(60*120)


//============================================================================================

// -- WiFi�Ɋ֘A�����萔 --
#define FILE_NUM				10
enum {
  ND_RESULT_EXECUTE,		// ���s��
  ND_RESULT_COMPLETE,		// ����_�E�����[�h�I��
  ND_RESULT_NOT_FOUND_FILES,	// �t�@�C����������Ȃ�����
  ND_RESULT_DOWNLOAD_CANCEL,	// ���[�U�[����L�����Z�����ꂽ
  ND_RESULT_DOWNLOAD_ERROR,	// �Ȃ�炩�̃G���[���N����
  ND_RESULT_MAX
};

#define WIFI_MYSTERYGIFT_BASE	0x1000
#define WIFI_MYSTERYFILE_SIZE	4096


enum {
  WIFI_MYSTERYGIFT_COMM_INIT = WIFI_MYSTERYGIFT_BASE,// ���C�u����������
  WIFI_MYSTERYGIFT_COMM_INIT_WAIT,		// ���C�u�����̏���������������܂ő҂�

  WIFI_MYSTERYGIFT_CONNECT_INET,		// �C���^�[�l�b�g�ւ̐ڑ�����
  WIFI_MYSTERYGIFT_CONNECT_WAIT_INET,		// �ڑ������܂ő҂�

  WIFI_MYSTERYGIFT_NASLOGIN_START,		// �F�؃T�[�o�[�փ��O�C���J�n
  WIFI_MYSTERYGIFT_NASLOGIN_WAIT,		// �F�؃T�[�o�[�փ��O�C�������܂ő҂�
  
  WIFI_MYSTERYGIFT_NDLIB_INIT,			// �_�E�����[�h�����J�n
  WIFI_MYSTERYGIFT_SET_ATTR,			// �t�@�C���A�N�Z�X�����̐ݒ�
  WIFI_MYSTERYGIFT_GET_FILENUM,			// �T�[�o�[�ɂ���t�@�C�����𓾂�
  WIFI_MYSTERYGIFT_GET_FILELIST,		// �T�[�o�[�ɂ���t�@�C�����X�g�𓾂�
  WIFI_MYSTERYGIFT_GET_FILE,			// �t�@�C�����擾�J�n
  WIFI_MYSTERYGIFT_GETTING_FILE,		// �t�@�C���擾��

  WIFI_MYSTERYGIFT_DOWNLOAD_CANCEL,		// �_�E�����[�h�����[�U�[���L�����Z������
  WIFI_MYSTERYGIFT_DOWNLOAD_COMPLETE,		// �_�E�����[�h����I��
  WIFI_MYSTERYGIFT_DOWNLOAD_ERROR_LOOP,		// �G���[�����҂�
  WIFI_MYSTERYGIFT_DOWNLOAD_ERROR_WAIT,		// callback���Ăяo�����܂ŏ�����҂�
  WIFI_MYSTERYGIFT_DOWNLOAD_ERROR,		// �G���[�\����̕��A����
WIFI_MYSTERYGIFT_DOWNLOAD_ERROR_WAIT2,
  
  WIFI_MYSTERYGIFT_WAIT_CALLBACK1,		// �R�[���o�b�N�������Ă΂��܂ő҂�
  WIFI_MYSTERYGIFT_WAIT_CALLBACK2,		// �R�[���o�b�N�������Ă΂��܂ő҂�

  WIFI_MYSTERYGIFT_MAX
};

//============================================================================================

// �ӂ����Ȃ�������̐�p���[�N��`
typedef struct {
  GF_BGL_INI *bgl;
  SAVEDATA *sv;
  CONFIG *cfg;
  WORDSET *word;	       			// �P�ꃂ�W���[�����䃏�[�N�ւ̃|�C���^
  MSGDATA_MANAGER *msgman;	    		// MSG�f�[�^�}�l�[�W���ւ̃|�C���^
  STRBUF *msg;					// �Q�s�ȏ゠�郁�b�Z�[�W�̎��̂ݎg�p

  GF_BGL_BMPWIN	msgwin;				// ���b�Z�[�W��\������E�B���h�E
  GF_BGL_BMPWIN	selwin[MYSTERYGIFT_WIN_MAX];	// ���j���[�I������E�B���h�E
  GF_BGL_BMPWIN	upwin;				// ��ʏ�̃^�C�g���E�B���h�E

  int msg_wait;					// ���b�Z�[�W�\�����x
  int m_id;
  int msg_seq;					// ���b�Z�[�W�\�����̃V�[�P���X
  int msg_next_seq;				// ���b�Z�[�W��\�����I������i�ނׂ��V�[�P���X
  
  BMPLIST_WORK *lw;				// ���j���[
  BMP_MENULIST_DATA *bmd;

  int gift_error;				// �󂯎�������������(Beacon)�Ɋւ���G���[���
  
  int from_seq;
  int next_seq;					// �t�F�[�h�̎��ɍs���V�[�P���X�ԍ�
  GIFT_COMM_PACK gift_data;			// �͂�����������̃f�[�^�̑S��(�r�[�R���{�f�[�^)
  int timeout;

  int connectIndex;
  int sync_wait;				// �������ʐM���̃^�C�~���O�҂�

  int to_seq;					// �s����


  // �Z���A�N�^�[�֘A
  CLACT_SET_PTR	clactSet;				// �Z���A�N�^�[�Z�b�g 
  CLACT_U_EASYRENDER_DATA renddata;			// �ȈՃ����_�[�f�[�^
  CLACT_U_RES_MANAGER_PTR resMan[CLACT_U_RES_MAX];	// ���\�[�X�}�l�[�W��
  CLACT_U_RES_OBJ_PTR resObjTbl[CLACT_U_RES_MAX];	// ���\�[�X�I�u�W�F�e�[�u��
  CLACT_HEADER clActHeaderMain;
  // �₶�邵�A�C�R��
  CLACT_WORK_PTR arrow;

  // AGB�J�[�g���b�W����̃_�E�����[�h�t���O
  //  int agb_cartridge;

  // �r�[�R���擾�V�[�P���X
  int beacon_seq;
  int beacon_wait;
  int found_beacon;

  u8 recvbuf[4*1024];
  int comm_type;

  // PROC��*seq�������ɒ�`���Ă����܂�
  int *seq;
  int wifi_wait;

  // WiFi�ӂ����Ȃ�������̌n
  int wifi_dl_seq;
  u8 *dwcWork;
  DWCInetControl stConnCtrl;
  BOOL callbackflag;
  int wifi_next_seq;
  int server_filenum;
  char filebuffer[WIFI_MYSTERYFILE_SIZE];
  int wifi_result;
  u32 recived,contentlen,percent;
  BOOL wifi_errflag;
  int wifi_errmsg;
  int wifi_errcode;
  int wifi_errtype;
  DWCErrorType wifi_cancel;
  BOOL (*wifi_check_func)(void);
  GF_BGL_BMPWIN	wifiwin;

  GIFT_DATA comm_data;					// �Í������ꂽ�f�[�^

  void (*func_hook)(void *);
  int save_cancel;
  void *time_icon;
  
  int	demo_state;
  
} MYSTERYGIFT_WORK;



enum {
  MYSTERYCOMM_TYPE_NULL,
  MYSTERYCOMM_TYPE_DIRECT,		// �_�C���N�g�ʐM
  MYSTERYCOMM_TYPE_AGBCARTRIDGE,	// �`�f�a�̃J�[�g���b�W����_�E�����[�h
  MYSTERYCOMM_TYPE_BEACON,		// �r�[�R������_�E�����[�h
  MYSTERYCOMM_TYPE_WIFI,		// Wi-Fi����̃_�E�����[�h
  
  MYSTERYCOMM_TYPE_MAX
};



extern PROC *MyseryGiftGetProcp(void);
extern void MysteryGiftSetProcp(PROC *proc);
extern void WifiMysteryGift_ErrorDisp(MYSTERYGIFT_WORK *wk);
extern void SetTimeWaitIconEx(MYSTERYGIFT_WORK *wk, int flag);


#endif	// __COMM_MYSTERY_GIFT_H__
/*  */
