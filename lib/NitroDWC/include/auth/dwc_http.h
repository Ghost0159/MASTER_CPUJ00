/*---------------------------------------------------------------------------*
  Project:  NitroDWC - auth - dwc_http
  File:     dwc_http.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_http.h,v $
  Revision 1.49  2006/11/07 04:49:50  nakata
  ���O�������IP�A�h���X���擾�ł���悤�ɕύX

  Revision 1.48  2006/03/10 04:07:15  nakata
  DWC_Http��128�����ȏ��URL�Ńt���[�Y��������C��

  Revision 1.47  2006/03/02 05:21:58  nakata
  DWC_Svc���W���[���ǉ�
  DWC_Util_Alloc/DWC_Util_Free�֐��̖��O��DWC_Auth_Alloc/DWC_Auth_Free�֐��ɖ��O��ύX

  Revision 1.46  2005/12/21 12:02:19  nakata
  Rollback Auth related code to versiont "2005-11-21 08:00:00"
  Fixed retry times count bug in DWC_Auth.

  Revision 1.31  2005/11/17 12:17:16  nakata
  Resurrected old memory management functions.
  DWC_Netcheck sends entire HTTP packet from Wayport router to NAS.

  Revision 1.30  2005/11/17 07:52:34  nakata
  Now DWC_Auth/DWC_Netcheck/DWC_Http don't use DWC_Alloc/DWC_Free.
  DWC_Netcheck sends whole HTTP packet including HTTP header to NAS
  when receives 302 message from conntest.

  Revision 1.29  2005/11/15 06:29:47  nakata
  Now, DWC_Auth sends ESSID when the NDS connects to Hotspot AP.
  Removed unneccesary file "util_alloc.c".

  Revision 1.28  2005/11/15 01:43:26  nakata
  DWC_Http now uses DWC_Alloc/DWC_Free in base library for memory management.
  For this modification, Makefiles for AcSample/UtilSample were modified.

  Revision 1.27  2005/11/15 01:05:47  nakata
  DWC_Http now recognises "http://xxx:yyy/zzz" style URL that has custom port number.

  Revision 1.26  2005/10/18 00:09:11  sasakit
  DWC��auth�ڑ����ɃX���b�h�v���C�I���e�B��������@�\����߂��B

  Revision 1.25  2005/10/17 06:33:14  sasakit
  SSL���s���̃X���b�h�v���C�I���e�B�𒲐�����@�\��L���������B

  Revision 1.24  2005/10/07 13:10:53  nakata
  Added SSL priority setting code to DWC_Http. But disabled by DWC_HTTP_SETSSLPRIO switch.
  Fixed ingamesn length detection part. It used strlen and now it uses wcslen for ingamesn.

  Revision 1.23  2005/09/14 10:36:47  nakata
  To fix stuck bug that can be occured when DWC_Netcheck is aborted, modified DWC_Http.
  Modified DWC_Http not to use OS_DisableIrq/RestoreIrq.
  Modified DWC_Http to use CPS_SocGetLength().
  Refine debug outputs.

  Revision 1.22  2005/09/12 06:19:29  nakata
  Added DWC_Netcheck_SetCustomBlacklist function.
  Refined DWC_Http timeout/receive finish detection.
  Modified some Makefile for successful compilation.

  Revision 1.21  2005/09/07 02:47:00  nakata
  Added progress notification function to DWC_Http.

  Revision 1.20  2005/09/05 23:34:51  nakata
  Added interface function for getting loading progress persentage to DWC_Http.

  Revision 1.19  2005/09/05 02:11:29  nakata
  Now, DWC_Http can download data to pre-allocated receive buffer.

  Revision 1.18  2005/08/22 11:48:02  nakata
  Add runtime custom nas setting function to DWC_Auth.

  Revision 1.17  2005/08/20 07:01:18  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_HTTP_H_
#define DWC_HTTP_H_

#include <nitroWiFi.h>
#include <nitrowifi/version.h>

#include "util_alloc.h"
#include "util_base64.h"

#ifdef __cplusplus
extern "C" {
#endif // [nakata] __cplusplus

//=============================================================================
// [nakata] ��`
//=============================================================================

// [nakata] URL�̍ő咷
#define DWCHTTP_MAXURL			256

// [nakata] �X�^�b�N�T�C�Y
#define DWCHTTP_STACKSIZE		4096

#define DWCHTTP_MAX_LABELVALUE	32

// [nakata] SSL�v���C�I���e�B�ݒ��ON�ɂ���Ƃ���define���邱��
// [nakata] �������s���̈���SVC_WaitVBlankIntr()���Ȃ����Ƃ��\���Ɋm�F���邱�ƁI
//#define DWC_HTTP_SETSSLPRIO // [sasakit] �\�z�ȏ�Ƀu���b�N����̂ł�߂��B

typedef enum
{
	DWCHTTP_E_NOERR,
	
	DWCHTTP_E_MEMERR,
	DWCHTTP_E_DNSERR,
	DWCHTTP_E_CONERR,
	DWCHTTP_E_SENDTOUT,
	DWCHTTP_E_SENDERR,
	DWCHTTP_E_RECVTOUT,
	DWCHTTP_E_ABORT,
	
	DWCHTTP_E_FINISH,
	DWCHTTP_E_MAX
} DWCHttpError;

// [nakata] POST��GET�������肷��t���O
typedef enum
{
	DWCHTTP_POST,
	DWCHTTP_GET
} DWCHttpAction;

// [nakata] �o�b�t�@���܂Ƃ߂ĊǗ����邽�߂̍\����
typedef struct {
	char *buffer;
	char *write_index;
	char *buffer_tail;
	unsigned long length;
} DWCHttpBuffer;

// [nakata] ���x���ƌ��ʂ̕������΂ɂ��ĊǗ����邽�߂̍\����
typedef struct
{
	char *label;
	char *value;
} DWCHttpLabelValue;

// [nakata] ���x���̉���Ǘ����邽�߂̍\����
typedef struct
{
	DWCHttpLabelValue *entry;
	int len;
	int index;
} DWCHttpParseResult;

// [nakata] DWCHttp�������p�����[�^
typedef struct {
	const char		*url;
	DWCHttpAction	action;
	unsigned long	len_recvbuf;
	
	DWCAuthAlloc	alloc;
	DWCAuthFree		free;
	
	BOOL			ignoreca;
	
	int				timeout;
} DWCHttpParam;

// [nakata] DWCHttp���g�p������������i�[���邽�߂̍\����

typedef struct {
	unsigned char	stack[DWCHTTP_STACKSIZE];
	
	// [nakata] �������t���O
	u8				initflag;
	
	// [nakata] �������p�����[�^
	DWCHttpParam	param;
	DWCHttpError	error;					// [nakata] �Ō�ɔ��������G���[�R�[�h
	
	// [nakata] �z�X�g/SSL�֘A
	char			url[DWCHTTP_MAXURL];	// [nakata] URL�ۑ��p�z��
	char			*hostname;				// [nakata] �z�X�g���ւ̃|�C���^(url���̂ǂ������w��)
	char			*filepath;				// [nakata] �t�@�C���p�X�ւ̃|�C���^(url���̂ǂ������w��)
	u32				hostip;					// [nakata] ���O�����̌��ʓ���ꂽIP�A�h���X
	BOOL			ssl_enabled;				// [nakata] SSL���g�p���邩���Ȃ����̃t���O
	unsigned short		port;
	
	// [nakata] CPS�\�P�b�g�֘A
	CPSSoc				soc;
	CPSSslConnection	con;
	unsigned char		*lowrecvbuf;//[1460*2];
	unsigned char		*lowsendbuf;//[1514];
	u32					lowentropydata[OS_LOW_ENTROPY_DATA_SIZE/sizeof(u32)];
	
	// [nakata] ����M�o�b�t�@�|�C���^�Ȃ�
	int				num_postitem;
	DWCHttpBuffer	req;
	DWCHttpBuffer	rep;
	
	// [nakata] �悭�g���l��ێ�����̈�
	OSMutex			content_len_mutex;
	int				content_len;
	int				receivedbody_len;
	
	// [nakata] ��M�f�[�^�p�[�X���ʊi�[�̈�
	DWCHttpLabelValue	labelvalue[DWCHTTP_MAX_LABELVALUE];

	// [nakata] ����M�X���b�h�֘A
	OSThread		thread;
	OSMutex			mutex;
	BOOL			abort;
} DWCHttp;

//=============================================================================
// [nakata] �֐�
//=============================================================================

//-----------------------------------------------------------------------------
// �@�\ : DWC_Http�Ŏg�p����\���̂̏�����/�������m��
// ���� : http              - �����Ώۂ�DWC_Http�\���̂ւ̃|�C���^
//      : max_recv_bodysize - ��M����ő�̃p�P�b�g��
//      : alloc             - �����Ŏg�p���郁�����m�ۊ֐��ւ̃|�C���^
//      : free              - �����Ŏg�p���郁��������֐��ւ̃|�C���^
// �Ԓl : BOOL  - TRUE  : ����
//                FALSE : ���s
//-----------------------------------------------------------------------------
DWCHttpError DWC_Http_Create(DWCHttp *http, DWCHttpParam *param);

//-----------------------------------------------------------------------------
// �@�\ : DWC_Http�Ŏg�p�������������/�\�P�b�g�N���[�Y/SSL���
// ���� : http  - �����Ώۂ�DWC_Http�\���̂ւ̃|�C���^
// �Ԓl : BOOL  - TRUE  : ����
//                FALSE : ���s
//-----------------------------------------------------------------------------
void DWC_Http_Destroy(DWCHttp *http);

//-----------------------------------------------------------------------------
// �@�\ : DWCHttp�X���b�h�쐬�֐�
//        
// ���� : http - �����Ώۂ�DWC_Http�\���̂ւ̃|�C���^
//      : prio - HTTP�X���b�h�D��x
// �Ԓl : DWCHttpError
//-----------------------------------------------------------------------------
void DWC_Http_StartThread(DWCHttp *http, u32 prio);

//-----------------------------------------------------------------------------
// �@�\ : DWCHttp�A�{�[�g
//        
// ���� : http  - �����Ώۂ�DWC_Http�\���̂ւ̃|�C���^
// �Ԓl : �Ȃ�
//-----------------------------------------------------------------------------
void DWC_Http_Abort(DWCHttp *http);

//-----------------------------------------------------------------------------
// �@�\ : DWCHttp��M���擾
//        
// ���� : http - �����Ώۂ�DWC_Http�\���̂ւ̃|�C���^
// �Ԓl : int  - 0����255 ��M�p�[�Z���e�[�W
//             - -1       ��M���łȂ�
//-----------------------------------------------------------------------------
int DWC_Http_GetRecvProgress(DWCHttp *http);

//-----------------------------------------------------------------------------
// �@�\ : ���M�w�b�_�ɍ��ڂ�ǉ�����
// ���� : http  - �����Ώۂ�DWC_Http�\���̂ւ̃|�C���^
//      : label - ���x��(NULL�I�[���Ă���K�v������)
//      : data  - �f�[�^(NULL�I�[���Ă���K�v������)
// �Ԓl : DWCHttpError
//-----------------------------------------------------------------------------
DWCHttpError DWC_Http_Add_HeaderItem(DWCHttp *http, char *label, char *data);

//-----------------------------------------------------------------------------
// �@�\ : ���MPOST����Base64�G���R�[�h�������ڂ�ǉ�����
// ���� : http      - �����Ώۂ�DWC_Http�\���̂ւ̃|�C���^
//      : label     - ���x��(NULL�I�[���Ă���K�v������)
//      : data      - �f�[�^(NULL�I�[���Ă��Ȃ��Ă��悢)
//      : data_len  - �f�[�^��
// �Ԓl : DWCHttpError
//-----------------------------------------------------------------------------
DWCHttpError DWC_Http_Add_PostBase64Item(DWCHttp *http, const char *label, const char *data, unsigned long data_len);

//-----------------------------------------------------------------------------
// �@�\ : ���M�p�P�b�g��BODY�Ɏw�肵���������ǉ�����
// ���� : http      - �����Ώۂ�DWC_Http�\���̂ւ̃|�C���^
//      : data      - �f�[�^(NULL�I�[����Ă��Ȃ���΂Ȃ�Ȃ�)
//      : data_len  - �f�[�^��
// �Ԓl : DWCHttpError
//-----------------------------------------------------------------------------
DWCHttpError DWC_Http_Add_Body(DWCHttp *http, const char *data);

//-----------------------------------------------------------------------------
// �@�\ : DWCHttp���̑��M�p�P�b�g��Content-Length�t�B�[���h��ǉ������M�\��Ԃɂ���
//        
// ���� : http  - �����Ώۂ�DWC_Http�\���̂ւ̃|�C���^
// �Ԓl : DWCHttpError
//-----------------------------------------------------------------------------
DWCHttpError DWC_Http_FinishHeader(DWCHttp *http);

//-----------------------------------------------------------------------------
// �@�\ : ��M�p�P�b�g�̃w�b�_�G���g��/FORM��񕶎�������x���t��������Ƃ���array_entry�Ɋi�[
//        ��M�p�P�b�g�̓��e�̓k�������Ŕj�󂳂��
//        
// ���� : http - �����Ώۂ�DWCHttp�ւ̃|�C���^
//        noparsebody - TRUE�Ȃ��HTTP BODY���p�[�X������httpbody�G���g���Ƃ��ēo�^����
//        buffer      - ��͑Ώۂ�HTTP�p�P�b�g
// �Ԓl : BOOL - TRUE  : ��͐��� 
//               FALSE : ��͕s�\��HTTP
//-----------------------------------------------------------------------------
BOOL DWC_Http_ParseResult(DWCHttp *http, BOOL noparsebody);

//-----------------------------------------------------------------------------
// �@�\ : DWCHttp��TCP�ڑ����v���C�I���e�B��ω�������
// ���� : prio - �v���C�I���e�B(0����31)
// �Ԓl : �Ȃ�
//-----------------------------------------------------------------------------
#ifdef DWC_HTTP_SETSSLPRIO
void DWC_Http_SetSSLPrio(u32 prio);
#endif

//-----------------------------------------------------------------------------
// �p�[�X���ʑ���n�֐��Q
//-----------------------------------------------------------------------------
char	*DWC_Http_GetResult(DWCHttp *http, char *label);
int		DWC_Http_GetBase64DecodedResult(DWCHttp *http, char *label, char *buffer, unsigned long buffer_len);
BOOL	DWC_Http_GetRawResult(DWCHttp *http, char *label, char *buffer, int buffer_len);

#ifdef __cplusplus
}
#endif // [nakata] __cplusplus

#endif // DWC_HTTP_H_
