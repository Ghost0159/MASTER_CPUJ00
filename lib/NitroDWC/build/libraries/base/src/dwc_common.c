#include <string.h>
#include <nitro.h>

#include <base/dwc_common.h>


//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// variable
//----------------------------------------------------------------------------
static MATHRandContext32 stRandContext32 = { 0, 0, 0 };  // �����R���e�L�X�g


//----------------------------------------------------------------------------
// prototype
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// function - external
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  �ėpkey/value�^������쐬�֐�
  �����@�Fkey       �ݒ肵����key������
          value     �ݒ肵����value������
          string    key/value�^������i�[��|�C���^
          separator �e������𕪂����؂蕶��
  �߂�l�F�Z�b�g���ꂽkey/value�����񒷁iNULL�I�[�܂܂��j
  �p�r�@�Fkey/value�̕�������w�肵�ADWC�ėpkey/value��������쐬����
 *---------------------------------------------------------------------------*/
int  DWC_SetCommonKeyValueString(const char* key, const char* value, char* string, char separator)
{

    SDK_ASSERT(key && value);

    OS_SNPrintf(string, DWC_COMMONSTR_MAX_KEY_VALUE_LEN,
                "%c%s%c%s",
                separator, key, separator, value);

    return (int)strlen(string);
}


/*---------------------------------------------------------------------------*
  �ėpkey/value�^������ǉ��֐�
  �����@�Fkey       �ݒ肵����key������
          value     �ݒ肵����value������
          string    key/value�^������i�[��|�C���^
          separator �e������𕪂����؂蕶��
  �߂�l�Fkey/value�����񂪒ǉ����ꂽ���key/value�����񒷁iNULL�I�[�܂܂��j
  �p�r�@�Fkey/value�̕�����������̕�����ɒǉ�����
 *---------------------------------------------------------------------------*/
int  DWC_AddCommonKeyValueString(const char* key, const char* value, char* string, char separator)
{

    (void)DWC_SetCommonKeyValueString(key, value, strchr(string, '\0'), separator);

    return (int)strlen(string);
}


/*---------------------------------------------------------------------------*
  �ėpkey/value�^������value�擾�֐�
  �����@�Fkey       ���o������key������
          value     ���o����value������̊i�[��|�C���^�B
          string    key/value�^������
          separator �e������𕪂����؂蕶��
  �߂�l�Fvalue�����񒷁iNULL�I�[�܂܂��j�B���݂��Ȃ�key���w�肵���ꍇ��-1��Ԃ�
  �p�r�@�F�w�肵����؂蕶���ŋ�؂�ꂽDWC�ėpkey/value�^�����񂩂�A
          �w�肳�ꂽkey������ɑΉ�����value��������擾����B
 *---------------------------------------------------------------------------*/
int  DWC_GetCommonValueString(const char* key, char* value, const char* string, char separator)
{
    const char* pSrcBegin;
    char* pSrcEnd;
    int len;

    SDK_ASSERT(key);

    if (!value) return -1;

    pSrcBegin = strchr(string, separator);  // key/value������̊J�n�|�C���^���擾
    if (!pSrcBegin) return -1;

    while (1){
        if (!strncmp(pSrcBegin+1, key, strlen(key)) &&
            (pSrcBegin[strlen(key)+1] == separator)){
            break;  // key����
        }

        // ����key��T���B
        pSrcBegin = strchr(pSrcBegin+1, separator);
        if (!pSrcBegin) return -1;
        pSrcBegin = strchr(pSrcBegin+1, separator);
        if (!pSrcBegin) return -1;
    }

    // value�̊J�n�|�C���^���擾
    pSrcBegin = strchr(pSrcBegin+1, separator);
    if (!pSrcBegin) return -1;
    pSrcBegin++;

    // value�̕����񒷂��擾
    pSrcEnd = strchr(pSrcBegin, separator);
    if (pSrcEnd) len = (int)(pSrcEnd-pSrcBegin);
    else len = (int)strlen(pSrcBegin);

    strncpy(value, pSrcBegin, (u32)len);  // value��������R�s�[
    value[len] = '\0';

    return len;
}


//----------------------------------------------------------------------------
// function - internal
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  32bit�����l�擾�֐�
  �����@�Fmax �擾���闐���̍ő�l�B�����Őݒ肵�� max-1 �������闐����
              �ő�l�ƂȂ�B0���w�肵���ꍇ�ɂ͂��ׂĂ͈̔͂̐���������B
  �߂�l�F32bit�����l
  �p�r�@�F�֐�MATH_Rand32()�Ő�������32bit�����l���擾����
 *---------------------------------------------------------------------------*/
u32  DWCi_GetMathRand32(u32 max)
{
    u64 seed;

    if (!stRandContext32.x && !stRandContext32.mul && !stRandContext32.add){
        // �����R���e�L�X�g������������Ă��Ȃ��ꍇ�͏���������
        OS_GetMacAddress((u8 *)&seed);
        seed = ((seed >> 24) & 0xffffff) | (OS_GetTick() << 24);
        MATH_InitRand32(&stRandContext32, seed);
    }

    return MATH_Rand32(&stRandContext32, max);
}


/*---------------------------------------------------------------------------*
  ���C�h������̕����񒷎擾
  �����@�Fstr ���C�h������
  �߂�l�F������
 *---------------------------------------------------------------------------*/
u32 DWCi_WStrLen( const u16* str )
{
    u32 length = 0;

    while ( str[length] != 0 )
    {
        length++;
    }

    return length;
}


//----------------------------------------------------------------------------
// function - static
//----------------------------------------------------------------------------
