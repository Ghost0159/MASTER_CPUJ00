/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./build/lobby/include/ppw_timer.h

  Copyright 2005-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/
/**
 * @file
 *
 * @brief encsession C++���b�p�[
 */

#ifndef DWCi_ENCSESSIONCPP_H_
#define DWCi_ENCSESSIONCPP_H_

#include <dwc.h>
#include <enc/dwci_encsession.h>
#include "dwci_lobbyUtil.h"

/**
 * @brief �f�[�^�̑���M�����������Ƃ��ɌĂяo����܂��B
 * 
 * �f�[�^�̑���M�����������Ƃ��ɌĂяo����܂��B�{�R�[���o�b�N����DWCi_EncSession::StartSessionAsync�֐����Ăяo�����Ƃ��ł��܂��B
 */
typedef void (*DWCi_EncSessionCompletedCallback)(BOOL success, const u8* data, u32 size, void* param);

/**
 * @brief encsession C++���b�p�[
 */
class DWCi_EncSession : public DWCi_Base
{
    static DWCi_EncSession* session;
    static DWCi_EncSession* endSession; // �ڑ����I�����Ă��Ƃ͍폜���邾���̂���
    
    u32 retryCount;
    DWCiEncServer server;
    DWCi_String initData;
    DWCi_String url;
    s32 pid;
    std::vector<u8, DWCi_Allocator<u8> > data;
    DWCi_EncSessionCompletedCallback callback;
    void* param;
    BOOL reuseHash;
    
    DWCi_EncSession(DWCiEncServer server_, const char* initData_, const char* url_, s32 pid_, const u8* data_, u32 dataSize_,
                    u32 retryCount_, DWCi_EncSessionCompletedCallback callback_, void* param_, BOOL reuseHash_ )
        : retryCount(retryCount_)
        , server(server_)
        , initData(initData_)
        , url(url_)
        , pid(pid_)
        , data()
        , callback(callback_)
        , param(param_)
        , reuseHash(reuseHash_)
    {
        data.assign(data_, data_ + dataSize_);
    }
    
    virtual ~DWCi_EncSession()
    {
    }
    
    BOOL PrivateStartSessionAsync()
    {
        DWCi_EncSessionInitialize( server, initData.c_str() );
        DWC_Printf(DWC_REPORTFLAG_INFO, "Requesting (%s_SERVER)%s\n", server == DWCi_ENC_SERVER_DEBUG ? "DEBUG" : "RELEASE", url.c_str());
        DWCiEncSessionResult result;
        if(reuseHash)
        {
    	    result = DWCi_EncSessionGetReuseHashAsync( url.c_str(), pid, (void*)DWCi_GetVectorBuffer(data), data.size(), NULL );
        }
        else
        {
    	    result = DWCi_EncSessionGetAsync( url.c_str(), pid, (void*)DWCi_GetVectorBuffer(data), data.size(), NULL );
        }
        if(result != DWCi_ENC_SESSION_SUCCESS)
        {
            switch(result)
            {
            case DWCi_ENC_SESSION_ERROR_NOTINITIALIZED :
                DWC_Printf(DWC_REPORTFLAG_ERROR, "PrivateStartSessionAsync: Internal Error: DWCi_ENC_SESSION_ERROR_NOTINITIALIZED\n");
                break;
            case DWCi_ENC_SESSION_ERROR_NOMEMORY:
                DWC_Printf(DWC_REPORTFLAG_ERROR, "PrivateStartSessionAsync: DWCi_ENC_SESSION_ERROR_NOMEMORY\n");
                break;
            case DWCi_ENC_SESSION_ERROR_INVALID_KEY:
                DWC_Printf(DWC_REPORTFLAG_ERROR, "PrivateStartSessionAsync: DWCi_ENC_SESSION_ERROR_INVALID_KEY\n");
                break;
            default:
                DWC_Printf(DWC_REPORTFLAG_ERROR, "PrivateStartSessionAsync: Unexpected Error\n");
                break;
            }
            DWCi_EncSessionShutdown();
            return FALSE;
        }
        return TRUE;
    }
    
    BOOL PrivateProcess()
    {
        void* buf = NULL;
        u32 recvSize = 0;
	    DWCiEncSessionState state = DWCi_EncSessionProcess();

		switch( state )
		{
		case DWCi_ENC_SESSION_STATE_COMPLETED:		// ����
        {
            void* ptr;
            buf = DWCi_EncSessionGetResponse( &recvSize );
            
            // �V���b�g�_�E����ɃR�[���o�b�N���ĂԕK�v�����邽�߃o�b�t�@���R�s�[
            ptr = DWC_Alloc((DWCAllocType)0, recvSize);
            if(ptr)
            {
                DWCi_Np_CpuCopy8(buf, ptr, recvSize);
            }
            
            if(ptr)
            {
                FinalizeSession(TRUE, (u8*)ptr, recvSize, param);
            }
            else
            {
                DWC_Printf(DWC_REPORTFLAG_ERROR, "PrivateProcess: No memory.\n");
                FinalizeSession(FALSE, NULL, 0, param);
            }
            DWC_SAFE_FREE(ptr);
            return TRUE;
        }
        case DWCi_ENC_SESSION_STATE_ERROR:			// �G���[����
            DWC_ClearError();
            if(--retryCount > 0)
            {
                DWCi_EncSessionShutdown();
                if(PrivateStartSessionAsync())
                {
                    // ���g���C�ő��s
                    return FALSE;
                }
            }
            
            FinalizeSession(FALSE, NULL, 0, param);
            return TRUE;
        case DWCi_ENC_SESSION_STATE_CANCELED:		// �L�����Z��
            DWC_ClearError();
            
            FinalizeSession(FALSE, NULL, 0, param);
            return TRUE;
        default:
            return FALSE;
		}
    }
    
    void FinalizeSession(BOOL success, const u8* data, u32 size, void* param)
    {
        DWCi_EncSessionShutdown();
        
        // �R�[���o�b�N����StartSessionAsync���Ăׂ�悤�ɑҔ�����B
        std::swap(endSession, session);
        callback(success, data, size, param);
    }
    
public:
/**
 * @brief �f�[�^�̑���M���J�n���܂��B
 * 
 * �f�[�^�̑���M���J�n���܂��B��������callback�Ŏw�肵���R�[���o�b�N���Ă΂�܂��B
 * 
 * @param retryCount ���s����񐔁B1�ȏ���w�肵�Ă��������B
 * 
 * @retval TRUE �������J�n����܂����B
 * @retval FALSE �������J�n����܂���ł����B�O��̏������p�������A�������s���ł��B
 */
    static BOOL StartSessionAsync(DWCiEncServer server, const char* initData, const char* url, s32 pid,
                             const u8* data, u32 dataSize, u32 retryCount, DWCi_EncSessionCompletedCallback callback, void* param, BOOL reuseHash=FALSE)
    {
        if(session)
        {
            DWC_Printf(DWC_REPORTFLAG_ERROR, "StartSessionAsync: Last session exists.\n");
            return FALSE;
        }
        session = new DWCi_EncSession( server, initData, url, pid, data, dataSize, retryCount, callback, param, reuseHash );
        BOOL result = session->PrivateStartSessionAsync();
        if(!result)
        {
            DWC_SAFE_DELETE(session);
        }
        return result;
    }
    
/**
 * @brief ����M�������s���܂��B
 * 
 * @retval �ʐM�������s���Ă��邩�B
 * 
 * ����M�������s���܂��BDWCi_EncSessionCompletedCallback�R�[���o�b�N���Ă΂��܂Ŗ��Q�[���t���[�����x�̕p�x�ŌĂяo���Ă��������B
 */
    static BOOL Process()
    {
        if(session)
        {
            if(session->PrivateProcess())
            {
                DWC_SAFE_DELETE(endSession);
            }
            return TRUE;
        }
        return FALSE;
    }
};

#endif
