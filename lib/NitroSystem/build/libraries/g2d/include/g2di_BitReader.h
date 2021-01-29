/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2di_BitReader.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.4 $
 *---------------------------------------------------------------------------*/
#ifndef G2DI_BITREADER_H_
#define G2DI_BITREADER_H_

#include <nitro.h>
#include <nnsys/g2d/g2d_config.h>
#ifdef __cplusplus
extern "C" {
#endif


typedef struct NNSiG2dBitReader
{
    const u8* src;          // �ǂݎ��ʒu�ւ̃|�C���^
    s8 availableBits;       // bits���̖��ǂݎ��r�b�g��
    u8 bits;                // �o�C�g�L���b�V��
    u8 padding_[2];         //
}
NNSiG2dBitReader;

//----------------------------------

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dBitReaderInit

  Description:  BitReader �����������܂��B

  Arguments:    reader: ���������� BitReader �ւ̃|�C���^�B
                src:    �r�b�g��ǂݎ��o�C�g��ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNSi_G2dBitReaderInit(NNSiG2dBitReader* reader, const void* src)
{
    reader->availableBits   = 0;
    reader->src             = (const u8*)src;
    reader->bits            = 0;
}



/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dBitReaderRead

  Description:  �r�b�g���ǂݎ��܂��B

  Arguments:    reader: BitReader �ւ̃|�C���^�B
                nBits:  �ǂݎ��r�b�g���B8�ȉ��łȂ���΂Ȃ�܂���B

  Returns:      �ǂݎ�����r�b�g��B
 *---------------------------------------------------------------------------*/
u32 NNSi_G2dBitReaderRead(NNSiG2dBitReader* reader, int nBits);



/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dBitReaderAlignByte

  Description:  �o�C�g�o�b�t�@���̗]��̃r�b�g���̂Ă܂��B
                ����ɂ�莟�ɓǂݍ��܂��r�b�g�͓ǂݍ��݌���
                �e�o�C�g�擪�ɂȂ�܂��B

  Arguments:    reader: BitReader �ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNSi_G2dBitReaderAlignByte(NNSiG2dBitReader* reader)
{
    // availableBits �� 8 �łȂ���� 0 �ɂ���
    // availableBits �� 8 �ȉ��ł��邱�Ƃ�����
    reader->availableBits &= 8;
}



#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // G2DI_BITREADER_H_

