/*---------------------------------------------------------------------------*
  Project:  NitroDWC - base - account
  File:     dwc_account.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.22 $

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <bm/util_wifiidtool.h>
#include <base/dwc_account.h>

#define DWCi_CHECK_FLAG(flag,mask)      ((flag&mask)==mask)

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_GetMaskBits

  Description:  u32�̃f�[�^�̈ꕔ�̃r�b�g����擾���܂��B

  Arguments:    data  : ���o�������f�[�^�̊i�[���ꂽu32�f�[�^
                shift : �r�b�g��̉E�ɃV�t�g���鐔
                mask  : �V�t�g������̃r�b�g��ɓK�p����r�b�g�}�X�N

  Returns:      �����o�����r�b�g��
 *---------------------------------------------------------------------------*/
static inline u32
DWCi_Acc_GetMaskBits( u32 data,
                      u32 shift,
                      u32 mask )
{
    return ((data >> shift) & mask);
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_GetMaskBits

  Description:  u32���Z���r�b�g���u32�̃f�[�^�̈ꕔ�Ɋi�[���܂��B

  Arguments:    ptr   : �i�[������u32�f�[�^�̃|�C���^
                data  : �i�[�������r�b�g��
                shift : �r�b�g��̍��ɃV�t�g���鐔
                mask  : �V�t�g����O�̃r�b�g��ɓK�p����r�b�g�}�X�N

  Returns:      �r�b�g�񂪃}�X�N�Ɏ��܂��Ă���΁A���ʂ����������TRUE
                ���܂��Ă��Ȃ���΁A����������FALSE
 *---------------------------------------------------------------------------*/
static BOOL
DWCi_Acc_SetMaskBits( u32* ptr,
                      u32  data,
                      u32  shift,
                      u32  mask )
{
    // data��mask���Ɏ��܂��Ă��邩�ǂ������`�F�b�N�B
    if ( (data & ~mask) != 0 )
    {
        return FALSE;
    }

    *ptr = (*ptr & ~(mask << shift)) | (data << shift);

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_GetUserId

  Description:  DWCAccLoginId�\���̂���UserID���擾���܂��B

  Arguments:    userdata : ���[�U�f�[�^�̊i�[���ꂽDWCAccLoginId�\���̂ւ̃|�C���^

  Returns:      UserID 64bits
 *---------------------------------------------------------------------------*/
u64
DWCi_Acc_GetUserId( const DWCAccLoginId* userdata )
{
    u32 hi32;

    hi32 = DWCi_Acc_GetMaskBits( userdata->id_data, DWC_ACC_USERID_HI32_SHIFT, DWC_ACC_USERID_HI32_MASK );

    return (u64)hi32 << 32 | userdata->userid_lo32;
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_GetPlayerId

  Description:  DWCAccLoginId�\���̂���PlayerID���擾���܂��B

  Arguments:    userdata : ���[�U�f�[�^�̊i�[���ꂽDWCAccLoginId�\���̂ւ̃|�C���^

  Returns:      PlayerID 32bits
 *---------------------------------------------------------------------------*/
u32
DWCi_Acc_GetPlayerId( const DWCAccLoginId* userdata )
{
    return userdata->playerid;
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_GetFriendKey

  Description:  DWCAccFriendKey�\���̂���F�B�o�^�����擾���܂��B

  Arguments:    data : ���[�U�f�[�^�̊i�[���ꂽDWCAccFriendKey�\���̂ւ̃|�C���^

  Returns:      �F�B�o�^�� 64bits
 *---------------------------------------------------------------------------*/
u64
DWCi_Acc_GetFriendKey( const DWCAccFriendKey* data )
{
    return (u64)data->friendkey_hi32 << 32 | data->friendkey_lo32;
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_GetGsProfileId

  Description:  DWCAccFriendKey�\���̂���gsProfileId���擾���܂��B

  Arguments:    data : ���[�U�f�[�^�̊i�[���ꂽDWCAccGsProfileId�\���̂ւ̃|�C���^

  Returns:      �F�B�o�^�� 64bits
 *---------------------------------------------------------------------------*/
int
DWCi_Acc_GetGsProfileId( const DWCAccGsProfileId* data )
{
    return data->id;
}


/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_SetUserId

  Description:  DWCAccLoginId�\���̂�UserID���i�[���܂��B

  Arguments:    userdata : ���[�U�f�[�^���i�[����DWCAccLoginId�\���̂ւ̃|�C���^
                userid   : UserID 64bits

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWCi_Acc_SetUserId( DWCAccLoginId*  userdata,
                    u64             userid      )
{
    if ( !DWCi_Acc_SetMaskBits( &userdata->id_data, (u32)(userid>>32), DWC_ACC_USERID_HI32_SHIFT, DWC_ACC_USERID_HI32_MASK ) )
    {
        // userid���͈͊O�̒l�B
    }
    userdata->userid_lo32 = (u32)userid;
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_SetPlayerId

  Description:  DWCAccLoginId�\���̂�PlayerID���i�[���܂��B

  Arguments:    userdata : ���[�U�f�[�^���i�[����DWCAccLoginId�\���̂ւ̃|�C���^
                playerid : PlayerID 64bits

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWCi_Acc_SetPlayerId( DWCAccLoginId*    userdata,
                      u32               playerid        )
{
    userdata->playerid = playerid;
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_SetPlayerId

  Description:  DWCAccLoginId�\���̂�PlayerID���i�[���܂��B

  Arguments:    userdata : ���[�U�f�[�^���i�[����DWCAccLoginId�\���̂ւ̃|�C���^
                playerid : PlayerID 64bits

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWCi_Acc_SetFriendKey( DWCAccFriendKey* data,
                       u64              friendkey )
{
    data->friendkey_lo32 = (u32)friendkey;
    data->friendkey_hi32 = (u32)(friendkey >> 32);
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_SetGsProfileId

  Description:  DWCAccLoginId�\���̂�GsProfileId���i�[���܂��B

  Arguments:    userdata  : ���[�U�f�[�^���i�[����DWCAccLoginId�\���̂ւ̃|�C���^
                friendkey : PlayerID 64bits

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWCi_Acc_SetGsProfileId( DWCAccGsProfileId* data, int gs_profile_id )
{
    data->id = gs_profile_id;
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_GetFlags

  Description:  DWCAccFlag�\���̂̃t���O���擾���܂��B

  Arguments:    userdata : ���[�U�f�[�^���i�[����DWCAccLoginId�\���̂ւ̃|�C���^

  Returns:      �t���O�i�L��12bits�j
 *---------------------------------------------------------------------------*/
static u32
DWCi_Acc_GetFlags( const DWCAccFlag* userdata )
{
    return DWCi_Acc_GetMaskBits( userdata->flags, DWC_ACC_FLAGS_SHIFT, DWC_ACC_FLAGS_MASK );
}

// �f�[�^�^�C�v
static u32
DWCi_Acc_GetFlag_DataType( const DWCAccFlag* userdata )
{
    return DWCi_Acc_GetFlags( userdata ) & DWC_ACC_FRIENDDATA_MASK;
}

/*---------------------------------------------------------------------------*
  Name:         DWC_IsBuddyFriendData

  Description:  DWCAccFriendData�\���̂�GS�v���t�@�C��ID���i�[����Ă���Ƃ��A
                �F�B��񂪊��ɃT�[�o��ŗF�B�֌W�ɂȂ��Ă��邩�ǂ������m�F����B

  Arguments:    frienddata : �t�����h�f�[�^���i�[����DWCAccFriendData�\���̂ւ̃|�C���^

  Returns:      TRUE  : ���ɗF�B�֌W�ɂȂ��Ă���
                FALSE : �܂��Ȃ��Ă��Ȃ��B
 *---------------------------------------------------------------------------*/
BOOL
DWC_IsBuddyFriendData( const DWCAccFriendData* frienddata )
{
    if ( DWCi_Acc_GetFlag_DataType( &frienddata->flags ) == DWC_ACC_FRIENDDATA_GS_PROFILE_ID )
    {
        u32 type;

        type = DWCi_Acc_GetFlags( &frienddata->flags );

        return ( DWCi_CHECK_FLAG( type, DWC_ACC_FRIENDDATA_ISBUDDY ) ) ? TRUE : FALSE;
    }

    return FALSE;
}


/*---------------------------------------------------------------------------*
  Name:         DWC_

  Description:  DWCAccFriendData�\���̂�GS�v���t�@�C��ID���i�[����Ă���Ƃ��A
                �F�B��񂪊��ɃT�[�o��ŗF�B�֌W�ɂȂ��Ă��邩�ǂ������m�F����B

  Arguments:    frienddata : �t�����h�f�[�^���i�[����DWCAccFriendData�\���̂ւ̃|�C���^

  Returns:      TRUE  : ���ɗF�B�֌W�ɂȂ��Ă���
                FALSE : �܂��Ȃ��Ă��Ȃ��B
 *---------------------------------------------------------------------------*/
int
DWC_GetFriendDataType( const DWCAccFriendData* frienddata )
{
    return (int)DWCi_Acc_GetFlag_DataType( &frienddata->flags );
}


/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_SetFlags

  Description:  DWCAccLoginId�\���̂Ƀt���O���i�[���܂��B

  Arguments:    userdata : ���[�U�f�[�^���i�[����DWCAccLoginId�\���̂ւ̃|�C���^
                flags    : �t���O�i�L��12bits�j

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
static void
DWCi_Acc_SetFlags( DWCAccFlag* userdata,
                   u32         flags )
{
    DWCi_Acc_SetMaskBits( &userdata->flags, flags, DWC_ACC_FLAGS_SHIFT, DWC_ACC_FLAGS_MASK );
}

// �f�[�^�^�C�v
static void
DWCi_Acc_SetFlag_DataType( DWCAccFlag* userdata,
                           int         type )
{
    u32 flags;

    flags = DWCi_Acc_GetFlags( userdata );

    flags = ( flags & ~DWC_ACC_FRIENDDATA_MASK ) | type;

    DWCi_Acc_SetFlags( userdata, flags );
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_SetBuddyFriendData

  Description:  DWCAccFriendData�\���̂�GS�v���t�@�C��ID���i�[����Ă���Ƃ��A
                �F�B��񂪊��ɃT�[�o��ŗF�B�֌W�ɂȂ��Ă�����A�Z�b�g����B

  Arguments:    frienddata : �t�����h�f�[�^���i�[����DWCAccFriendData�\���̂ւ̃|�C���^

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWCi_SetBuddyFriendData( DWCAccFriendData* frienddata )
{
    if ( DWCi_Acc_GetFlag_DataType( &frienddata->flags ) == DWC_ACC_FRIENDDATA_GS_PROFILE_ID )
    {
        u32 flags;

        flags = DWCi_Acc_GetFlags( &frienddata->flags );

        flags = ( flags &  ~DWC_ACC_FRIENDDATA_ISBUDDY_MASK ) | DWC_ACC_FRIENDDATA_ISBUDDY;

        DWCi_Acc_SetFlags( &frienddata->flags, flags );
    }
}

/*---------------------------------------------------------------------------*
  Name:         DWC_ClearBuddyFlagFriendData

  Description:  DWCAccFriendData�\���̂�GS�v���t�@�C��ID���i�[����Ă���Ƃ��ŁA
                �F�B���ɃZ�b�g���ꂽ�F�B�����t���O���N���A����B

  Arguments:    frienddata : �t�����h�f�[�^���i�[����DWCAccFriendData�\���̂ւ̃|�C���^

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWC_ClearBuddyFlagFriendData( DWCAccFriendData* frienddata )
{
    if ( DWCi_Acc_GetFlag_DataType( &frienddata->flags ) == DWC_ACC_FRIENDDATA_GS_PROFILE_ID )
    {
        u32 flags;

        flags = DWCi_Acc_GetFlags( &frienddata->flags );

        flags &=  ~DWC_ACC_FRIENDDATA_ISBUDDY_MASK;

        DWCi_Acc_SetFlags( &frienddata->flags, flags );
    }
}

/*---------------------------------------------------------------------------*
  Name:         DWC_Acc_CreateFriendKey

  Description:  GS ProfileId����A�F�B�o�^���𐶐����܂��B

  Arguments:    userdata : �F�B�o�^���̌��ƂȂ�GS Profile ID�̊i�[���ꂽ�\���̂ւ̃|�C���^

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
u64
DWC_Acc_CreateFriendKey( int gs_profile_id,
                         u32 gamecode )
{
    MATHCRC8Table table;
    u32 code[2];
    int crc;

    code[0] = (u32)gs_profile_id;
    code[1] = gamecode;

    MATH_CRC8InitTable( &table );
    crc = MATH_CalcCRC8( &table, code, sizeof( code ) ) & 0x7f; // 7bits�̂ݗL��

    return ( ((u64)crc << 32) | (u32)gs_profile_id );
}


/*---------------------------------------------------------------------------*
  Name:         DWC_CheckFriendKey

  Description:  �F�B�o�^�������������̂��ǂ������m�F���܂��B

  Arguments:    userdata  : ���[�U�f�[�^
                friendkey : �F�B�o�^��

  Returns:      TRUE  : ������
                FALSE : �s���ȃR�[�h
 *---------------------------------------------------------------------------*/
BOOL
DWC_CheckFriendKey( const DWCAccUserData* userdata, u64 friendkey )
{
    return DWC_Acc_CheckFriendKey( friendkey, userdata->gamecode );
}

/*---------------------------------------------------------------------------*
  Name:         DWC_Acc_CheckFriendKey

  Description:  �F�B�o�^�������������̂��ǂ������m�F���܂��B

  Arguments:    friendkey : �F�B�o�^��
                gamecode  : �s���ȃR�[�h�̃L�[�ƂȂ�l

  Returns:      TRUE  : ������
                FALSE : �s���ȃR�[�h
 *---------------------------------------------------------------------------*/
BOOL
DWC_Acc_CheckFriendKey( u64 friendkey,
                        u32 gamecode )
{
    MATHCRC8Table table;
    u32 code[2];
    int crc;

    // ����GS�v���t�@�C��ID�͕s��
    if ( friendkey & 0x80000000 ) return FALSE;

    code[0] = (u32)friendkey;
    code[1] = gamecode;

    MATH_CRC8InitTable( &table );
    crc = MATH_CalcCRC8( &table, code, sizeof( code ) ) & 0x7f; // 7bits�̂ݗL��

    if ( crc != (friendkey >> 32) )
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         DWC_Acc_FriendKeyToGsProfileId

  Description:  �F�B�o�^������GS ProfileId�����o���܂��B

  Arguments:    friendkey : �F�B�o�^��
                gamecode  : �s���ȃR�[�h�̃L�[�ƂȂ�l

  Returns:      0�̂Ƃ��́A�擾���s�i�s���ȗF�B�o�^���j
                ����ȊO�̂Ƃ��́AGS ProfileId
 *---------------------------------------------------------------------------*/
int
DWC_Acc_FriendKeyToGsProfileId( u64 friendkey,
                                u32 gamecode )
{
    if ( !DWC_Acc_CheckFriendKey( friendkey, gamecode ) )
    {
        return 0;
    }

    return (int)( friendkey & 0xffffffff );
}

/*---------------------------------------------------------------------------*
  Name:         DWC_FriendKeyToString

  Description:  �F�B�o�^���𐔎��ŕ\�����ꂽ������ɕϊ����܂��B

  Arguments:    string      ������̏o�͐�
                friendkey   ���͂���F�B�o�^���i���l�\���j

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void DWC_FriendKeyToString( char* string, u64 friendKey )
{
    OS_SNPrintf( string, DWC_ACC_FRIENDKEY_STRING_BUFSIZE, "%012llu", friendKey );
}

/*---------------------------------------------------------------------------*
  Name:         DWC_StringToFriendKey

  Description:  �����ŕ\�����ꂽ�������F�B�o�^���i���l�\���j�ɕϊ����܂��B

  Arguments:    string      ���͂��鐔���ŕ\�����ꂽ������

  Returns:      �F�B�o�^���i���l�\���j
 *---------------------------------------------------------------------------*/
u64 DWC_StringToFriendKey( const char* string )
{
    int i;
    u64 result = 0;
    u64 dec = 1;

    for ( i = 0 ; i < DWC_ACC_FRIENDKEY_STRING_LENGTH ; ++i )
    {
        char tmp = *(string + DWC_ACC_FRIENDKEY_STRING_LENGTH - 1 - i);
        if(tmp < '0' || tmp > '9')
            return 0;
        
        result +=  (tmp - '0') * dec;

        dec *= 10;
    }

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_U64ToString32

  Description:  32�i������ɕϊ����܂��B5bits��1�����ŕ\�����܂��B

  Arguments:    num    : �ϊ����鐔�l
                bitnum : �ϊ�����r�b�g��
                result : ���ʂ��i�[���邽�߂̃o�b�t�@�B(bitnum+4)/5 + 1�o�C�g
                         ���̗̈悪�K�v�ł��B

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
static void
DWCi_Acc_U64ToString32( u64   num,
                        int   bitnum,
                        char* result )
{
    int i;
    const int loopnum = (bitnum + 4) / 5;
    const char* const sStringTable = "0123456789abcdefghijklmnopqrstuv";
    //                                          0123456789012345678901

    for ( i = 0 ; i < loopnum ; ++i )
    {
        *(result + loopnum - 1 - i) = *(sStringTable + (num & 0x1f));

        num >>= 5;
    }

    *( result + loopnum ) = 0; // NULL�I�[������B
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_LoginIdToUserName

  Description:  DWCAccLoginId�\���̂̃��O�C��ID�𕶎���\���ɕϊ����܂��B
                ���[�UID9���{�Q�[���R�[�h�{�v���C���[ID7���ɂȂ�܂��B
                NULL�I�[���܂߂�21�o�C�g�̏o�͗̈悪�K�v�ł��B

  Arguments:    loginid  : ���[�U�f�[�^�̊i�[���ꂽDWCAccLoginId�\���̂ւ̃|�C���^
                gamecode : �Q�[���R�[�h
                username : ���ʂ��i�[���邽�߂̃o�b�t�@�B21�o�C�g�K�v�ł��B

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWCi_Acc_LoginIdToUserName( const DWCAccLoginId* loginid,
                            u32                  gamecode,
                            char*                username )
{
    char userid[DWC_ACC_USERNAME_STRING_BUFSIZE];
    char playerid[DWC_ACC_USERNAME_STRING_BUFSIZE];

    DWCi_Acc_U64ToString32( DWCi_Acc_GetUserId( loginid ), DWC_ACC_USERID_BITS, userid );
    DWCi_Acc_U64ToString32( DWCi_Acc_GetPlayerId( loginid ), DWC_ACC_PLAYERID_BITS, playerid );

    OS_SNPrintf( username, DWC_ACC_USERNAME_STRING_BUFSIZE,
                 "%s%c%c%c%c%s",
                 userid,
                 (u8)(gamecode >> 24), (u8)(gamecode >> 16), (u8)(gamecode >> 8), (u8)gamecode,
                 playerid );
}

/*---------------------------------------------------------------------------*
  Name:         DWC_Acc_CreateUserData

  Description:  ���[�U�f�[�^���쐬���܂��B

  Arguments:    userdata  : ���[�U�f�[�^���i�[���邽�߂̃o�b�t�@
                gamecode  : �A�v���P�[�V�����ɔC�V�������蓖�Ă�Q�[���R�[�h

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWCi_Acc_CreateUserData( DWCAccUserData* userdata, u32 gamecode )
{
    MATHCRC32Table      table;

    MI_CpuClear8( userdata, DWC_ACC_USERDATA_BUFSIZE );

    // ���[�U���̐ݒ�
    userdata->size = DWC_ACC_USERDATA_BUFSIZE;
    userdata->gs_profile_id = 0;
    userdata->gamecode = gamecode;

    // �e���|������LoginId�𐶐�����B
    DWCi_Acc_CreateTempLoginId( &userdata->pseudo );

    // Userdata���쐬����Ƃ��́A�F�؂��ꂽID�͐����ł��Ȃ��̂ŁA����Ȃ��B
    DWCi_Acc_SetFlag_DataType( (DWCAccFlag*)&userdata->authentic, DWC_ACC_FRIENDDATA_NODATA   );

    // CRC�̌v�Z
    MATH_CRC32InitTable( &table );
    userdata->crc32 = MATH_CalcCRC32( &table, userdata, sizeof( DWCAccUserData ) - 4 );

    // dirty�t���O�𗧂Ă�B
    userdata->flag |= DWC_ACC_USERDATA_DIRTY;
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_CreateTempLoginId

  Description:  �e���|�������O�C��ID���쐬���܂��B

  Arguments:    loginid  : �m�F���郍�O�C��ID���i�[���ꂽ�|�C���^

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWCi_Acc_CreateTempLoginId( DWCAccLoginId* loginid )
{
    DWCAuthWiFiId       authid;
    MATHRandContext32   randcontext;
	u32					buffer[8];
	u32					i;
	
	// [nakata] �����_���R���e�L�X�g�̏�������Tick�x�[�X����LowEntropyData�x�[�X�ɕύX
	OS_GetLowEntropyData(buffer);
	for(i=1; i<8; i++) {
		buffer[i] = buffer[i]^buffer[i-1];
	}
    MATH_InitRand32( &randcontext, buffer[7] );

    // �{�̂̃��[�UID������B
    DWC_Auth_GetId( &authid );
    if ( authid.flg )
    {
        DWCi_Acc_SetUserId( loginid, authid.uId );
    }
    else
    {
        DWCi_Acc_SetUserId( loginid, authid.notAttestedId );
    }
#if 0
    DWCi_Acc_SetPlayerId( loginid, 1 ); // �f�o�b�O�p�ɌŒ�l�ɂ���B
#else
    DWCi_Acc_SetPlayerId( loginid, MATH_Rand32( &randcontext, 0 ) );
#endif
    DWCi_Acc_SetFlag_DataType( (DWCAccFlag*)loginid,    DWC_ACC_FRIENDDATA_LOGIN_ID );
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_CheckConsoleUserId

  Description:  ���O�C��ID�̃��[�UID�ƃR���\�[���̃��[�UID���r����

  Arguments:    loginid  : �m�F���郍�O�C��ID���i�[���ꂽ�|�C���^

  Returns:      TRUE  : ����
                FALSE : �Ⴄ
 *---------------------------------------------------------------------------*/
BOOL
DWCi_Acc_CheckConsoleUserId( const DWCAccLoginId* loginid )
{
    DWCAuthWiFiId       authid;
    
    DWC_Auth_GetId( &authid );
    if ( authid.flg )
    {
        return (DWCi_Acc_GetUserId( loginid ) == authid.uId) ? TRUE : FALSE;
    }
    else
    {
        return (DWCi_Acc_GetUserId( loginid ) == authid.notAttestedId) ? TRUE : FALSE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_IsValidLoginId

  Description:  ���O�C��ID�����������̂��ǂ������m�F���܂��B

  Arguments:    loginid  : �m�F���郍�O�C��ID���i�[���ꂽ�|�C���^

  Returns:      TRUE  : loginID���i�[����Ă���
                FALSE : �i�[����Ă�����̂̓��O�C��ID�ł͂Ȃ�
 *---------------------------------------------------------------------------*/
BOOL
DWCi_Acc_IsValidLoginId( const DWCAccLoginId* loginid )
{
    return ( DWCi_Acc_GetFlag_DataType( (DWCAccFlag*)loginid ) == DWC_ACC_FRIENDDATA_LOGIN_ID ) ? TRUE : FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_IsAuthentic

  Description:  ���[�U�f�[�^���F�؍ς݂��ǂ������m�F���܂��B

  Arguments:    userdata  : �m�F���郆�[�U�f�[�^���i�[���ꂽ�|�C���^

  Returns:      TRUE  : ���ɔF�؍ς݁B
                FALSE : �܂��F�؂��󂯂Ă��Ȃ��B
 *---------------------------------------------------------------------------*/
BOOL
DWCi_Acc_IsAuthentic( const DWCAccUserData* userdata )
{
    return DWCi_Acc_IsValidLoginId( &userdata->authentic );
}

/*---------------------------------------------------------------------------*
  Name:         DWC_IsValidFriendData / DWCi_Acc_IsValidFriendData

  Description:  FriendData�ɗL���ȃf�[�^���Z�b�g����Ă��邩�ǂ����𒲂ׂ܂��B

  Arguments:    frienddata : FriendData�\���̂ւ̃|�C���^

  Returns:      TRUE  : �L���ȃf�[�^
                FALSE : �����ȃf�[�^�i�f�[�^�Ȃ��j
 *---------------------------------------------------------------------------*/
BOOL
DWC_IsValidFriendData( const DWCAccFriendData* frienddata )
{
    u32 type = DWCi_Acc_GetFlag_DataType( &frienddata->flags );

    if ( type ==  DWC_ACC_FRIENDDATA_NODATA )
    {
        return FALSE;
    }

    return TRUE;
}

// �ߋ��̌݊����̂��߁B
BOOL
DWCi_Acc_IsValidFriendData( const DWCAccFriendData* frienddata )
{
    return DWC_IsValidFriendData( frienddata );
}

/*---------------------------------------------------------------------------*
  Name:         DWC_CreateUserData

  Description:  ���[�U�f�[�^�𐶐����܂��B���̃��[�U�f�[�^���o�b�N�A�b�v��������
                �ۑ����Ă��������B

  Arguments:    userdata  : �������ꂽ���[�U�f�[�^���i�[����|�C���^

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWC_CreateUserData( DWCAccUserData* userdata, u32 gamecode )
{
    DWCi_Acc_CreateUserData( userdata, gamecode );
}

/*---------------------------------------------------------------------------*
  Name:         DWC_CheckUserData

  Description:  ���[�U�f�[�^���������܂��B

  Arguments:    userdata  : �������郆�[�U�f�[�^���i�[���ꂽ�|�C���^

  Returns:      TRUE  : ����
                FALSE : �j�󂳂�Ă���
 *---------------------------------------------------------------------------*/
BOOL
DWC_CheckUserData( const DWCAccUserData* userdata )
{
    u32                 crc32;
    MATHCRC32Table      table;

    MATH_CRC32InitTable( &table );
    crc32 = MATH_CalcCRC32( &table, userdata, sizeof( DWCAccUserData ) - 4 );

    return ( crc32 == userdata->crc32 ) ? TRUE : FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         DWC_CheckHasProfile

  Description:  ���[�U�f�[�^�����Ƀv���t�@�C��ID���擾���Ă��邩���m�F����B

  Arguments:    userdata  : �������郆�[�U�f�[�^���i�[���ꂽ�|�C���^

  Returns:      TRUE  : ����ProfileId���擾���Ă���
                FALSE : ProfileId�͂܂��擾����Ă��Ȃ�
 *---------------------------------------------------------------------------*/
BOOL
DWC_CheckHasProfile( const DWCAccUserData* userdata )
{
    return ( DWCi_Acc_IsValidLoginId( &userdata->authentic ) && userdata->gs_profile_id > 0 ) ? TRUE : FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         DWC_CheckValidConsole

  Description:  ���[�U�f�[�^�Ɩ{�̂̐��������m�F���܂��B

  Arguments:    userdata  : �������郆�[�U�f�[�^���i�[���ꂽ�|�C���^

  Returns:      TRUE  : �{�̂Ɋi�[����Ă��郆�[�UID�ƃp�X���[�h�͎g�p�\ 
                FALSE : �g�p�s��
 *---------------------------------------------------------------------------*/
BOOL
DWC_CheckValidConsole( const DWCAccUserData* userdata )
{
    DWCAuthWiFiId       authid;

    if ( DWCi_Acc_GetFlag_DataType( (DWCAccFlag*)&userdata->authentic ) == DWC_ACC_FRIENDDATA_NODATA )
    {
        // �F�؂��ꂽ���O�C��ID���Ȃ���΁ATRUE
        return TRUE;
    }

    DWC_Auth_GetId( &authid );

    if ( !authid.flg )
    {
        // �F�؂��ꂽ���O�C��ID������ɂ�������炸�A�{�̂̃��[�UID���F�؂���Ă��Ȃ���΁AFALSE
        return FALSE;
    }

    return ( DWCi_Acc_GetUserId( &userdata->authentic ) == authid.uId ) ? TRUE : FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_SetUserIdToUserData

  Description:  ���[�U�f�[�^��UserID/PlayerID�Ȃǂ̏����Z�b�g���܂��B

  Arguments:    userdata  : ���[�U�f�[�^���i�[���邽�߂̃o�b�t�@
                userid    : ���[�UID
                playerid  : �v���[���[ID

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWCi_Acc_SetLoginIdToUserData( DWCAccUserData*      userdata,
                               const DWCAccLoginId* loginid,
                               int                  gs_profile_id )
{
    MATHCRC32Table      table;

    // loginID�̐ݒ�
    userdata->authentic = *loginid;

    // gs_profile_id�̐ݒ�
    userdata->gs_profile_id = gs_profile_id;

    // CRC�̌v�Z
    MATH_CRC32InitTable( &table );
    userdata->crc32 = MATH_CalcCRC32( &table, userdata, sizeof( DWCAccUserData ) - 4 );

    // dirty�t���O�𗧂Ă�B
    userdata->flag |= DWC_ACC_USERDATA_DIRTY;
}


/*---------------------------------------------------------------------------*
  Name:         DWC_CheckDirtyFlag / DWCi_Acc_IsDirty

  Description:  ���[�U�f�[�^�̕ύX�t���O�`�F�b�N���܂��B

  Arguments:    userdata  : ���[�U�f�[�^���i�[���邽�߂̃o�b�t�@

  Returns:      TRUE  : �ύX����Ă���B
                FALSE : �ύX����Ă��Ȃ��B
 *---------------------------------------------------------------------------*/
BOOL
DWC_CheckDirtyFlag( const DWCAccUserData* userdata )
{
    return DWCi_Acc_IsDirty( userdata );
}

BOOL
DWCi_Acc_IsDirty( const DWCAccUserData* userdata )
{
    return DWCi_CHECK_FLAG( userdata->flag, DWC_ACC_USERDATA_DIRTY ) ? TRUE : FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_ClearDirty

  Description:  ���[�U�f�[�^�̕ύX�t���O���N���A���܂��B

  Arguments:    userdata  : ���[�U�f�[�^���i�[���邽�߂̃o�b�t�@

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWC_ClearDirtyFlag( DWCAccUserData* userdata )
{
    DWCi_Acc_ClearDirty( userdata );
}

void
DWCi_Acc_ClearDirty( DWCAccUserData* userdata )
{
    MATHCRC32Table      table;

    userdata->flag &= ~DWC_ACC_USERDATA_DIRTY;

    // CRC�̌v�Z
    MATH_CRC32InitTable( &table );
    userdata->crc32 = MATH_CalcCRC32( &table, userdata, sizeof( DWCAccUserData ) - 4 );
}

/*---------------------------------------------------------------------------*
  Name:         DWCi_Acc_TestFlagFunc

  Description:  �e�X�g�p�̓����֐��B

  Arguments:    �Ȃ�

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWCi_Acc_TestFlagFunc( void )
{
    DWCAccLoginId id;
    u32 flags;

    MI_CpuClear8( &id, sizeof( DWCAccLoginId ) );

    flags = DWCi_Acc_GetFlags( (DWCAccFlag*)&id );
    OS_TPrintf("%08x: %08x\n",flags, DWCi_Acc_GetFlag_DataType((DWCAccFlag*)&id ) );
    if ( DWCi_Acc_IsValidLoginId( &id ) )
    {
        OS_TPrintf("valid\n");
    }
    else
    {
        OS_TPrintf("invalid\n");
    }

    DWCi_Acc_SetFlag_DataType( (DWCAccFlag*)&id , DWC_ACC_FRIENDDATA_LOGIN_ID );

    flags = DWCi_Acc_GetFlags( (DWCAccFlag*)&id );
    OS_TPrintf("%08x: %08x\n",flags, DWCi_Acc_GetFlag_DataType((DWCAccFlag*)&id ) );
    if ( DWCi_Acc_IsValidLoginId( &id ) )
    {
        OS_TPrintf("valid\n");
    }
    else
    {
        OS_TPrintf("invalid\n");
    }
}

/*---------------------------------------------------------------------------*
  Name:         DWC_GetFriendKey

  Description:  FriendData����F�B�o�^�����擾���܂��B

  Arguments:    frienddata : FriendData�\���̂ւ̃|�C���^

  Returns:      0�ȊO : �F�B�o�^��
                0     : ���͂��������Ȃ�
 *---------------------------------------------------------------------------*/
u64
DWC_GetFriendKey( const DWCAccFriendData* frienddata )
{
    if ( DWCi_Acc_GetFlag_DataType( &frienddata->flags ) == DWC_ACC_FRIENDDATA_FRIEND_KEY )
    {
        return DWCi_Acc_GetFriendKey( &frienddata->friend_key );
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         DWC_GetGsProfileId

  Description:  FriendData����gsProfileId���擾���܂��B

  Arguments:    userdata   : �����̃��[�U�f�[�^�̊i�[����Ă���|�C���^
                frienddata : FriendData�\���̂ւ̃|�C���^

  Returns:      0�ȊO : gsProfileId
                0     : ���͂��������Ȃ�
                -1    : ��ID -> LoginID������f�[�^���擾���āA�������Ă��������B
 *---------------------------------------------------------------------------*/
int
DWC_GetGsProfileId( const DWCAccUserData* userdata,
                    const DWCAccFriendData* frienddata )
{
    u32 type = DWCi_Acc_GetFlag_DataType( &frienddata->flags );

    switch ( type )
    {
    case DWC_ACC_FRIENDDATA_FRIEND_KEY:
        {
            u64 friend_key = DWCi_Acc_GetFriendKey( &frienddata->friend_key );

            if ( DWC_Acc_CheckFriendKey( friend_key, userdata->gamecode ) )
            {
                return DWC_Acc_FriendKeyToGsProfileId( friend_key, userdata->gamecode );
            }

            return 0;
        }
        // NOT REACH HERE
        break;

    case DWC_ACC_FRIENDDATA_GS_PROFILE_ID:
        return DWCi_Acc_GetGsProfileId( &frienddata->gs_profile_id );
        // NOT REACH HERE
        break;
    case DWC_ACC_FRIENDDATA_LOGIN_ID:
        return -1;
        // NOT REACH HERE
        break;
    default:
        // NOT REACH HERE
        break;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         DWC_CreateFriendKey

  Description:  ���[�U�f�[�^����A����ɓn���F�B�o�^���𐶐����܂��B
                �F�B�o�^���̓Q�[���R�[�h�Ɉˑ����܂��B

  Arguments:    userdata : �����̃��[�U�f�[�^�̊i�[����Ă���|�C���^

  Returns:      0�ȊO : �F�B�o�^��
                0     : ���͂��������Ȃ��i���Ƃ��΁AgsProfileId���擾�Ȃǁj
 *---------------------------------------------------------------------------*/
u64
DWC_CreateFriendKey( const DWCAccUserData* userdata )
{
    u64 ret = 0;

    if ( userdata->gs_profile_id != 0 )
    {
        ret = DWC_Acc_CreateFriendKey( userdata->gs_profile_id, userdata->gamecode );
    }

    return ret;
}

/*---------------------------------------------------------------------------*
  Name:         DWC_CreateFriendKeyToken

  Description:  �F�B���X�g�Ɋi�[���邽�߂�FriendData��F�B�o�^������쐬���܂��B

  Arguments:    token      : FriendData���i�[���邽�߂̃|�C���^
                friend_key : �F�B�o�^��

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWC_CreateFriendKeyToken( DWCAccFriendData* token,
                          u64 friend_key )
{
    MI_CpuClear8( token, sizeof( DWCAccFriendData ) );
    DWCi_Acc_SetFriendKey( &token->friend_key, friend_key );
    DWCi_Acc_SetFlag_DataType( &token->flags, DWC_ACC_FRIENDDATA_FRIEND_KEY );
}

/*---------------------------------------------------------------------------*
  Name:         DWC_CreateExchangeToken

  Description:  �F�B���X�g�Ɋi�[���邽�߂�FriendData��UserData����쐬���܂��B

  Arguments:    userdata : ���[�U�f�[�^���i�[����Ă���|�C���^
                token      : FriendData���i�[���邽�߂̃|�C���^

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWC_CreateExchangeToken( const DWCAccUserData* userdata,
                         DWCAccFriendData* token )
{
    MI_CpuClear8( token, sizeof( DWCAccFriendData ) );

    if ( DWCi_Acc_IsAuthentic( userdata ) )
    {
        DWCi_Acc_SetGsProfileId( &token->gs_profile_id, userdata->gs_profile_id );
        DWCi_Acc_SetFlag_DataType( &token->flags, DWC_ACC_FRIENDDATA_GS_PROFILE_ID );
    }
    else
    {
        // ���낢��ȃt���O�����āA�m�����グ�悤�Ƃ������A��ID�𐶐�����{�̂�
        // �l�b�g�ɂȂ��{ID���擾����{�̂̔�������Ȃ��悤�ɂ������߁A�قƂ�ǂ�
        // ���͖��ʂɂȂ��Ă��܂��B
        token->login_id = userdata->pseudo;
    }
}

/*---------------------------------------------------------------------------*
  Name:         DWC_SetGsProfileId

  Description:  �F�B���X�g��gsProfileId���i�[���܂��B

  Arguments:    frienddata    : FriendData�\���̂ւ̃|�C���^
                gs_profile_id : gsProfileId

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWC_SetGsProfileId( DWCAccFriendData* frienddata,
                    int gs_profile_id )
{
    MI_CpuClear8( frienddata, sizeof( DWCAccFriendData ) );

    DWCi_Acc_SetGsProfileId( &frienddata->gs_profile_id, gs_profile_id );
    DWCi_Acc_SetFlag_DataType( &frienddata->flags, DWC_ACC_FRIENDDATA_GS_PROFILE_ID );
}

/*---------------------------------------------------------------------------*
  Name:         DWC_LoginIdToUserName

  Description:  �F�B���������邽�߂̃��[�U�l�[�����擾���܂��B

  Arguments:    userdata : ���[�U�f�[�^���i�[����Ă���|�C���^
                token    : FriendData���i�[����Ă���|�C���^
                username : ���ʂ��o�͂���o�b�t�@(DWC_ACC_USERNAME_STRING_BUFSIZE)

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWC_LoginIdToUserName( const DWCAccUserData* userdata,
                       const DWCAccFriendData* frienddata,
                       char* username )
{
    DWCi_Acc_LoginIdToUserName( &frienddata->login_id, userdata->gamecode, username );
}

/*---------------------------------------------------------------------------*
  Name:         DWC_IsEqualFriendData

  Description:  �F�B���DWCFriendData���������ǂ������m�F���܂��B

  Arguments:    frienddata1 : ��r����F�B���ւ̃|�C���^
                frienddata2 : ��r����F�B���ւ̃|�C���^

  Returns:      TRUE  : ����
                FALSE : �Ⴄ
 *---------------------------------------------------------------------------*/
BOOL
DWC_IsEqualFriendData( const DWCAccFriendData* frienddata1,
                       const DWCAccFriendData* frienddata2 )
{
    u32 type1, type2;
    type1 = DWCi_Acc_GetFlag_DataType( &frienddata1->flags );
    type2 = DWCi_Acc_GetFlag_DataType( &frienddata2->flags );

    if ( type1 != type2 )
    {
        return FALSE;
    }
    else if ( type1 == DWC_ACC_FRIENDDATA_GS_PROFILE_ID )
    {
        return ( DWCi_Acc_GetGsProfileId( &frienddata1->gs_profile_id ) == DWCi_Acc_GetGsProfileId( &frienddata2->gs_profile_id ) ) ?
            TRUE : FALSE;
    }
    else if ( type1 == DWC_ACC_FRIENDDATA_LOGIN_ID )
    {
        return ( ( DWCi_Acc_GetUserId  ( &frienddata1->login_id ) == DWCi_Acc_GetUserId  ( &frienddata2->login_id ) ) &&
                 ( DWCi_Acc_GetPlayerId( &frienddata1->login_id ) == DWCi_Acc_GetPlayerId( &frienddata2->login_id ) )    ) ?
            TRUE : FALSE;
    }
    else if ( type1 == DWC_ACC_FRIENDDATA_FRIEND_KEY )
    {
        return ( DWCi_Acc_GetFriendKey( &frienddata1->friend_key ) == DWCi_Acc_GetFriendKey( &frienddata2->friend_key ) ) ?
            TRUE : FALSE;
    }

    return FALSE;
}

void
DWC_ReportFriendData( const DWCAccUserData* userdata, const DWCAccFriendData* frienddata )
{
    if ( DWCi_Acc_GetFlag_DataType( &frienddata->flags ) == DWC_ACC_FRIENDDATA_GS_PROFILE_ID )
    {
        if ( DWC_IsBuddyFriendData( frienddata ) )
        {
            OS_TPrintf(" GS_ID : %d (ok)\n", DWCi_Acc_GetGsProfileId( &frienddata->gs_profile_id ));
        }
        else
        {
            OS_TPrintf(" GS_ID : %d\n", DWCi_Acc_GetGsProfileId( &frienddata->gs_profile_id ));
        }
    }
    else if ( DWCi_Acc_GetFlag_DataType( &frienddata->flags ) == DWC_ACC_FRIENDDATA_FRIEND_KEY )
    {
        char name[DWC_ACC_FRIENDKEY_STRING_BUFSIZE];
        DWC_FriendKeyToString( name, DWCi_Acc_GetFriendKey( &frienddata->friend_key ));
        OS_TPrintf(" F_KEY : %s\n", name );
    }
    else if ( DWCi_Acc_GetFlag_DataType( &frienddata->flags ) == DWC_ACC_FRIENDDATA_LOGIN_ID )
    {
        char name[DWC_ACC_USERNAME_STRING_BUFSIZE];
        DWCi_Acc_LoginIdToUserName( &frienddata->login_id, userdata->gamecode, name );
        OS_TPrintf(" LN_ID : %s\n", name );
    }
    else
    {
        OS_TPrintf(" NO_DATA \n");
    }
    
}

void
DWC_ReportUserData( const DWCAccUserData* userdata )
{
    OS_TPrintf("*******************************\n");
    OS_TPrintf(" [pseudo login id]\n");
    DWC_ReportFriendData( userdata, (DWCAccFriendData*)&userdata->pseudo );
    OS_TPrintf("+++++++++++++++++++++++++++++++\n");
    OS_TPrintf(" [authentic login id]\n");
    DWC_ReportFriendData( userdata, (DWCAccFriendData*)&userdata->authentic );
    OS_TPrintf("+++++++++++++++++++++++++++++++\n");
    OS_TPrintf(" GS_ID : %d\n", userdata->gs_profile_id );
    OS_TPrintf("*******************************\n");
}
