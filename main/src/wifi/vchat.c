//=============================================================================
/**
 * @file	vchat.c
 * @bfief	�{�C�X�`���b�g���b�p�[�B��l�d�b���[�h��p
 * @author	kazuki yoshihara
 * @date	06/02/27
 */
//=============================================================================

#include "common.h"
#include "communication/communication.h"
#include "communication/comm_state.h"
#include "communication/comm_local.h"
#include "wifi/dwc_rap.h"
#include "wifi/vchat.h"
#include <vct.h>

//#undef OHNO_PRINT
//#define OHNO_PRINT(...) (void) ((OS_Printf(__VA_ARGS__)))


#define MYNNSFORMAT NNS_SND_STRM_FORMAT_PCM16

//----------------- ���̕����̓f�t�H���g�Ō��܂��Ă��܂��B���M�ʂƂ͊֌W���܂���
#define VCHAT_SAMPLING_RATE       8000   // Hz
#define SAMPLING_BYTE        2      // byte = 16bit
#define MAX_SAMPLING_TIME   68     // ms
#define MAX_CHANNELS 1
#define VCHAT_WAVE_SAMPLE ((int) (VCHAT_SAMPLING_RATE * MAX_SAMPLING_TIME * SAMPLING_BYTE) / 1000)  //1088
//-----------------

//#define VCFINFO_SHOW
//#define VCT_ERROR_NONE (VCT_SUCCESS)


// �}�C�N�Q�C����ݒ�
#define MY_AMPGAIN PM_AMPGAIN_160 
//#define MY_AMPGAIN PM_AMPGAIN_80
//#define MY_AMPGAIN PM_AMPGAIN_40
//#define MY_AMPGAIN PM_AMPGAIN_20

// ���t���[���܂ŗ����������āA�`�F�b�N���邩
#define HAWLING_CHECKFRAME 16

// �d�b�������Ă�����x���g���C���鎞��
#define VCT_START_TIMEOUT_NUM 60

#define _MAX_PLAYER_NUM  (4)

typedef struct{
    u8 sRecBuffer[VCHAT_WAVE_SAMPLE * 2 * MAX_CHANNELS];
    void* _vWork_temp;    // �A���C�����g����Ă��Ȃ����̍\���̂̃A�h���X
	u8* pAudioBuffer;
	u8* pAudioBufferOrg;  // �A���C�����g����Ă��Ȃ�AudioBuff
	u8 sPlayBuffer[VCHAT_WAVE_SAMPLE * 2 * MAX_CHANNELS];
    // �����T�E���h�Ƃ��ēn���p�̃o�b�t�@�i��ɂO�j
    u8 sSilentBuffer[VCHAT_WAVE_SAMPLE * 2 * MAX_CHANNELS];
	void (*disconnectCallback)();
	VCTSession sSession[(_MAX_PLAYER_NUM-1)];
    BOOL bConf[_MAX_PLAYER_NUM];  //�Θb���[�h���ǂ���
    int mode;     // vct�̉�b���[�h 
	int state;
	int off_flag;
	int heapID;
	struct NNSSndStrm sSndStream;
	VCTSession *session;
    u8 bSendVoice;
    u8 firstCallback;
    u16 vctTime;  //���N�G�X�g�̎��Ԍv��
    MICAutoParam micParam;
    
    // �n�E�����O�A�N���b�v���o�܂��
    int hc_state;
    int hc_ampgain;
    int hc_ampchangeflag;
    int hc_seqcount;
    int hc_hawlingflag;			// ��x�n�E�����O���Ă���A�܂���x���L�[���͂��Ȃ��B
    u16 hc_check[HAWLING_CHECKFRAME];
    int hc_index;
    
}MYVCT_WORK;

enum{
	VCTSTATE_INIT,
	VCTSTATE_WAIT,
	VCTSTATE_CALLING
};

static MYVCT_WORK* _vWork = NULL;

static void StartSoundLoop();

// �����������B�e�q���ʁB�Z�b�V�����m����ɌĂ΂��B
//
// �����n�̏������i�ŏ��̂P��̂݁j
//

static u8 *_test_buffer;
static u32 _test_buffer_index;
#define TEST_BUFFER_SIZE 0x100000

static OSTick _tick_time;
static int _difftime = 0;

static void InitFirst()
{
    OHNO_PRINT("Init sound system\n");
    // �}�C�N�֘A�̏�����
    //
    MIC_Init();
    PM_Init();
  //  VCT_SetReportLevel( VCT_REPORTLEVEL_ALL );

#if 0
	sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_VCTTEST, TEST_BUFFER_SIZE + 0x1000 );
	_test_buffer = sys_AllocMemory(HEAPID_VCTTEST, TEST_BUFFER_SIZE);
	MI_CpuClear8( _test_buffer, TEST_BUFFER_SIZE);
 	_test_buffer_index = 0;
#endif
    {
	    u32 ret;
	    ret = PM_SetAmp(PM_AMP_ON);
	    if( ret == PM_RESULT_SUCCESS )
	    {
		    OHNO_PRINT("AMP���I���ɂ��܂����B\n");
	    }
	    else
	    {
		    OHNO_PRINT("AMP�̏������Ɏ��s�i%d�j", ret);
	    }
    }

    {
	    u32 ret;
	    ret = PM_SetAmpGain(MY_AMPGAIN);
	    if( ret == PM_RESULT_SUCCESS )
	    {
		    OHNO_PRINT("AMP�̃Q�C����ݒ肵�܂����B\n");
	    }
	    else
	    {
		    OHNO_PRINT("AMP�̃Q�C���ݒ�Ɏ��s�i%d�j", ret);
	    }
    }
    
	_vWork->hc_state    = 0;
	_vWork->hc_seqcount = 0;    
	_vWork->hc_ampgain  = 0;
	_vWork->hc_ampchangeflag = 0;
	_vWork->hc_hawlingflag = 0;
	{
		int i;
		for( i = 0; i < HAWLING_CHECKFRAME; i++ )
		{
			_vWork->hc_check[i] = 0;
		}
    	_vWork->hc_index = 0;		
	}
	VCT_EnableVAD( TRUE );
    // �T�E���h�V�X�e���̏�����
    //
    NNS_SndInit();
    NNS_SndStrmInit(&_vWork->sSndStream);

	MI_CpuClearFast(_vWork->sSilentBuffer, sizeof(_vWork->sSilentBuffer) );
    OHNO_PRINT("Init sound system done.\n");
    
    _difftime = 0;

    VCT_EnableEchoCancel(TRUE);
}



#define HAWLING_SEQ_A 4
#define HAWLING_SEQ_B 8
#define HAWLING_SEQ_C 12
#define HAWLING_SEQ_D 100
#define HAWLING_SEQ_E 4
// �n�E�����O���o���[�`��
// HAWLING_SEQ_A�t���[���A���ŋK��lA�𒴂����ꍇ�A�}�C�N�Q�C�����P�i�K������B
// ����ɂ��̂܂�HAWLING_SEQ_B�t���[���n�E�����O����������A
// HAWLING_SEQ_C�t���[���A�}�C�N����̑��M���Ƃ߂�
// HAWLING_SEQ_D�t���[���A���ŋK��lA�𒴂��邱�ƂȂ��A�������͂�����΃Q�C�����P�i�K�グ��B
// �Ԃ�l���P�Ȃ�A���͂͂O���B

#define HAWLING_BORDER_1 0x0300
#define HAWLING_BORDER_2 0x0200
#define HAWLING_BORDER_3 0x0100

#if 0
static int is_clip( int c1, int c2, int c3 )
{
	int ans = (c1 >= 5 || c2 >= 2 || c3 > 0 );
	OHNO_PRINT("(%d, %d, %d) - %d\n", c1, c2, c3, ans);
	return ans;	
}
#else
static int is_clip( )
{
	int i;
	int count = 0;
	for(i = 0; i < HAWLING_CHECKFRAME; i++ )
	{
		count += _vWork->hc_check[i];
	}
	
	return (count >= 5 * HAWLING_CHECKFRAME);
}
#endif

static void change_gain(int d)
{
	if( d < 0 ) d = 0;
	if( d >= 4 ) d = 3;
	
	_vWork->hc_ampgain = d;
	_vWork->hc_ampchangeflag = 1;
}

// �Q�[�����͂�����΁A�����ɓ����B�Ƃ肠�����APAD_KEY_READ�̓��͂�����΂Ƃ��Ă����B
static int hasGameInput()
{
	return PAD_Read();	
}
#if 0
static int check_hawling(s16 *data, int length)
{
    int i;
    u16 *ptr;
//    int c1 = 0;
//    int c2 = 0;
//    int c3 = 0;
    ptr = _vWork->hc_check + _vWork->hc_index;
    *ptr = 0;
    _vWork->hc_index = ( _vWork->hc_index + 1 ) % HAWLING_CHECKFRAME;
    for( i = 0; i < length / 2; i++)
    {
	    if( data[i] < -(0x8000 - HAWLING_BORDER_1) || data[i] > (0x8000 - HAWLING_BORDER_1)) (*ptr)++;
//    	if( data[i] < -(0x8000 - HAWLING_BORDER_1) || data[i] > (0x8000 - HAWLING_BORDER_1)) c1++;
//    	if( data[i] < -(0x8000 - HAWLING_BORDER_2) || data[i] > (0x8000 - HAWLING_BORDER_2)) c2++;
//    	if( data[i] < -(0x8000 - HAWLING_BORDER_3) || data[i] > (0x8000 - HAWLING_BORDER_3)) c3++;
    }
    
//    if( is_clip( c1, c2, c3 ) )
    if( is_clip( ) )
    {
    	if( _vWork->hc_state == 0 )
    	{
    		// ���܂ł͐���
    		_vWork->hc_state = 1;
    		_vWork->hc_seqcount = 0;
    	}
    	else if( _vWork->hc_state == 1 )
    	{
    		// �O�����l�z��
    		if( _vWork->hc_seqcount++ >= HAWLING_SEQ_A )
    		{
    			// HAWLING_SEQ_A�ȏ��l�z��
    			// �A���v����i�K������
    			change_gain( _vWork->hc_ampgain + 1 );
    			OHNO_PRINT("���ʂ̓��͂���莞�ԋK��l�𒴂��Ă܂��B�}�C�N�̃Q�C������i�K�����܂�(%d)\n", _vWork->hc_ampgain);
	    		_vWork->hc_state = 2;
	    		_vWork->hc_seqcount = HAWLING_SEQ_E;
	    		_vWork->hc_hawlingflag = 1;
    		}
    	}
    	else if( _vWork->hc_state == 2 )
    	{
    		// �Q�C�����������Ă�����A�����Ċ�z����
    		if( _vWork->hc_seqcount++ >= HAWLING_SEQ_B )
    		{
    			// �������񉹂��~�߂郂�[�h�ֈڍs
    			OHNO_PRINT("�Q�C���������Ă��܂��K��l�𒴂������Ă��܂��B�n�E�����O�̋��ꂪ����̂ň�U�}�C�N���~�߂܂��B(%d)\n", _vWork->hc_ampgain);
	    		_vWork->hc_state = 3;
	    		_vWork->hc_seqcount = 0;
    		}
    	}
    }
    else
    {
    	if( _vWork->hc_state == 0 )
    	{
    		// ����
    		if( _vWork->hc_seqcount++ >= HAWLING_SEQ_D && (hasGameInput() || _vWork->hc_hawlingflag == 0) ) 
    		{
  				// �Q�C������i�K�グ��
  				if( _vWork->hc_ampgain > 0 )
  				{
	    			change_gain( _vWork->hc_ampgain - 1 );
	    			if( _vWork->hc_hawlingflag == 1 )
	    			{
		    			OHNO_PRINT("��莞�ԋK��������܂���ł����̂ŁA�L�[���͂Ɠ����ɃQ�C���������܂���(%d)\n", _vWork->hc_ampgain);
			    		_vWork->hc_hawlingflag = 0;	    			
	    			}
	    			else
	    			{
		    			OHNO_PRINT("��莞�ԋK��������܂���ł����̂ŁA�Q�C���������܂���(%d)\n", _vWork->hc_ampgain);
	    			}
	    				
  				}
	    		_vWork->hc_seqcount = 0;
    		}
    	}
    	else if( _vWork->hc_state == 1 ) 
    	{
    		// �O��͊�z��
    		_vWork->hc_state = 0; 
    		_vWork->hc_seqcount = 0;    		
    	}
    	else if (_vWork->hc_state == 2 )
    	{
    		if( _vWork->hc_seqcount-- <= 0 )
    		{
	    		_vWork->hc_state = 0; 
	    		_vWork->hc_seqcount = 0;    		
    		}
    	}
    	else if( _vWork->hc_state == 3 )
    	{
    		// �����Ƃ߂Ă���Œ�
    		if( _vWork->hc_seqcount++ >= HAWLING_SEQ_C )
    		{
    			// ���~�ߏI��
    			OHNO_PRINT("�}�C�N���͂𕜋A�����܂�\n");
	    		_vWork->hc_state = 0; 
	    		_vWork->hc_seqcount = 0;        				
    		}
    	}
    }
    
//    OHNO_PRINT("{%d-%d}\n", _vWork->hc_state, _vWork->hc_seqcount);
    
    if( _vWork->hc_state == 3 )
   	{
   		// �����Ƃ߂Ă���Œ�
   		return 1;
   	}
   	
   	return 0;
}
#endif
//
// ���������R�[���o�b�N�iNitroSystem NNS_SndStrm�𗘗p�j
//
static void micCallback(MICResult result, void *arg)
{
#pragma unused(result, arg)
}

static void SndCallback(NNSSndStrmCallbackStatus sts,
                        int nChannels,
                        void* buffer[],
                        u32 length,
                        NNSSndStrmFormat format,
                        void* arg)
{
#pragma unused(format)

	OSTick      start;
    const void *micAddr;
    u32         offset;
    u8*         micSrc;
    u32         ch;

	
    micSrc = (u8*)arg;

 
    
    if (sts == NNS_SND_STRM_CALLBACK_SETUP) {
        for (ch = 0; ch < nChannels; ++ch) {
            MI_CpuClear8(buffer[ch], length);
        }
        return;
    }
    
    if (_vWork->firstCallback) {
        MIC_StartAutoSamplingAsync( &(_vWork->micParam), micCallback, NULL);
        _vWork->firstCallback = 0;
    }
       
    micAddr = MIC_GetLastSamplingAddress();
    offset  = (u32)((u8*)micAddr - micSrc);

    if ( offset < length ) {
        micSrc = micSrc + length;
    }

#if 0    
    MI_CpuCopy8( micSrc, _test_buffer + _test_buffer_index, length );
    _test_buffer_index += length;
#endif


	if( PAD_DetectFold() )
	{
		// DS������ꍇ
   		micSrc = _vWork->sSilentBuffer;
	}

    // ��������M�͎�M�͏�ɍs���܂��BVAD��SSP�̃X�e�[�g�Ǘ��ɂ���āA�Z�b�V����������Ƃ��̂�
    // ���ۂɑ��M�E��M���s���܂��B
    //
    // length�͌��݃I�[�f�B�I�̃T���v�����O���[�g�E�r�b�g���ɉ�����32ms���ɂ̂ݑΉ����Ă��܂��B
    // �i��F8KHz, 8Bit ��256�o�C�g�j�B����ȊO�̃T�C�Y��n����assertion���܂��B
    //
    
    if( _vWork->off_flag == 0 ) 
    {
#if 0	    
    	if( check_hawling((s16*)micSrc, length) )
    	{
    		micSrc = _vWork->sSilentBuffer;
    	}
#endif    	

	    VCT_SendAudio(micSrc, length);
	  
//	    OHNO_PRINT("�� length 0x%x\n", length);  
    }else{
//   	    OHNO_PRINT("�~");
    }  
    for (ch = 0; ch < nChannels; ++ch) {
        if( !VCT_ReceiveAudio(buffer[ch], length, NULL) )
        {
            // �����f�[�^���󂯎��Ȃ������B
            // �o�b�t�@���O�N���A
            //        	MI_CpuClear32(buffer[ch], length);	
            //OHNO_PRINT("�����f�[�^���󂯎��܂���ł����B");
            
        }
    }
#if 0
    if(_vWork->mode != VCT_MODE_CONFERENCE){  // �J���t�@�����X�̏ꍇ���łɉ�b�������ɂ���
        for (ch = 0; ch < nChannels; ++ch) {
            if( !VCT_ReceiveAudio(buffer[ch], length, NULL) )
            {
                // �����f�[�^���󂯎��Ȃ������B
                // �o�b�t�@���O�N���A
                //        	MI_CpuClear32(buffer[ch], length);	
                //OHNO_PRINT("�����f�[�^���󂯎��܂���ł����B");
                
            }
        }
    }
    else{
        // �\�t�g�~�L�T�[���g���ꍇ�́A�P�`�����l���ɂ̂ݏ������݂܂�
        VCT_ReceiveAudio(buffer[0], length, NULL);
        // �c��̃`�����l���ɂ̓R�s�[���������ނ��ƂŁA�Đ����̉����o�̓��x�����グ�Ă��܂�
        // �i�J���t�@�����X���[�h���̂݁j
        if (nChannels > 1) {
            for (ch = 1; ch < nChannels; ++ch) {
                MI_CpuCopyFast(buffer[0], buffer[ch], length);
            }
        }
    }
#endif
    
    
    _vWork->bSendVoice = 2;	  
    return;
}



static void ClearSession(VCTSession *session);

/////////////////////////////////////////////////////////////////////////////////////
//
// �d�b��������B
//

static int startCall( u8 aid )
{
	BOOL ret;
    VCTSession *session;


    if (_vWork->session == NULL) {
        session = VCT_CreateSession(aid);
        if (session == NULL) {
            // �Z�b�V�������g���؂��Ă���A�܂���aid���������g�̏ꍇ
            // CraeteSession �����s���܂�
            OHNO_PRINT("Can't create session!\n");
            return 0;
        }
	
        ret = VCT_Request(session, VCT_REQUEST_INVITE);  //send
    }
    else{
        session = _vWork->session;
        ret = VCT_Request(session, VCT_REQUEST_INVITE);  //������x����
    }
    if (ret != VCT_ERROR_NONE){
        OHNO_PRINT("Can't request Invite [%d]\n", ret);
        VCT_DeleteSession(session);
        _vWork->session = NULL;
        return 0;
    }
    else {
        _vWork->session = session;
    }
    
    return 1;
}

static int receiveCall( u8 aid )
{
	BOOL ret;
	if ( _vWork->session != NULL && _vWork->session->state == VCT_STATE_INCOMING ) {
         // ���̒[�������b�v�����������ꍇ�A������OK��Ԃ��ăX�g���[�~���O���J�n
        
        ret = VCT_Response(_vWork->session, VCT_RESPONSE_OK);
        if (ret != VCT_ERROR_NONE){
            OHNO_PRINT("Can't send response OK [%d]\n", ret);
            return 0;
        }
        
        if( !VCT_StartStreaming(_vWork->session) )
        {
	        OHNO_PRINT("can't start session! %d\n", _vWork->session->aid);
	        return 0;	        
        } else {
   	        OHNO_PRINT("�X�g���[�~���O���J�n���܂�%d\n", _vWork->session->aid);	        
		}
	        
                
        return 1;
    }
    
    return 0;

}


/////////////////////////////////////////////////////////////////////////////////////
//
//  VoiceChat�̃C�x���g�R�[���o�b�N�֐� �J���t�@�����X���[�h
//
static void VoiceChatEventCallbackConference(u8 aid, VCTEvent event, VCTSession *session, void *data)
{
#pragma unused(aid, data)
    
    switch (event) {
    case VCT_EVENT_DISCONNECTED:
        OHNO_PRINT("Disconnected\n");
        ClearSession(session);
        break;
    case VCT_EVENT_CONNECTED:
        VCT_StartStreaming(session);
        OHNO_PRINT("Connected\n");
        break;
    case VCT_EVENT_ABORT:
        OHNO_PRINT("Session abrot\n");
        ClearSession(session);
        break;
    default:
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
// VoiceChat�̃C�x���g�R�[���o�b�N�֐� �d�b���[�h
//
static void VoiceChatEventCallbackPhone(u8 aid, VCTEvent event, VCTSession *session, void *data)
{
#pragma unused(aid, data)
    
    int ret;

    switch (event) {
    case VCT_EVENT_INCOMING:
        if (_vWork->session) {
            OHNO_PRINT("Return busy to %d\n", session->aid);
            ret = VCT_Response(session, VCT_RESPONSE_BUSY_HERE);
            VCT_DeleteSession(session);
            break;
        }
            
        OHNO_PRINT("Invite From %d\n", session->aid);
        _vWork->session = session;
        break;

    case VCT_EVENT_RESPONDBYE:
        OHNO_PRINT("Bye From %d\n", session->aid);
        ret = VCT_Response(session, VCT_RESPONSE_OK);
        if (ret != VCT_ERROR_NONE){
            OHNO_PRINT("Can't send response Ok [%d]\n", ret);
        }
        ClearSession(session);
        mydwc_stopvchat();
        break;

    case VCT_EVENT_DISCONNECTED:
        OHNO_PRINT("Disconnected\n");
        ClearSession(session);
        mydwc_stopvchat();
        break;

    case VCT_EVENT_CANCEL:
        OHNO_PRINT("Cancel From %d\n", session->aid);
        ret = VCT_Response(session, VCT_RESPONSE_TERMINATED);
        if (ret != VCT_ERROR_NONE){
            OHNO_PRINT("Can't send response RequestTerminated [%d]\n", ret);
        }
        ClearSession(session);
        break;

    case VCT_EVENT_CONNECTED:
        OHNO_PRINT("200 OK From %d\n", session->aid);
        if(session->mode != _vWork->mode){
            ClearSession(session);
            OHNO_PRINT("�Z�b�V�������Ⴄ���̂�����\n");
            return;
        }
        if( VCT_StartStreaming(session) )
        {
	        OHNO_PRINT("�X�g���[�~���O���J�n���܂�%d\n", session->aid);	        
	        _vWork->state = VCTSTATE_CALLING;
        } else 
        {
	        OHNO_PRINT("can't start session! %d\n", session->aid);	        
        }
        break;

    case VCT_EVENT_REJECT:
        OHNO_PRINT("Reject From %d\n", session->aid);
        ClearSession(session);
        break;

    case VCT_EVENT_BUSY:
    case VCT_EVENT_TIMEOUT:
    case VCT_EVENT_ABORT:
        OHNO_PRINT("Clear session by '%d'\n", event);
        ClearSession(session);
        break;

    default:
        OHNO_PRINT(" not handler (%d)\n", event);
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
// �Z�b�V�����̃N���A
//
static void ClearSession(VCTSession *session)
{
//    if (session == _vWork->session) {
        VCT_StopStreaming(session);
        VCT_DeleteSession(session);
        _vWork->session = NULL;
//    }
  //  else {
    //    SDK_WARNING(FALSE, "What is this session??\n");
      //  VCT_DeleteSession(session);
//    }
}


#ifdef VCFINFO_SHOW
static int s_count = 0;
#endif

// ���t���[���Ă΂��B
void myvct_main( )
{
    // �Q�[���t���[���Ɉ�x�Ăяo�����C���֐��B
	OSTick      start;
    start = OS_GetTick(); 
//    OHNO_PRINT("VCT_Main[%d]", OS_TicksToMicroSeconds32(start - _tick_time) );
    _difftime += OS_TicksToMicroSeconds32(start - _tick_time) - 1000 * 1000 / 60;
    if( _difftime < -10000 ) _difftime = 0;
    
    _tick_time = start;    
    VCT_Main();
    
    while( _difftime >= 1000 * 1000 / 60 )
    {
//	    OHNO_PRINT("!");
	    VCT_Main();
	    _difftime -= 1000 * 1000 / 60 ;
    }
//    OHNO_PRINT("\n");
	if( _vWork->hc_ampchangeflag )
	{
		switch(_vWork->hc_ampgain)
		{
			case 0:
				PM_SetAmpGain(PM_AMPGAIN_160);
				break;
			case 1:
				PM_SetAmpGain(PM_AMPGAIN_80);
				break;
			case 2:
				PM_SetAmpGain(PM_AMPGAIN_40);
				break;
			case 3:
				PM_SetAmpGain(PM_AMPGAIN_20);
				break;
		}
		_vWork->hc_ampchangeflag = 0;
	}
    
    
#ifdef VCFINFO_SHOW
    if( s_count++ % 60 == 0 )
    {
	    VCTVADInfo outInfo;
	    VCT_GetVADInfo( &outInfo );
	    OHNO_PRINT("VCTVADInfo[%d][%d(%d,%d)]\n", outInfo.activity, outInfo.level, outInfo.Ts, outInfo.Tn);
    }
#endif	    


    if(_vWork->mode != VCT_MODE_CONFERENCE){  // �J���t�@�����X�̏ꍇ���łɉ�b�������ɂ���

//    OHNO_PRINT("st: %d %d\n",_vWork->state, mydwc_getaid());

    switch( _vWork->state )
	{
		case VCTSTATE_INIT:
		{
			if( mydwc_getaid() == 0 ){
				// �e���d�b��������B
				if( startCall(1) ) {
					// ����̔����҂�
					_vWork->state = VCTSTATE_WAIT;
                    _vWork->vctTime = VCT_START_TIMEOUT_NUM;
				}
			}
			else if(mydwc_getaid() == 1)
			{
				// �e����d�b���������Ă���̂�҂�
				if( receiveCall(0) )
				{
					// �������Ă���
					_vWork->state = VCTSTATE_CALLING;
				}
			}
			break;
		}
		
		case VCTSTATE_WAIT:
		 // �q�@���d�b�ɏo��̂�҂��Ă���B
        _vWork->vctTime --;
        if(_vWork->vctTime ==0){
            _vWork->state = VCTSTATE_INIT; //������x�d�b��������
        }
		 break;
		 
		case VCTSTATE_CALLING:
			// �d�b��
			break;
	}
    }
}


BOOL myvct_checkData( int aid, void *data, int size )
{
	if( _vWork == NULL ) return FALSE;
	
	if( VCT_HandleData (aid, data, size ) ){
#ifdef VCFINFO_SHOW
	    OHNO_PRINT(".");
#endif
	    return TRUE;
    }
	return FALSE;
}

static void* AllocFunc( u32 size )
{
	return mydwc_AllocFunc( NULL, size, 32 );
}

static void FreeFunc(void *ptr, u32 size)
{
#pragma unused(size)
	mydwc_FreeFunc( NULL, ptr,  size  );
}


//==============================================================================
/**
 * �A���C�����g�����낦����������Ԃ�
 * @param   none
 * @retval  none
 */
//==============================================================================

static void align32Alloc(void** pOrg,void** pAlign,int size,int heapID)
{
    *pOrg = sys_AllocMemory( heapID, size + 32 );
    MI_CpuClear8(*pOrg, size + 32);
    *pAlign = (MYVCT_WORK *) (( (u32)*pOrg + 31 ) / 32 * 32);
}





void myvct_init( int heapID, int codec,int maxEntry )
{
    u8 cArray[3] = {13, 13, 13};
    u32 length;
    BOOL ret;
    int size;

    OS_TPrintf("myvct%d %d\n",codec,maxEntry);
	if( _vWork == NULL )
	{
		void* vWork_temp=NULL;
        align32Alloc(&vWork_temp, (void**)&_vWork, sizeof(MYVCT_WORK), heapID);
        _vWork->_vWork_temp = vWork_temp;
	
		align32Alloc( (void**)&_vWork->pAudioBufferOrg, (void**)&_vWork->pAudioBuffer,
                      VCT_AUDIO_BUFFER_SIZE * maxEntry * VCT_DEFAULT_AUDIO_BUFFER_COUNT + 32 , heapID);

	    _vWork->heapID = heapID;
		_vWork->disconnectCallback = NULL;
		
		InitFirst();
	}
	
	// �}�C�N�̏�����
	
    length = (u32)(VCHAT_SAMPLING_RATE * VCT_AUDIO_FRAME_LENGTH * SAMPLING_BYTE) / 1000;
    
    {
	    _vWork->micParam.type   = MIC_SAMPLING_TYPE_SIGNED_12BIT;
	    _vWork->micParam.buffer = _vWork->sRecBuffer;
	    _vWork->micParam.size   = length * 2;
	    _vWork->micParam.rate = (u32)((NNS_SND_STRM_TIMER_CLOCK / VCHAT_SAMPLING_RATE) * 64);
	    _vWork->micParam.loop_enable = TRUE;
	    _vWork->micParam.full_callback = NULL;
	    _vWork->micParam.full_arg = NULL;
	    _vWork->firstCallback = 1;
    }
        
    // �T�E���h�X�g���[���Đ��̏������B�P�΂P��b����
    NNS_SndStrmAllocChannel(&_vWork->sSndStream, 1, cArray);
    NNS_SndStrmSetVolume(&_vWork->sSndStream, 0);

    
    ret = NNS_SndStrmSetup(&_vWork->sSndStream,
                           NNS_SND_STRM_FORMAT_PCM16,
                           _vWork->sPlayBuffer,
                           length * 2 * 1,
                           NNS_SND_STRM_TIMER_CLOCK / VCHAT_SAMPLING_RATE,
                           2,
                           SndCallback,
                           _vWork->sRecBuffer);
    SDK_ASSERT(ret);
    
    _vWork->state = VCTSTATE_INIT;
	_vWork->session = NULL;
		
	{
	    VCTConfig config;
	    
        if(!CommLocalIsWiFiQuartetGroup(CommStateGetServiceNo())){   // �S�l�ڑ��̎��̓{�C�X�`���b�g�������N�����Ȃ�
            config.mode         = VCT_MODE_PHONE;
        }
        else{
            config.mode         = VCT_MODE_CONFERENCE;
        }
        _vWork->mode = config.mode;
	    config.session      = _vWork->sSession;
	    config.numSession   = maxEntry;
	    config.aid          = DWC_GetMyAID();
        GF_ASSERT((config.aid != -1));
        if(_vWork->mode == VCT_MODE_CONFERENCE){
            config.callback = VoiceChatEventCallbackConference;
        }
        else{
            config.callback = VoiceChatEventCallbackPhone;
        }
	    config.userData     = NULL;

		config.audioBuffer     = _vWork->pAudioBuffer;
		config.audioBufferSize = VCT_AUDIO_BUFFER_SIZE * maxEntry * VCT_DEFAULT_AUDIO_BUFFER_COUNT + 32;

	    
	    if (!VCT_Init(&config)) {
	        OHNO_PRINT("ERROR: Can't initialize VoiceChat!!!\n");
	    }	    
	}

	_vWork->off_flag = 0;
	
	VCT_SetCodec( codec );
	StartSoundLoop();
	
    VCT_EnableEchoCancel( TRUE );

   OS_TPrintf("myvct_init\n"); 
    return;	
}

static void StartSoundLoop()
{
    NNS_SndStrmStart(&_vWork->sSndStream);
}

void myvct_setCodec( int codec )
{
	VCT_SetCodec( codec );
}




//==============================================================================
/**
 * ��b�I���v���������܂��B�܂��ʘb�ł��Ă��Ȃ��Ƃ��͑����ɏI�����܂��B
 * myvct_setDisconnectCallback�Őݒ肳�ꂽ�R�[���o�b�N���Ăяo����܂��B
 * @param   none
 * @retval  none
 */
//==============================================================================
void myvct_endConnection(){
	int ret;
	
	// �܂���b�v�����o���O�ŁA�󂯎��O
	if( _vWork->session == NULL || _vWork->state == VCTSTATE_INIT ) {
		mydwc_stopvchat();
		return;
	}
	
	if( _vWork->state == VCTSTATE_WAIT )
	{
		// ��b�v�����o���Ă܂��Ԏ�������O�B
		ret = VCT_Request( _vWork->session, VCT_REQUEST_CANCEL );
        if (ret != VCT_ERROR_NONE){
			OHNO_PRINT("Can't request Cancel [%d]\n", ret);
			mydwc_stopvchat();
			return;
		}			
	}

	// ��b���B��b�I���v�����o���B
	ret = VCT_Request( _vWork->session, VCT_REQUEST_BYE );
    if (ret != VCT_ERROR_NONE){
		OHNO_PRINT("Can't request Bye [%d]\n", ret);
		mydwc_stopvchat();
		return;
	}
	return;
}

//==============================================================================
/**
 * ��b�I�����������R�[���o�b�N��ݒ肵�܂��B
 * ���肩��؂�ꂽ�ꍇ���Ăяo����܂��B
 * ���̊֐����Ăяo����钼�O�ɁAvchat.c�p�̃��[�N���������܂��B
 * @param   none
 * @retval  none
 */
//==============================================================================
void myvct_setDisconnectCallback( void (*disconnectCallback)() )
{
	_vWork->disconnectCallback = disconnectCallback;
}

//==============================================================================
/**
 * ���C�u�����I������
 * @param   none
 * @retval  none
 */
//==============================================================================
void myvct_free(void){
	void (*callback)();
	
	if( _vWork != NULL )
	{
		callback = _vWork->disconnectCallback;
		
		// �}�C�N�̃T���v�����O�ƃX�g���[�����Ƃ߂�B
	    (void)MIC_StopAutoSampling();
	    NNS_SndStrmStop(&_vWork->sSndStream);	
		NNS_SndStrmFreeChannel(&_vWork->sSndStream);
	
		// VCT���C�u�����I������	
		VCT_Cleanup();
		
		// �����������
        sys_FreeMemory( _vWork->heapID, _vWork->pAudioBufferOrg );
        sys_FreeMemory( _vWork->heapID, _vWork->_vWork_temp  );
		_vWork = NULL;
		
		// �R�[���o�b�N�̌Ăяo���B
		if( callback != NULL ) callback();
	}
}

//==============================================================================
/**
 * �����E�����̂��ǂ����𒲂ׂ�
 * @param   
 * @retval  �E������TRUE
 */
//==============================================================================

BOOL myvct_IsSendVoiceAndInc(void)
{
    if(_vWork){
        VCTVADInfo outInfo;
        VCT_GetVADInfo( &outInfo );
        if(outInfo.scale > 2){
            return outInfo.activity;
        }
    }
    return FALSE;
}

void myvct_offVchat()
{
	_vWork->off_flag = 1;	
}

void myvct_onVchat()
{
	_vWork->off_flag = 0;	
}

BOOL myvct_isVchatOn(){
	return ( _vWork->off_flag == 0 );
}



//==============================================================================
/**
 * VCT�J���t�@�����X�ɏ��҂���
 * @param   bitmap   �ڑ����Ă���CLIENT��BITMAP
 * @param   myAid    ������ID
 * @retval  TRUE �ݒ�ł���
 * @retval  FALSE ����̂܂� �������͕K�v���Ȃ�
 */
//==============================================================================

BOOL myvct_AddConference(int bitmap, int myAid)
{
    int i,ret;
    
    if(!(_vWork)  || (_vWork->mode != VCT_MODE_CONFERENCE)){
        return FALSE;
    }

    for(i = 0; i < _MAX_PLAYER_NUM;i++){
        if(i == myAid){
            continue;
        }
        if( bitmap & (1<<i)){  //����ꍇ
            if(_vWork->bConf[i]==TRUE){
                continue;
            }
            ret = VCT_AddConferenceClient(i);
            if (ret != VCT_ERROR_NONE){
                OHNO_PRINT("AddConferenceError [%d]\n", ret);
                return FALSE;
            }
            else{
                _vWork->bConf[i] = TRUE;
                OHNO_PRINT("AddConference�ݒ� %d\n", i);
            }
        }
    }
    return TRUE;
}

//==============================================================================
/**
 * VCT�J���t�@�����X����S���͂���
 * @param   bitmap   �ڑ����Ă���CLIENT��BITMAP
 * @param   myAid    ������ID
 * @retval  TRUE �ݒ�ł���
 * @retval  FALSE ����̂܂� �������͕K�v���Ȃ�
 */
//==============================================================================

BOOL myvct_DelConference(int myAid)
{
    int i,ret;
    
    if(!(_vWork)  || (_vWork->mode != VCT_MODE_CONFERENCE)){
        return FALSE;
    }

    for(i = 0; i < _MAX_PLAYER_NUM;i++){
        if(i == myAid){
            continue;
        }
        if(_vWork->bConf[i]==TRUE){
            ret = VCT_RemoveConferenceClient(i);
            if (ret != VCT_ERROR_NONE){
                OHNO_PRINT("DELConferenceError [%d]\n", ret);
                return FALSE;
            }
            else{
                _vWork->bConf[i] = FALSE;
            }
        }
    }
    return TRUE;
}

