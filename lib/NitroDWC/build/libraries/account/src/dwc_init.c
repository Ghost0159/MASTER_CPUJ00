/*---------------------------------------------------------------------------*
  Project:  NitroDWC - base - init
  File:     dwc_init.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_init.c,v $
  Revision 1.11  2007/02/16 08:24:00  takayama
  DWC_GetAuthenticatedUserId()��ǉ��B

  Revision 1.10  2007/02/13 07:42:45  nakata
  DWCisconsoleidreadable�t���O�������B

  Revision 1.9  2007/02/13 02:52:32  nakata
  DWC_GetConsoleId�֐��̂��߂̖{��ID�ǂݏo���\�t���O��ǉ��B

  Revision 1.8  2005/12/15 05:35:16  arakit
  �f�o�b�O�p��NVRAM�N���A�֐�DWC_Debug_ClearConsoleWiFiInfo()���폜���A
  DWC_Init()�̑S�Ă̖߂�l��������悤��NVRAM��j�󂷂�f�o�b�O�֐�
  DWC_Debug_DWCInitError()��ǉ������B

  Revision 1.7  2005/11/01 10:38:48  arakit
  enum�̒Ԃ���C�������B

  Revision 1.6  2005/10/06 09:42:37  sasakit
  version�𖄂ߍ��ނ悤�ɂ����B

  Revision 1.5  2005/09/21 06:56:02  sasakit
  �f�o�b�O�p�̖{��Wi-Fi���[�U�����폜����֐���ǉ��B

  Revision 1.4  2005/09/02 10:14:23  sasakit
  OS_Init()����NitroSDK�������֐����ĂԂ̂���߂āA���t�@�����X�ɖ��L����悤�ɂ����B

  Revision 1.3  2005/08/31 09:47:01  sasakit
  dwc_init�̖߂�l��ݒ肵���B

  Revision 1.2  2005/08/31 04:12:30  sasakit
  util_wifiidtool.h�̈ړ��ɑΉ��B

  Revision 1.1  2005/08/31 02:29:44  sasakit
  �������p�̃R�[�h��overlay�ŕ�������悤�ɁA���W���[���𕪂����B

  Revision 1.4  2005/08/23 05:40:05  nakata
  Fixed the problem that is related to BM initialize.
  Before : Pass 32 BIT  alignmented memory space to BM as work memory.
  After  : Pass 32 BYTE alignmented memory space to BM as work memory.

  Revision 1.3  2005/08/22 12:06:57  sasakit
  WiFiID���Ȃ��Ƃ��ɍ쐬����悤�ɖ߂����B

  Revision 1.2  2005/08/20 06:59:56  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B

  Revision 1.1  2005/08/19 12:12:10  sasakit
  �����[�X�Ɍ����ďC��

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

#include <bm/dwc_bm_init.h>
#include <bm/util_wifiidtool.h>
#include <base/dwc_report.h>
#include <base/dwc_init.h>

#include <dwc_version.h>

#include <nitro/version_begin.h>
static char id_string [] = SDK_MIDDLEWARE_STRING("NINTENDO", DWC_VERSION_STRING);
#include <nitro/version_end.h>

extern BOOL DWCi_AUTH_MakeWiFiID( void* work );
int
DWC_Init( void* work )
{
    int ret;
    BOOL created = FALSE;

    SDK_USING_MIDDLEWARE(id_string);

    SDK_ASSERTMSG( ( (u32)work & 0x1f ) == 0, "work must be 32bytes alignment - %p\n", work );

    ret = DWC_BM_Init( work );
    
    if ( DWC_Auth_CheckWiFiIDNeedCreate() )
    {
        DWC_Printf( DWC_REPORTFLAG_INFO, "Create UserID\n" );
        DWCi_AUTH_MakeWiFiID( work );

        created = TRUE;
    }
    else
    {
        DWC_Printf( DWC_REPORTFLAG_INFO, "Read UserID from DS\n" );
    }

    if ( ret < 0 )
    {
        // NVRAM�̓��e���j�󂳂�Ă���B
        if ( created )
        {
            return DWC_INIT_RESULT_DESTROY_USERID;
        }
        else
        {
            return DWC_INIT_RESULT_DESTROY_OTHER_SETTING;
        }
    }
    else if ( created )
    {
        // NVRAM�͖��Ȃ����ǁA�Ȃ�������܂����B
        return DWC_INIT_RESULT_CREATE_USERID;
    }
    
    return DWC_INIT_RESULT_NOERROR;
}

/*---------------------------------------------------------------------------*
  Name:         DWC_GetAuthenticatedUserId

  Description:  DS�{�̂̔F�؍ς݃��[�UID���擾����

  Arguments:    �Ȃ�

  Returns:      �F�؍ς݃��[�UID�i�F�؍ς݂łȂ��ꍇ�� 0 )
 *---------------------------------------------------------------------------*/
u64 DWC_GetAuthenticatedUserId( void )
{
    DWCAuthWiFiId wifiid;
    
    DWC_Auth_GetId( &wifiid );
    return wifiid.uId;
}

extern BOOL  DWCi_BACKUPlInit(void* work);
extern BOOL  DWCi_BACKUPlRead(void* mem);
extern BOOL  DWCi_BACKUPlWritePage(const void* data, const BOOL* page, void* work);
static    char s_work[0x400] ATTRIBUTE_ALIGN(32);

// NVRAM�̓���̉ӏ���j�󂵂�DWC_Init()�̃G���[�Ԃ�l���������߂̃f�o�b�O�֐�
void DWC_Debug_DWCInitError( void* work, int dwc_init_error )
{
    BOOL needCrc = FALSE;
    int  crcPage;

    if ( dwc_init_error == DWC_INIT_RESULT_NOERROR ) return;

    DWCi_BACKUPlInit(work);  // NVRAM�A�N�Z�X������

    if ( dwc_init_error == DWC_INIT_RESULT_CREATE_USERID )
    {
        DWCi_BACKUPlRead( s_work );  // NVRAM�ǂݍ���

        // �y�[�W0��WiFi���̃����onotAttestedId, attestedUserId�̂݃N���A
        MI_CpuClear8( &s_work[0xf0], 10 );
        s_work[0xf0+0x0a] &= ~0x3f;

        needCrc = TRUE;
        crcPage = 0;     // �y�[�W0�̃f�[�^������������
    }
    else if ( dwc_init_error == DWC_INIT_RESULT_DESTROY_USERID )
    {
        DWCi_BACKUPlRead( s_work );  // NVRAM�ǂݍ���

        // �y�[�W1��WiFi���̃����onotAttestedId, attestedUserId�̂݃N���A
        MI_CpuClear8( &s_work[0x100+0xf0], 10 );
        s_work[0x100+0xf0+0x0a] &= ~0x3f;

        // ���̌�y�[�W0�̃f�[�^��j�󂷂�̂ŁA�y�[�W0�ɃR�s�[����邱�ƂɂȂ�
        // �y�[�W1��ap.state�ɁA�y�[�W0�ɂƂ��Đ������l���Z�b�g����
        s_work[0x100+0xef] = 1;

        needCrc = TRUE;
        crcPage = 1;     // �y�[�W1�̃f�[�^������������
    }

    // WiFi���������������ꍇ�́ACRC�̍Čv�Z������NVRAM������������
    if ( needCrc )
    {
        u16 hash;
        BOOL page[4] = { FALSE, FALSE, FALSE, FALSE };
        MATHCRC16Table crc16_tbl;
        
        // CRC ������
    	MATH_CRC16InitTable( &crc16_tbl );

        // CRC �Čv�Z
  		hash = MATH_CalcCRC16( &crc16_tbl, (u16 *)&s_work[crcPage*0x100], 0xFE );
        *(u16 *)(&s_work[crcPage*0x100+0xFE]) = hash;

        // �w��̃y�[�W�̂�NVRAM��������
        page[crcPage] = TRUE;
        DWCi_BACKUPlWritePage( s_work, page, work );
    }

    // DWC_BM_Init()��DWC_INIT_OK�ȊO�ŏI�������邽�߂Ƀy�[�W0�̂݃N���A����
    if ( dwc_init_error == DWC_INIT_RESULT_DESTROY_USERID ||
         dwc_init_error == DWC_INIT_RESULT_DESTROY_OTHER_SETTING )
    {
        BOOL page[4] = { TRUE, FALSE, FALSE, FALSE };
        
        MI_CpuClear8( s_work, sizeof(s_work) );
        DWCi_BACKUPlWritePage( s_work, page, work );
    }
}
