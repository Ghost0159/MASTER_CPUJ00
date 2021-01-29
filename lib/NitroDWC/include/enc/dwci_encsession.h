/*---------------------------------------------------------------------------*
  Project:  RevolutionDWC Libraries
  File:     ./include/enc/dwci_encsession.h

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
 * @brief HTTP�ʐM�̃��b�p�[���[�e�B���e�B�[ �w�b�_
 */

#ifndef DWC_ENCSESSION_H_
#define DWC_ENCSESSION_H_

#ifdef __cplusplus
extern "C"
{
#endif


    /* -------------------------------------------------------------------------
            enum
       ------------------------------------------------------------------------- */

/// �Z�b�V�����X�e�[�^�X��\���񋓌^
    typedef enum{

        DWCi_ENC_SESSION_STATE_CANCELED,		//!< �L�����Z��
        DWCi_ENC_SESSION_STATE_ERROR,			//!< �G���[����
        DWCi_ENC_SESSION_STATE_INITIAL,			//!< �������
        DWCi_ENC_SESSION_STATE_INITIALIZED,		//!< ��������
        DWCi_ENC_SESSION_STATE_REQUEST,			//!< ���N�G�X�g���s
        DWCi_ENC_SESSION_STATE_GETTING_TOKEN,	//!< �g�[�N���擾��
        DWCi_ENC_SESSION_STATE_GOT_TOKEN,		//!< �g�[�N���擾����
        DWCi_ENC_SESSION_STATE_SENDING_DATA,	//!< �f�[�^���M��
        DWCi_ENC_SESSION_STATE_COMPLETED		//!< ����

    }DWCiEncSessionState;

/// �֐��Ԃ�l
    typedef enum{

        DWCi_ENC_SESSION_SUCCESS,				//!< ����I��
        DWCi_ENC_SESSION_ERROR_NOTINITIALIZED,	//!< ��������
        DWCi_ENC_SESSION_ERROR_NOMEMORY,		//!< �������[�s��
        DWCi_ENC_SESSION_ERROR_INVALID_KEY		//!< �s���ȃL�[

    }DWCiEncSessionResult;

/// �ڑ���T�[�o�[��\���񋓌^
    typedef enum{

        DWCi_ENC_SERVER_RELEASE,    //!< ���i�p�T�[�o�[�ɐڑ����܂�
        DWCi_ENC_SERVER_DEBUG,      //!< �J���p�T�[�o�[�ɐڑ����܂�
        DWCi_ENC_SERVER_TEST        //!< (�g�p���Ȃ��ł�������)

    }DWCiEncServer;


    /* -------------------------------------------------------------------------
            function type
       ------------------------------------------------------------------------- */

/// ��M�f�[�^�����؂��邽�߂̃R�[���o�b�N
    typedef BOOL (*DWCiEncSessionVaridateCallback)( u32* buf, int len );

    /* -------------------------------------------------------------------------
            prototype
       ------------------------------------------------------------------------- */

    void DWCi_EncSessionInitialize( DWCiEncServer server, const char* initdata );

    void DWCi_EncSessionShutdown( void );

    DWCiEncSessionResult DWCi_EncSessionGetAsync( 	const char* url,
            s32 pid,
            void* data,
            u32 size,
            DWCiEncSessionVaridateCallback cb );

    DWCiEncSessionResult DWCi_EncSessionGetReuseHashAsync(	const char* url,
        s32 pid,
        void* data,
        u32 size,
        DWCiEncSessionVaridateCallback cb );

    DWCiEncSessionState DWCi_EncSessionProcess( void );

    void* DWCi_EncSessionGetResponse( u32* size );

    void DWCi_EncSessionCancel( void );


#ifdef __cplusplus
}
#endif


#endif
