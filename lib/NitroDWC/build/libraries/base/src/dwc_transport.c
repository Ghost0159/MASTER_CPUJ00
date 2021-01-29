#include <nitro.h>
#include <base/dwc_base_gamespy.h>

#include <base/dwc_report.h>
#include <base/dwc_account.h>
#include <base/dwc_error.h>
#include <base/dwc_login.h>
#include <base/dwc_friend.h>
#include <base/dwc_match.h>
#include <base/dwc_transport.h>
#include <base/dwc_main.h>


/** -----------------------------------------------------------------
  �����ϐ�
  -------------------------------------------------------------------*/
static DWCTransportInfo*			sTransInfo = NULL;

/** -----------------------------------------------------------------
  �����֐�
  -------------------------------------------------------------------*/
static DWCTransportConnection*	DWCs_GetTransConnection	( u8 aid );

static int			DWCs_GetSendState					( u8 aid );
static int			DWCs_GetRecvState					( u8 aid );
static void			DWCs_Send							( u8 aid, const u8* buffer, int size, BOOL reliable );
static void			DWCs_HandleUnreliableMessage		( GT2Connection	connection, void* message, int size );
static void			DWCs_HandleReliableMessage			( GT2Connection connection, u8* message, int size );
static void			DWCs_RecvDataHeader					( u8 aid, DWCTransportHeader* message, int size );
static void			DWCs_RecvDataBody					( u8 aid, void* message, int size );
static void			DWCs_RecvSystemDataBody				( u8 aid, void*	message, int size );
static void			DWCs_DelayCheck						( DArray delayedMessages, OSTick now, BOOL send );
static void			DWCs_AddDelayMessage				( DArray array, u16 delayValue, GT2Connection connection, int filterID, const u8* message, int size, BOOL reliable );
static void			DWCs_DelayedMessageFree				( void* elem );
static int			DWCs_GetOutgoingBufferFreeSize		( u8 aid );
static void			DWCs_SendFilterCallbackDrop			( GT2Connection connection, int filterID, const u8* message, int size, BOOL reliable );
static void			DWCs_SendFilterCallbackDelay		( GT2Connection	connection, int	filterID, const u8* message, int size, BOOL reliable );
static void			DWCs_RecvFilterCallbackDrop			( GT2Connection connection, int	filterID, u8* message, int size, BOOL reliable );
static void			DWCs_RecvFilterCallbackDelay		( GT2Connection connection, int filterID, u8* message, int size, BOOL reliable );
static void			DWCs_EncodeHeader					( DWCTransportHeader* header, u16 type, int size );
static u16			DWCs_DecodeHeader					( const DWCTransportHeader* header );
static s32			DWCs_GetRequiredHeaderSize			( u16 type );


/** -----------------------------------------------------------------
  ���[�U�[�Ɍ��J����Reliable���M�\����֐�
  -------------------------------------------------------------------*/
BOOL
DWC_IsSendableReliable( u8 aid )
{
    return DWCi_IsSendableReliable( aid, DWC_SEND_TYPE_USERDATA );
}

/** -----------------------------------------------------------------
  ���C�u�����݂̂Ŏg�p����Reliable���M�\����֐�
  -------------------------------------------------------------------*/
BOOL
DWCi_IsSendableReliable( u8 aid, u16 type )
{
	s32 freeSpace;

    if ( DWCi_IsError() ||
         ( ( type == DWC_SEND_TYPE_USERDATA ) && ! DWC_IsValidAID( aid ) ) ||
         ! DWCi_IsValidAID( aid ) )
    {
        // ���ɃN���[�Y���ꂽ�R�l�N�V�����ɑ��M���悤�Ƃ����ꍇ
        // ������AID�ɑ��M���悤�Ƃ����ꍇ�������ɗ���
		DWC_Printf( DWC_REPORTFLAG_WARNING, "aid %d is unavailable.\n", aid );
		return FALSE;
	}
    
	// ���M���̃f�[�^������Ȃ�FALSE��Ԃ��ďI������
	if ( DWCs_GetSendState( aid ) == DWC_TRANSPORT_SEND_BUSY )
	{
		DWC_Printf( DWC_REPORTFLAG_SEND_INFO, "+++ Cannot send to %d from %d (busy)\n", aid, DWC_GetMyAID() );
		return FALSE;
	}

	// �w�b�_�[�𑗐M����ׂ�OutgoingBuffer�̋󂫂����邩�m�F
	freeSpace = DWCs_GetOutgoingBufferFreeSize( aid );

	if ( freeSpace < DWCs_GetRequiredHeaderSize( type ) )
	{
		DWC_Printf( DWC_REPORTFLAG_SEND_INFO, "+++ Cannot send to %d from %d (outgoing buffer is not enough) %d < %d\n", aid, DWC_GetMyAID(), freeSpace, DWCs_GetRequiredHeaderSize( type ) );
		return FALSE;
	}

	return TRUE;
}

/** -----------------------------------------------------------------
  ���[�U�[�Ɍ��J����Reliable���M
  -------------------------------------------------------------------*/
BOOL
DWC_SendReliable(
    u8						aid,
	const void*				buffer,
	int						size )
{
    return DWCi_SendReliable( DWC_SEND_TYPE_USERDATA, aid, buffer, size );
}

/** -----------------------------------------------------------------
  ���C�u�����݂̂Ŏg�p����Reliable���M
  -------------------------------------------------------------------*/
BOOL
DWCi_SendReliable(
	u16						type,
    u8						aid,
	const void*				buffer,
	int						size )
{
	DWCTransportConnection*	transConnection = DWCs_GetTransConnection( aid );
	DWCTransportHeader		header;
	int						sendSize;
	int						freeSpace;

    SDK_ASSERT( buffer );
    SDK_ASSERT( size > 0 );

    //if ( DWCi_IsError() ) return FALSE;

    if ( ! DWCi_IsSendableReliable( aid, type ) )
    {
        return FALSE;
    }

    // ���M��Ԃ̃p�����[�^��������
    transConnection->sendState			= DWC_TRANSPORT_SEND_BUSY; 
	transConnection->sendBuffer			= buffer;				
	transConnection->sendingSize		= 0;					
	transConnection->requestSendSize	= size;					

    // ���M�f�[�^�S�̂̃T�C�Y�𑗐M����
	DWCs_EncodeHeader( &header, type, size );
    DWCs_Send( aid, (const u8*) &header, sizeof( DWCTransportHeader ), TRUE ); 

    // ���M�T�C�Y�����߂�
	if( size > sTransInfo->sendSplitMax ){
		sendSize = sTransInfo->sendSplitMax;
	}else{
		sendSize = size;
	}

    // �f�[�^�{�̂𑗐M����ׂ�OutgoingBuffer�̋󂫂����邩���ׂ�
	freeSpace = DWCs_GetOutgoingBufferFreeSize( aid );
#if 1
	DWC_Printf(
		DWC_REPORTFLAG_SEND_INFO,
		"DWCi_SendReliable:aid = %d,%d,%d,%d,%d,vcount = %d\n",
		aid,sendSize,freeSpace,size,sTransInfo->sendSplitMax,OS_GetVBlankCount()
	);
#endif
	if ( sendSize > freeSpace )
    {
		// OutgoingBuffer�̋󂫂��Ȃ��I
        // DWCi_TransportProcess�ő��M����
		DWC_Printf(
			DWC_REPORTFLAG_SEND_INFO,
			"DWCi_SendReliable:sendSize > freeSpace:aid = %d,%d,%d,vcount = %d\n",
			aid,sendSize,freeSpace,OS_GetVBlankCount()
		);
        return TRUE;
    }

    // �f�[�^�{�̂𑗐M����
	DWCs_Send( aid, buffer, sendSize, TRUE ); 
    
    // ���M�ς݃T�C�Y���X�V
    transConnection->sendingSize += sendSize;

	DWC_Printf(
		DWC_REPORTFLAG_SEND_INFO,
		"[R] aid = %d, size = %d/%d, outgoing buffer = %d, vcount = %d\n",
		aid, transConnection->sendingSize, transConnection->requestSendSize, freeSpace - sendSize, OS_GetVBlankCount()
	);

    // ���M�ς݃T�C�Y�Ƒ��M�\��T�C�Y����v�����犮��
	if ( transConnection->sendingSize == transConnection->requestSendSize )
	{
        // [arakit] main 051026
        int reqSendSize = transConnection->requestSendSize;
        
        // ���M��Ԃ̃p�����[�^�����Z�b�g
		transConnection->sendState			= DWC_TRANSPORT_SEND_READY;
		transConnection->sendBuffer			= NULL;
        transConnection->sendingSize		= 0;	
		transConnection->requestSendSize	= 0;
        // [arakit] main 051026
        
		// ���M�����R�[���o�b�N�i���[�U�f�[�^���M���̂݁j
		if ( sTransInfo->sendCallback && type == DWC_SEND_TYPE_USERDATA )
		{
            // [arakit] main 051026
            sTransInfo->sendCallback( reqSendSize, aid );
		}
	}

    return TRUE;
}

/** -----------------------------------------------------------------
  Bitmap�w��Reliable���M
  -------------------------------------------------------------------*/
u32
DWC_SendReliableBitmap(
    u32						bitmap,
	const void*				buffer,
	int						size )
{
    u8	aid;
    u32 aidbit;

    for ( aid = 0; aid < DWC_MAX_CONNECTIONS; ++aid )
    {
        aidbit = aid ? ( 1 << aid ) : 1U;
        if ( ( bitmap & aidbit ) && ( aid != DWC_GetMyAID() ) )
        {
            // Reliable���M���s���A���s�����玸�s����aid��bit��������
            if ( ! DWC_SendReliable( aid, buffer, size ) )
                bitmap &= ~aidbit;
        }
        // ������AID�̃r�b�g�����Ăēn���ꂽ�ꍇ�́A������������ɕԂ�
    }

    return bitmap;
}

/** -----------------------------------------------------------------
  Unreliable���M
  -------------------------------------------------------------------*/
BOOL
DWC_SendUnreliable(
	u8						aid,
	const void*				buffer,
	int						size )
{
    SDK_ASSERT( buffer );
    SDK_ASSERT( size > 0 );

    if ( DWCi_IsError() ) return FALSE;

    if ( ! DWC_IsValidAID( aid ) )
    {
        // ���ɃN���[�Y���ꂽ�R�l�N�V�����ɑ��M���悤�Ƃ����ꍇ
        DWC_Printf( DWC_REPORTFLAG_WARNING, "aid %d is now unavailable.\n", aid );
        return FALSE;
    }

    if ( size > sTransInfo->sendSplitMax )
    {
        DWC_Printf( DWC_REPORTFLAG_SEND_INFO, "+++ SendUnreliable size is too large ( %d > %d )\n", size, sTransInfo->sendSplitMax );
		return FALSE;
    }
    
	DWCs_Send( aid, buffer, size, FALSE );

	DWC_Printf( DWC_REPORTFLAG_SEND_INFO, "[U] aid = %d, size = %d, vcount = %d\n", aid, size, OS_GetVBlankCount() );    
    
	// ���M�����R�[���o�b�N
	if ( sTransInfo->sendCallback )
	{
		sTransInfo->sendCallback( size, aid );
	}

    return TRUE;
}

/** -----------------------------------------------------------------
  Bitmap�w��Unreliable���M
  -------------------------------------------------------------------*/
u32
DWC_SendUnreliableBitmap(
    u32						bitmap,
	const void*				buffer,
	int						size )
{
    u8	aid;
    u32 aidbit;

    for ( aid = 0; aid < DWC_MAX_CONNECTIONS; ++aid )
    {
        aidbit = aid ? ( 1 << aid ) : 1U;
        if ( bitmap & aidbit && ( aid != DWC_GetMyAID() ) )
        {
            // Unreliable���M���s���A���s�����玸�s����aid��bit��������
            if ( ! DWC_SendUnreliable( aid, buffer, size ) )
                bitmap &= ~aidbit;
        }
        // ������AID�̃r�b�g�����Ăēn���ꂽ�ꍇ�́A������������ɕԂ�
    }

    return bitmap;
}

/** -----------------------------------------------------------------
  ��M�o�b�t�@��ݒ�
  -------------------------------------------------------------------*/
BOOL
DWC_SetRecvBuffer(
	u8						aid,
	void*					recvBuffer,
	int						size )
{
	DWCTransportConnection*	connection = DWCs_GetTransConnection( aid );

    SDK_ASSERT( recvBuffer );
    SDK_ASSERT( size > 0 );    

    // �f�[�^����M���Ȃ�FALSE��Ԃ��ďI��
    if ( DWCs_GetRecvState( aid ) == DWC_TRANSPORT_RECV_BODY )
    {
	    DWC_Printf( DWC_REPORTFLAG_RECV_INFO, "+++ Cannot set recv buffer\n" );

        return FALSE;
    }
         
	// ��M�o�b�t�@��ݒ�
	connection->recvBuffer		= recvBuffer;
	connection->recvBufferSize	= size;	

    // ��M��������������
	connection->recvState		= DWC_TRANSPORT_RECV_HEADER;    
    connection->recvingSize		= 0;
	connection->requestRecvSize	= 0;

	return TRUE;
}

/** -----------------------------------------------------------------
  Ping
  -------------------------------------------------------------------*/
BOOL
DWC_Ping( u8 aid )
{
    GT2Connection connection;
    
    if ( DWCi_IsError() ) return FALSE;
    
    connection = DWCi_GetGT2Connection( aid );

    if ( ( aid == DWC_GetMyAID() ) || ( ! connection ) || ( gt2GetConnectionState( connection ) != GT2Connected ) )
    {
		DWC_Printf( DWC_REPORTFLAG_SEND_INFO, "DWC_Ping:not connected yet:%d\n",aid);
		return FALSE;
    }

//    SDK_ASSERT( connection );

    gt2Ping( connection );

    return TRUE;
}

/** -----------------------------------------------------------------
  ���M�R�[���o�b�N��ݒ�
  -------------------------------------------------------------------*/
BOOL
DWC_SetUserSendCallback( DWCUserSendCallback callback )
{
    if ( sTransInfo == NULL ) return FALSE;
    
	sTransInfo->sendCallback = callback;

    return TRUE;
}

/** -----------------------------------------------------------------
  ��M�R�[���o�b�N��ݒ�
  -------------------------------------------------------------------*/
BOOL
DWC_SetUserRecvCallback( DWCUserRecvCallback callback )
{
    if ( sTransInfo == NULL ) return FALSE;
    
	sTransInfo->recvCallback = callback;

    return TRUE;
}

/** -----------------------------------------------------------------
  ��M�^�C���A�E�g�R�[���o�b�N��ݒ�
  -------------------------------------------------------------------*/
BOOL
DWC_SetUserRecvTimeoutCallback( DWCUserRecvTimeoutCallback callback )
{
    if ( sTransInfo == NULL ) return FALSE;
    
	sTransInfo->recvTimeoutCallback = callback;

    return TRUE;
}

/** -----------------------------------------------------------------
  Ping�R�[���o�b�N��ݒ�
  -------------------------------------------------------------------*/
BOOL
DWC_SetUserPingCallback( DWCUserPingCallback callback )
{
    if ( sTransInfo == NULL ) return FALSE;
    
	sTransInfo->pingCallback = callback;

    return TRUE;
}

/** -----------------------------------------------------------------
  ���M�����ő�T�C�Y��ݒ�
  -------------------------------------------------------------------*/
BOOL
DWC_SetSendSplitMax( u16 sendSplitMax )
{
    if ( sTransInfo == NULL ) return FALSE;
#if 1 // 2006/02/21
	SDK_ASSERT(sendSplitMax <= DWC_TRANSPORT_SEND_MAX);
#else    
	if( sendSplitMax > DWC_TRANSPORT_SEND_MAX ){
		DWC_Printf( DWC_REPORTFLAG_SEND_INFO, "DWC_SetSendSplitMax:sendSplitMax > DWC_TRANSPORT_SEND_MAX:%d,%d\n",sendSplitMax,DWC_TRANSPORT_SEND_MAX);
		sendSplitMax = DWC_TRANSPORT_SEND_MAX;
	}
#endif
	sTransInfo->sendSplitMax = sendSplitMax;

    return TRUE;
}

/** -----------------------------------------------------------------
  Reliable���M�̃w�b�_�[���쐬����
  -------------------------------------------------------------------*/
static void
DWCs_EncodeHeader(
    DWCTransportHeader*	header,
	u16					type,
    int					size )
{
    // Reliable���M�̃w�b�_�[�ł��邱�Ƃ����ʂ���ׂɕt���镶����
    strncpy( header->magicStrings, DWC_MAGIC_STRINGS, DWC_MAGIC_STRINGS_LEN );

    header->type = type;
	header->size = size;
}

/** -----------------------------------------------------------------
  Reliable���M�̃w�b�_�[����͂���
  -------------------------------------------------------------------*/
static u16
DWCs_DecodeHeader( const DWCTransportHeader* message )
{
    DWCTransportHeader			header;

    // �A���C�����g���ꂽ�o�b�t�@�ɃR�s�[����
    MI_CpuCopy8( message, (u8*) &header, sizeof( DWCTransportHeader ) );

    if ( ( memcmp( header.magicStrings, DWC_MAGIC_STRINGS, DWC_MAGIC_STRINGS_LEN ) == 0 ) )
    {
        return header.type;
    }

    return DWC_SEND_TYPE_INVALID;
}

/** -----------------------------------------------------------------
  Reliable���M�̃w�b�_�[���M�ɕK�v�ȗe�ʂ��擾
  -------------------------------------------------------------------*/
static s32
DWCs_GetRequiredHeaderSize( u16 type )
{
    s32 size;

    switch ( type )
    {
    case DWC_SEND_TYPE_MATCH_SYN:
    case DWC_SEND_TYPE_MATCH_SYN_ACK:
    case DWC_SEND_TYPE_MATCH_ACK:
        // �}�b�`���C�N���������f�[�^���M���́A�󂫂�����Ȃ���TransportProcess()
        // �ł̑��M�ɉ񂳂��ƍ���̂ŁA�f�[�^�{�̕����܂߂��󂫂��K�v
        size = sizeof( DWCTransportHeader ) + DWC_MATCH_SYN_DATA_BODY_SIZE;
        break;
    default:
        // �ʏ�̓w�b�_�T�C�Y���̋󂫂�����Ηǂ�
        size = sizeof( DWCTransportHeader );
        break;
    }

    return size;
}

/** -----------------------------------------------------------------
  ��M�^�C���A�E�g���ԁi�P�ʃ~���b�j��ݒ�
  -------------------------------------------------------------------*/
BOOL
DWC_SetRecvTimeoutTime( u8 aid, u32 time )
{
	// GT2Connected�ɂȂ�O�ɐݒ�ł���悤�ɂ���
#if 0
    GT2Connection connection = DWCi_GetGT2Connection( aid );

    if ( ( aid == DWC_GetMyAID() ) || ( ! connection ) || ( gt2GetConnectionState( connection ) != GT2Connected ) )
    {
		return;
    }
#else
    if ( !sTransInfo ) return FALSE;
#endif

	sTransInfo->connections[aid].recvTimeoutTime = time;
	sTransInfo->connections[aid].previousRecvTick = OS_GetTick();

    return TRUE;
}

#ifndef SDK_FINALROM

/** -----------------------------------------------------------------
  ���M���ɒx�������鎞�ԁi�P��: ms�j��ݒ肷��
  -------------------------------------------------------------------*/
BOOL
DWC_SetSendDelay( u16 delay, u8 aid )
{
    GT2Connection connection;
    
    if ( sTransInfo == NULL ) return FALSE;
    
    connection = DWCi_GetGT2Connection( aid );

    if ( ( aid == DWC_GetMyAID() ) || ( ! connection ) || ( gt2GetConnectionState( connection ) != GT2Connected ) )
    {
		return FALSE;
    }
    
	if ( delay )
    {
		// �R�[���o�b�N��ݒ肷��
        // ���ɒx���̒l���ݒ肳��Ă���Ȃ�A�R�[���o�b�N���ݒ肳��Ă���͂�
        if ( ! sTransInfo->connections[aid].sendDelay )
        {
	        if ( ! gt2AddSendFilter( connection, DWCs_SendFilterCallbackDelay ) )
            {
                DWCi_SetError( DWC_ERROR_FATAL, DWC_ECODE_SEQ_ETC + DWC_ECODE_GS_GT2 + DWC_ECODE_TYPE_ALLOC );
                return FALSE;
            }
        }
    }
    else
    {
		gt2RemoveSendFilter( connection, DWCs_SendFilterCallbackDelay );
    }

	sTransInfo->connections[aid].sendDelay = delay;

    return TRUE;
}

/** -----------------------------------------------------------------
  ��M���ɒx�������鎞�ԁi�P��: ms�j��ݒ肷��
  -------------------------------------------------------------------*/
BOOL
DWC_SetRecvDelay( u16 delay, u8 aid )
{
    GT2Connection connection;
    
    if ( sTransInfo == NULL ) return FALSE;
    
    connection = DWCi_GetGT2Connection( aid );

    if ( ( aid == DWC_GetMyAID() ) || ( ! connection ) || ( gt2GetConnectionState( connection ) != GT2Connected ) )
    {
		return FALSE;
    }
    
    if ( delay )
    {
		// �R�[���o�b�N��ݒ肷��
        // ���ɒx���̒l���ݒ肳��Ă���Ȃ�A���ɃR�[���o�b�N���ݒ肳��Ă���͂�
        if ( ! sTransInfo->connections[aid].recvDelay )
        {
	        if ( ! gt2AddReceiveFilter( connection, DWCs_RecvFilterCallbackDelay ) )
            {
                DWCi_SetError( DWC_ERROR_FATAL, DWC_ECODE_SEQ_ETC + DWC_ECODE_GS_GT2 + DWC_ECODE_TYPE_ALLOC );
                return FALSE;
            }
        }
    }
    else
    {
		gt2RemoveReceiveFilter( connection, DWCs_RecvFilterCallbackDelay );
    }

    sTransInfo->connections[aid].recvDelay = delay;

    return TRUE;
}

/** -----------------------------------------------------------------
  ���M���̃p�P�b�g���X����ݒ�
  -------------------------------------------------------------------*/
BOOL
DWC_SetSendDrop( u8 drop, u8 aid )
{
    GT2Connection connection;
    
    if ( sTransInfo == NULL ) return FALSE;
    
    connection = DWCi_GetGT2Connection( aid );

    if ( ( aid == DWC_GetMyAID() ) || ( ! connection ) || ( gt2GetConnectionState( connection ) != GT2Connected ) )
    {
		return FALSE;
    }
    
    sTransInfo->sendDrop = drop;	

	if ( drop )
    {
		if ( ! gt2AddSendFilter( connection, DWCs_SendFilterCallbackDrop ) )
        {
            DWCi_SetError( DWC_ERROR_FATAL, DWC_ECODE_SEQ_ETC + DWC_ECODE_GS_GT2 + DWC_ECODE_TYPE_ALLOC );
            return FALSE;
        }
    }
    else
    {
		gt2RemoveSendFilter( connection, DWCs_SendFilterCallbackDrop );
    }

    return TRUE;
}

/** -----------------------------------------------------------------
  ��M���̃p�P�b�g���X����ݒ�
  -------------------------------------------------------------------*/
BOOL
DWC_SetRecvDrop( u8 drop, u8 aid )
{
    GT2Connection connection;
    
    if ( sTransInfo == NULL ) return FALSE;
    
    connection = DWCi_GetGT2Connection( aid );

    if ( ( aid == DWC_GetMyAID() ) || ( ! connection ) || ( gt2GetConnectionState( connection ) != GT2Connected ) )
    {
		return FALSE;
    }
    
    sTransInfo->recvDrop = drop;	

	if ( drop )
    {
		if ( ! gt2AddReceiveFilter( connection, DWCs_RecvFilterCallbackDrop ) )
        {
            DWCi_SetError( DWC_ERROR_FATAL, DWC_ECODE_SEQ_ETC + DWC_ECODE_GS_GT2 + DWC_ECODE_TYPE_ALLOC );
            return FALSE;
        }
    }
    else
    {
		gt2RemoveReceiveFilter( connection, DWCs_RecvFilterCallbackDrop );
    }

    return TRUE;
}

#endif	// SDK_FINALROM

/** -----------------------------------------------------------------
  ������
  -------------------------------------------------------------------*/
void
DWCi_InitTransport( DWCTransportInfo* info )
{
	SDK_ASSERT( info );

    sTransInfo = info;

	MI_CpuClear8( sTransInfo, sizeof( DWCTransportInfo ) );
	sTransInfo->sendSplitMax = DWC_TRANSPORT_SEND_MAX;

#ifndef	SDK_FINALROM
	// �x���ƃp�P�b�g���X�̃G�~�����[�g�ɕK�v�ȏ�����
    
    // �����̏�����
	MATH_InitRand16( &sTransInfo->context, sTransInfo->seed );

    // �x�������Ɏg�p����z����m��
	sTransInfo->delayedSend = ArrayNew( sizeof( DWCDelayedMessage ), 10, DWCs_DelayedMessageFree );
	sTransInfo->delayedRecv = ArrayNew( sizeof( DWCDelayedMessage ), 10, DWCs_DelayedMessageFree );

#endif	// SDK_FINALROM
}

/** -----------------------------------------------------------------
  ��M�R�[���o�b�N
  -------------------------------------------------------------------*/
void
DWCi_RecvCallback(
	GT2Connection			connection,
	GT2Byte*				message,
	int						size,
	GT2Bool					reliable )
{
    if ( ! sTransInfo )
    {
		return;
    }

    DWC_Printf( DWC_REPORTFLAG_RECV_INFO, "%s aid = %d, size = %d, vcount = %d\n", reliable ? "[R]" : "[U]", DWCi_GetConnectionAID( connection ), size, OS_GetVBlankCount() );

	if( message == NULL || size == 0 )
    {
        DWC_Printf( DWC_REPORTFLAG_WARNING, "Recv NULL message %x, size = %d\n", message, size );
		return;
	}

    if ( reliable )
	{
        DWCs_HandleReliableMessage( connection, message, size );
	}
	else
	{
		DWCs_HandleUnreliableMessage( connection, message, size );
	}
}

/** -----------------------------------------------------------------
  Ping�R�[���o�b�N
  -------------------------------------------------------------------*/
void
DWCi_PingCallback
(
	GT2Connection			connection,
    int						latency )
{
	if ( sTransInfo->pingCallback )
    {
		u8 aid = DWCi_GetConnectionAID( connection );

        sTransInfo->pingCallback( latency, aid );
    }
}

/** -----------------------------------------------------------------
  ��x�ɑ��M�ł��Ȃ������f�[�^������ꍇ�́A�����ő��M����
  -------------------------------------------------------------------*/
void
DWCi_TransportProcess( void )
{
	u8* aidList;
	s32 hostCount;
	s32 i;

	if ( ! sTransInfo )
    {
		return;
    }

	hostCount = DWC_GetAIDList( &aidList );

    for ( i = 0; i < hostCount ; i++ )
    {
		u8 aid;

		aid = aidList[i];

		// ��M�^�C���A�E�g���Ԃ̃`�F�b�N
		if( DWC_IsValidAID( aid ) )
		{
		    DWCTransportConnection*	transConnection;
			transConnection = DWCs_GetTransConnection( aid );

			if ( sTransInfo->recvTimeoutCallback && (transConnection->recvTimeoutTime > 0) )
			{
				u32 time;
				OSTick currentTick;

				currentTick = OS_GetTick();
				time = (u32)OS_TicksToMilliSeconds( currentTick - transConnection->previousRecvTick );
				if ( time > transConnection->recvTimeoutTime )
				{
					DWC_Printf( DWC_REPORTFLAG_RECV_INFO, "DWCi_TransportProcess:timeout aid=%d,time=%d[ms],timeout time=%d[ms]\n", aid, time, transConnection->recvTimeoutTime );
					sTransInfo->recvTimeoutCallback( aid );

					//���Ԃ����Z�b�g����
					transConnection->previousRecvTick = currentTick;
				}
			}
		}

        // ���M���̃f�[�^������
		if( aid != DWC_GetMyAID() && DWCs_GetSendState( aid ) == DWC_TRANSPORT_SEND_BUSY )
		{
			s32 restSize;
			s32 sendSize;
			s32 freeSpace;
		    DWCTransportConnection*	transConnection;

            transConnection = DWCs_GetTransConnection( aid );

			// �����M�T�C�Y�����߂�            
            restSize = transConnection->requestSendSize - transConnection->sendingSize; 

			// ���M�T�C�Y�����߂�
			if( restSize > sTransInfo->sendSplitMax ){
				sendSize = sTransInfo->sendSplitMax;
			}else{
				sendSize = restSize;
			}

            // OutgoingBuffer�̋󂫃T�C�Y���m�F����
			freeSpace = DWCs_GetOutgoingBufferFreeSize( aid );
			if ( freeSpace < sendSize )
            {
				DWC_Printf( DWC_REPORTFLAG_SEND_INFO, "DWCi_TransportProcess:freeSpace < sendSize:aid:%d, %d < %d\n", aid, freeSpace, sendSize );
                continue;
            }

            DWCs_Send( aid, (u8*) transConnection->sendBuffer + transConnection->sendingSize, sendSize, TRUE );

		    // ���M�ς݃T�C�Y���X�V            
			transConnection->sendingSize += sendSize;

			DWC_Printf(
				DWC_REPORTFLAG_SEND_INFO,
				"DWCi_TransportProcess:aid = %d, size = %d/%d, outgoing buffer = %d, vcont = %d\n",
				aid, transConnection->sendingSize, transConnection->requestSendSize, freeSpace - sendSize, OS_GetVBlankCount()
			);

			if ( transConnection->sendingSize == transConnection->requestSendSize )
			{
                // [arakit] main 051026
                int reqSendSize = transConnection->requestSendSize;
                
                transConnection->sendState			= DWC_TRANSPORT_SEND_READY;
				transConnection->sendBuffer			= NULL;
                transConnection->sendingSize		= 0;	
				transConnection->requestSendSize	= 0;
                // [arakit] main 051026
                
				// ���M�����R�[���o�b�N
				if ( sTransInfo->sendCallback )
				{
                    // [arakit] main 051026
                    sTransInfo->sendCallback( reqSendSize, aid );
				}
			}			
        }

#ifndef	SDK_FINALROM
//nakatat
#if 0
		{
			OSTick now = OS_GetTick();
			// �x���������`�F�b�N����
			if ( sTransInfo->connections[aid].sendDelay )        
			{
				DWCs_DelayCheck( sTransInfo->delayedSend, now, TRUE );
			}

			if ( sTransInfo->connections[aid].recvDelay )        
			{
				DWCs_DelayCheck( sTransInfo->delayedRecv, now, FALSE );
			}
		}
#endif
#endif // SDK_FINALROM
    }

#ifndef	SDK_FINALROM
//nakatat
	{
		OSTick now = OS_GetTick();
		DWCs_DelayCheck( sTransInfo->delayedSend, now, TRUE );
		DWCs_DelayCheck( sTransInfo->delayedRecv, now, FALSE );
	}
#endif // SDK_FINALROM
}

/** -----------------------------------------------------------------
  �R�l�N�V�����Ǘ������N���A
  -------------------------------------------------------------------*/
void
DWCi_ClearTransConnection( u8 aid )
{

    if ( !sTransInfo ) return ;

#if 1  // �ЂƂ܂��o�b�t�@�̓N���A���Ȃ�
    sTransInfo->connections[aid].sendingSize     = 0;
	sTransInfo->connections[aid].recvingSize     = 0;
    sTransInfo->connections[aid].requestSendSize = 0;
    sTransInfo->connections[aid].requestRecvSize = 0;
    sTransInfo->connections[aid].sendState       = 0;
	//sTransInfo->connections[aid].recvState       = 0;
    if ( sTransInfo->connections[aid].recvState != DWC_TRANSPORT_RECV_NOBUF )
    {
        sTransInfo->connections[aid].recvState = DWC_TRANSPORT_RECV_HEADER;
    }
    sTransInfo->connections[aid].lastRecvType    = 0;
#else
    MI_CpuClear8( &sTransInfo->connections[aid], sizeof( DWCTransportConnection ) );
#endif
}

/** -----------------------------------------------------------------
  �g�����X�|�[�g����I�u�W�F�N�g���N���A
  -------------------------------------------------------------------*/
void
DWCi_ShutdownTransport( void )
{
#ifndef	SDK_FINALROM
    // �x�������Ɏg�p����z����J���B
	ArrayFree( sTransInfo->delayedSend );
	ArrayFree( sTransInfo->delayedRecv );
#endif	// SDK_FINALROM

    sTransInfo = NULL;
}

/** -----------------------------------------------------------------
  Aid����DWCTransportConnection���擾
  -------------------------------------------------------------------*/
static DWCTransportConnection*
DWCs_GetTransConnection( u8 aid )
{
	SDK_ASSERT( ( aid >= 0 ) && ( aid < DWC_MAX_CONNECTIONS ) );
    
    return &sTransInfo->connections[ aid ];
}

/** -----------------------------------------------------------------
  ���M��Ԃ��擾
  -------------------------------------------------------------------*/
static int
DWCs_GetSendState( u8 aid )
{
	return sTransInfo->connections[ aid ].sendState;
}

/** -----------------------------------------------------------------
  ��M��Ԃ��擾
  -------------------------------------------------------------------*/
static int
DWCs_GetRecvState( u8 aid )
{
	return sTransInfo->connections[ aid ].recvState;
}

/** -----------------------------------------------------------------
  gt2Send�̃��b�v�֐��@(���M���Aid�Ŏw�肷��)
  -------------------------------------------------------------------*/
void
DWCs_Send(
    u8						aid,
    const u8*				buffer,
    int 					size,
    BOOL					reliable )
{
    GT2Connection connection = DWCi_GetGT2Connection( aid );

    gt2Send( connection, buffer, size, reliable );
}

/** -----------------------------------------------------------------
  Reliable���M���ꂽ�f�[�^����M
  -------------------------------------------------------------------*/
static void
DWCs_HandleReliableMessage( 
	GT2Connection			connection, 
	u8*						message, 
	int						size )
{
	u8	aid			= DWCi_GetConnectionAID( connection );
    u16 type;

    switch ( DWCs_GetRecvState( aid ) )
    {
    case DWC_TRANSPORT_RECV_NOBUF:
        type = DWCs_DecodeHeader( (DWCTransportHeader*) message );
        if ( type >= DWC_SEND_TYPE_MATCH_SYN &&
             type <= DWC_SEND_TYPE_MATCH_ACK )
        {
            // ��M�o�b�t�@���ݒ肳��Ă��Ȃ����Ă��A�V�X�e���f�[�^�w�b�_�Ȃ�
            // �w�b�_�����Ŏ�M����
            DWCs_RecvDataHeader( aid, (DWCTransportHeader*) message, size );
        }
        else
        {
        	// ��M�o�b�t�@���ݒ肳��Ă��Ȃ��̂ŁA��M�����f�[�^���̂Ă�
			DWC_Printf( DWC_REPORTFLAG_RECV_INFO, "+++ Recv buffer is not set\n" );
        }
        break;
    case DWC_TRANSPORT_RECV_HEADER:
		// �w�b�_�[�i���M����Ă���\��̃T�C�Y�j����M
        DWCs_RecvDataHeader( aid, (DWCTransportHeader*) message, size );	
        break;
    case DWC_TRANSPORT_RECV_BODY:
		// �f�[�^�{�̂���M
        DWCs_RecvDataBody( aid, message, size );
        break;
    case DWC_TRANSPORT_RECV_SYSTEM_DATA:
        // �V�X�e���f�[�^�i�����g�p�f�[�^�j����M
        DWCs_RecvSystemDataBody ( aid, message, size );
        break;
    case DWC_TRANSPORT_RECV_ERROR:
		// �f�[�^���̂Ă�
		DWC_Printf( DWC_REPORTFLAG_RECV_INFO, "+++ Recv size is too large ( buffer size = %d < %d )\n", sTransInfo->connections[ aid ].recvBufferSize, size );
        // �w�b�_�[�҂���Ԃɖ߂�		
		sTransInfo->connections[ aid ].recvState		= DWC_TRANSPORT_RECV_HEADER; 
		sTransInfo->connections[ aid ].recvingSize		= 0;
		sTransInfo->connections[ aid ].requestRecvSize	= 0;
        break;
    default:
		//OS_Panic( "[DWC] Recv error (state is %d)",  DWCs_GetRecvState( aid ) );
        DWC_Printf( DWC_REPORTFLAG_ERROR, "Recv error (state is %d).\n", DWCs_GetRecvState( aid ) );
        DWCi_SetError( DWC_ERROR_NETWORK, DWC_ECODE_SEQ_ETC + DWC_ECODE_GS_GT2 + DWC_ECODE_TYPE_TRANS_HEADER );
        break;
    }
}

/** -----------------------------------------------------------------
  Unreliable���M���ꂽ�f�[�^����M
  -------------------------------------------------------------------*/
static void
DWCs_HandleUnreliableMessage( 
	GT2Connection			connection, 
	void*					message, 
	int						size )
{
	u8 aid = DWCi_GetConnectionAID( connection );
	DWCTransportConnection* transConnection = &sTransInfo->connections[ aid ];

	if ( transConnection->recvBuffer &&
		 transConnection->recvBufferSize >= size )
	{
		// ��M�R�[���o�b�N
		if ( sTransInfo->recvCallback )
		{
			sTransInfo->recvCallback( aid, message, size );
		}

		if ( sTransInfo->recvTimeoutCallback && (transConnection->recvTimeoutTime > 0) )
		{
			transConnection->previousRecvTick = OS_GetTick();
		}
	}
	else
	{
		DWC_Printf( DWC_REPORTFLAG_RECV_INFO, "Recv data size is too large (%d > %d)\n", size, transConnection->recvBufferSize );
		return;
	}
}

/** -----------------------------------------------------------------
  Reliable���M���ꂽ�f�[�^�̃w�b�_�[���擾
  -------------------------------------------------------------------*/
static void
DWCs_RecvDataHeader( 
	u8						aid,
	DWCTransportHeader*		message,
	int						size )
{
    u16 type;
	DWCTransportConnection*	connection = &sTransInfo->connections[ aid ];
	DWCTransportHeader		header;

    // �w�b�_��M�O��recvState���L�^
    connection->lastRecvState = (u8)DWCs_GetRecvState( aid );

    // �w�b�_�[����͂���
    switch ( type = DWCs_DecodeHeader( message ) )
    {
    case DWC_SEND_TYPE_USERDATA:
		// DWC_SEND_TYPE_USERDATA�̓��[�U�[�����M�����f�[�^
 		if ( size != sizeof( DWCTransportHeader ) )
 		{
			DWC_Printf( DWC_REPORTFLAG_RECV_INFO, "+++ Invalid header from aid %d\n", aid );
	        return;
	    }
        
        MI_CpuCopy8( message, &header, sizeof(DWCTransportHeader) );

        connection->requestRecvSize	= header.size;	// ���M����Ă���\��̃T�C�Y���擾
        connection->recvingSize		= 0;			// ��M���T�C�Y��0�ɖ߂�

        // �K�؂ȃT�C�Y�̎�M�o�b�t�@���ݒ肳��Ă���Ύ�M����
        if ( connection->recvBuffer &&
             connection->recvBufferSize >= connection->requestRecvSize )
        {
            connection->recvState = DWC_TRANSPORT_RECV_BODY;
        }
        else
        {
            connection->recvState = DWC_TRANSPORT_RECV_ERROR;
        }
        break;

    case DWC_SEND_TYPE_MATCH_SYN:
    case DWC_SEND_TYPE_MATCH_SYN_ACK:
    case DWC_SEND_TYPE_MATCH_ACK:
        // �}�b�`���C�N�������������w�b�_����M
        DWC_Printf( DWC_REPORTFLAG_RECV_INFO, "Received system header.\n" );
        connection->recvState = DWC_TRANSPORT_RECV_SYSTEM_DATA;
        break;

    default:
		DWC_Printf( DWC_REPORTFLAG_RECV_INFO, "+++ Invalid header from aid %d\n", aid );
        break;
    }

    // �Ō�Ɏ�M�����w�b�_�^�C�v���L�^
    connection->lastRecvType = type;
}

/** -----------------------------------------------------------------
  Reliable���M���ꂽ�f�[�^�̖{�̂���M
  -------------------------------------------------------------------*/
static void
DWCs_RecvDataBody( 
	u8						aid,
	void*					message,
	int						size )
{
	DWCTransportConnection*	connection;
	int						requestSize;
    
	connection = &sTransInfo->connections[ aid ];

	if ( DWCs_GetRecvState( aid ) == DWC_TRANSPORT_RECV_BODY )
	{		
	    // �I�[�o�t���[�̃`�F�b�N
        if ( connection->recvingSize + size > connection->recvBufferSize )
        {
            //OS_Panic( "[DWC] Recv buffer over flow\n" );
            DWC_Printf( DWC_REPORTFLAG_ERROR, "Recv buffer over flow.\n" );
            DWCi_SetError( DWC_ERROR_NETWORK, DWC_ECODE_SEQ_ETC + DWC_ECODE_GS_GT2 + DWC_ECODE_TYPE_TRANS_BODY );

            return;
        }

        MI_CpuCopy8( message, (u8 *)connection->recvBuffer + connection->recvingSize, (u32) size );
    }

    connection->recvingSize += size;

    DWC_Printf( DWC_REPORTFLAG_RECV_INFO, "aid = %d size = %d/%d state = %d incoming buffer = %d\n",
                						  aid,
                						  connection->recvingSize, connection->requestRecvSize,
										  DWCs_GetRecvState( aid ),
                						  gt2GetIncomingBufferFreeSpace( DWCi_GetGT2Connection( aid ) ) );

    if ( connection->recvingSize == connection->requestRecvSize )
	{
		requestSize = connection->requestRecvSize;

        // �w�b�_�[�҂���Ԃɖ߂�		
		connection->recvState		= DWC_TRANSPORT_RECV_HEADER; 
		connection->recvingSize		= 0;
		connection->requestRecvSize	= 0;				

        // ��M�����R�[���o�b�N
		if ( sTransInfo->recvCallback )
		{
			sTransInfo->recvCallback( aid, connection->recvBuffer, requestSize );
        }
    }

	if ( sTransInfo->recvTimeoutCallback && (connection->recvTimeoutTime > 0) )
	{
		connection->previousRecvTick = OS_GetTick();
	}
}

/** -----------------------------------------------------------------
  Reliable���M���ꂽ�V�X�e���f�[�^�̖{�̂���M
  -------------------------------------------------------------------*/
static void
DWCs_RecvSystemDataBody( 
	u8						aid,
	void*					message,
	int						size )
{
#pragma unused( size )
    DWCTransportConnection*	transConnection = DWCs_GetTransConnection( aid );

    // recvState���A�V�X�e���f�[�^��M�O�̏�Ԃɖ߂��B
    // �R�[���o�b�N����DWC_SetRecvBuffer()���Ă΂�邱�Ƃ�����̂ŁA
    // �����Ŗ߂��K�v������B
    transConnection->recvState = transConnection->lastRecvState;

    switch ( transConnection->lastRecvType )
    {
    case DWC_SEND_TYPE_MATCH_SYN:
    case DWC_SEND_TYPE_MATCH_SYN_ACK:
    case DWC_SEND_TYPE_MATCH_ACK:
        // �}�b�`���C�N���������f�[�^��M��
        DWCi_ProcessMatchSynPacket( aid, transConnection->lastRecvType, (u8 *)message );
        break;
    }
}

/** -----------------------------------------------------------------
  �x���𔭐��������f�[�^������Ȃ瑗��M����
  -------------------------------------------------------------------*/
static void
DWCs_DelayCheck(
    DArray				delayedMessages,
    OSTick				now,
    BOOL				send )
{
    DWCDelayedMessage*	msg;
	int					i;	
	int					num;
	int					freeSpace;
	u8					aid;
    
//nakatat
	if( delayedMessages == NULL ){
		return;
	}

	num = ArrayLength( delayedMessages );

	for ( i = ( num - 1 ); i >= 0; --i )
    {
		msg = (DWCDelayedMessage*) ArrayNth( delayedMessages, i );

        // startTime����߂������Ԃ�delayTime���傫���Ȃ����瑗��M����
		if ( OS_TicksToMilliSeconds( now - msg->startTime ) > msg->delayTime )
		{
			if ( send )
            {
//nakatat
#if 0
				aid = DWCi_GetConnectionAID( msg->connection );
                if ( ! DWCi_IsValidAID( aid ) )
#else
				if( gt2GetConnectionState( msg->connection ) != GT2Connected )
#endif
                {
					ArrayDeleteAt( delayedMessages, i );
					continue;	
                }
                
#if 1
				aid = DWCi_GetConnectionAID( msg->connection );
#endif
                freeSpace = DWCs_GetOutgoingBufferFreeSize( aid );				

			    if ( freeSpace > msg->size )
			    {
	                // ���M
	                gt2FilteredSend( msg->connection, msg->filterID, msg->message, msg->size, msg->reliable );
			    }
                else
                {
					return;
                }
            }
            else
            {
                // ��M
                gt2FilteredReceive( msg->connection, msg->filterID, msg->message, msg->size, msg->reliable );
            }

            // ����M�����f�[�^���폜
			ArrayDeleteAt( delayedMessages, i );
        }
        else
        {
			return;
        }
    }
}

/** -----------------------------------------------------------------
  �x�������p�̃o�b�t�@�ɑ���M�f�[�^���i�[����
  -------------------------------------------------------------------*/
static void
DWCs_AddDelayMessage(
	DArray				array,
    u16					delayValue,
    GT2Connection		connection,
	int					filterID,
    const u8*			message,
    int					size,
    BOOL				reliable )
{     
    DWCDelayedMessage	msg;

    MI_CpuClear8( &msg, sizeof( DWCDelayedMessage ) );

    msg.connection	= connection;
    msg.message		= gsimalloc( (u32) size );
	//SDK_ASSERT( msg.message );
    if ( msg.message == NULL )
    {
        DWCi_SetError( DWC_ERROR_FATAL, DWC_ECODE_SEQ_ETC + DWC_ECODE_GS_GT2 + DWC_ECODE_TYPE_ALLOC );
        return;
    }
    
	MI_CpuCopy8( message, msg.message, (u32) size );
	msg.filterID	= filterID;		
    msg.size		= size;
    msg.reliable	= reliable;
    msg.startTime	= OS_GetTick();
    msg.delayTime	= delayValue;

    // �z��̐擪�ɒǉ�
	ArrayInsertAt( array, &msg, 0 );    
}

/** -----------------------------------------------------------------
  �z��J��
  -------------------------------------------------------------------*/
static void
DWCs_DelayedMessageFree( void* elem )
{
	DWCDelayedMessage* message = (DWCDelayedMessage*) elem;

    gsifree( message->message );
}

/** -----------------------------------------------------------------
  Outgoing Buffer�̋󂫃T�C�Y���擾
  -------------------------------------------------------------------*/
static int
DWCs_GetOutgoingBufferFreeSize( u8 aid )
{
	static const int	gamespyUseSize	= 512; // ���M�ȊO�Ŏg�p���镪���m�ۂ���K�v������݂���
	GT2Connection		connection		= DWCi_GetGT2Connection( aid );
    int					free;

    SDK_ASSERT( connection );

    free = gt2GetOutgoingBufferFreeSpace( connection ) - DWC_TRANSPORT_GT2HEADER_SIZE - gamespyUseSize;

    return	( free > 0 ) ? free : 0;
}

#ifndef	SDK_FINALROM

/** -----------------------------------------------------------------
  ���M�p�P�b�g���X�𔭐�������
  -------------------------------------------------------------------*/
static void
DWCs_SendFilterCallbackDrop(
    GT2Connection		connection,
    int					filterID,
    const GT2Byte * 	message,
    int					size,
    GT2Bool 			reliable )
{
    if ( ! reliable && MATH_Rand16(  &sTransInfo->context, 100 ) < sTransInfo->sendDrop ) 
    {
		// ���M�����ɏI������
        DWC_Printf( DWC_REPORTFLAG_SEND_INFO, "Packet loss\n" );
        return;				
    }

	gt2FilteredSend( connection, filterID, message, size, reliable );
}

/** -----------------------------------------------------------------
  ��M�p�P�b�g���X�𔭐�������
  -------------------------------------------------------------------*/
static void
DWCs_RecvFilterCallbackDrop(
    GT2Connection		connection,
    int					filterID,
    GT2Byte*		 	message,
    int					size,
    GT2Bool 			reliable )
{
    if ( ! reliable && MATH_Rand16(  &sTransInfo->context, 100 ) < sTransInfo->recvDrop ) 
    {
		// ��M�����ɏI������
        DWC_Printf( DWC_REPORTFLAG_RECV_INFO, "Packet loss\n" );
        return;				
    }

	gt2FilteredReceive( connection, filterID, message, size, reliable );
}

/** -----------------------------------------------------------------
  ���M�ɒx���𔭐�������
  -------------------------------------------------------------------*/
static void
DWCs_SendFilterCallbackDelay(
    GT2Connection		connection,
    int					filterID,
    const u8*	 		message,
    int					size,
    BOOL				reliable )
{
	u8	aid = DWCi_GetConnectionAID( connection );

    DWCs_AddDelayMessage( sTransInfo->delayedSend, sTransInfo->connections[aid].sendDelay,  connection, filterID, message, size, reliable );
}

/** -----------------------------------------------------------------
  ��M�ɒx���𔭐�������
  -------------------------------------------------------------------*/
static void
DWCs_RecvFilterCallbackDelay(
    GT2Connection		 connection,
    int					 filterID,
    u8*					 message,
    int					 size,
    BOOL				 reliable )
{
	u8	aid = DWCi_GetConnectionAID( connection );
    
	DWCs_AddDelayMessage( sTransInfo->delayedRecv, sTransInfo->connections[aid].recvDelay, connection, filterID, message, size, reliable );
}

#endif	// SDK_FINALROM


