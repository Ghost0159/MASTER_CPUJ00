#include <nitro.h>

// the original header
//---------------------------------------------------------
#include <base/dwc_report.h>
#include <base/dwc_error.h>
#include <base/dwc_memfunc.h>
#include <base/dwc_ghttp.h>

// �\���̐錾
//---------------------------------------------------------
typedef struct DWCGHTTPParamEntry {
	DWCGHTTPParam param;
	char *buf;
	int req;
	struct DWCGHTTPParamEntry *nextentry;
}DWCGHTTPParamEntry;

// �����ϐ��錾
//---------------------------------------------------------
static DWCGHTTPParamEntry *paramhead = NULL;
static int ghttpinitcount = 0;

// �v���g�^�C�v�錾
//---------------------------------------------------------
static GHTTPBool GHTTPCompletedCallback( GHTTPRequest   request,
										 GHTTPResult	result,
										 char *			buffer,
										 GHTTPByteCount	bufferLen,
										 void *			param);

static void GHTTPProgressCallback(GHTTPRequest   request, 
                                  GHTTPState     state, 
                                  const char *   buffer, 
                                  GHTTPByteCount bufferLen, 
                                  GHTTPByteCount bytesReceived, 
                                  GHTTPByteCount totalSize, 
                                  void *         param );

static DWCGHTTPResult DWCi_HandleGHTTPError(DWCGHTTPResult result);

static DWCGHTTPParamEntry *DWCi_AppendDWCGHTTPParam(const DWCGHTTPParam *param);
static void DWCi_RemoveDWCGHTTPParamEntry(DWCGHTTPParamEntry *entry);
static void DWCi_RemoveAllDWCGHTTPParamEntry(void);
static DWCGHTTPParamEntry *DWCi_FindDWCGHTTPParamEntryByReq(int req);

//=============================================================================
/*!
 *	@brief	���C�u�����̏�����
 *
 *	@param	gamename	[in] �Q�[����
 *
 *	@retval	TRUE		����
 *	@retval FALSE   	���s
 */
//=============================================================================
BOOL	DWC_InitGHTTP( const char* gamename ){
#pragma unused(gamename)

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_InitGHTTP\n");

    //
	// GHTTP �̊J�n
	//
	ghttpStartup();
	
	ghttpinitcount++;
	
	return TRUE;

}

//=============================================================================
/*!
 *	@brief	���C�u�����̃V���b�g�_�E��
 *
 *	@retval	TRUE		����
 *	@retval	FALSE   	���s
 */
//=============================================================================
BOOL	DWC_ShutdownGHTTP( void )
{
	DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_ShutdownGHTTP\n");
	if(ghttpinitcount <= 0)
		return TRUE;
	
	//
	// GHTTP �̏I��
	//
	ghttpCleanup();
	
	//
	// �p�����[�^�����
	//
	ghttpinitcount--;
	if(ghttpinitcount == 0)
		DWCi_RemoveAllDWCGHTTPParamEntry();

	return TRUE;
}


//=============================================================================
/*!
 *	@brief	�f�[�^�ʐM
 *
 *	@retval	TRUE		����
 *	@retval	FALSE   	���s
 */
//=============================================================================
BOOL	DWC_ProcessGHTTP( void ){

    if (DWCi_IsError()) return FALSE;

	//
	// GHTTP �̃��X�|���X�҂�
	//
	ghttpThink();

	return TRUE;

}

//=============================================================================
/*!
 *	@brief	HTTP���N�G�X�g�̃R�[���o�b�N
 *
 *	@retval	GHTTPTrue		�o�b�t�@�J��
 *	@retval	GHTTPFalse   	�o�b�t�@�J������
 */
//=============================================================================
static GHTTPBool GHTTPCompletedCallback( GHTTPRequest   request,
										 GHTTPResult	result,
										 char *			buffer,
										 GHTTPByteCount	bufferLen,
										 void *			param)
{
#pragma unused(request)

    DWCGHTTPParamEntry *entry = (DWCGHTTPParamEntry *)param;
	DWCGHTTPParam *parameter = &entry->param;

	// �R�[���o�b�N�֐��̃|�C���^���擾
	DWCGHTTPCompletedCallback callback = (DWCGHTTPCompletedCallback)parameter->completedCallback;

    BOOL buffer_clear = parameter->buffer_clear;

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "GHTTPCompleteCallback result : %d\n", result);

    if(callback){
        if(result == GHTTPSuccess){
            callback( buffer, (int)bufferLen, (DWCGHTTPResult)result, parameter->param );
        }else{
            DWCi_HandleGHTTPError((DWCGHTTPResult)result);
            callback( NULL, 0, (DWCGHTTPResult)result, parameter->param );
        }
    }
    else{
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Callback is NULL\n");
    }
	
	// �o�b�t�@�����b�p�Ŋm�ۂ��ꂽ���̂Ȃ�������
	if(result != GHTTPSuccess || // GHTTP���s�̏ꍇ�̓o�b�t�@�𖳏����Ŏ����I�ɊJ������(�R�[���o�b�N��buf��NULL�̂���)
	   buffer_clear == TRUE // �o�b�t�@�N���A�t���O��TRUE�̏ꍇ�͎����I�ɉ������
	  ) {
		if(entry->buf)
			DWC_Free(DWC_ALLOCTYPE_BASE, (void*)entry->buf, 0);
		else
			buffer_clear = TRUE;
	}

	//DWC_Free(DWC_ALLOCTYPE_BASE, (void*)parameter, 0);
    DWCi_RemoveDWCGHTTPParamEntry(entry);
	
    if(!buffer_clear){
        return GHTTPFalse;
    }

	return GHTTPTrue;
}

//=============================================================================
/*!
 *	@brief	HTTP���N�G�X�g���̃R�[���o�b�N
 */
//=============================================================================
static void GHTTPProgressCallback(GHTTPRequest   request, 
                                  GHTTPState     state, 
                                  const char *   buffer, 
                                  GHTTPByteCount bufferLen, 
                                  GHTTPByteCount bytesReceived, 
                                  GHTTPByteCount totalSize, 
                                  void *         param )
{
#pragma unused(request)

    DWCGHTTPParam* parameter = &((DWCGHTTPParamEntry *)param)->param;

	// �R�[���o�b�N�֐��̃|�C���^���擾
	DWCGHTTPProgressCallback callback = (DWCGHTTPProgressCallback)parameter->progressCallback;

    if(callback){
        callback( (DWCGHTTPState)state, buffer, (int)bufferLen, (int)bytesReceived, (int)totalSize, parameter->param );
    }

}

//=============================================================================
/*!
 *	@brief	post����I�u�W�F�N�g�̍쐬
 *
 *	@param	post	  [in] �쐬�����I�u�W�F�N�g
 *
 */
//=============================================================================
void    DWC_GHTTPNewPost( DWCGHTTPPost* post ){

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_GHTTPNewPost\n");
    
    *post = ghttpNewPost();

    if (post == NULL){
        DWCi_HandleGHTTPError((DWCGHTTPResult)DWC_GHTTP_INSUFFICIENT_MEMORY);
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_Alloc Error\n");
    }
}

//=============================================================================
/*!
 *	@brief	Addpost�p
 *
 *	@param	post	  [in] Add�����I�u�W�F�N�g
 *	@param	key 	  [in] post����f�[�^��key��
 *	@param	buf 	  [in] post����f�[�^�̓��e
 *
 *	@retval	TRUE		����
 *	@retval	FALSE   	���s
 */
//=============================================================================
BOOL    DWC_GHTTPPostAddString( DWCGHTTPPost* post, const char* key, const char* value ){
    
    if(!ghttpPostAddString(*post, key, value)){
        DWC_Printf(DWC_REPORTFLAG_WARNING, "DWC_GHTTPPostAddString FALSE\n");
        return FALSE;
    }
    else {
        return TRUE;
    }
}

//=============================================================================
/*!
 *	@brief	Addpost�p
 *
 *	@param	post	    [in] Add�����I�u�W�F�N�g
 *	@param	key 	    [in] post����f�[�^��key��
 *	@param	buffer 	    [in] post����f�[�^�̓��e
 *	@param  bufferlen   [in] post����f�[�^�̒���
 *	@param  filename    [in] post����f�[�^�̃t�@�C���l�[��(�T�[�o���Ŏ擾�ł���)
 *	@param  contentType [in] post����f�[�^�̃R���e���g�^�C�v(�T�[�o���Ŏ擾�ł���)
 *
 *	@retval	TRUE		����
 *	@retval	FALSE   	���s
 */
//=============================================================================
BOOL    DWC_GHTTPPostAddFileFromMemory( DWCGHTTPPost *post, const char *key, const char *buffer, int bufferlen, const char *filename, const char *contentType ){
  if (! ghttpPostAddFileFromMemory(*post, key, buffer, bufferlen, filename, contentType)) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "DWC_GHTTPPostAddFileFromMemory FALSE\n");
        return FALSE;
    }
    else {
        return TRUE;
    }
}



//=============================================================================
/*!
 *	@brief	�f�[�^�̑��M
 *
 *	@param	url		[in] ���X�g�擾��̂t�q�k
 *	@param	post	[in] post����I�u�W�F�N�g
 *	@param	proc	[in] ���X�|���X����������R�[���o�b�N
 *	@param	param	[in] �R�[���o�b�N�p�p�����[�^
 *
 *	@retval	req		 0�ȏ� ���N�G�X�g���ʎq�i�����j
 *                  -1�ȉ� ���N�G�X�g�G���[�l
 */
//=============================================================================
int 	DWC_PostGHTTPData( const char* url, GHTTPPost* post, DWCGHTTPCompletedCallback completedCallback, void* param){

   	GHTTPRequest req;
	DWCGHTTPParamEntry *entry = NULL;
	DWCGHTTPParam parameter_instance;

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_PostGHTTPData\n");

    if (DWCi_IsError()) return DWC_GHTTP_IN_ERROR;
    
    parameter_instance.param = param;
    parameter_instance.completedCallback = completedCallback;
    parameter_instance.progressCallback = NULL;
    parameter_instance.buffer_clear = TRUE;

	//parameter = DWC_Alloc(DWC_ALLOCTYPE_BASE, sizeof(DWCGHTTPParam));
    entry = DWCi_AppendDWCGHTTPParam(&parameter_instance);
    
    if(!entry){
        DWCi_HandleGHTTPError((DWCGHTTPResult)DWC_GHTTP_INSUFFICIENT_MEMORY);
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_Alloc Error\n");
        completedCallback( NULL, 0, (DWCGHTTPResult)DWC_GHTTP_INSUFFICIENT_MEMORY, param );
        return (int)DWC_GHTTP_INSUFFICIENT_MEMORY;
    }
    
    //parameter->param = param;
    //parameter->completedCallback = completedCallback;
    //parameter->progressCallback = NULL;
    //parameter->buffer_clear = TRUE;

	req = ghttpPost( url, *post, GHTTPFalse, 
                      GHTTPCompletedCallback, (void *)entry );
   
    if(req < 0){
        DWCi_HandleGHTTPError((DWCGHTTPResult)req);
        completedCallback( NULL, 0, (DWCGHTTPResult)req, param );
		DWCi_RemoveDWCGHTTPParamEntry(entry);
    }
	
	entry->req = req;

	return (int)req;

}

//=============================================================================
/*!
 *	@brief	�f�[�^�̎�M
 *
 *	@param	url		[in] ���X�g�擾��̂t�q�k
 *	@param	proc	[in] ���X�|���X����������R�[���o�b�N
 *	@param	param	[in] �R�[���o�b�N�p�p�����[�^
 *
 *	@retval	req		 0�ȏ� ���N�G�X�g���ʎq�i�����j
 *                  -1�ȉ� ���N�G�X�g�G���[�l
 */
//=============================================================================
int 	DWC_GetGHTTPData( const char* url, DWCGHTTPCompletedCallback completedCallback, void* param ){
    
   	GHTTPRequest req;
    DWCGHTTPParamEntry *entry = NULL;
	DWCGHTTPParam parameter_instance;
	
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_GetGHTTPData\n");

    if (DWCi_IsError()) return DWC_GHTTP_IN_ERROR;
    
    parameter_instance.param = param;
    parameter_instance.completedCallback = completedCallback;
    parameter_instance.progressCallback  = NULL;
    parameter_instance.buffer_clear = TRUE;

	entry = DWCi_AppendDWCGHTTPParam(&parameter_instance);
    
    if(!entry){
        DWCi_HandleGHTTPError((DWCGHTTPResult)DWC_GHTTP_INSUFFICIENT_MEMORY);
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_Alloc Error\n");
        completedCallback( NULL, 0, (DWCGHTTPResult)DWC_GHTTP_INSUFFICIENT_MEMORY, param );
        return (int)DWC_GHTTP_INSUFFICIENT_MEMORY;
    }

    req = ghttpGet( url, GHTTPFalse, GHTTPCompletedCallback, (void *)entry );

    if(req < 0){
        DWCi_HandleGHTTPError((DWCGHTTPResult)req);
        completedCallback( NULL, 0, (DWCGHTTPResult)req, param );
        DWCi_RemoveDWCGHTTPParamEntry(entry);
    }
    
	entry->req = req;
	
	return (int)req;

}

//=============================================================================
/*!
 *	@brief	�f�[�^�̎�M(�o�b�t�@�T�C�Y�w��AProgressCallback�w��)
 *
 *	@param	url		            [in] ���X�g�擾��̂t�q�k
 *	@param	bufferlen           [in] ��M�o�b�t�@�̃T�C�Y
 *	@param	post	[in] post����I�u�W�F�N�g
 *	@param	progressCallback	[in] �������ɌĂ΂��R�[���o�b�N
 *  @param  completedCallback   [in] ���X�|���X����������R�[���o�b�N
 *	@param	param	            [in] �R�[���o�b�N�p�p�����[�^
 *
 *	@retval	req		 0�ȏ� ���N�G�X�g���ʎq�i�����j
 *                  -1�ȉ� ���N�G�X�g�G���[�l
 */
//=============================================================================
static int 	DWCi_GHTTPGetEx( const char* url, int bufferlen, BOOL buffer_clear, DWCGHTTPPost *post, DWCGHTTPProgressCallback progressCallback, DWCGHTTPCompletedCallback completedCallback, void* param ){

    char* buffer = NULL;
   	GHTTPRequest req;
    DWCGHTTPParamEntry *entry = NULL;
	DWCGHTTPParam parameter_instance;

    if (DWCi_IsError()) return DWC_GHTTP_IN_ERROR;

	parameter_instance.param = param;
    parameter_instance.completedCallback = completedCallback;
    parameter_instance.progressCallback  = progressCallback;
    parameter_instance.buffer_clear = buffer_clear;

    //parameter = DWC_Alloc(DWC_ALLOCTYPE_BASE, sizeof(DWCGHTTPParam));
   	entry = DWCi_AppendDWCGHTTPParam(&parameter_instance);

    if(!entry){
        DWCi_HandleGHTTPError((DWCGHTTPResult)DWC_GHTTP_INSUFFICIENT_MEMORY);
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_Alloc Error\n");
        completedCallback( NULL, 0, (DWCGHTTPResult)DWC_GHTTP_INSUFFICIENT_MEMORY, param );
        return (int)DWC_GHTTP_INSUFFICIENT_MEMORY;
    }

    if(bufferlen > 0){
        buffer = (char*)DWC_Alloc(DWC_ALLOCTYPE_BASE, (u32)bufferlen);

        if(!buffer){
            DWCi_HandleGHTTPError((DWCGHTTPResult)DWC_GHTTP_INSUFFICIENT_MEMORY);
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_Alloc Error\n");
            completedCallback( NULL, 0, (DWCGHTTPResult)DWC_GHTTP_INSUFFICIENT_MEMORY, param );
			DWCi_RemoveDWCGHTTPParamEntry(entry);
            return (int)DWC_GHTTP_INSUFFICIENT_MEMORY;
        }
		
		// entry�Ƀ��b�p�ɂ���ăo�b�t�@���m�ۂ��ꂽ���Ƃ��L�^
		entry->buf = buffer;
    }

    //parameter->param = param;
    //parameter->completedCallback = completedCallback;
    //parameter->progressCallback  = progressCallback;
    //parameter->buffer_clear = buffer_clear;

    if(post)
        req = ghttpGetEx( url, NULL, buffer, bufferlen, *post, GHTTPFalse, GHTTPFalse,
                          GHTTPProgressCallback, GHTTPCompletedCallback, (void *)entry );
    else
        req = ghttpGetEx( url, NULL, buffer, bufferlen, NULL, GHTTPFalse, GHTTPFalse,
                          GHTTPProgressCallback, GHTTPCompletedCallback, (void *)entry );
        
    if(req < 0){
        DWCi_HandleGHTTPError((DWCGHTTPResult)req);
        completedCallback( NULL, 0, (DWCGHTTPResult)req, param );
		if(entry->buf != NULL)
            DWC_Free(DWC_ALLOCTYPE_BASE, (void*)entry->buf, 0);
        DWCi_RemoveDWCGHTTPParamEntry(entry);
    }
    
	entry->req = req;
	
	return (int)req;

}

//=============================================================================
/*!
 *	@brief	�f�[�^�̎�M(�o�b�t�@�T�C�Y�w��AProgressCallback�w��)
 *
 *	@param	url		            [in] ���X�g�擾��̂t�q�k
 *	@param	bufferlen           [in] ��M�o�b�t�@�̃T�C�Y
 *	@param	progressCallback	[in] �������ɌĂ΂��R�[���o�b�N
 *  @param  completedCallback   [in] ���X�|���X����������R�[���o�b�N
 *	@param	param	            [in] �R�[���o�b�N�p�p�����[�^
 *
 *	@retval	req		 0�ȏ� ���N�G�X�g���ʎq�i�����j
 *                  -1�ȉ� ���N�G�X�g�G���[�l
 */
//=============================================================================
int DWC_GetGHTTPDataEx( const char* url, int bufferlen, BOOL buffer_clear, DWCGHTTPProgressCallback progressCallback, DWCGHTTPCompletedCallback completedCallback, void* param ){

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_GetGHTTPDataEx\n");
    return DWCi_GHTTPGetEx(url, bufferlen, buffer_clear, NULL, progressCallback, completedCallback, param);
}

//=============================================================================
/*!
 *	@brief	�f�[�^�̎�M(�o�b�t�@�T�C�Y�w��AProgressCallback�w��)
 *
 *	@param	url		            [in] ���X�g�擾��̂t�q�k
 *	@param	bufferlen           [in] ��M�o�b�t�@�̃T�C�Y
 *	@param	post	[in] post����I�u�W�F�N�g
 *	@param	progressCallback	[in] �������ɌĂ΂��R�[���o�b�N
 *  @param  completedCallback   [in] ���X�|���X����������R�[���o�b�N
 *	@param	param	            [in] �R�[���o�b�N�p�p�����[�^
 *
 *	@retval	req		 0�ȏ� ���N�G�X�g���ʎq�i�����j
 *                  -1�ȉ� ���N�G�X�g�G���[�l
 */
//=============================================================================
int DWC_GetGHTTPDataEx2( const char* url, int bufferlen, BOOL buffer_clear, DWCGHTTPPost *post, DWCGHTTPProgressCallback progressCallback, DWCGHTTPCompletedCallback completedCallback, void* param ){

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_GetGHTTPDataEx2\n");
    return DWCi_GHTTPGetEx(url, bufferlen, buffer_clear, post, progressCallback, completedCallback, param);
}

//=============================================================================
/*!
 *	@brief	HTTP���N�G�X�g�̃L�����Z��
 *
 *	@param	req		[in] ��~����HTTP���N�G�X�g��ID(DWC_GetGHTTPDataEx�̕Ԃ�l)
 *
 *	@retval	�Ȃ�
 */
//=============================================================================
void DWC_CancelGHTTPRequest(int req)
{
	DWCGHTTPParamEntry *entry;
	ghttpCancelRequest((GHTTPRequest)req); 
	
	entry = DWCi_FindDWCGHTTPParamEntryByReq(req);
	if(entry == NULL)
		return;
	if(entry->buf != NULL)
		DWC_Free(DWC_ALLOCTYPE_BASE, (void*)entry->buf, 0);
	DWCi_RemoveDWCGHTTPParamEntry(entry);
}

//=============================================================================
/*!
 *	@brief	�ʐM��Ԃ̊m�F
 *
 *	@param	req		[in] ���N�G�X�g���ʎq
 *
 *	@retval	state	�ʐM���
 *  @retval False   ���s
 */
//=============================================================================
DWCGHTTPState DWC_GetGHTTPState( int req )
{
    if(req < 0){
        return DWC_GHTTP_FALSE;
    }
    else{
        return (DWCGHTTPState)(ghttpGetState(req));
    }
}

//=============================================================================
/*!
 *	@brief	GHTTP�Ή��G���[�����֐�
 *
 *	@param	result	[in] GHTTP�̏�������
 *
 *	@retval	state	GHTTP�̏������ʌ^�i���������̂܂ܕԂ��j
 */
//=============================================================================
static DWCGHTTPResult DWCi_HandleGHTTPError(DWCGHTTPResult result)
{
    int errorCode = DWC_ECODE_SEQ_ETC + DWC_ECODE_GS_HTTP;
    DWCError dwcError = DWC_ERROR_GHTTP_ANY;

    if (result == DWC_GHTTP_SUCCESS) return DWC_GHTTP_SUCCESS;

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Main, DWCGHTTP error %d\n", result);

    // Possible Error values returned from GHTTP functions.
    switch (result){
	case DWC_GHTTP_FAILED_TO_OPEN_FILE:
        errorCode += DWC_ECODE_TYPE_OPEN_FILE;
        break;
	case DWC_GHTTP_INVALID_POST:
        errorCode += DWC_ECODE_TYPE_INVALID_POST;
        break;
	case DWC_GHTTP_INSUFFICIENT_MEMORY:
        errorCode += DWC_ECODE_TYPE_BUFF_OVER;
        break;
	case DWC_GHTTP_INVALID_FILE_NAME:
	case DWC_GHTTP_INVALID_BUFFER_SIZE:
	case DWC_GHTTP_INVALID_URL:
        errorCode += DWC_ECODE_TYPE_REQ_INVALID;
        break;
	case DWC_GHTTP_UNSPECIFIED_ERROR:
        errorCode += DWC_ECODE_TYPE_UNSPECIFIED;
        break;
        
    // The result of an HTTP request.
	case DWC_GHTTP_OUT_OF_MEMORY:
    case DWC_GHTTP_MEMORY_ERROR:
        dwcError = DWC_ERROR_FATAL;
        errorCode += DWC_ECODE_TYPE_ALLOC;
        break;
	case DWC_GHTTP_BUFFER_OVERFLOW:
        errorCode += DWC_ECODE_TYPE_BUFF_OVER;
        break;
	case DWC_GHTTP_PARSE_URL_FAILED:
        errorCode += DWC_ECODE_TYPE_PARSE_URL;
        break;
	case DWC_GHTTP_HOST_LOOKUP_FAILED:
        errorCode += DWC_ECODE_TYPE_DNS;
        break;
    case DWC_GHTTP_SOCKET_FAILED:
        errorCode += DWC_ECODE_TYPE_SOCKET;
        break;
	case DWC_GHTTP_CONNECT_FAILED:
	case DWC_GHTTP_FILE_NOT_FOUND:
	case DWC_GHTTP_SERVER_ERROR:
        errorCode += DWC_ECODE_TYPE_SERVER;
        break;
	case DWC_GHTTP_BAD_RESPONSE:
        errorCode += DWC_ECODE_TYPE_BAD_RESPONSE;
        break;
	case DWC_GHTTP_REQUEST_REJECTED:
	case DWC_GHTTP_UNAUTHORIZED:
	case DWC_GHTTP_FORBIDDEN:
        errorCode += DWC_ECODE_TYPE_REJECTED;
        break;
	case DWC_GHTTP_FILE_WRITE_FAILED:
	case DWC_GHTTP_FILE_READ_FAILED:
        errorCode += DWC_ECODE_TYPE_FILE_RW;
        break;
	case DWC_GHTTP_FILE_INCOMPLETE:
        errorCode += DWC_ECODE_TYPE_INCOMPLETE;
        break;
	case DWC_GHTTP_FILE_TOO_BIG:
        errorCode += DWC_ECODE_TYPE_TO_BIG;
        break;
	case DWC_GHTTP_ENCRYPTION_ERROR:
        errorCode += DWC_ECODE_TYPE_ENCRYPTION;
        break;

    default:
        break;
    }

    DWCi_SetError(dwcError, errorCode);
    return result;
}


//=============================================================================
/*!
 *	@brief	GHTTP�p�����[�^���X�g�G���g����ǉ�
 *
 *	@param	param	[in] �ǉ�����DWCGHTTPParam�ւ̃|�C���^
 *
 *	@retval	NULL�ȊO	�m�ۂ��ꂽDWCGHTTPParamEntry�ւ̃|�C���^
 *	@retval	NULL   		���s(�������m�ێ��s�����̃G���[�������s������)
 */
//=============================================================================
static DWCGHTTPParamEntry *DWCi_AppendDWCGHTTPParam(const DWCGHTTPParam *param)
{
	DWCGHTTPParamEntry *entry;
	
	// �����N���X�g�ɉ�����G���g�����쐬
	entry = DWC_Alloc(DWC_ALLOCTYPE_BASE, sizeof(DWCGHTTPParamEntry));
	if(entry == NULL)
		return NULL;
	
	// �����Ƃ��ė^����ꂽparam���R�s�[
	entry->param = *param;
	
	// ParamEntry�Ǝ��̃����o��������
	entry->nextentry = NULL;
	entry->buf = NULL;
	
	// �擪�����Ȃ��ꍇ
	if(paramhead == NULL) {
		paramhead = entry;
		return entry;
	}
	
	// �擪������ꍇ�͐擪�ɕt��������
	entry->nextentry = paramhead;
	paramhead = entry;
	
	return entry;
}

//=============================================================================
/*!
 *	@brief	GHTTP�p�����[�^���X�g�G���g�����폜
 *
 *	@param	param	[in] �폜����DWCGHTTPParam�ւ̃|�C���^
 *
 *	@retval	�Ȃ�
 */
//=============================================================================
static void DWCi_RemoveDWCGHTTPParamEntry(DWCGHTTPParamEntry *entry)
{
	DWCGHTTPParamEntry *cursor, *target;
	
	// �擪�����Ȃ��ꍇ�͉������Ȃ�
	if(paramhead == NULL)
		return;
	
	// �擪���폜�Ώۂ̏ꍇ�͓��ʂȍ폜���@���g���K�v������
	if(paramhead == entry) {
		cursor = paramhead->nextentry;
		DWC_Free(DWC_ALLOCTYPE_BASE, (void*)paramhead, 0);
		paramhead = cursor;
		return;
	}
	
	// �J�[�\���̎����폜�ΏۂȂ�΁c
	cursor = paramhead;
	while(cursor->nextentry != NULL) {
		if(cursor->nextentry != entry) {
			cursor = cursor->nextentry;
			continue;
		}
		
		target = cursor->nextentry;
		cursor->nextentry = cursor->nextentry->nextentry;
		DWC_Free(DWC_ALLOCTYPE_BASE, (void*)target, 0);
		return;
	}
	
	return;
}

//=============================================================================
/*!
 *	@brief	req�̒l����GHTTP�p�����[�^���X�g�G���g��������
 *
 *	@param	req	[in] �����Ώۂ�DWCGHTTPParam�̎���reqid
 *
 *	@retval	�Ȃ�
 */
//=============================================================================
static DWCGHTTPParamEntry *DWCi_FindDWCGHTTPParamEntryByReq(int req)
{
	DWCGHTTPParamEntry *cursor;
	
	// �J�[�\���̎��������ΏۂȂ�΁c
	cursor = paramhead;
	while(cursor != NULL && cursor->req != req)
		cursor = cursor->nextentry;
	
	return cursor;
}

//=============================================================================
/*!
 *	@brief	GHTTP�p�����[�^���X�g�G���g����S�폜
 *
 *	@param	�Ȃ�
 *
 *	@retval	�Ȃ�
 */
//=============================================================================
static void DWCi_RemoveAllDWCGHTTPParamEntry(void)
{
	DWCGHTTPParamEntry *cursor, *target;
	
	cursor = paramhead;
	while(cursor != NULL) {
		target = cursor;
		cursor = cursor->nextentry;
		
		// ���b�p���o�b�t�@���m�ۂ��Ă����ꍇ�͉��
		if(target->buf != NULL)
			DWC_Free(DWC_ALLOCTYPE_BASE, (void*)target->buf, 0);
		// �G���g�����̂��̂����
		DWC_Free(DWC_ALLOCTYPE_BASE, (void*)target, 0);
	}
	
	paramhead = NULL;
	return;
}
