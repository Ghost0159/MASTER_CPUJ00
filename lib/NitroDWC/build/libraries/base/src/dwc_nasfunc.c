#include <nitro.h>

#include <dwc_report.h>
#include <dwc_error.h>
#include <dwc_nasfunc.h>
#include <dwc_memfunc.h>

/* =======================================================================
        define
   ======================================================================== */

/* =======================================================================
        typedef
   ======================================================================== */

/* [nakata] NAL�R���g���[���\���� */
typedef struct {
	DWCHttp			http;		/* [nakata] HTTP�\���� */
	DWCAuthParam	authparam;	/* [nakata] DWCAuth�p�p�����[�^ */
	DWCAuthResult	result;		/* [nakata] �F�،��ʊi�[�p�\���� */
} DWCNalCnt;

/* =======================================================================
        external variable
   ======================================================================== */
extern s64	DWCnastimediff;
extern BOOL	DWCnastimediffvalid;

/* =======================================================================
        static variable
   ======================================================================== */
static DWCNalCnt *nalcnt = NULL;

/* =======================================================================
        prototype
   ======================================================================== */

/* =======================================================================
        external functions(ingamesn�֌W)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:			DWC_GetIngamesnCheckResult

  Description:	NAS�ɂ��ingamesn����̌��ʂ��擾����

  Arguments:	�Ȃ�

  Returns:		DWC_INGAMESN_NOTCHECKED	���݃`�F�b�N��(���O�C��������)
                DWC_INGAMESN_VALID		���O�̃��O�C����ingamesn��NAS�Ɏ󂯓����ꂽ
				DWC_INGAMESN_INVALID	���O�̃��O�C����ingamesn��NAS�ɋ��ۂ��ꂽ
 *---------------------------------------------------------------------------*/
DWCIngamesnCheckResult DWC_GetIngamesnCheckResult(void)
{
	return (DWCIngamesnCheckResult)DWCauthingamesncheckresult;
}

/* =======================================================================
        external function(NAS���Ԋ֘A)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:			DWC_GetIngamesnCheckResult

  Description:	NAS�̎��Ԃ�RTC�`���Ŏ擾����

  Arguments:	date	�擾���ʂ�RTCDate���������ޗ̈�ւ̃|�C���^
				time	�擾���ʂ�RTCTime���������ޗ̈�ւ̃|�C���^

  Returns:		TRUE	�擾����
				FALSE - �擾���s
 *---------------------------------------------------------------------------*/
extern s64     DWCnastimediffbase;

BOOL DWC_GetDateTime(RTCDate *date, RTCTime *time)
{
	s64 localsec;

	if(DWCnastimediffvalid == FALSE)
		return FALSE;
	
	// [nakata] �{�̎��v�����擾
	if(RTC_GetDateTime(date, time) != 0)
		return FALSE;
	
	// [nakata] �{�̎��v�̒l��b�ɕϊ�
	localsec = RTC_ConvertDateTimeToSecond(date, time);
	if(localsec == -1)
		return FALSE;
	
	// [nakata] �b�ɃT�[�o���ԂƂ̍��𑫂�
    if(localsec < DWCnastimediffbase) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "RTC overflow detected.\n");
        localsec += 3155760000;
    }
	localsec -= DWCnastimediff;
	if(localsec < 0 || localsec > 3155759999)
		return FALSE;
	
	// [nakata] Date/Time�𐶐�
	RTC_ConvertSecondToDateTime(date, time, localsec);
	return TRUE;
}

/* =======================================================================
        external function(SVL�֘A)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:			DWC_SVLGetTokenAsync

  Description:	�g�[�N���擾�֐�

  Arguments:	svl		�T�[�r�X��ʂ��w�肷��4������ASCII������
				result	�擾�������ʂ��i�[����̈�ւ̃|�C���^

  Returns:		TRUE	�擾�J�n
				FALSE	�擾�J�n�ł��Ȃ�
 *---------------------------------------------------------------------------*/
BOOL DWC_SVLGetTokenAsync(char *svl, DWCSvlResult *result)
{
	BOOL flag;
	
	SDK_ASSERT(svl);
	SDK_ASSERT(result);

	/* [nakata] ���������� */
	flag = DWC_Svl_Init((DWCAuthAlloc)DWC_Alloc, (DWCAuthFree)DWC_Free);
	if(flag == FALSE) {
		/* [nakata] ������DWC�G���[�ݒ�ς� */
		return FALSE;
	}
	
	/* [nakata] �g�[�N���擾�����J�n */
	if(strlen(svl) == 0)
		flag = DWC_Svl_GetTokenAsync("0000", result);
	else
		flag = DWC_Svl_GetTokenAsync(svl, result);
	
	if(flag == FALSE) {
		/* [nakata] DWC_SVL�̃��[�N�G���A���J�� */
		DWC_Svl_Cleanup();
		/* [nakata] ������DWC�G���[�ݒ�ς� */
		return FALSE;
	}
	return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:			DWC_SVLProcess

  Description:	SVL�g�[�N���擾�����i�s�֐�

  Arguments:	�Ȃ�

  Returns:		DWCSvcState
 *---------------------------------------------------------------------------*/
DWCSvlState DWC_SVLProcess(void)
{
	DWCSvlState state;
	state = DWC_Svl_Process();
	if(state == DWC_SVL_STATE_ERROR || state == DWC_SVL_STATE_SUCCESS || state == DWC_SVL_STATE_CANCELED)
		/* [nakata] DWC_SVL�̃��[�N�G���A���J�� */
		/* [nakata] ������DWC�G���[�ݒ�ς� */
		DWC_Svl_Cleanup();
	return state;
}

/*---------------------------------------------------------------------------*
  Name:			DWC_SVLAbort

  Description:	SVL�g�[�N���擾�����i�s�֐�

  Arguments:	�Ȃ�

  Returns:		�Ȃ�
 *---------------------------------------------------------------------------*/
void DWC_SVLAbort(void)
{
	DWC_Svl_Abort();
}

/* =======================================================================
        external function(NAS�F�؊֘A)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:			DWC_NASLoginAsync

  Description:	NAS���O�C���J�n

  Arguments:	�Ȃ�

  Returns:		TRUE  - ���O�C�������J�n����
				FALSE - ���O�C�������J�n���s
 *---------------------------------------------------------------------------*/
BOOL DWC_NASLoginAsync(void)
{
	if(nalcnt != NULL)
		OS_Panic("You can't request more than one NAS login request at once.\n");
	
	/* [nakata] �������[�N�G���A���m��/������ */
	nalcnt = DWC_Alloc(DWC_ALLOCTYPE_BASE, sizeof(DWCNalCnt));
	if(nalcnt == NULL) {
		DWCi_SetError(DWC_ERROR_FATAL, -20100);
		return FALSE;
	}
	MI_CpuClear8(nalcnt, sizeof(DWCNalCnt));
	
	/* [nakata] Auth�p�����[�^������ */
	/* [nakata] nalcnt->authparam.ingamesn�͋󕶎��� */
	/* [nakata] nalcnt->authparam.gsbrcd�͋󕶎��� */
	MI_CpuCopy8("", nalcnt->authparam.gsbrcd, 9);
	nalcnt->authparam.alloc	= (DWCAuthAlloc)DWC_Alloc;
	nalcnt->authparam.free	= (DWCAuthFree)DWC_Free;
	
	/* [nakata] Auth�J�n */
#if 1
	DWC_Auth_Create(&nalcnt->authparam, &nalcnt->http);
#else
	if(DWC_Auth_Create(&nalcnt->authparam, &nalcnt->http) != DWCAUTH_E_NOERR) {
		DWC_Free(DWC_ALLOCTYPE_BASE, nalcnt, sizeof(DWCNalCnt));
		nalcnt = NULL;
		DWCi_SetError(DWC_ERROR_FATAL, -20100);
		return FALSE;
	}
#endif
	return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:			DWC_NASLoginProcess

  Description:	NAS���O�C�������i�s

  Arguments:	�Ȃ�

  Returns:		DWCNasLoginState
 *---------------------------------------------------------------------------*/
DWCNasLoginState DWC_NASLoginProcess(void)
{
	DWCAuthResult result;
	
	if(nalcnt == NULL)
		return DWC_NASLOGIN_STATE_DIRTY;
	
	/* [nakata] HTTP�ʐM��Ԋm�F */
	// [nakata] ���̕����̏����͕K�v�Ȃ��A�܂��o�O�𔭐������Ă����̂ō폜����(DWC��login�����ɏ����鏈���ɂȂ���)
	//if(nalcnt->http.thread.id == 0)
	//	return DWC_NASLOGIN_STATE_IDLE;
	
	// [nakata] �X���b�hID�����݂��Ă��邽�߁A����ɂ���ĕ��򂷂�
	switch(DWC_Auth_GetError()) {
	case DWCAUTH_E_NOERR:
		/* [nakata] HTTP�ʐM�� */
		return DWC_NASLOGIN_STATE_HTTP;
	case DWCAUTH_E_FINISH:
		DWC_Auth_Destroy();
		DWC_Free(DWC_ALLOCTYPE_BASE, nalcnt, sizeof(DWCNalCnt));
		nalcnt = NULL;
		return DWC_NASLOGIN_STATE_SUCCESS;
	case DWCAUTH_E_ABORT:
		/* [nakata] ���f�̏ꍇ�ɂ̓G���[���Z�b�g���Ȃ� */
		DWC_Auth_Destroy();
		DWC_Free(DWC_ALLOCTYPE_BASE, nalcnt, sizeof(DWCNalCnt));
		nalcnt = NULL;
		return DWC_NASLOGIN_STATE_CANCELED;
	default:
		DWC_Auth_GetResult(&result);
		DWC_Auth_Destroy();
		DWC_Free(DWC_ALLOCTYPE_BASE, nalcnt, sizeof(DWCNalCnt));
		nalcnt = NULL;
		DWCi_SetError(DWC_ERROR_AUTH_ANY, result.returncode);
		return DWC_NASLOGIN_STATE_ERROR;
	}
}

/*---------------------------------------------------------------------------*
  Name:			DWC_NASLoginAbort

  Description:	�C�V���F�؃T�[�o���O�C���������f�֐�

  Arguments:	�Ȃ�

  Returns:		�Ȃ�
 *---------------------------------------------------------------------------*/
void DWC_NASLoginAbort(void)
{
	DWC_Auth_Abort();
}
