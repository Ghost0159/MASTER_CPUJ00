/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - libraries
  File:     wm_ks.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wm_ks.c,v $
  Revision 1.7  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.6  2005/06/07 05:45:26  seiki_masashi
  Key Sharing �Ɋւ�����ʏ�����ጸ���邽�߂̕ύX

  Revision 1.5  2005/03/03 14:46:19  seiki_masashi
  WM_DISABLE_KEYSHARING �X�C�b�`�̒ǉ�
  WM_EndKeySharing �֐�����ɃG���[�l��Ԃ��Ă����̂��C��

  Revision 1.4  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.3  2004/11/02 07:26:51  terui
  �R�����g�C���B

  Revision 1.2  2004/10/22 04:37:28  terui
  WMErrCode�Ƃ���int�^��Ԃ��Ă����֐��ɂ��āAWMErrCode�񋓌^��Ԃ��悤�ɕύX�B

  Revision 1.1  2004/09/10 11:03:30  terui
  wm.c�̕����ɔ����A�V�Kupload�B

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    <nitro/wm.h>
#include    "wm_arm9_private.h"


/*---------------------------------------------------------------------------*
  Name:         WM_StartKeySharing

  Description:  �L�[�V�F�A�����O�@�\��L���ɂ���B
                �@�\��L���ɂ�����MP�ʐM���s�����ƂŁAMP�ʐM�ɕt������
                �L�[�V�F�A�����O�ʐM���s����B

  Arguments:    buf         -   �L�[�����i�[����o�b�t�@�ւ̃|�C���^�B
                                ���̂� WMDataSharingInfo �\���̂ւ̃|�C���^�B
                port        -   �g�p���� port �ԍ�

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartKeySharing(WMKeySetBuf *buf, u16 port)
{
    return WM_StartDataSharing(buf, port, 0xffff, WM_KEYDATA_SIZE, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         WM_EndKeySharing

  Description:  �L�[�V�F�A�����O�@�\�𖳌��ɂ���B

  Arguments:    buf         -   �L�[�����i�[����o�b�t�@�ւ̃|�C���^�B
                                ���̂� WMDataSharingInfo �\���̂ւ̃|�C���^�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_EndKeySharing(WMKeySetBuf *buf)
{
    return WM_EndDataSharing(buf);
}

/*---------------------------------------------------------------------------*
  Name:         WM_GetKeySet

  Description:  �L�[�V�F�A�����O���ꂽ�L�[�Z�b�g�f�[�^���ЂƂǂݏo���B

  Arguments:    buf         -   �L�[�����i�[����o�b�t�@�ւ̃|�C���^�B
                                ���̂� WMDataSharingInfo �\���̂ւ̃|�C���^�B
                keySet      -   �L�[�Z�b�g��ǂݏo���o�b�t�@�ւ̃|�C���^�B
                                WM_StartKeySharing�ɂė^�����o�b�t�@�Ƃ�
                                �ʂ̃o�b�t�@���w�肷��B

  Returns:      MWErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_GetKeySet(WMKeySetBuf *buf, WMKeySet *keySet)
{
    WMErrCode result;
    u16     sendData[WM_KEYDATA_SIZE / sizeof(u16)];
    WMDataSet ds;
    WMArm9Buf *p = WMi_GetSystemWork();

    {
        sendData[0] = (u16)PAD_Read();
        result = WM_StepDataSharing(buf, sendData, &ds);
        if (result == WM_ERRCODE_SUCCESS)
        {
            keySet->seqNum = buf->currentSeqNum;

            {
                u16     iAid;
                for (iAid = 0; iAid < 16; iAid++)
                {
                    u16    *keyData;
                    keyData = WM_GetSharedDataAddress(buf, &ds, iAid);
                    if (keyData != NULL)
                    {
                        keySet->key[iAid] = keyData[0];
                    }
                    else
                    {
                        // ��M�Ɏ��s��������� 0
                        keySet->key[iAid] = 0;
                    }
                }
            }
            return WM_ERRCODE_SUCCESS; // �����I��
        }
        else
        {
            return result;
        }
    }
}

/*---------------------------------------------------------------------------*
    End of file
 *---------------------------------------------------------------------------*/
