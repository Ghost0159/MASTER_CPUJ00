/*--------------------------------------------------------------------------
  Project:  DWCi_Nd
  File:     DWCi_Nd.h

  Copyright 2006 Nintendo.  All rights reserved.

  $Log: DWCi_Nd.h,v $
  Revision 1.14  2007/02/21 04:38:01  takayama
  DlLibs_20070116 �Ɠ����B
  �������A�����ύX�Ȃ��B

  Revision 1.24  2006/06/23 06:54:22  aida
  replaced DWC_ND_COUNTBUFFER_LENGTH by DWC_ND_RECVBUF_MIN
  added CA

  Revision 1.23  2006/06/02 01:05:01  aida
  modified conditional operator

  Revision 1.22  2006/05/17 05:46:09  aida
  modified macro name

  Revision 1.21  2006/05/12 05:47:13  aida
  modified macro name
  modified structure name

  Revision 1.20  2006/05/11 07:57:38  aida
  modified DWC_ND_COUNTBUFFER_LENGTH

  Revision 1.19  2006/05/11 04:40:24  aida
  added DWC_ND_COUNTBUFFER_LENGTH
  added DWC_ND_LISTBUFFER_HEAD
  added DWC_ND_LISTBUFFER_NUM

  Revision 1.18  2006/05/11 04:38:39  aida
  deleted DWC_ND_RECEIVEBUFFER_LENGTH

  Revision 1.17  2006/05/08 10:33:30  aida
  added gamecd, passwd on DWCi_Nd_Startup()
  fixed macro names

  Revision 1.16  2006/05/01 04:34:02  aida
  deleted DWC_ND_USERAGENT

  Revision 1.15  2006/05/01 03:24:49  aida
  modified enum macro names
  modified behaviour of DWCi_NdGetFileAsync()

  Revision 1.14  2006/04/26 02:52:40  aida
  deleted "#include <dwc.h>"

  Revision 1.13  2006/04/25 05:05:55  aida
  deleted global definition #include <dwc_backup.h>

  Revision 1.12  2006/04/25 05:05:02  aida
  changed copyright description

  Revision 1.11  2006/04/24 07:58:53  aida
  changed DWCNdFileInfo::explain[] char to short

  Revision 1.10  2006/04/17 07:09:36  aida
  changed k-option to -kkv

  Revision 1.9  2006/04/17 07:00:20  aida
  fixed log tag

  Revision 1.5  2006/03/28 09:38:51  aida
  no message

  $NoKeywords: $
 *-------------------------------------------------------------------------*/
#ifndef DWC_ND_DWC_ND_H
#define DWC_ND_DWC_ND_H

#if ((defined WIN32) || (defined _WIN32))

#pragma once
#define BOOL int
typedef unsigned long u32;
typedef unsigned long long u64;
#include <stdlib.h>

#else

#ifndef _NITRO
#define _NITRO
#endif
#include <nitro.h>
#include <nitroWiFi.h>

#endif

#ifdef __cplusplus
extern "C" {
#endif


//////////////////////////////////////////////////////////////

// NHTTP���C�u�����ɗ^�������  HTTP�ʐM�X���b�h�̗D��x
#define DWC_ND_NHTTP_THREAD_PRIORITY		10

// ��M�o�b�t�@�̍Œᒷ
#define DWC_ND_RECVBUF_MIN						128		// [byte]	

// �t�@�C�����X�g��M�p�o�b�t�@
// DWCi_NdGetFileListAsync()�Ŏg�p
#define DWC_ND_LISTBUFFER_PERNUM			256  // [byte]

// DWCi_NdStartup()�ɓn�����DL�Q�[���R�[�h(gamecd)�̃o�C�g��
#define DWC_ND_LENGTH_GAMECODE				4

// DWCi_NdStartup()�ɓn�����閧��(passwd)�̃o�C�g��
#define DWC_ND_LENGTH_PASSWORD				16


typedef enum {
	DWC_ND_ERROR_NONE,     // �G���[�Ȃ�
	DWC_ND_ERROR_ALLOC,    // �q�[�v�������m�ۂɎ��s
	DWC_ND_ERROR_BUSY,     // �_�E�����[�h���ɐV�K���N�G�X�g��v������
	DWC_ND_ERROR_HTTP,     // HTTP�ʐM�Ɏ��s
	DWC_ND_ERROR_BUFFULL,  // ��M�o�b�t�@������Ȃ�
	DWC_ND_ERROR_DLSERVER, // �T�[�o���G���[�R�[�h��Ԃ��Ă���
	DWC_ND_ERROR_CANCELED, // �_�E�����[�h���L�����Z�����ꂽ
	DWC_ND_ERROR_PARAM,		 // �s���ȃp�����[�^
	DWC_ND_ERROR_FATAL,		 // ����`�̒v���I�ȃG���[
	DWC_ND_ERROR_MAX
} DWCNdError;

typedef enum {
	DWC_ND_CBREASON_INITIALIZE,			// �����������I�� (DWCi_Nd�ł͎g�p���Ȃ�)
	DWC_ND_CBREASON_GETFILELISTNUM, // �t�@�C�����X�g�G���g�����擾�����I��
	DWC_ND_CBREASON_GETFILELIST,    // �t�@�C�����X�g�擾�����I��
	DWC_ND_CBREASON_GETFILE,        // �t�@�C���擾�����I��
	DWC_ND_CBREASON_MAX
} DWCNdCallbackReason;

#define DWC_ND_FILEATTR_LEN	10

typedef struct {
	char name[32+1];			// �R���e���c��(NULL�I�[ASCII������)
	short explain[50+1];	// ������(UTF16 LE������)
	char param1[DWC_ND_FILEATTR_LEN+1];	// ����1
	char param2[DWC_ND_FILEATTR_LEN+1];	// ����2
	char param3[DWC_ND_FILEATTR_LEN+1];	// ����3
	unsigned int size;	// �t�@�C���T�C�Y
} DWCNdFileInfo;

typedef void* (*DWCNdAlloc)( u32 size, int align );
typedef void (*DWCNdFree)( void* ptr );
typedef void (*DWCNdCallback)( DWCNdCallbackReason reason, DWCNdError errpr, int servererror );
typedef void (*DWCNdCleanupCallback)( void );

BOOL DWCi_NdStartup( DWCNdAlloc alloc, DWCNdFree free, char* url, char* token, char* gamecd, char* passwd, DWCNdCallback callback );
void DWCi_NdCleanupAsync( DWCNdCleanupCallback callback );
DWCNdError DWCi_NdGetError( void );

BOOL DWCi_NdSetAttr( char* attr1, char* attr2, char* attr3 );
BOOL DWCi_NdGetFileListNumAsync( int* entrynum );
BOOL DWCi_NdGetFileListAsync( DWCNdFileInfo* filelist, int offset, int num );
BOOL DWCi_NdGetFileAsync( DWCNdFileInfo* filelist, char* buf, unsigned int bufsize );
BOOL DWCi_NdCancelAsync( void );
BOOL DWCi_NdGetProgress( u32* received, u32* contentlen );


#ifdef __cplusplus
}
#endif

#endif
