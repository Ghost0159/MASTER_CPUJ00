/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d
  File:     g2d_Screen.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.8 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_G2D_SCREEN_H_
#define NNS_G2D_SCREEN_H_

#include <nitro.h>
#include <nnsys/g2d.h>
#include <nnsys/g2d/fmt/g2d_Screen_data.h>

#ifdef __cplusplus
extern "C" {
#endif



//-------------------------------------------------------------------------
// �񋓎q
//-------------------------------------------------------------------------

// �����Ώۂ�BG�w��p
typedef enum NNSG2dBGSelect
{
    NNS_G2D_BGSELECT_MAIN0,
    NNS_G2D_BGSELECT_MAIN1,
    NNS_G2D_BGSELECT_MAIN2,
    NNS_G2D_BGSELECT_MAIN3,
    NNS_G2D_BGSELECT_SUB0,
    NNS_G2D_BGSELECT_SUB1,
    NNS_G2D_BGSELECT_SUB2,
    NNS_G2D_BGSELECT_SUB3,
    NNS_G2D_BGSELECT_NUM
}
NNSG2dBGSelect;

#define NNS_G2D_BG_ASSERT(bg) SDK_MINMAX_ASSERT( bg, NNS_G2D_BGSELECT_MAIN0, NNS_G2D_BGSELECT_NUM-1 )


//-------------------------------------------------------------------------
// �֐��錾
//-------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dBGSetupEx

  Description:  BG�̐ݒ���s���ABG�f�[�^��VRAM�ɓǂݍ��݂܂��B

  Arguments:    bg:         �ΏۂƂ��� BG
                pScnData:   BG �Ŏg�p����X�N���[���f�[�^�ւ̃|�C���^�B
                            NULL �ł����Ă͂Ȃ�܂���B
                pChrData:   BG �Ŏg�p����L�����N�^�f�[�^�ւ̃|�C���^�B
                            NULL �̏ꍇ��VRAM�ɓǂݍ��݂܂���B
                pPltData:   BG �Ŏg�p����p���b�g�f�[�^�ւ̃|�C���^�B
                            NULL �̏ꍇ��VRAM�ɓǂݍ��݂܂���B
                pPosInfo:   �L�����N�^���o�̈���ւ̃|�C���^�B
                pCmpInfo:   �p���b�g���k���ւ̃|�C���^�B
                scnBase:    BG �̃X�N���[���x�[�X�B
                chrBase:    BG �̃L�����N�^�x�[�X�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNS_G2dBGSetupEx(
    NNSG2dBGSelect bg,
    const NNSG2dScreenData* pScnData,
    const NNSG2dCharacterData* pChrData,
    const NNSG2dPaletteData* pPltData,
    const NNSG2dCharacterPosInfo* pPosInfo,
    const NNSG2dPaletteCompressInfo* pCmpInfo,
    GXBGScrBase scnBase,
    GXBGCharBase chrBase
);

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dBGLoadElements

  Description:  BG �ŗp����O���t�B�b�N�X�f�[�^��K�؂�VRAM�ɓǂݍ��݂܂��B
                �Ώ� BG �� BGControl ���K�؂ɐݒ肳��Ă���K�v������܂��B
                BG0-1�̊g���p���b�g�̓Ǎ���͊g���p���b�g�ւ� VRAM ��
                ���蓖�ď󋵂��玩���I�ɔ��f���܂��B
                Text BG �� 256x1 �p���b�g�͊g���p���b�g���L�����ǂ�����
                ���[�h���؂�ւ��܂��B

  Arguments:    bg:         ���[�h����f�[�^���g�p���� BG
                pScnData:   VRAM �Ƀ��[�h����X�N���[���f�[�^�ւ̃|�C���^�B
                            NULL �ł��\���܂��񂪁ApPltData �� NULL �łȂ�
                            �ꍇ�� NULL �ł����Ă͂Ȃ�܂���B
                pChrData:   VRAM �Ƀ��[�h����L�����N�^�f�[�^�ւ̃|�C���^�B
                            NULL �̏ꍇ��VRAM�ɓǂݍ��݂܂���B
                pPltData:   VRAM �Ƀ��[�h����p���b�g�f�[�^�ւ̃|�C���^�B
                            NULL �̏ꍇ��VRAM�ɓǂݍ��݂܂���B
                pPosInfo:   �L�����N�^���o�̈���ւ̃|�C���^�B
                pCmpInfo:   �p���b�g���k���ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNS_G2dBGLoadElementsEx(
    NNSG2dBGSelect bg,
    const NNSG2dScreenData* pScnData,
    const NNSG2dCharacterData* pChrData,
    const NNSG2dPaletteData* pPltData,
    const NNSG2dCharacterPosInfo* pPosInfo,
    const NNSG2dPaletteCompressInfo* pCmpInfo
);

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dBGLoadScreenRect

  Description:  �X�N���[���f�[�^���̎w�肳�ꂽ��`���o�b�t�@�̎w�肳�ꂽ�ʒu��
                �R�s�[���܂��B

  Arguments:    pScreenDst: �]�����_�ւ̃|�C���^�B
                pScnData:   �]�����ƂȂ�X�N���[���f�[�^�ւ̃|�C���^�B
                srcX:       �]�����������X���W�B�i�L�����N�^�P�ʁj
                srcY:       �]�����������Y���W�B�i�L�����N�^�P�ʁj
                dstX:       �]���捶�����X���W�B�i�L�����N�^�P�ʁj
                dstY:       �]���捶�����Y���W�B�i�L�����N�^�P�ʁj
                dstW:       �]����̈�̕��B�i�L�����N�^�P�ʁj
                dstH:       �]����̈�̍����B�i�L�����N�^�P�ʁj
                width:      �]������̈�̕��B�i�L�����N�^�P�ʁj
                height:     �]������̈�̍����B�i�L�����N�^�P�ʁj

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNS_G2dBGLoadScreenRect(
    void* pScreenDst,
    const NNSG2dScreenData* pScnData,
    int srcX,
    int srcY,
    int dstX,
    int dstY,
    int dstW,
    int dstH,
    int width,
    int height
);


//-------------------------------------------------------------------------
// ���ϐ�(����J)
//-------------------------------------------------------------------------

extern GXBGAreaOver NNSi_G2dBGAreaOver;




//-------------------------------------------------------------------------
// inline �֐�
//-------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dBGGetScreenColorMode

  Description:  �X�N���[���f�[�^�̃J���[���[�h���擾���܂��B

  Arguments:    pScnData:   �X�N���[���f�[�^�ւ̃|�C���^

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE GXBGColorMode NNSi_G2dBGGetScreenColorMode(const NNSG2dScreenData* pScnData)
{
    NNS_G2D_POINTER_ASSERT( pScnData );
    return (pScnData->colorMode == NNS_G2D_SCREENCOLORMODE_16x16) ?
                GX_BG_COLORMODE_16: GX_BG_COLORMODE_256;
}



/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dBGGetScreenFormat

  Description:  �X�N���[���f�[�^�̃X�N���[���t�H�[�}�b�g���擾���܂��B

  Arguments:    pScnData:   �X�N���[���f�[�^�ւ̃|�C���^

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE NNSG2dScreenFormat NNSi_G2dBGGetScreenFormat(const NNSG2dScreenData* pScnData)
{
    NNS_G2D_POINTER_ASSERT( pScnData );
    return (NNSG2dScreenFormat)pScnData->screenFormat;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetBGAreaOver

  Description:  NNS_G2dLoadBGScreenSet() �� Affine (�g��) BG ��
                �G���A�I�[�o�[�����w��Ƃ��ėp��������w�肵�܂��B

  Arguments:    areaOver:   �ȍ~�� NNS_G2dLoadBGScreenSet() �ŗp������
                            �G���A�I�[�o�[�������w�肵�܂��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dSetBGAreaOver( GXBGAreaOver areaOver )
{
    SDK_MINMAX_ASSERT( areaOver, GX_BG_AREAOVER_XLU, GX_BG_AREAOVER_REPEAT );
    NNSi_G2dBGAreaOver = areaOver;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetBGAreaOver

  Description:  NNS_G2dLoadBGScreenSet() �ŗp������G���A�I�[�o�[������
                �擾���܂��B

  Arguments:    �Ȃ��B

  Returns:      NNS_G2dLoadBGScreenSet() �ŗp������G���A�I�[�o�[�����B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE GXBGAreaOver NNS_G2dGetBGAreaOver( void )
{
    return NNSi_G2dBGAreaOver;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dBGSetup

  Description:  BG�̐ݒ���s���ABG�f�[�^��VRAM�ɓǂݍ��݂܂��B

  Arguments:    bg:         �ΏۂƂ��� BG
                pScnData:   BG �Ŏg�p����X�N���[���f�[�^�ւ̃|�C���^�B
                            NULL �ł����Ă͂Ȃ�܂���B
                pChrData:   BG �Ŏg�p����L�����N�^�f�[�^�ւ̃|�C���^�B
                            NULL �̏ꍇ��VRAM�ɓǂݍ��݂܂���B
                pPltData:   BG �Ŏg�p����p���b�g�f�[�^�ւ̃|�C���^�B
                            NULL �̏ꍇ��VRAM�ɓǂݍ��݂܂���B
                scnBase:    BG �̃X�N���[���x�[�X�B
                chrBase:    BG �̃L�����N�^�x�[�X�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dBGSetup(
    NNSG2dBGSelect bg,
    const NNSG2dScreenData* pScnData,
    const NNSG2dCharacterData* pChrData,
    const NNSG2dPaletteData* pPltData,
    GXBGScrBase scnBase,
    GXBGCharBase chrBase
)
{
    NNS_G2dBGSetupEx(bg, pScnData, pChrData, pPltData, NULL, NULL, scnBase, chrBase);
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dBGLoadElements

  Description:  BG �ŗp����O���t�B�b�N�X�f�[�^��K�؂�VRAM�ɓǂݍ��݂܂��B
                �Ώ� BG �� BGControl ���K�؂ɐݒ肳��Ă���K�v������܂��B
                BG0-1�̊g���p���b�g�̓Ǎ���͊g���p���b�g�ւ� VRAM ��
                ���蓖�ď󋵂��玩���I�ɔ��f���܂��B
                Text BG �� 256x1 �p���b�g�͊g���p���b�g���L�����ǂ�����
                ���[�h���؂�ւ��܂��B

  Arguments:    bg:         ���[�h����f�[�^���g�p���� BG
                pScnData:   VRAM �Ƀ��[�h����X�N���[���f�[�^�ւ̃|�C���^�B
                            NULL �ł��\���܂��񂪁ApPltData �� NULL �łȂ�
                            �ꍇ�� NULL �ł����Ă͂Ȃ�܂���B
                pChrData:   VRAM �Ƀ��[�h����L�����N�^�f�[�^�ւ̃|�C���^�B
                            NULL �̏ꍇ��VRAM�ɓǂݍ��݂܂���B
                pPltData:   VRAM �Ƀ��[�h����p���b�g�f�[�^�ւ̃|�C���^�B
                            NULL �̏ꍇ��VRAM�ɓǂݍ��݂܂���B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dBGLoadElements(
    NNSG2dBGSelect bg,
    const NNSG2dScreenData* pScnData,
    const NNSG2dCharacterData* pChrData,
    const NNSG2dPaletteData* pPltData
)
{
    NNS_G2dBGLoadElementsEx(bg, pScnData, pChrData, pPltData, NULL, NULL);
}


//-------------------------------------------------------------------------





#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // NNS_G2D_SCREEN_H_

