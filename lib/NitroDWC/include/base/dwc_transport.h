/*---------------------------------------------------------------------------*
  Project:  NitroDWC - base - dwc_transport
  File:     dwc_transport.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_transport.h,v $
  Revision 1.18  2006/01/23 04:50:43  arakit
  ����p��ɍ��킹�ăR�����g���C��

  Revision 1.17  2006/01/17 07:22:35  arakit
  �EDWC_SetUserSendCallback(), DWC_SetUserRecvCallback(),
  �@DWC_SetUserRecvTimeoutCallback(), DWC_SetSendDrop(), DWC_SetRecvDrop(),
  �@DWC_SetSendDelay(), DWC_SetRecvDelay(), DWC_Ping(),
  �@DWC_SetUserPingCallback(), DWC_SetSendSplitMax() �̖߂�l��void����BOOL��
  �@�ύX���A����ϐ��m�ۑO�͉���������FALSE��Ԃ��悤�ɂ����B

  Revision 1.16  2005/09/24 12:56:30  arakit
  �T�[�o�N���C�A���g�^�}�b�`���O���ɁA�}�b�`���O���������z�X�g�݂̂�L����AID�Ƃ���
  �����悤�AAID��R�l�N�V�������ւ̃A�N�Z�X�֐����C�������B

  Revision 1.15  2005/09/24 08:16:26  nakatat
  ���M�\����֐�DWC_IsSendableReliable�ǉ��

  Revision 1.14  2005/09/24 07:17:12  nakatat
  ���M�����ő�T�C�Y�ݒ�֐�DWC_SetSendSplitMax�ǉ��B
  ���M�\����֐�DWCi_SendReliable�ǉ��
  �֘A�����̏C���

  Revision 1.13  2005/09/22 10:45:01  arakit
  �R�l�N�V�����N���[�Y���ɁAsTransInfo��connection���ꕔ�N���A����悤�ɂ����B

  Revision 1.12  2005/09/22 08:56:21  nakatat
  �R�����g�ǉ��

  Revision 1.11  2005/09/22 07:50:45  nakatat
  ��M�^�C���A�E�g�ʒm�@�\��ǉ��B
  �iDWC_SetUserRecvTimeoutCallback�ADWC_SetRecvTimeoutTime�j
  DWCstTransportConnection�ADWCstTransportInfo�Ɋ֘A�����o��ǉ��B

  Revision 1.10  2005/09/13 11:10:56  arakit
  �E���ɃN���[�Y���ꂽaid�ɑ΂��đ��M���悤�Ƃ�����f�[�^�A�N�Z�X��O��
  �@�~�܂��Ă��܂��s����C�������B
  �EAID�r�b�g�}�b�v�擾�֐�   u32 DWC_GetAIDBitmap( void )�A
  �@AID�w��Reliable���M�֐�   u32 DWC_SendReliableBitmap( u32 bitmap, const void* buffer, int size )�A
  �@AID�w��Unreliable���M�֐� u32 DWC_SendUnreliableBitmap( u32 bitmap, const void* buffer, int size )
  �@��p�ӂ����B

  Revision 1.9  2005/09/01 12:15:00  arakit
  �}�b�`���O�������̓�������������ǉ������B

  Revision 1.8  2005/08/30 10:39:45  ito
  �EDWCi_SendReliable��ǉ�
  �E�x���ݒ�̕s����C��

  Revision 1.7  2005/08/26 08:15:23  arakit
  �EASSERT�ƃ��W���[������ϐ��̃`�F�b�N�𐮗������B
  �E�O���֐�DWC_ShutdownFriendsMatch()�̈������Ƃ�Ȃ��悤�ɕύX�����B
  �܂��A���֐�����e���W���[���̐���ϐ���NULL�N���A����悤�ɂ����B
  �E�G���[�����֐���ύX���A�G���[�Z�b�g�A�G���[�`�F�b�N��K�v�ȉӏ��ɓ��ꂽ�B

  Revision 1.6  2005/08/20 07:01:20  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_TRANSPORT_H_
#define DWC_TRANSPORT_H_

#include <darray.h>

#ifdef  __cplusplus
extern "C" {
#endif

//#pragma warn_padding on


#define	DWC_MAGIC_STRINGS					"DT"	// Reliable���M�̃w�b�_�[�ɕt���镶����	   
#define	DWC_MAGIC_STRINGS_LEN				2		// Reliable���M�̃w�b�_�[�ɕt���镶����̒���	   
#define	DWC_TRANSPORT_SEND_MAX				1465	// ��x�ɑ��M�ł���ő�T�C�Y
#define	DWC_TRANSPORT_GT2HEADER_SIZE		7		// gt2���t����w�b�_�̃T�C�Y

/** -----------------------------------------------------------------
  Reliable���M����Ɏg�p
  -------------------------------------------------------------------*/
enum
{
	DWC_TRANSPORT_SEND_READY,						// �f�[�^���M�\�B
	DWC_TRANSPORT_SEND_BUSY,						// �f�[�^���M��

	DWC_TRANSPORT_SEND_LAST			
};

/** -----------------------------------------------------------------
  ��M����Ɏg�p
  -------------------------------------------------------------------*/
enum
{
	DWC_TRANSPORT_RECV_NOBUF,						// ��M�o�b�t�@���ݒ肳��Ă��Ȃ�
	DWC_TRANSPORT_RECV_HEADER,						// �w�b�_�[��M�҂�
	DWC_TRANSPORT_RECV_BODY,						// �f�[�^�{�̑҂�
    DWC_TRANSPORT_RECV_SYSTEM_DATA,					// �����g�p�f�[�^�҂�
	DWC_TRANSPORT_RECV_ERROR,

	DWC_TRANSPORT_RECV_LAST			
};

/** -----------------------------------------------------------------
  ���M�f�[�^�̎��ʎq
  -------------------------------------------------------------------*/
enum
{
	DWC_SEND_TYPE_INVALID,
    DWC_SEND_TYPE_USERDATA,							// ���[�U�[�f�[�^

    // ���V�X�e���f�[�^�i�����g�p�f�[�^�j�p�̎��ʎq���ȉ��ɕ��ׂ�
    DWC_SEND_TYPE_MATCH_SYN,						// �}�b�`���C�N�I�������pSYN
    DWC_SEND_TYPE_MATCH_SYN_ACK,					// �}�b�`���C�N�I�������pSYN-ACK
    DWC_SEND_TYPE_MATCH_ACK,						// �}�b�`���C�N�I�������pACK
    // �������܂�

    DWC_SEND_TYPE_MAX
};

/** -----------------------------------------------------------------
  �\���̂�typedef
  -------------------------------------------------------------------*/
typedef struct DWCstTransportInfo			DWCTransportInfo;
typedef struct DWCstTransportHeader			DWCTransportHeader;
typedef struct DWCstTransportConnection		DWCTransportConnection;
typedef	struct DWCstDelayedMessage			DWCDelayedMessage;

/** -----------------------------------------------------------------
  �֐���typedef
  -------------------------------------------------------------------*/
typedef void	(*DWCUserSendCallback)( int size, u8 aid );
typedef void	(*DWCUserRecvCallback)( u8 aid, u8* buffer, int size );
typedef void	(*DWCUserRecvTimeoutCallback)( u8 aid );
typedef void	(*DWCUserPingCallback)( int latency, u8 aid );

/** -----------------------------------------------------------------
  ����M�𐧌䂷��ׂɎg�p����\����
  -------------------------------------------------------------------*/
// �R�l�N�V�������ɊǗ�������
struct DWCstTransportConnection
{
	const u8*				sendBuffer;				// ���M�o�b�t�@�ւ̃|�C���^	
	u8*						recvBuffer;				// ��M�o�b�t�@�ւ̃|�C���^
	int						recvBufferSize;			// ��M�o�b�t�@�̃T�C�Y

	int						sendingSize;			// ���M���T�C�Y
	int						recvingSize;			// ��M���T�C�Y
    int						requestSendSize;		// ���M�v���T�C�Y
	int						requestRecvSize;		// ��M�v���T�C�Y

    u8						sendState;				// ���M���	
	u8						recvState;				// ��M���

    u8						lastRecvState;			// �w�b�_��M�O�̍ŏI��M���
    u8						pads[3];
    u16						lastRecvType;			// �ŏI��M�w�b�_�^�C�v
    
	OSTick					previousRecvTick;
	u32						recvTimeoutTime;

#ifndef	SDK_FINALROM

    u16						sendDelay;				// ���M���̒x��
    u16						recvDelay;				// ��M���̒x��    

#endif	// SDK_FINALROM
};

// ����M�̐���
struct DWCstTransportInfo
{
	// �R�l�N�V�������ɊǗ�������
    DWCTransportConnection		connections[ DWC_MAX_CONNECTIONS ];

    DWCUserSendCallback			sendCallback;			// ���M�����R�[���o�b�N
	DWCUserRecvCallback			recvCallback;			// ��M�����R�[���o�b�N
	DWCUserRecvTimeoutCallback	recvTimeoutCallback;	// ��M�^�C���A�E�g�R�[���o�b�N
	DWCUserPingCallback			pingCallback;			// Ping�R�[���o�b�N

	u16							sendSplitMax;			// ���M�����ő�T�C�Y
    
#ifndef	SDK_FINALROM

    MATHRandContext16			context;				// ��������
	u32							seed;					// ��������

	DArray						delayedSend;			// ���M�x�����G�~�����[�g����ׂɎg�p����o�b�t�@
	DArray						delayedRecv;			// ��M�x�����G�~�����[�g����ׂɎg�p����o�b�t�@

    u8							sendDrop;				// ���M�p�P�b�g���X��
    u8							recvDrop;				// ��M�p�P�b�g���X��
	u8							pads[2];
    
#endif	// SDK_FINALROM
};	

/** -----------------------------------------------------------------
  Reliable���M�Ɏg���w�b�_
  -------------------------------------------------------------------*/
struct DWCstTransportHeader
{
	int						size;					// ���M�T�C�Y
    u16						type;					// ���M�f�[�^�̃^�C�v
    char					magicStrings[ DWC_MAGIC_STRINGS_LEN ];	// Reliable���M�̃w�b�_�[���ʂׂ̈ɕt���镶����	   
};

/** -----------------------------------------------------------------
  �x���̃G�~�����[�g�Ɏg�p����\����
  -------------------------------------------------------------------*/
struct DWCstDelayedMessage
{
    GT2Connection			connection;
    int						filterID;
    u8*						message;
    int						size;
    BOOL					reliable;
    OSTick					startTime;				// ���M����������
    OSTick					delayTime;	    		// �x�������鎞��
};

/** -----------------------------------------------------------------
  �O���֐�		
  -------------------------------------------------------------------*/
extern BOOL		DWC_SendReliable				( u8 aid, const void* buffer, int size );		// Relible���M
extern u32		DWC_SendReliableBitmap			( u32 bitmap, const void* buffer, int size );	// Bitmap�w��Relible���M
extern BOOL		DWC_SendUnreliable				( u8 aid, const void* buffer, int size );		// Unreliable���M
extern u32		DWC_SendUnreliableBitmap		( u32 bitmap, const void* buffer, int size );	// Bitmap�w��Unreliable���M
extern BOOL		DWC_Ping						( u8 aid );										// Ping�l����
extern BOOL		DWC_SetRecvBuffer				( u8 aid, void* recvBuffer, int size );			// ��M�o�b�t�@��ݒ�
extern BOOL		DWC_SetUserSendCallback			( DWCUserSendCallback callback );				// ���M�R�[���o�b�N
extern BOOL		DWC_SetUserRecvCallback			( DWCUserRecvCallback callback );				// ��M�R�[���o�b�N
extern BOOL		DWC_SetUserRecvTimeoutCallback	( DWCUserRecvTimeoutCallback callback );		// ��M�^�C���A�E�g�R�[���o�b�N
extern BOOL		DWC_SetRecvTimeoutTime			( u8 aid, u32 time );							// ��M�^�C���A�E�g���ԁi�P�ʃ~���b�j��ݒ�
extern BOOL		DWC_SetUserPingCallback			( DWCUserPingCallback callback );				// Ping���芮���R�[���o�b�N
extern BOOL		DWC_SetSendSplitMax				( u16 sendSplitMax );							// ���M�����ő�T�C�Y��ݒ�
extern BOOL		DWC_IsSendableReliable			( u8 aid );										// Reliable���M�\����֐�

#ifndef	SDK_FINALROM

extern BOOL		DWC_SetSendDelay				( u16 delay, u8 aid );							// ���M���ɒx����ݒ�
extern BOOL		DWC_SetRecvDelay				( u16 delay, u8 aid );							// ��M���ɒx����ݒ�
extern BOOL		DWC_SetSendDrop					( u8 drop, u8 aid );							// ���M���p�P�b�g���X����ݒ�
extern BOOL		DWC_SetRecvDrop					( u8 drop, u8 aid );							// ��M���p�P�b�g���X����ݒ�

#else

#define			DWC_SetSendDelay( delay,  aid )	( (void) 0 )
#define			DWC_SetRecvDelay( delay,  aid )	( (void) 0 )
#define			DWC_SetSendDrop( drop, aid )	( (void) 0 )
#define			DWC_SetRecvDrop( drop, aid )	( (void) 0 )

#endif	// SDK_FINALROM

void			DWCi_InitTransport				( DWCTransportInfo* info );
BOOL			DWCi_IsSendableReliable			( u8 aid, u16 type );
BOOL			DWCi_SendReliable				( u16 type, u8 aid, const void* buffer, int size );
void			DWCi_RecvCallback				( GT2Connection	connection, GT2Byte* message, int size, GT2Bool	reliable );
void		    DWCi_PingCallback				( GT2Connection connection, int latency );
void			DWCi_TransportProcess			( void );
void			DWCi_ShutdownTransport			( void );
void			DWCi_ClearTransConnection		( u8 aid );


//#pragma warn_padding reset


#ifdef  __cplusplus
}		/* extern "C" */
#endif


#endif // DWC_TRANSPORT_H_
