/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MATH - 
  File:     math/fft.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: fft.h,v $
  Revision 1.4  2007/04/24 12:04:00  yosizaki
  update copyright.

  Revision 1.3  2007/04/20 09:59:46  yosizaki
  fix include dependency.

  Revision 1.2  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.1  2005/05/13 09:23:56  seiki_masashi
  FFT �֐��̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_MATH_FFT_H_
#define NITRO_MATH_FFT_H_

#include <nitro/types.h>
#include <nitro/fx/fx.h>

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// Type definition
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Declaration of function
//----------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  Name:         MATH_MakeFFTSinTable

  Description:  �����t�[���G�ϊ��ŗp���� sin �e�[�u�����쐬����

  Arguments:    sinTable - 2^nShift * 3/4 �� sin �e�[�u�����i�[����ꏊ�ւ̃|�C���^
                nShift - �f�[�^���� log2

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MATH_MakeFFTSinTable(fx16 *sinTable, u32 nShift);

/*---------------------------------------------------------------------------*
  Name:         MATHi_FFT

  Description:  �����t�[���G�ϊ����s�������֐�

  Arguments:    data - �����ԖڂɎ����A��Ԗڂɋ��������߂��ϊ����s���f�[�^�B
                       �ϊ����ʂ͏㏑������ĕԂ����B
                nShift - �f�[�^���� log2
                sinTable - �~�����f�[�^���œ����������� sin �̒l�̃e�[�u��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MATHi_FFT(fx32 *data, u32 nShift, const fx16 *sinTable);

/*---------------------------------------------------------------------------*
  Name:         MATHi_IFFT

  Description:  �����t�[���G�ϊ��̋t�ϊ����s�������֐�

  Arguments:    data - �����ԖڂɎ����A��Ԗڂɋ��������߂��ϊ����s���f�[�^�B
                       �ϊ����ʂ͏㏑������ĕԂ����B
                nShift - �f�[�^���� log2
                sinTable - �~�����f�[�^���œ����������� sin �̒l�̃e�[�u��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MATHi_IFFT(fx32 *data, u32 nShift, const fx16 *sinTable);

/*---------------------------------------------------------------------------*
  Name:         MATH_FFT

  Description:  �����t�[���G�ϊ����s��

  Arguments:    data - �����ԖڂɎ����A��Ԗڂɋ��������߂��ϊ����s���f�[�^�B
                       �ϊ����ʂ͏㏑������ĕԂ����B
                nShift - �f�[�^���� log2
                sinTable - �~�����f�[�^���œ����������� sin �̒l�̃e�[�u��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MATH_FFT(fx32 *data, u32 nShift, const fx16 *sinTable);

/*---------------------------------------------------------------------------*
  Name:         MATH_IFFT

  Description:  �����t�[���G�ϊ��̋t�ϊ����s��

  Arguments:    data - �����ԖڂɎ����A��Ԗڂɋ��������߂��ϊ����s���f�[�^�B
                       �ϊ����ʂ͏㏑������ĕԂ����B
                nShift - �f�[�^���� log2
                sinTable - �~�����f�[�^���œ����������� sin �̒l�̃e�[�u��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MATH_IFFT(fx32 *data, u32 nShift, const fx16 *sinTable);

/*---------------------------------------------------------------------------*
  Name:         MATH_FFTReal

  Description:  �����t�[���G�ϊ����s��

  Arguments:    data - �����݂̂����߂��f�[�^�B
                       �ϊ����ʂ͏㏑������ĕԂ����B
                nShift - �f�[�^���� log2
                sinTable - �~�����f�[�^���œ����������� sin �̒l�̃e�[�u��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MATH_FFTReal(fx32 *data, u32 nShift, const fx16 *sinTable, const fx16 *sinTable2);

/*---------------------------------------------------------------------------*
  Name:         MATH_IFFTReal

  Description:  �����t�[���G�ϊ��̋t�ϊ����s��

  Arguments:    data - �����݂̂����߂��f�[�^�B
                       �ϊ����ʂ͏㏑������ĕԂ����B
                nShift - �f�[�^���� log2
                sinTable - �~�����f�[�^���œ����������� sin �̒l�̃e�[�u��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MATH_IFFTReal(fx32 *data, u32 nShift, const fx16 *sinTable, const fx16 *sinTable2);


#ifdef __cplusplus
}/* extern "C" */
#endif

/* NITRO_MATH_FFT_H_ */
#endif
