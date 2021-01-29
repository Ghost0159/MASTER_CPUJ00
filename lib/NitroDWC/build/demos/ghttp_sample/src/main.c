/*===========================================================================*
 *                                                                           *
 *  ghttp���C�u�����@�T���v���v���O����                                      *
 *                                                                           *
 *                                           Copyright 2005 Nintendo Co.,Ltd *
 *===========================================================================*/
#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

// �w�b�_�t�@�C���C���N���[�h
//-------------------------------------
#include "main.h"
#include "init.h"
#include "utility.h"

// �萔�錾
//---------------------------------------------------------
#define GAMENAME "dwctest"		//! �Q�[����
#define USERID "12345"   		//! ���[�U�[ID
#define RECV_SIZE 256           //! ��M�f�[�^�T�C�Y

// �O���[�o���ϐ��錾
//---------------------------------------------------------
DWCInetControl stConnCtrl;//! 
int		g_pending;		  //! �ʐM�����҂����ۂ�
char*   buffer = NULL;           //! 

// ����M�f�[�^�`��
//---------------------------------------------------------
typedef struct _tagGHTTPDATA{
	u8	tex[128];	//! �e�L�X�g
	u8	title[10];	//! �^�C�g��
}GHTTPDATA;


// �֐��v���g�^�C�v
//---------------------------------------------------------
static void POSTCallback(const char*, int, DWCGHTTPResult result, void*);
static void GETCallback(const char* buf, int buflen, DWCGHTTPResult result, void* param);
static void GETCallbackEx(const char* buf, int buflen, DWCGHTTPResult result, void* param);
static void GETProgressCallbackEx(DWCGHTTPState state, const char* buf, int len, int bytesReceived, int totalSize, void* param);
static BOOL StartIPMain(void);
static void VBlankIntr(void);

/*---------------------------------------------------------------------------*
  �������m�ۊ֐�
 *---------------------------------------------------------------------------*/
static void*
AllocFunc( DWCAllocType name, u32   size, int align )
{
    void * ptr;
    OSIntrMode old;
    (void)name;
    (void)align;

    old = OS_DisableInterrupts();

    ptr = OS_AllocFromMain( size );

    OS_RestoreInterrupts( old );

    return ptr;
}

/*---------------------------------------------------------------------------*
  �������J���֐�
 *---------------------------------------------------------------------------*/
static void
FreeFunc( DWCAllocType name, void* ptr,  u32 size  )
{
    OSIntrMode old;
    (void)name;
    (void)size;

    if ( !ptr ) return;

    old = OS_DisableInterrupts();

    OS_FreeToMain( ptr );

    OS_RestoreInterrupts( old );
}

//=============================================================================
/*!
 *	@brief	�f�[�^���M�������̃R�[���o�b�N
 *
 *	@param	buf		[in] �f�[�^�o�b�t�@
 *	@param	buflen	[in] buf�̃T�C�Y
 *
 *	@retval	TRUE	buf�J��
 *	@retval	FALSE   buf�J������
*/
//=============================================================================
static void POSTCallback(const char* buf, int buflen, DWCGHTTPResult result, void* param){
#ifdef SDK_FINALROM
#pragma unused(result)
#endif
#pragma unused(buf)
#pragma unused(buflen)
#pragma unused(param)

	g_pending = 0;

    OS_Printf("Post Result : %d\n", result);

}

//=============================================================================
/*!
 *	@brief	�f�[�^��M�������̃R�[���o�b�N
 *
 *	@param	buf		[in] �f�[�^�o�b�t�@
 *	@param	buflen	[in] buf�̃T�C�Y
 *  @param  param   [in] �R�[���o�b�N�p�p�����[�^
 *
 *	@retval	TRUE	buf�J��
 *	@retval	FALSE   buf�J������
 */
//=============================================================================
static void GETCallback(const char* buf, int buflen, DWCGHTTPResult result, void* param){
#ifdef SDK_FINALROM
#pragma unused(result)
#endif
    char*	g_data;	//! ��M�f�[�^�̃o�b�t�@

    g_data = param;
    
	if(buf){
        MI_CpuCopy8(buf, g_data, (u32)buflen);
	}

	g_pending = 0;

    OS_Printf("Recv Result : %d\n", result);

}

//=============================================================================
/*!
 *	@brief	�f�[�^��M�������̃R�[���o�b�N(���[�U��M�o�b�t�@�w��)
 *
 *	@param	buf		[in] �f�[�^�o�b�t�@
 *	@param	buflen	[in] buf�̃T�C�Y
 *  @param  param   [in] �R�[���o�b�N�p�p�����[�^
 *
 *	@retval	TRUE	buffer�J��
 *	@retval	FALSE   buffer�J������
 */
//=============================================================================
static void GETCallbackEx(const char* buf, int buflen, DWCGHTTPResult result, void* param){
#ifdef SDK_FINALROM
#pragma unused(result)
#pragma unused(buflen)
#endif
#pragma unused(param)

    if(buf){
        buffer = (char*)buf;
    }
    
    OS_Printf("Recv Result : %d\n", result);
    OS_Printf("buflen : %d\n", buflen);
    
	g_pending = 0;

}

//=============================================================================
/*!
 *	@brief	�f�[�^��M�i�s���̃R�[���o�b�N
 *
 *	@param	buf		[in] �f�[�^�o�b�t�@
 *	@param	buflen	[in] buf�̃T�C�Y
 *  @param  param   [in] �R�[���o�b�N�p�p�����[�^
 *
 *	@retval	TRUE	buffer�J��
 *	@retval	FALSE   buffer�J������
 */
//=============================================================================
static void GETProgressCallbackEx(DWCGHTTPState state, const char* buf, int len, int bytesReceived, int totalSize, void* param){
#ifdef SDK_FINALROM
#pragma unused(state)
#pragma unused(len)
#pragma unused(bytesReceived)
#pragma unused(totalSize)
#endif
#pragma unused(buf)
#pragma unused(param)

    OS_Printf("state: %d\n", state);
    OS_Printf("size : %d/%d\n", len, totalSize);
    OS_Printf("totalsize : %d/%d\n", bytesReceived, totalSize);
    
}

//=============================================================================
/*!
 *	@brief	IP�̎擾
 *
 *	@retval	TRUE		����
 *	@retval	FALSE   	���s
 */
//=============================================================================
static BOOL StartIPMain(void)
{
    DWC_InitInet( &stConnCtrl );

    DWC_ConnectInetAsync();
    // ����ȃX�e�[�g�܂ő҂B
    while ( !DWC_CheckInet() )
    {
        DWC_ProcessInet();

        OS_WaitVBlankIntr();

    }

    switch ( DWC_GetInetStatus() )
    {
    case DWC_CONNECTINET_STATE_ERROR:
        {
            // �G���[�\��
            DWCError err;
            int errcode;
            err = DWC_GetLastError( &errcode );

            OS_TPrintf("   Error occurred %d %d.\n", err, errcode );
        }
        DWC_ClearError();

        DWC_CleanupInet();
        break;
    case DWC_CONNECTINET_STATE_FATAL_ERROR:
        {
            // �G���[�\��
            DWCError err;
            int errcode;
            err = DWC_GetLastError( &errcode );

            OS_Panic("   Error occurred %d %d.\n", err, errcode );
        }
        break;

    case DWC_CONNECTINET_STATE_CONNECTED:
        OS_TPrintf("   CONNECTED!!!.\n");
        return TRUE;
    }
    return FALSE;
}

/*---------------------------------------------------------------------------*
  V�u�����N���荞�݊֐�
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

//=============================================================================
/*!
 *	@brief	���C��
 */
//=============================================================================
static u8 s_Work[ DWC_INIT_WORK_SIZE ] ATTRIBUTE_ALIGN( 32 );

void NitroMain()
{
	// �f�[�^�p
	GHTTPDATA data;
	int    len;
    char   g_data[1024];	//! ��M�f�[�^�p�o�b�t�@
    
    int req;//���N�G�X�g���ʎq
    
    DWCGHTTPPost post;

	// initialize
	//-------------------------------------------
	OS_Printf("initialize\n");

	OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    RTC_Init();

    // �X�^�b�N���`�F�b�N�����ݒ�
    OS_SetThreadStackWarningOffset(OS_GetCurrentThread(), 0x100);
    
    PXI_Init();
    
    Heap_Init();
	Alarm_Init();

    GX_Init();
    FS_Init( MI_DMA_MAX_NUM ); 

    GX_DispOff();
    GXS_DispOff();

    // V�u�����N���荞�݋���
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);
	GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    // �\���J�n
    OS_WaitVBlankIntr();              // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    // DWC���C�u����������
    DWC_Init( s_Work );
    
    // �f�o�b�O�\�����x���w��
    DWC_SetReportLevel(DWC_REPORTFLAG_ALL);
    
    DWC_SetMemFunc( AllocFunc, FreeFunc );

	//IP�̎擾
	while(!StartIPMain())
	{
		OS_Printf("error at StartIPMain()\n");	
	}
    
	// ghttp�̏�����
	if(DWC_InitGHTTP(GAMENAME) != TRUE){
		OS_Printf("error at SCInitialize()\n");
	}

	// send data
	//-------------------------------------------
	memset(&data, 0, sizeof(data));
	len = sizeof(data);
    
	// *debug code* put some string
	strcpy((char*)data.tex, "this is a test message");
	strcpy((char*)data.title, "test");
    OS_Printf("[sent] %s\n", (char*)data.tex);
	OS_Printf("[sent] %s\n", (char*)data.title);

	// post data
	//-------------------------------------------
    DWC_GHTTPNewPost(&post);
    DWC_GHTTPPostAddString(&post, "data", (const char*)&data);
                           
	g_pending = 1;
	req = DWC_PostGHTTPData("URL", &post, POSTCallback, NULL);// post���邽�߂�URL�̎w�肪�K�v
	do{
		DWC_ProcessGHTTP();
//	    OS_Printf("get request state: %d\n", DWC_GetGHTTPState(req));// �ʐM�󋵂̃`�F�b�N
	}while(g_pending);

	// receive data
	//-------------------------------------------
	g_pending = 1;
	req = DWC_GetGHTTPData("URL", GETCallback, (void*)g_data);// get���邽�߂�URL�̎w�肪�K�v
	do{
		DWC_ProcessGHTTP();
//	    OS_Printf("get request state: %d\n", DWC_GetGHTTPState(req));// �ʐM�󋵂̃`�F�b�N
	}while(g_pending);

	// *debug code* put some string
	OS_Printf("[recv] %s\n", g_data);
	
	// receive data
	//-------------------------------------------
	g_pending = 1;
	req = DWC_GetGHTTPDataEx("URL", RECV_SIZE, FALSE, GETProgressCallbackEx, GETCallbackEx, NULL);// get���邽�߂�URL�̎w�肪�K�v
	do{
		DWC_ProcessGHTTP();
//	    OS_Printf("get request state: %d\n", DWC_GetGHTTPState(req));// �ʐM�󋵂̃`�F�b�N
	}while(g_pending);

    if(buffer){
        // *debug code* put some string
        OS_Printf("[recv] %s\n", buffer);
        DWC_Free((DWCAllocType)0, (void*)buffer, 0);
    }
    // shutdown
	//-------------------------------------------
	OS_TPrintf("-- done --\n");

	DWC_ShutdownGHTTP();

	OS_Terminate();
}
