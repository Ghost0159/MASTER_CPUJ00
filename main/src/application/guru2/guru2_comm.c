//******************************************************************************
/**
 * @file	guru2_comm.c
 * @brief	���邮������@�ʐM
 * @author	kagaya
 * @data	05.07.13
 */
//******************************************************************************
#include "common.h"
#include "communication/communication.h"
#include "communication/comm_state.h"
#include "system/procsys.h"
#include "system/clact_tool.h"
#include "system/msgdata_util.h"
#include "system/wordset.h"
#include "system/bmp_menu.h"
#include "application/record_corner.h"
#include "system/snd_tool.h"
#include "communication/wh.h"

#include "guru2_local.h"
#include "guru2_receipt.h"

#ifdef DEBUG_ONLY_FOR_kagaya
#define D_GURU2_PRINTF_ON	//��`��OS_Printf�L��
#endif

//==============================================================================
//	typedef
//==============================================================================
//--------------------------------------------------------------
///	WUSE_SENDWORK
///	�ʐM�]���p�ėp���[�N�B
///	�T�C�Y��GURU2_WIDEUSE_SENDWORK_SIZE�ŌŒ�
//--------------------------------------------------------------
typedef struct
{
	u32 cmd;
	u8 buf[GURU2_WIDEUSE_SENDWORK_SIZE-4];	//-4=cmd size
}WUSE_SENDWORK;

//==============================================================================
//	proto
//==============================================================================
static BOOL guru2Comm_WideUseSendWorkSend(
	GURU2COMM_WORK *g2c, u32 code, const void *buf, u32 size );

static void guru2Comm_DataSend( GURU2COMM_WORK *g2c, int no );

static void Guru2Comm_PokeSelOyaEndTcbSet( GURU2COMM_WORK *g2c );

static const CommPacketTbl DATA_CommPacketTbl[CF_COMMAND_MAX];
static const PTRCommRecvFunc DATA_Guru2CommPacketTbl[G2COMM_MAX];

//==============================================================================
//	���邮������@�ʐM�R�}���h
//==============================================================================
//--------------------------------------------------------------
/**
 * �ʐM�R�}���h���[�N������
 * @param	g2p	GURU2PROC_WORK
 * @retval	g2c	GURU2COMM_WORK
 */
//--------------------------------------------------------------
GURU2COMM_WORK * Guru2Comm_WorkInit( GURU2PROC_WORK *g2p, u32 heap_id )
{
	GURU2COMM_WORK *g2c;
	
	g2c = sys_AllocMemory( heap_id, sizeof(GURU2COMM_WORK) );
	GF_ASSERT( g2c != NULL );
	memset( g2c, 0, sizeof(GURU2COMM_WORK) );
	
	g2c->g2p = g2p;
	
	g2c->recv_poke_party_buf = sys_AllocMemory(
			heap_id, G2MEMBER_MAX*POKEPARTY_SEND_ONCE_SIZE );
	g2c->friend_poke_party_buf = sys_AllocMemory(
			heap_id, G2MEMBER_MAX*POKEPARTY_SEND_ONCE_SIZE );
	
	return( g2c );
}

//--------------------------------------------------------------
/**
 * �ʐM�R�}���h���[�N�폜
 * @param	g2c	GURU2COMM_WORK
 * @retval	nothing
 */
//--------------------------------------------------------------
void Guru2Comm_WorkDelete( GURU2COMM_WORK *g2c )
{
	sys_FreeMemoryEz( g2c->recv_poke_party_buf );
	sys_FreeMemoryEz( g2c->friend_poke_party_buf );
	sys_FreeMemoryEz( g2c );
}

//--------------------------------------------------------------
/**
 * �ʐM�R�}���h������ ��M�R�[���o�b�N�ݒ�
 * @param	g2c	GURU2COMM_WORK
 * @retval	nothing
 */
//--------------------------------------------------------------
void Guru2Comm_CommandInit( GURU2COMM_WORK *g2c )
{
    CommCommandInitialize( DATA_CommPacketTbl, CF_COMMAND_MAX, g2c );
}

//--------------------------------------------------------------
/**
 * ���j�I�����[���ʐM�֖߂�
 * @param	g2c	GURU2COMM_WORK
 * @retval	nothing
 */
//--------------------------------------------------------------
void Guru2Comm_CommUnionRoomChange( GURU2COMM_WORK *g2c )
{
	CommStateSetLimitNum(2);
	CommStateUnionBconCollectionRestart();
	UnionRoomView_ObjInit( g2c->g2p->param.union_view );
	Union_BeaconChange( UNION_PARENT_MODE_FREE );
}

//==============================================================================
//	���M�p�ėp���[�N
//==============================================================================
//--------------------------------------------------------------
/**
 * �ėp���[�N���M
 * @param
 * @retval
 */
//--------------------------------------------------------------
static BOOL guru2Comm_WideUseSendWorkSend(
	GURU2COMM_WORK *g2c, u32 cmd, const void *buf, u32 size )
{
	GF_ASSERT( cmd < G2COMM_MAX );
	GF_ASSERT( size+4 <= GURU2_WIDEUSE_SENDWORK_SIZE );
	
	{
		BOOL ret;
		WUSE_SENDWORK *work = (void *)g2c->wideuse_sendwork;
		work->cmd = cmd;
		memcpy( work->buf, buf, size );
		ret = CommSendData( CG_GURU2_CMD, work, GURU2_WIDEUSE_SENDWORK_SIZE );
		
		#ifdef PM_DEBUG
		if( ret == FALSE ){
			#ifdef D_GURU2_PRINTF_ON
			OS_Printf( "���邮��@�ėp�f�[�^���M���s �R�}���h(%d)\n", cmd );
			#endif
		}
		#endif
		
		return( ret );
	}
}

//==============================================================================
//	���M
//==============================================================================
//--------------------------------------------------------------
/**
 * ���邮������f�[�^���M
 * @param	g2c		GURU2COMM_WORK
 * @param   code	G2C
 * @param	data	data
 * @param   size	size
 * @retval	BOOL	���M�L���[�ɓ��������ǂ���
 * */
//--------------------------------------------------------------
BOOL Guru2Comm_SendData(
	GURU2COMM_WORK *g2c, u32 code, const void *data, u32 size )
{
	if( g2c->send_stop_flag == TRUE ){	//���M��~
		return( FALSE );
	}
	
	return( guru2Comm_WideUseSendWorkSend(g2c,code,data,size) );
}

//==============================================================================
//	�R�[���o�b�N�֐��@���C��
//==============================================================================
//--------------------------------------------------------------
/**
 * ���邮��������C���R�[���o�b�N
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval	nothing
 */
//--------------------------------------------------------------
static void CommCB_Guru2Cmd(
		int netID, int size, void* pBuff, void* pWork )
{
	WUSE_SENDWORK *work = pBuff;
	
	if( work->cmd >= G2COMM_MAX ){
		#ifdef D_GURU2_PRINTF_ON
		OS_Printf( "GURU2 CALLBACK ERROR CMD cmd=%d\n", work->cmd );
		#endif
		GF_ASSERT( 0 );
		return;
	}
	
	DATA_Guru2CommPacketTbl[work->cmd]( netID, size, work->buf, pWork );
}

//--------------------------------------------------------------
/**
 * ��t�@�|�P�����p�[�e�B����M
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval  none		
 */
//--------------------------------------------------------------
static void CommCB_Guru2PokeParty(
		int netID, int size, void *pBuff, void *pWork )
{
	void *buf;
	GURU2COMM_WORK *g2c = pWork;
	
	g2c->comm_game_egg_recv_bit |= 1 << netID;
	buf = Guru2Comm_FriendPokePartyGet( g2c, netID );
	memcpy( buf, pBuff, POKEPARTY_SEND_ONCE_SIZE );
	#ifdef D_GURU2_PRINTF_ON
	OS_Printf( "�^�}�S��M" );
	#endif
}

//==============================================================================
//	�R�[���o�b�N�֐��@��t
//==============================================================================
//--------------------------------------------------------------
/**
 * ��t�@���E����t���O����
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval	nothing
 */
//--------------------------------------------------------------
static void CommCB_Receipt_BanFlag(
		int netID, int size, void* pBuff, void* pWork )
{
	GURU2COMM_WORK *g2c = pWork;
	u8 *flag = (u8*)pBuff;
	
	if( netID==0 ){					//�e
		if( g2c->banFlag != *flag ){
			#ifdef D_GURU2_PRINTF_ON
			OS_Printf( "�e�@����̗��E�֎~�t���O banFlag=%d\n", *flag );
			#endif
		}
		g2c->banFlag = *flag;
	}
}

//--------------------------------------------------------------
/**
 * ��t�@�f�[�^��M�R�[���o�b�N
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval	nothing
 */
//--------------------------------------------------------------
static void CommCB_Receipt_Data(
		int netID, int size, void* pBuff, void* pWork )
{
	GURU2COMM_WORK *g2c = pWork;
	g2c->recv_count++;
	#ifdef D_GURU2_PRINTF_ON
	OS_Printf( "���邮������f�[�^��M���� id=%d\n", netID );
	#endif
}

//------------------------------------------------------------------
/**
 * ��t�@���邮������@�|�P�����Z���N�g�J�n�@�e���M
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval  nothing
 */
//------------------------------------------------------------------
static void CommCB_Receipt_Start(
		int netID, int size, void* pBuff, void* pWork )
{
	if( CommGetCurrentID() != 0 ){	//�e���M �q�̃V�[�P���X�ύX bug 0212 fix
		GURU2COMM_WORK *wk = pWork;
	
		wk->recv_count = 0;
		wk->record_execute = TRUE;
	
		//���R�[�h�f�[�^���M�J�n
		//Guru2Comm_DataSend( wk, CommGetCurrentID() );
		
		//��ʏ㕔���u���R�[�h�������񂿂イ�I�v
		//RecordCornerTitleChange( wk );
		//Guru2Rc_MainSeqForceChange(
		//wk->g2p->g2r, RECORD_MODE_RECORD_SEND_DATA, netID );
		
		Guru2Rc_MainSeqForceChange(
			wk->g2p->g2r, RECORD_MODE_GURU2_POKESEL_START, netID );
		
	//	CommStateSetErrorCheck( TRUE,TRUE ); // �����ȍ~�͐ؒf���G���[
	}
}

//--------------------------------------------------------------
/**
 * ��t�@�ʐM��ʂ��ĊJ����
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval	nothing
 */
//--------------------------------------------------------------
static void CommCB_Receipt_ReStart(
		int netID, int size, void* pBuff, void* pWork)
{
	GURU2COMM_WORK *g2c = pWork;
	#ifdef D_GURU2_PRINTF_ON
	OS_Printf("�e�@���烌�R�[�h��W�ĊJ�ʒm\n");
	#endif
	Guru2Rc_MainSeqForceChange( g2c->g2p->g2r, RECORD_MODE_NEWMEMBER_END, 0 );
}

//--------------------------------------------------------------
/**
 * ��t�@�q�@�����E�B
 * pBuff �q(���E��)����̑��M��0:���E���m�F�B�@1:���E���s
 * �e����̑��M�����4�r�b�g�F�e��shareNum(0xf�̏ꍇ�͗��ENG)
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval	nothing
 */
//--------------------------------------------------------------
static void CommCB_Receipt_EndChild(
		int netID, int size, void* pBuff, void* pWork)
{
	GURU2COMM_WORK *g2c = pWork;
	GURU2COMM_END_CHILD_WORK trans_work;
	GURU2COMM_END_CHILD_WORK *recieve_work = pBuff;
	
	//�q�@(���E��)����̑��M
	if( netID != 0 ){						//���E���܂��ʒm
		if( CommGetCurrentID()==0){			// �q�@����(�e�@���󂯎��)
			// �q�@�����Ȃ��Ȃ�������S���ɒʒm����
			trans_work = *recieve_work;
			trans_work.ridatu_id = netID;
			trans_work.oya_share_num = g2c->shareNum;
			
			switch(recieve_work->request){
			case CREC_REQ_RIDATU_CHECK:
				if(g2c->shareNum != CommGetConnectNum() 
					|| g2c->shareNum != Guru2Comm_OnlineNumGet()
					|| g2c->shareNum != MATH_CountPopulation(WH_GetBitmap())){
					trans_work.ridatu_kyoka = FALSE;	//���ENG�I
				}else{
					g2c->ridatu_bit |= 1 << netID;
					trans_work.ridatu_kyoka = TRUE;
					//���EOK�Ȃ̂ŎQ��������������
					//(����������΂����瑤�Ő������͂������͂�)
					CommStateSetLimitNum( CommGetConnectNum() );
				}
				break;
			case CREC_REQ_RIDATU_EXE:
				break;
			}
		
			Guru2Comm_SendData( g2c, G2COMM_RC_END_CHILD,
				&trans_work, sizeof(GURU2COMM_END_CHILD_WORK) );
			#ifdef D_GURU2_PRINTF_ON
			OS_Printf("�q�@%d���痣�E���󂯎���������M\n",netID);
			#endif
		}
	}else{	//�e����̑��M
		switch(recieve_work->request){
		case CREC_REQ_RIDATU_CHECK:
			//���E�m�F�Ȃ̂ŁA���E���悤�Ƃ����q�@�ɂ̂݌��ʂ𑗂�
			if(recieve_work->ridatu_id == CommGetCurrentID()){
				if(recieve_work->ridatu_kyoka == FALSE){
					Guru2Rc_MainSeqForceChange(
						g2c->g2p->g2r, RECORD_MODE_END_SELECT_ANSWER_NG,
						recieve_work->ridatu_id );
				}else{
					g2c->oya_share_num = recieve_work->oya_share_num;
					Guru2Rc_MainSeqForceChange(
						g2c->g2p->g2r, RECORD_MODE_END_SELECT_ANSWER_OK,
						recieve_work->ridatu_id );
				}
			}
			
			break;
		case CREC_REQ_RIDATU_EXE:
			#ifdef D_GURU2_PRINTF_ON
			OS_Printf("�e�@���q�@%d�̗��E�ʒm����\n",recieve_work->ridatu_id);
			#endif
			Guru2Rc_MainSeqForceChange(
				g2c->g2p->g2r, RECORD_MODE_LOGOUT_CHILD, recieve_work->ridatu_id );
			break;
		}
	}
}

//--------------------------------------------------------------
/**
 *	��t�u�q�@���������Ă����̂ň�U�G�𑗂��~�܂��Ăˁv
 *	�Ɛe�@�����M���Ă������̃R�[���o�b�N
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval  nothing
 */
//--------------------------------------------------------------
static void CommCB_Receipt_Stop(
		int netID, int size, void* pBuff, void* pWork)
{
	GURU2COMM_WORK *wk = pWork;
	u8 id;

	id = *(u8*)pBuff;
	Guru2Rc_MainSeqCheckChange( wk->g2p->g2r, RECORD_MODE_NEWMEMBER, id );

	if(CommGetCurrentID()==0){	// �e�@���摜�f�[�^���M���J�n����
		wk->send_num = 0;
	}
	
	#ifdef D_GURU2_PRINTF_ON
	OS_Printf("�e�@����́u�q�@%d�ɊG�𑗂邩��~�܂��Ăˁv�ʒm\n",id);
	#endif
}

//--------------------------------------------------------------
/**
 * ��t�@�e�@����߂�̂ŋ����I��������
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval  none		
 */
//--------------------------------------------------------------
static void CommCB_Receipt_End(
	int netID, int size, void* pBuff, void* pWork)
{
	GURU2COMM_WORK *wk = pWork;
	
	#ifdef D_GURU2_PRINTF_ON
	OS_Printf("�e�@����̏I���ʒm�������̂ł�߂�\n");
	#endif
	
	// �e�@�ȊO�͂��̃R�}���h�ŋ����I��
	if( CommGetCurrentID() != 0 ){
		Guru2Rc_MainSeqForceChange( wk->g2p->g2r, RECORD_MODE_FORCE_END, 0  );
	}
}

//--------------------------------------------------------------
/**
 * ��t�@�R��ځE�S��ځE�T��ڂ̎q�@���u�G������[�����v�ƌ���
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval  none		
 */
//--------------------------------------------------------------
static void CommCB_Receipt_ChildJoin(
	int netID, int size, void* pBuff, void* pWork )
{
	u8 id;
	GURU2COMM_WORK *g2c = pWork;
	
	// �e�@���󂯎������(�Ƃ������e�����󂯎��Ȃ����ǁj
	if( CommGetCurrentID() == 0 ){
		id  = netID;
		// �S��Ɂu���ꂩ��G�𑗂�̂Ŏ~�܂��Ă��������v�Ƒ��M����
		Guru2Comm_SendData( g2c, G2COMM_RC_STOP, &id, 1 );
		#ifdef D_GURU2_PRINTF_ON
		OS_Printf("�q�@(%d = %d)����̗����^�G�̂��������ʒm\n",id,netID);
		#endif
	}
}

//==============================================================================
//	�R�[���o�b�N�֐��@�Q�[�����C��
//==============================================================================
//--------------------------------------------------------------
/**
 * �Q�[�����C���@�V�O�i����M
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval  none		
 */
//--------------------------------------------------------------
static void CommCB_Main_Signal(
		int netID, int size, void *pBuff, void *pWork )
{
	u16 *buff = pBuff;
	GURU2COMM_WORK *wk = pWork;
	wk->comm_game_signal_bit |= *buff;
	
	#ifdef D_GURU2_PRINTF_ON
	OS_Printf("���邮�� �V�O�i��(0x%x)��M ��MID=0x%x\n",*buff,netID);
	#endif
}

//--------------------------------------------------------------
/**
 * �Q�[�����C���@�Q��
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval  none		
 */
//--------------------------------------------------------------
static void CommCB_Main_Join(
		int netID, int size, void *pBuff, void *pWork )
{
	GURU2COMM_WORK *wk = pWork;
	wk->comm_game_join_bit |= 1 << netID;
	#ifdef D_GURU2_PRINTF_ON
	OS_Printf( "���邮�� �Q�[���Q���󂯎�� ID= 0x%d\n", netID );
	#endif
}

//--------------------------------------------------------------
/**
 * �Q�[�����C���@�v���C�ԍ�����M
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval  none		
 */
//--------------------------------------------------------------
static void CommCB_Main_PlayNo(
		int netID, int size, void *pBuff, void *pWork )
{
	GURU2COMM_WORK *g2c = pWork;
	GURU2COMM_PLAYNO *no = pBuff;
	Guru2Main_CommPlayNoDataSet( g2c->g2p->g2m, no );
}

//--------------------------------------------------------------
/**
 * �Q�[�����C���@�v���C�ő吔����M
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval  none		
 */
//--------------------------------------------------------------
static void CommCB_Main_PlayMax(
		int netID, int size, void *pBuff, void *pWork )
{
	int max = *(int *)pBuff;
	
	GURU2COMM_WORK *g2c = pWork;
	Guru2Main_CommPlayMaxSet( g2c->g2p->g2m, max );
}

//--------------------------------------------------------------
/**
 * �Q�[���@�{�^��������M
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval  none		
 */
//--------------------------------------------------------------
static void CommCB_Main_Button(
		int netID, int size, void *pBuff, void *pWork )
{
	u8 btn = *(u8*)pBuff;
	GURU2COMM_WORK *g2c = pWork;
	Guru2Main_CommButtonSet( g2c->g2p->g2m, btn );
}

//--------------------------------------------------------------
/**
 * �Q�[���@�Q�[��������M
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval  none		
 */
//--------------------------------------------------------------
static void CommCB_Main_GameData(
		int netID, int size, void *pBuff, void *pWork )
{
	if( CommGetCurrentID() != 0 ){	//���M�����e�ȊO
		GURU2COMM_WORK *g2c = pWork;
		GURU2COMM_GAMEDATA *data = pBuff;
		Guru2Main_CommGameDataSet( g2c->g2p->g2m, data );
	}
}

//--------------------------------------------------------------
/**
 * �Q�[�����C���@�����ʒu����M
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval  none		
 */
//--------------------------------------------------------------
static void CommCB_Main_TradeNo(
		int netID, int size, void *pBuff, void *pWork )
{
	int no = *(int *)pBuff;
	GURU2COMM_WORK *g2c = pWork;
	Guru2Main_TradeNoSet( g2c->g2p->g2m, netID, no );
}

//--------------------------------------------------------------
/**
 * �Q�[�����C���@�Q�[�����ʂ���M
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval  none		
 */
//--------------------------------------------------------------
static void CommCB_Main_GameResult(
		int netID, int size, void *pBuff, void *pWork )
{
	GURU2COMM_WORK *g2c = pWork;
	GURU2COMM_GAMERESULT *result = pBuff;
	Guru2Main_GameResultSet( g2c->g2p->g2m, result );
}

//--------------------------------------------------------------
/**
 * �Q�[�����C���@���f�[�^�ُ��M
 * @param   netID		
 * @param   size		
 * @param   pBuff		
 * @param   pWork		
 * @retval  none		
 */
//--------------------------------------------------------------
static void CommCB_Main_EggDataNG(
		int netID, int size, void *pBuff, void *pWork )
{
	u32 bit = 1 << netID;
	GURU2COMM_WORK *g2c = pWork;
	g2c->comm_game_egg_check_error_bit |= bit;
}

//--------------------------------------------------------------
/**
 * �Q�[�����C���@���f�[�^�����M
 * @param   netID
 * @param   size
 * @param   pBuff
 * @param   pWork
 * @retval  none
 */
//--------------------------------------------------------------
static void CommCB_Main_EggDataOK(
		int netID, int size, void *pBuff, void *pWork )
{
	u32 bit = 1 << netID;
	GURU2COMM_WORK *g2c = pWork;
	g2c->comm_game_egg_check_ok_bit |= bit;
}

//==============================================================================
//	�T�C�Y
//==============================================================================
//--------------------------------------------------------------
///	GURU2_WIDEUSE_SENDWORK_SIZE
//--------------------------------------------------------------
static int _getGuru2WideUseSendWorkSize( void )
{
	return( GURU2_WIDEUSE_SENDWORK_SIZE );
}

//--------------------------------------------------------------
///	GURU2COMM_END_CHILD_WORK
//--------------------------------------------------------------
static int _getCRECW(void)
{
	return sizeof(GURU2COMM_END_CHILD_WORK);
}

//--------------------------------------------------------------
///	POKEPARTY_SEND_ONCE_SIZE
//--------------------------------------------------------------
static int _getPokePartyOnceSize( void )
{
	return POKEPARTY_SEND_ONCE_SIZE;
}

//==============================================================================
//	��M�o�b�t�@
//==============================================================================
//--------------------------------------------------------------
/**
 * �|�P�����f�[�^��M�o�b�t�@
 * @param
 * @retval
 */
//--------------------------------------------------------------
static u8 * _getPokePartyRecvBuff( int netID, void *pWork, int size )
{
	u32 buf;
	GURU2COMM_WORK *g2c = pWork;
	
	buf = (u32)(g2c->recv_poke_party_buf);
	buf += netID * POKEPARTY_SEND_ONCE_SIZE;
	return( (u8*)buf );
}

//==============================================================================
//	�p�[�c
//==============================================================================
//------------------------------------------------------------------
/**
 * ���݂̃I�����C�������擾
 * @param   nothing
 * @retval  int		�I�����C����
 */
//------------------------------------------------------------------
int Guru2Comm_OnlineNumGet( void )
{
	int i,result;
	
	for(result=0,i=0;i<G2MEMBER_MAX;i++){
		if(CommInfoGetMyStatus(i)!=NULL){
			result++;
		}
	}
	return result;
}

//------------------------------------------------------------------
/**
 * $brief   �O���t�B�b�N�f�[�^�𕪊����M
 * @param   g2c
 * @param   no		
 * @retval  none		
 */
//------------------------------------------------------------------
static void guru2Comm_DataSend( GURU2COMM_WORK *g2c, int no )
{
#if 0
	//xor�擾�E�i�[
	{
		int i;
		u32 *p,result;
		p = (u32*)g2c->send_data.data;
		for(result=0,i=0;i<sizeof(GURU2COMM_RC_SEND_DATABUF_SIZE)/4;i++){
			result ^= p[i];
		}
		g2c->send_data._xor        = result;
	}
	
	// �����ԍ�����������(�e�@�̒l�������g�p�����j
	g2c->send_data.seed = gf_rand();
	
	// ���M�J�n
	CommSendHugeData(
		G2COMM_RC_CMD, &g2c->send_data, sizeof(GURU2COMM_RC_SENDDATA) );
	
	OS_Printf("���M�f�[�^ no=%d, xor = %08x\n",
				g2c->send_data.seed,  g2c->send_data._xor );
#endif
}

//--------------------------------------------------------------
/**
 * �|�P�����p�[�e�B��M�ς݃o�b�t�@��Ԃ�
 * @param
 * @retval
 */
//--------------------------------------------------------------
void * Guru2Comm_FriendPokePartyGet( GURU2COMM_WORK *g2c, int id )
{
	u32 buf;
	buf = (u32)(g2c->friend_poke_party_buf);
	buf += id * POKEPARTY_SEND_ONCE_SIZE;
	return( (void*)buf );
}

//==============================================================================
//	data
//==============================================================================
//--------------------------------------------------------------
///	�ʐM�R�[���o�b�N�֐�
//--------------------------------------------------------------
static const CommPacketTbl DATA_CommPacketTbl[CF_COMMAND_MAX] =
{
	// comm_command_field.c�œo�^����Ă���R�}���h�e�[�u���̖�����
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
	
	// ���R�[�h���g�p����R�}���h����
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
	
	// ���������̃R�}���h�𖳎����邽�߂̕���
	{ CommDummyCallBack, _getZero, NULL},
	{ CommDummyCallBack, _getZero, NULL},
	{ CommDummyCallBack, _getZero, NULL},
	{ CommDummyCallBack, _getZero, NULL},
	{ CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
    { CommDummyCallBack, _getZero, NULL},
	{ CommDummyCallBack, _getZero, NULL},
	{ CommDummyCallBack, _getZero, NULL},
	
	//�������炮�邮��
	{CommCB_Guru2Cmd, _getGuru2WideUseSendWorkSize, NULL }, //CG_GURU2_CMD
	{CommCB_Guru2PokeParty,_getPokePartyOnceSize,_getPokePartyRecvBuff}, //CG_GURU2_EGG
};

//--------------------------------------------------------------
///	���邮���p�ʐM�R�[���o�b�N�֐�
//--------------------------------------------------------------
static const PTRCommRecvFunc DATA_Guru2CommPacketTbl[G2COMM_MAX] =
{
	CommCB_Receipt_Stop,
	CommCB_Receipt_ReStart,
	CommCB_Receipt_EndChild,
	CommCB_Receipt_End,
	CommCB_Receipt_ChildJoin,
	CommCB_Receipt_Start,
	CommCB_Receipt_Data,
	CommCB_Receipt_BanFlag,
	
	CommCB_Main_Signal,
	CommCB_Main_Join,
	CommCB_Main_Button,
	CommCB_Main_GameData,
	CommCB_Main_PlayNo,
	CommCB_Main_PlayMax,
	CommCB_Main_TradeNo,
	CommCB_Main_GameResult,
	CommCB_Main_EggDataNG,
	CommCB_Main_EggDataOK,
};

