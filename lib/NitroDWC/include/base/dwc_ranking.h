/*---------------------------------------------------------------------------*
  Project:  NitroDWC -  - 
  File:     dwc_ranking.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_ranking.h,v $
  Revision 1.19  2007/08/23 23:56:15  takayama
  DWC_2_2_RC1�Ƀ��[���o�b�N�B

  Revision 1.15  2007/06/22 06:41:10  takayama
  ��`�̕ύX�B
  DWC_RNK_CATEGORY_MAX 100 -> 1000
  DWC_RNK_GET_MAX      10  -> 30

  Revision 1.14  2007/05/31 07:05:22  nakata
  DWC_RNK_GET_MODE_NEAR_HI��DWC_RNK_GET_MODE_NEAR_LOW��ǉ�

  Revision 1.13  2007/04/25 11:43:45  hayashi
  adding Korean region support

  Revision 1.12  2007/04/18 01:57:43  hayashi
  userdata size was decreased to 764bytes

  Revision 1.11  2006/05/30 14:08:50  hayashi
  added DWC_RnkResGetTotal

  Revision 1.10  2006/05/22 07:29:58  hayashi
  changed to be set the server by DWC_SetAuthServer()

  Revision 1.9  2006/05/12 08:28:54  hayashi
  NOTINITIALIZED -> NOTREADY
  RIVAL -> NEAR
  DWCAccUserData -> DWCUserData
  BOOL	DWC_RnkShutdown() -> DWCRnkError

  Revision 1.8  2006/04/26 09:25:41  hayashi
  specification is changed related to PID handling

  Revision 1.7  2006/04/14 05:03:50  hayashi
  do indent

  Revision 1.6  2006/04/13 11:35:19  hayashi
  Change prefix Ranking->Rnk

  Revision 1.5  2006/04/13 09:01:38  hayashi
  character code converting

  Revision 1.4  2006/04/11 09:01:33  hayashi
  DWC_RnkInitialize�ŁA�ڑ���̃T�[�o�[��I���ł���悤�ɂ���

  Revision 1.3  2006/04/11 07:04:19  hayashi
  base64�̃G���R�[�h�T�C�Y�̌v�Z���Ԉ���Ă����̂ŏC��

  Revision 1.2  2006/04/10 13:26:11  hayashi
  since�p�����[�^�[�̒ǉ�

  Revision 1.1  2006/04/10 13:06:11  hayashi
  added the ranking module


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_RNK_H_
#define DWC_RNK_H_


#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------
        define
   ------------------------------------------------------------------------- */

// ���[�U�[��`�f�[�^�Ɏg�p�ł���ő�T�C�Y
#define DWC_RNK_DATA_MAX		764

// �J�e�S���[�Ƃ��Ďw��ł���ő�l
#define DWC_RNK_CATEGORY_MAX	1000

// �擾���郉���L���O���X�g�̏����ƍ~���������萔
#define DWC_RNK_ORDER_ASC		0	// ����
#define DWC_RNK_ORDER_DES		1	// �~��

// �擾���郉���L���O���X�g�̍ő吔�̏���l
#define DWC_RNK_GET_MAX         30

// �F�B�̍ő吔
#define DWC_RNK_FRIENDS_MAX		64


/* -------------------------------------------------------------------------
        enum
   ------------------------------------------------------------------------- */

// �G���[�R�[�h
//
typedef enum{

	DWC_RNK_SUCCESS = 0,						// ����
	DWC_RNK_IN_ERROR,							// �G���[������

	DWC_RNK_ERROR_INVALID_PARAMETER,			// �s���ȃp�����[�^�[

	// 
	// DWC_RnkInitialize �̃G���[�l
	// 
	DWC_RNK_ERROR_INIT_ALREADYINITIALIZED,		// ��������
	DWC_RNK_ERROR_INIT_INVALID_INITDATASIZE,	// �s���ȏ������f�[�^�T�C�Y
	DWC_RNK_ERROR_INIT_INVALID_INITDATA,		// �s���ȏ������f�[�^
	DWC_RNK_ERROR_INIT_INVALID_USERDATA,		// ���[�U�[��񂪕s��

	//
	// DWC_RnkPutScoreAsync �̃G���[�l
	//
	DWC_RNK_ERROR_PUT_NOTREADY,					// ����������Ă��Ȃ�
	DWC_RNK_ERROR_PUT_INVALID_KEY,				// �Í����L�[���s��
	DWC_RNK_ERROR_PUT_NOMEMORY,					// �������[�s��
 	
	//
	// DWC_RnkGetScoreAsync �̃G���[�l
	//
	DWC_RNK_ERROR_GET_NOTREADY,					// ����������Ă��Ȃ�
	DWC_RNK_ERROR_GET_INVALID_KEY,				// �Í����L�[���s��
	DWC_RNK_ERROR_GET_NOMEMORY,					// �������[�s��
 
	//
	// DWC_RnkCancelProcess �̃G���[�l
	//
	DWC_RNK_ERROR_CANCEL_NOTASK,				// �񓯊��������̏����Ȃ�

	//
	// DWC_RnkProcess �̃G���[�l
	//
	DWC_RNK_PROCESS_NOTASK,				// �񓯊��������̏����Ȃ�

	//
	// DWC_RnkGetResponse �̃G���[�l
	//
	DWC_RNK_ERROR_INVALID_MODE,					// �s���ȃ��[�h
	DWC_RNK_ERROR_NOTCOMPLETED,					// �ʐM������
	DWC_RNK_ERROR_EMPTY_RESPONSE				// ��̃��X�|���X


} DWCRnkError;


// ���W���[���X�e�[�^�X
//
typedef enum{

	DWC_RNK_STATE_NOTREADY	= 0,	// �����������

	DWC_RNK_STATE_INITIALIZED,		// ����������
	DWC_RNK_STATE_PUT_ASYNC,		// PUT�񓯊�������
	DWC_RNK_STATE_GET_ASYNC,		// GET�񓯊�������
	DWC_RNK_STATE_COMPLETED,		// ����

	DWC_RNK_STATE_ERROR				// �G���[����

} DWCRnkState;


// ���[�W�����R�[�h
//
typedef enum{

	DWC_RNK_REGION_JP	= 0x01,	// Japan
	DWC_RNK_REGION_US	= 0x02,	// United States
	DWC_RNK_REGION_EU	= 0x04,	// Europe
	DWC_RNK_REGION_KR	= 0x08,	// Korea

	DWC_RNK_REGION_ALL	= 0xff	// World

} DWCRnkRegion;


// DWC_RnkGetScoreAsync �擾���[�h
//
typedef enum{

	DWC_RNK_GET_MODE_ORDER,		// ����
	DWC_RNK_GET_MODE_TOPLIST,	// �n�C�X�R�A
	DWC_RNK_GET_MODE_NEAR,		// ���ӃX�R�A
	DWC_RNK_GET_MODE_FRIENDS,	// �F�B�w�胉���L���O
	DWC_RNK_GET_MODE_NEAR_HI,
	DWC_RNK_GET_MODE_NEAR_LOW

} DWCRnkGetMode;


/* -------------------------------------------------------------------------
        typedef - function
   ------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------
        struct
   ------------------------------------------------------------------------- */

// �X�R�A���i�[�p�\����
//
typedef struct{

	u32					order;		// ����
	s32					pid;		// GameSpy Profile ID
	s32					score;		// �X�R�A
	DWCRnkRegion		region;		// ���[�W�����R�[�h
	u32					lastupdate;	// �O��̍X�V����̌o�ߎ���(��)

	u32					size;		// userdata�̃T�C�Y(bytes)
	void*				userdata;	// ���[�U�[��`�f�[�^�̃o�b�t�@

} DWCRnkData;


// DWC_RnkGetScoreAsync�Ŏw�肷��p�����[�^
//
typedef struct{

	u32				size;	// �T�C�Y(bytes)

	union{

		//
		// ���[�h = DWC_RNK_GET_MODE_ORDER
		//
		struct{

			u32	sort;							// DWC_RNK_ORDER_ASC:����
												// DWC_RNK_ORDER_DES:�~��
			u32	since;							// ���b�O���炩

		}			order;

		//
		// ���[�h = DWC_RNK_GET_MODE_TOPLIST
		//
		struct{

			u32	sort;							// DWC_RNK_ORDER_ASC:����
												// DWC_RNK_ORDER_DES:�~��
			u32	limit;							// �擾����ő吔
			u32	since;							// ���b�O���炩

		}			toplist;

		//
		// ���[�h = DWC_RNK_GET_MODE_NEAR
		//
		struct{

			u32	sort;							// DWC_RNK_ORDER_ASC:����
												// DWC_RNK_ORDER_DES:�~��
			u32	limit;							// �擾����ő吔
			u32	since;							// ���b�O���炩

		}			near;

		//
		// ���[�h = DWC_RNK_GET_MODE_FRIENDS
		//
		struct{


			u32	sort;							// DWC_RNK_ORDER_ASC:����
												// DWC_RNK_ORDER_DES:�~��
			u32	limit;							// �擾����ő吔
			u32	since;							// ���b�O���炩
			s32	friends[DWC_RNK_FRIENDS_MAX];	// �F�B��PID���X�g

		}			friends;
	
		u32			data;

	};

} DWCRnkGetParam;


/* -------------------------------------------------------------------------
        function - external
   ------------------------------------------------------------------------- */

extern DWCRnkError	DWC_RnkInitialize(		const char* initdata,
											const DWCUserData* userdata);

extern DWCRnkError	DWC_RnkShutdown();

extern DWCRnkError	DWC_RnkPutScoreAsync(	u32 category,
											DWCRnkRegion region,
											s32 score,
											void* data,
											u32 size );

extern DWCRnkError	DWC_RnkGetScoreAsync(	DWCRnkGetMode mode,
											u32 category,
											DWCRnkRegion region,
											DWCRnkGetParam* param );

extern DWCRnkError	DWC_RnkCancelProcess();

extern DWCRnkError	DWC_RnkProcess();

extern DWCRnkState	DWC_RnkGetState();

//
// ���X�|���X�擾�p
//
extern DWCRnkError	DWC_RnkResGetRow(		DWCRnkData* out,
											u32 index );

extern DWCRnkError	DWC_RnkResGetRowCount(	u32* count );

extern DWCRnkError	DWC_RnkResGetOrder(		u32* order );

extern DWCRnkError	DWC_RnkResGetTotal(		u32* total );


#ifdef __cplusplus
}
#endif

#endif // DWC_RNK_H_
