#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"

#include "dbs.h"

#include "transport.h"

extern GameSequence gameSeqList[GAME_MODE_NUM];
extern GameControl stGameCnt;  // �Q�[��������\����
extern KeyControl stKeyCnt;    // �L�[���͐���\����

//callback�v���g�^�C�v
static void ConnectionClosedCallback(DWCError error,
                                     BOOL isLocal,
                                     BOOL isServer,
                                     u8 aid,
                                     int index,
                                     void* param);

/** -------------------------------------------------------------------------
  DWC Transport
  ---------------------------------------------------------------------------*/
#define			MAX_PLAYERS				3
const int		SIZE_SEND_BUFFER = 512;
const int		SIZE_RECV_BUFFER = 4 * 1024;

static u8		sSendBuffer[ SIZE_SEND_BUFFER ];
static u8		sRecvBuffer[ MAX_PLAYERS ][ SIZE_RECV_BUFFER ];

/** -------------------------------------------------------------------------
  ���M�����R�[���o�b�N  
  ---------------------------------------------------------------------------*/
static void
SendDoneCallback( int size, u8 aid )
{
#ifdef SDK_FINALROM
#pragma unused(size, aid)
#endif
	OS_TPrintf( "to aid = %d  size = %d\n", aid, size );
}

/** -------------------------------------------------------------------------
  ��M�����R�[���o�b�N  
  ---------------------------------------------------------------------------*/
static void
UserRecvCallback( u8 aid, u8* buffer, int size )
{
#ifdef SDK_FINALROM
#pragma unused(aid, buffer, size)
#endif
	OS_TPrintf( "from aid = %d  size = %d  buffer[0] = %X\n", aid, size, buffer[0] );
}

/** -------------------------------------------------------------------------
  DWC Transport�p�@��M�o�b�t�@��ݒ�  
  ---------------------------------------------------------------------------*/
void
SetRecvBuffer( void )
{
	int i;
    int j = 0;
	int connectingNum = DWC_GetNumConnectionHost()	;

    OS_TPrintf( "connection host = %d\n", connectingNum );
    
    for ( i = 0; i < connectingNum; ++i )
    {
		if ( i == DWC_GetMyAID() )
        {
            j++;
			continue;
        }

        DWC_SetRecvBuffer( (u8)i, &sRecvBuffer[i-j], SIZE_RECV_BUFFER );
    }
}

/** -------------------------------------------------------------------------
  Reliabe���M
  ---------------------------------------------------------------------------*/
static void
BroadCastReliable( void )
{
    u8* pAidList;
    int numHost;
    int i;

    numHost = DWC_GetAIDList(&pAidList);

    for (i = 0; i < numHost; i++){
        if (pAidList[i] == DWC_GetMyAID())
        {
            continue;
        }

        DWC_SendReliable( pAidList[i], sSendBuffer, SIZE_SEND_BUFFER );
    }
}

/** -------------------------------------------------------------------------
  Unreliable���M
  ---------------------------------------------------------------------------*/
static void
BroadCastUnreliable( void )
{
    u8* pAidList;
    int numHost;
    int i;

    numHost = DWC_GetAIDList(&pAidList);

    for (i = 0; i < numHost; i++){
        if (pAidList[i] == DWC_GetMyAID()) continue;

        DWC_SendUnreliable( pAidList[i], sSendBuffer, SIZE_SEND_BUFFER );
    }
}

/*---------------------------------------------------------------------------*
  ���b�V���ڑ������チ�C���֐�
 *---------------------------------------------------------------------------*/
GameMode GameConnectedMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_CONNECTED];
    GameMode returnSeq = GAME_MODE_NUM;
    int curIdx = 0;
    
    // ���M�R�[���o�b�N�̐ݒ�	
    DWC_SetUserSendCallback( SendDoneCallback ); 

    // ��M�R�[���o�b�N�̐ݒ�	
    DWC_SetUserRecvCallback( UserRecvCallback ); 
    
    // ��M�o�b�t�@�̐ݒ�	
    SetRecvBuffer();

    // �R�l�N�V�����N���[�Y�R�[���o�b�N��ݒ�
    DWC_SetConnectionClosedCallback(ConnectionClosedCallback, &returnSeq);

#if 0
	// �p�P�b�g���X����ݒ�
	if ( DWC_GetMyAID() != 0 )
    {
		DWC_SetSendDrop( 30, 0 );
		DWC_SetRecvDrop( 30, 0 );        
    }
#endif
#if 0
    // �x����ݒ�i�P��: ms�j
	if ( DWC_GetMyAID() != 0 )
    {
		DWC_SetSendDelay( 500, 0 );
		DWC_SetRecvDelay( 500, 0 );        
    }
#endif    

    OS_TPrintf( "my aid = %d\n", DWC_GetMyAID() );
    
    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );

        ReadKeyData();  // �L�[�f�[�^�擾

        DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

        if (DWC_GetLastError(NULL)){
            // �}�b�`���C�N���s���̏���
            ShutdownInet();  // �l�b�g���[�N�ؒf
            DWC_ClearError();
            returnSeq = GAME_MODE_MAIN;
            break;
        }

        if (stGameCnt.blocking){
            // DWC�������̓L�[������֎~����
            dbs_DemoUpdate();
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
            dbs_DemoLoad();
            // �X�^�b�N���`�F�b�N
            OS_CheckStack(OS_GetCurrentThread());
            continue;
	    }

        // ���ɐi�ނׂ����[�h���Z�b�g����Ă����烋�[�v�𔲂���
        if (returnSeq != GAME_MODE_NUM) break;

        ////////// �ȉ��L�[���쏈��
        if (stKeyCnt.trg & PAD_BUTTON_A){
            // A�{�^���Ń��j���[����
            returnSeq = gameSeq->menuList[curIdx].mode;
            stGameCnt.blocking = gameSeq->menuList[curIdx].blocking;

            switch (curIdx){
            case 0:             // Reliable���M
                BroadCastReliable();
				break;
            case 1:             // Unreliable���M
				BroadCastUnreliable();
                break;
            case 2:             // �R�l�N�V�����N���[�Y
                DWC_CloseConnectionsAsync();

                if (DWC_GetNumConnectionHost() <= 1){
                    // ���ɐڑ��z�X�g����0�Ȃ炷���ɏI�����A�R�[���o�b�N�͌Ă΂�Ȃ�
                    stGameCnt.blocking = 0;
                }
                break;
            default:
                break;
            }
        }
        else if (stKeyCnt.trg & PAD_KEY_UP){
            // �\���L�[��ŃJ�[�\���ړ�
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu-1;
            // ���j���[���X�g�ĕ\��
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_DOWN){
            // �\���L�[���ŃJ�[�\���ړ�
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
            // ���j���[���X�g�ĕ\��
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        ////////// �L�[���쏈�������܂�

        dbs_DemoUpdate();
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
        dbs_DemoLoad();
        // �X�^�b�N���`�F�b�N
        OS_CheckStack(OS_GetCurrentThread());
    }

    return returnSeq;
}

/*---------------------------------------------------------------------------*
  �R�l�N�V�����N���[�Y�R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void ConnectionClosedCallback(DWCError error,
                                     BOOL isLocal,
                                     BOOL isServer,
                                     u8  aid,
                                     int index,
                                     void* param)
{
#ifdef SDK_FINALROM
#pragma unused(aid, index)
#endif

    if (error == DWC_ERROR_NONE){
        if (!isLocal && isServer){
            // �T�[�o�N���C�A���g�}�b�`���C�N�ŁA�������N���C�A���g�̎��ɁA
            // �T�[�oDS���R�l�N�V�������N���[�Y�����B
            // �T�[�oDS���N���[�Y�����̂Ŏ������N���[�Y����B
            DWC_CloseConnectionsAsync();
        }
        
        if (DWC_GetNumConnectionHost() == 1){
            // �S�ẴR�l�N�V�������N���[�Y���ꂽ�ꍇ
            if (isLocal){
                // ��������N���[�Y�����ꍇ�͑S���Ƃ̃R�l�N�V������
                // �N���[�Y�ł����玟�ɐi��
                stGameCnt.blocking = FALSE;  // �u���b�L���O����
            }
            if ((DWC_GetLastMatchingType() != DWC_MATCH_TYPE_SC_SV) || isServer){
                // isServer==TRUE�Ŏ����ŃN���[�Y�����ꍇ�ƁA
                // ���肩��̃N���[�Y�̏ꍇ�́A�����Ŏ��̃��[�h���Z�b�g����
                *(GameMode *)param = GAME_MODE_LOGIN;
            }
            // �������T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�oDS�ŁA
            // �N���C�A���g���S���N���[�Y�����ꍇ�͉������Ȃ�
        }

        if (isLocal){
            OS_TPrintf("Closed connection to aid %d (friendListIndex = %d) Rest %d.\n",
                       aid, index, DWC_GetNumConnectionHost());
        }
        else {
            OS_TPrintf("Connection to aid %d (friendListIndex = %d) was closed. Rest %d.\n",
                       aid, index, DWC_GetNumConnectionHost());
        }
    }
    else {
        DispErrorMessage();
        OS_TPrintf("Failed to close connections. %d\n\n", error);

        *(GameMode *)param = GAME_MODE_MAIN;

        stGameCnt.blocking = FALSE;  // �u���b�L���O����
    }
}


