/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MI - 
  File:     mi_compress.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mi_compress.c,v $
  Revision 1.17  2007/11/02 04:29:07  takano_makoto
  LZEx���k�̍�����������ǉ�

  Revision 1.16  2007/11/02 02:19:21  takano_makoto
  LZEx���k�̑Ή��R����C��

  Revision 1.15  2007/11/02 00:49:16  takano_makoto
  LZ77�g�����k�ɑΉ�
  �n�t�}�����k�Œl��16bit�ȏ�ɕ��������ꂽ�ꍇ�̕s����C��

  Revision 1.14  2007/02/20 00:28:10  kitase_hirotake
  indent source

  Revision 1.13  2006/07/06 13:33:02  takano_makoto
  LZ���k�Ɏ��s�����ꍇ�Ƀo�b�t�@�̎��̂P�o�C�g�ɃI�[�o�[�A�N�Z�X����ꍇ���������s����C��

  Revision 1.12  2006/05/02 08:03:05  takano_makoto
  1.9�ł̕s��C����MI_CompressLZ()�ɂ��K�p����Ă��Ȃ������̂��C��

  Revision 1.11  2006/04/24 00:04:47  okubata_ryoma
  CW2.0�̌x�����x�������ɔ������ύX

  Revision 1.10  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.9  2006/01/05 04:04:46  takano_makoto
  �I�[�f�[�^��3�o�C�g�ň��k�����ꍇ�̕s����C��

  Revision 1.8  2005/11/29 05:06:56  takano_makoto
  MI_CompressLZFast��ǉ�

  Revision 1.7  2005/06/29 04:24:51  takano_makoto
  Huffman���k�Œl���P��ނ����Ȃ��ꍇ�̕s����C��

  Revision 1.6  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.5  2005/02/18 07:09:08  yasu
  Signed/Unsigned �ϊ��x���}��

  Revision 1.4  2005/02/01 02:16:07  takano_makoto
  �n�t�}�����k�̍ۂɈ����Ń��[�N�o�b�t�@��n���悤�ɕύX

  Revision 1.3  2005/01/31 11:41:00  takano_makoto
  �擪3�o�C�g���̔�����ŏ��ɂ����Ȃ����ƂŁA��������������������B

  Revision 1.2  2005/01/31 09:32:32  takano_makoto  Revision 1.2  2005/01/31 09:32:32  takano_makoto
  LZ77���k������������

  Revision 1.1  2005/01/28 13:11:28  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro/types.h>
#include <nitro/mi/uncompress.h>
#include <nitro/mi/compress.h>

static u32 SearchLZ(const u8 *startp, const u8 *nextp, u32 remainSize, u16 *offset, u32 maxLength );

//===========================================================================
//  LZ������
//===========================================================================

/*---------------------------------------------------------------------------*
  Name:         MI_CompressLZ

  Description:  LZ77���k���s�Ȃ��֐�

  Arguments:    srcp            ���k���f�[�^�ւ̃|�C���^
                size            ���k���f�[�^�T�C�Y
                dstp            ���k��f�[�^�ւ̃|�C���^
                                ���k���f�[�^�����傫���T�C�Y�̃o�b�t�@���K�v�ł��B

  Returns:      ���k��̃f�[�^�T�C�Y�B
                ���k��̃f�[�^�����k�O�����傫���Ȃ�ꍇ�ɂ͈��k�𒆒f��0��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
u32 MI_CompressLZImpl(const u8 *srcp, u32 size, u8 *dstp, BOOL exFormat)
{
    u32     LZDstCount;                // ���k�f�[�^�̃o�C�g��
    u32     LZSrcCount;                // ���k�Ώۃf�[�^�̏����σf�[�^��(�o�C�g�P��)
    const u8 *srcStartp;
    u8      LZCompFlags;               // ���k�̗L���������t���O�n��
    u8     *LZCompFlagsp;              // LZCompFlags ���i�[���郁�����̈���|�C���g
    u16     lastOffset;                // ��v�f�[�^�܂ł̃I�t�Z�b�g (���̎��_�ł̍Œ���v�f�[�^) 
    u32     lastLength;                // ��v�f�[�^�� (���̎��_�ł̍Œ���v�f�[�^)
    u8      i;
    u32     dstMax;
    const u32 MAX_LENGTH = (exFormat)? (0xFFFF + 0xFF + 0xF + 3U) : (0xF + 3U);
    
    SDK_ALIGN2_ASSERT(srcp);

    *(u32 *)dstp = size << 8 | MI_COMPRESSION_LZ | (exFormat? 1 : 0); // �f�[�^�E�w�b�_
    dstp += 4;
    LZDstCount = 4;
    LZSrcCount = 0;
    srcStartp = srcp;
    dstMax = size;

    while (size > 0)
    {
        LZCompFlags = 0;
        LZCompFlagsp = dstp++;         // �t���O�n��̊i�[��
        LZDstCount++;

        // �t���O�n��8�r�b�g�f�[�^�Ƃ��Ċi�[����邽�߁A8�񃋁[�v
        for (i = 0; i < 8; i++)
        {
            LZCompFlags <<= 1;         // ���� (i=0) �͓��ɈӖ��͂Ȃ�
            if (size <= 0)
            {
                // �I�[�ɗ����ꍇ�̓t���O���Ō�܂ŃV�t�g�����Ă���I��
                continue;
            }

            if ((lastLength = SearchLZ(srcStartp, srcp, size, &lastOffset, MAX_LENGTH)) != 0)
            {
                u32 length;
                // ���k�\�ȏꍇ�̓t���O�𗧂Ă�
                LZCompFlags |= 0x1;

                if (LZDstCount + 2 >= dstMax)   // �\�[�X�����傫�ȃT�C�Y�ɂȂ�ꍇ�̓G���[�I��
                {
                    return 0;
                }
                
                if ( exFormat )
                {
                    if ( lastLength >= 0xFF + 0xF + 3 )
                    {
                        length  = (u32)( lastLength - 0xFF - 0xF - 3 );
                        *dstp++ = (u8)( 0x10 | (length >> 12) );
                        *dstp++ = (u8)( length >> 4 );
                        LZDstCount += 2;
                    }
                    else if ( lastLength >= 0xF + 2 )
                    {
                        length  = (u32)( lastLength - 0xF - 2 );
                        *dstp++ = (u8)( length >> 4 );
                        LZDstCount += 1;
                    }
                    else
                    {
                        length = (u32)( lastLength - 1 );
                    }
                }
                else
                {
                    length = (u32)( lastLength - 3 );
                }
                
                // �I�t�Z�b�g�͏��4�r�b�g�Ɖ���8�r�b�g�ɕ����Ċi�[
                *dstp++ = (u8)( length << 4 | (lastOffset - 1) >> 8 );
                *dstp++ = (u8)((lastOffset - 1) & 0xff);
                LZDstCount += 2;
                srcp += lastLength;
                size -= lastLength;
            }
            else
            {
                // ���k�Ȃ�
                if (LZDstCount + 1 >= dstMax)   // �\�[�X�����傫�ȃT�C�Y�ɂȂ�ꍇ�̓G���[�I��
                {
                    return 0;
                }
                *dstp++ = *srcp++;
                size--;
                LZDstCount++;
            }
        }                              // 8�񃋁[�v�I��
        *LZCompFlagsp = LZCompFlags;   // �t���O�n����i�[
    }

    // 4�o�C�g���E�A���C�������g
    //   �A���C�������g�p�f�[�^0 �̓f�[�^�T�C�Y�Ɋ܂߂Ȃ�
    i = 0;
    while ((LZDstCount + i) & 0x3)
    {
        *dstp++ = 0;
        i++;
    }

    return LZDstCount;
}

//--------------------------------------------------------
// LZ77���k�ŃX���C�h���̒�����Œ���v����������܂��B
//  Arguments:    startp                 �f�[�^�̊J�n�ʒu�������|�C���^
//                nextp                  �������J�n����f�[�^�̃|�C���^
//                remainSize             �c��f�[�^�T�C�Y
//                offset                 ��v�����I�t�Z�b�g���i�[����̈�ւ̃|�C���^
//  Return   :    ��v�񂪌��������ꍇ��   TRUE
//                ������Ȃ������ꍇ��     FALSE
//--------------------------------------------------------
static u32 SearchLZ(const u8 *startp, const u8 *nextp, u32 remainSize, u16 *offset, u32 maxLength )
{
    const u8 *searchp;
    const u8 *headp, *searchHeadp;
    u16     maxOffset;
    u32     currLength = 2;
    u32     tmpLength;

    if (remainSize < 3)
    {
        return 0;
    }

    // VRAM��2�o�C�g�A�N�Z�X�Ȃ̂� (VRAM����f�[�^��ǂݏo���ꍇ�����邽��)�A
    // �����Ώۃf�[�^��2�o�C�g�O����̃f�[�^�ɂ��Ȃ���΂Ȃ�Ȃ��B
    // 
    // �I�t�Z�b�g��12�r�b�g�Ŋi�[����邽�߁A4096�ȉ�
    searchp = nextp - 4096;
    if (searchp < startp)
    {
        searchp = startp;
    }

    while (nextp - searchp >= 2)
    {
        headp = nextp;
        searchHeadp = searchp;
        tmpLength = 0;

        while ((*headp != *searchp) || (*(headp + 1) != *(searchp + 1))
               || (*(headp + 2) != *(searchp + 2)))
        {
            searchp++;
            if (nextp - searchp < 2)
            {
                goto search_end;
            }
        }
        searchHeadp = searchp + 3;
        headp += 3;
        tmpLength = 3;

        // �f�[�^�̏I�[�܂��́A�قȂ�f�[�^�ɑ�������܂ň��k�T�C�Y���C���N�������g����B
        while (((u32)(headp - nextp) < remainSize) && (*headp == *searchHeadp))
        {
            headp++;
            searchHeadp++;
            tmpLength++;

            // �f�[�^����4�r�b�g�Ŋi�[����邽�߁A18�ȉ� (3�̉��ʂ��͂�����)
            if (tmpLength == maxLength)
            {
                break;
            }
        }

        if (tmpLength > currLength)
        {
            // �ő咷�I�t�Z�b�g���X�V
            currLength = tmpLength;
            maxOffset = (u16)(nextp - searchp);
            if (currLength == maxLength || currLength == remainSize)
            {
                // ��v�����ő�Ȃ̂ŁA�������I������B
                break;
            }
        }

        searchp++;
    }

  search_end:
    if (currLength < 3)
    {
        return 0;
    }
    *offset = maxOffset;
    return currLength;
}


//===========================================================================
//  LZ�������i������)
//===========================================================================

// LZ�������k�p�ꎞ���
typedef struct
{
    u16     windowPos;                 // ���𑋂̐擪�ʒu
    u16     windowLen;                 // ���𑋂̒���

    s16    *LZOffsetTable;             // ���𑋂̃I�t�Z�b�g�o�b�t�@
    s16    *LZByteTable;               // �L�����N�^�̍ŐV�����ւ̃|�C���^
    s16    *LZEndTable;                // �L�����N�^�̍Ō×����ւ̃|�C���^
}
LZCompressInfo;

static void LZInitTable(LZCompressInfo * info, u8 *work);
static void SlideByte(LZCompressInfo * info, const u8 *srcp);
static inline void LZSlide(LZCompressInfo * info, const u8 *srcp, u32 n);
static u32 SearchLZFast(LZCompressInfo * info, const u8 *nextp, u32 remainSize, u16 *offset, u32 maxLength );


//--------------------------------------------------------
// �����C���f�b�N�X�̏�����
//--------------------------------------------------------
static void LZInitTable(LZCompressInfo * info, u8 *work)
{
    u16     i;

    info->LZOffsetTable = (s16 *)work;
    info->LZByteTable = (s16 *)work + 4096;
    info->LZEndTable = (s16 *)work + 4096 + 256;

    for (i = 0; i < 256; i++)
    {
        info->LZByteTable[i] = -1;
        info->LZEndTable[i] = -1;
    }
    info->windowPos = 0;
    info->windowLen = 0;
}

//--------------------------------------------------------
// ������1�o�C�g�X���C�h
//--------------------------------------------------------
static void SlideByte(LZCompressInfo * info, const u8 *srcp)
{
    s16     offset;
    u8      in_data = *srcp;
    u16     insert_offset;

    s16    *const LZByteTable = info->LZByteTable;
    s16    *const LZOffsetTable = info->LZOffsetTable;
    s16    *const LZEndTable = info->LZEndTable;
    const u16 windowPos = info->windowPos;
    const u16 windowLen = info->windowLen;

    if (windowLen == 4096)
    {
        u8      out_data = *(srcp - 4096);
        if ((LZByteTable[out_data] = LZOffsetTable[LZByteTable[out_data]]) == -1)
        {
            LZEndTable[out_data] = -1;
        }
        insert_offset = windowPos;
    }
    else
    {
        insert_offset = windowLen;
    }

    offset = LZEndTable[in_data];
    if (offset == -1)
    {
        LZByteTable[in_data] = (s16)insert_offset;
    }
    else
    {
        LZOffsetTable[offset] = (s16)insert_offset;
    }
    LZEndTable[in_data] = (s16)insert_offset;
    LZOffsetTable[insert_offset] = -1;

    if (windowLen == 4096)
    {
        info->windowPos = (u16)((windowPos + 1) % 0x1000);
    }
    else
    {
        info->windowLen++;
    }
}

//--------------------------------------------------------
// ������n�o�C�g�X���C�h
//--------------------------------------------------------
static inline void LZSlide(LZCompressInfo * info, const u8 *srcp, u32 n)
{
    u32     i;

    for (i = 0; i < n; i++)
    {
        SlideByte(info, srcp++);
    }
}


/*---------------------------------------------------------------------------*
  Name:         MI_CompressLZFastImpl

  Description:  LZ77���k���s�Ȃ��֐�

  Arguments:    srcp            ���k���f�[�^�ւ̃|�C���^
                size            ���k���f�[�^�T�C�Y
                dstp            ���k��f�[�^�ւ̃|�C���^
                                ���k���f�[�^�����傫���T�C�Y�̃o�b�t�@���K�v�ł��B
                work            ���k�p�ꎞ�o�b�t�@
                                MI_LZ_FAST_COMPRESS_WORK_SIZE ���̗̈悪�K�v�ł��B

  Returns:      ���k��̃f�[�^�T�C�Y�B
                ���k��̃f�[�^�����k�O�����傫���Ȃ�ꍇ�ɂ͈��k�𒆒f��0��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
u32 MI_CompressLZFastImpl(const u8 *srcp, u32 size, u8 *dstp, u8 *work, BOOL exFormat)
{
    u32     LZDstCount;                // ���k�f�[�^�̃o�C�g��
    u8      LZCompFlags;               // ���k�̗L���������t���O�n��
    u8     *LZCompFlagsp;              // LZCompFlags ���i�[���郁�����̈���|�C���g
    u16     lastOffset;                // ��v�f�[�^�܂ł̃I�t�Z�b�g (���̎��_�ł̍Œ���v�f�[�^) 
    u32     lastLength;                // ��v�f�[�^�� (���̎��_�ł̍Œ���v�f�[�^)
    u8      i;
    u32     dstMax;
    LZCompressInfo info;               // LZ���k�ꎞ���
    const u32 MAX_LENGTH = (exFormat)? (0xFFFF + 0xFF + 0xF + 3U) : (0xF + 3U);

    SDK_ALIGN2_ASSERT(srcp);
    SDK_NULL_ASSERT(work);

    *(u32 *)dstp = size << 8 | MI_COMPRESSION_LZ | (exFormat? 1 : 0); // �f�[�^�E�w�b�_
    dstp += 4;
    LZDstCount = 4;
    dstMax = size;
    LZInitTable(&info, work);

    while (size > 0)
    {
        LZCompFlags = 0;
        LZCompFlagsp = dstp++;         // �t���O�n��̊i�[��
        LZDstCount++;

        // �t���O�n��8�r�b�g�f�[�^�Ƃ��Ċi�[����邽�߁A8�񃋁[�v
        for (i = 0; i < 8; i++)
        {
            LZCompFlags <<= 1;         // ���� (i=0) �͓��ɈӖ��͂Ȃ�
            if (size <= 0)
            {
                // �I�[�ɗ����ꍇ�̓t���O���Ō�܂ŃV�t�g�����Ă���I��
                continue;
            }

            if ( (lastLength = SearchLZFast(&info, srcp, size, &lastOffset, MAX_LENGTH)) != 0 )
            {
                u32 length;
                // ���k�\�ȏꍇ�̓t���O�𗧂Ă�
                LZCompFlags |= 0x1;

                if (LZDstCount + 2 >= dstMax)   // �\�[�X�����傫�ȃT�C�Y�ɂȂ�ꍇ�̓G���[�I��
                {
                    return 0;
                }
                
                if ( exFormat )
                {
                    if ( lastLength >= 0xFF + 0xF + 3 )
                    {
                        length  = (u32)( lastLength - 0xFF - 0xF - 3 );
                        *dstp++ = (u8)( 0x10 | (length >> 12) );
                        *dstp++ = (u8)( length >> 4 );
                        LZDstCount += 2;
                    }
                    else if ( lastLength >= 0xF + 2 )
                    {
                        length  = (u32)( lastLength - 0xF - 2 );
                        *dstp++ = (u8)( length >> 4 );
                        LZDstCount += 1;
                    }
                    else
                    {
                        length = (u32)( lastLength - 1 );
                    }
                }
                else
                {
                    length = (u32)( lastLength - 3 );
                }
                
                // �I�t�Z�b�g�͏��4�r�b�g�Ɖ���8�r�b�g�ɕ����Ċi�[
                *dstp++ = (u8)(length << 4 | (lastOffset - 1) >> 8);
                *dstp++ = (u8)((lastOffset - 1) & 0xff);
                LZDstCount += 2;
                LZSlide(&info, srcp, lastLength);
                srcp += lastLength;
                size -= lastLength;
            }
            else
            {
                // ���k�Ȃ�
                if (LZDstCount + 1 >= dstMax)   // �\�[�X�����傫�ȃT�C�Y�ɂȂ�ꍇ�̓G���[�I��
                {
                    return 0;
                }
                LZSlide(&info, srcp, 1);
                *dstp++ = *srcp++;
                size--;
                LZDstCount++;
            }
        }                              // 8�񃋁[�v�I��
        *LZCompFlagsp = LZCompFlags;   // �t���O�n����i�[
    }

    // 4�o�C�g���E�A���C�������g
    //   �A���C�������g�p�f�[�^0 �̓f�[�^�T�C�Y�Ɋ܂߂Ȃ�
    i = 0;
    while ((LZDstCount + i) & 0x3)
    {
        *dstp++ = 0;
        i++;
    }

    return LZDstCount;
}

//--------------------------------------------------------
// LZ77���k�ŃX���C�h���̒�����Œ���v����������܂��B
//  Arguments:    startp                 �f�[�^�̊J�n�ʒu�������|�C���^
//                nextp                  �������J�n����f�[�^�̃|�C���^
//                remainSize             �c��f�[�^�T�C�Y
//                offset                 ��v�����I�t�Z�b�g���i�[����̈�ւ̃|�C���^
//  Return   :    ��v�񂪌��������ꍇ��   TRUE
//                ������Ȃ������ꍇ��     FALSE
//--------------------------------------------------------
static u32 SearchLZFast(LZCompressInfo * info, const u8 *nextp, u32 remainSize, u16 *offset, u32 maxLength )
{
    const u8 *searchp;
    const u8 *headp, *searchHeadp;
    u16     maxOffset;
    u32     currLength = 2;
    u32     tmpLength;
    s32     w_offset;
    s16    *const LZOffsetTable = info->LZOffsetTable;
    const u16 windowPos = info->windowPos;
    const u16 windowLen = info->windowLen;

    if (remainSize < 3)
    {
        return 0;
    }

    w_offset = info->LZByteTable[*nextp];

    while (w_offset != -1)
    {
        if (w_offset < windowPos)
        {
            searchp = nextp - windowPos + w_offset;
        }
        else
        {
            searchp = nextp - windowLen - windowPos + w_offset;
        }

        /* �����Ă��ǂ����A�͂��ɍ������������߂� */
        if (*(searchp + 1) != *(nextp + 1) || *(searchp + 2) != *(nextp + 2))
        {
            w_offset = LZOffsetTable[w_offset];
            continue;
        }

        if (nextp - searchp < 2)
        {
            // VRAM��2�o�C�g�A�N�Z�X�Ȃ̂� (VRAM����f�[�^��ǂݏo���ꍇ�����邽��)�A
            // �����Ώۃf�[�^��2�o�C�g�O����̃f�[�^�ɂ��Ȃ���΂Ȃ�Ȃ��B
            // 
            // �I�t�Z�b�g��12�r�b�g�Ŋi�[����邽�߁A4096�ȉ�
            break;
        }
        tmpLength = 3;
        searchHeadp = searchp + 3;
        headp = nextp + 3;

        // �f�[�^�̏I�[�܂��́A�قȂ�f�[�^�ɑ�������܂ň��k�T�C�Y���C���N�������g����B
        while (((u32)(headp - nextp) < remainSize) && (*headp == *searchHeadp))
        {
            headp++;
            searchHeadp++;
            tmpLength++;

            // �f�[�^����4�r�b�g�Ŋi�[����邽�߁A18�ȉ� (3�̉��ʂ��͂�����)
            if (tmpLength == maxLength)
            {
                break;
            }
        }

        if (tmpLength > currLength)
        {
            // �ő咷�I�t�Z�b�g���X�V
            currLength = tmpLength;
            maxOffset = (u16)(nextp - searchp);
            if (currLength == maxLength || currLength == remainSize)
            {
                // ��v�����ő�Ȃ̂ŁA�������I������B
                break;
            }
        }
        w_offset = LZOffsetTable[w_offset];
    }

    if (currLength < 3)
    {
        return 0;
    }
    *offset = maxOffset;
    return currLength;
}


//===========================================================================
//  ���������O�X������
//===========================================================================

/*---------------------------------------------------------------------------*
  Name:         MI_CompressRL

  Description:  ���������O�X���k���s�Ȃ��֐�

  Arguments:    srcp            ���k���f�[�^�ւ̃|�C���^
                size            ���k���f�[�^�T�C�Y
                dstp            ���k��f�[�^�ւ̃|�C���^
                                ���k���f�[�^�����傫���T�C�Y�̃o�b�t�@���K�v�ł��B

  Returns:      ���k��̃f�[�^�T�C�Y�B
                ���k��̃f�[�^�����k�O�����傫���Ȃ�ꍇ�ɂ͈��k�𒆒f��0��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
u32 MI_CompressRL(const u8 *srcp, u32 size, u8 *dstp)
{
    u32     RLDstCount;                // ���k�f�[�^�̃o�C�g��
    u32     RLSrcCount;                // ���k�Ώۃf�[�^�̏����σf�[�^��(�o�C�g�P��)
    u8      RLCompFlag;                // ���������O�X���������s���ꍇ�P
    u8      runLength;                 // ���������O�X
    u8      rawDataLength;             // �����ɂȂ��Ă��Ȃ��f�[�^�̃����O�X
    u32     i;

    const u8 *startp;                  // ���̏������[�v�ɂ�����A���k�Ώۃf�[�^�̐擪���|�C���g

    //  �f�[�^�w�b�_        (�T�C�Y�͓W�J��)
    *(u32 *)dstp = size << 8 | MI_COMPRESSION_RL;       // �f�[�^�E�w�b�_
    RLDstCount = 4;

    RLSrcCount = 0;
    rawDataLength = 0;
    RLCompFlag = 0;

    while (RLSrcCount < size)
    {
        startp = &srcp[RLSrcCount];    // ���k�Ώۃf�[�^�̐ݒ�

        for (i = 0; i < 128; i++)      // 7�r�b�g�ŕ\���ł���f�[�^�ʂ� 0~127
        {
            // ���k�Ώۃf�[�^�̖����ɓ��B
            if (RLSrcCount + rawDataLength >= size)
            {
                rawDataLength = (u8)(size - RLSrcCount);
                break;
            }

            if (RLSrcCount + rawDataLength + 2 < size)
            {
                if (startp[i] == startp[i + 1] && startp[i] == startp[i + 2])
                {
                    RLCompFlag = 1;
                    break;
                }
            }
            rawDataLength++;
        }

        // ���������Ȃ��f�[�^���i�[
        // �f�[�^���i�[�o�C�g��8�r�b�g�ڂ�0�Ȃ�A���������Ȃ��f�[�^�n��
        // �f�[�^���� -1 �������ɂȂ�̂ŁA0-127 �� 1-128 �ƂȂ�
        if (rawDataLength)
        {
            if (RLDstCount + rawDataLength + 1 >= size) // �\�[�X�����傫�ȃT�C�Y�ɂȂ�ꍇ�̓G���[�I��
            {
                return 0;
            }
            dstp[RLDstCount++] = (u8)(rawDataLength - 1);       // "�f�[�^��-1" �i�[(7�r�b�g)
            for (i = 0; i < rawDataLength; i++)
            {
                dstp[RLDstCount++] = srcp[RLSrcCount++];
            }
            rawDataLength = 0;
        }

        // ���������O�X������
        if (RLCompFlag)
        {
            runLength = 3;
            for (i = 3; i < 128 + 2; i++)
            {
                // ���k�p�f�[�^�̖����ɓ��B
                if (RLSrcCount + runLength >= size)
                {
                    runLength = (u8)(size - RLSrcCount);
                    break;
                }

                // �������r�؂ꂽ�ꍇ
                if (srcp[RLSrcCount] != srcp[RLSrcCount + runLength])
                {
                    break;
                }
                // �����p����
                runLength++;
            }

            // �f�[�^���i�[�o�C�g��8�r�b�g�ڂ�1�Ȃ�A�����������f�[�^�n��
            if (RLDstCount + 2 >= size) // �\�[�X�����傫�ȃT�C�Y�ɂȂ�ꍇ�̓G���[�I��
            {
                return 0;
            }
            dstp[RLDstCount++] = (u8)(0x80 | (runLength - 3));  // �R�̉��ʂ��͂����āA3~130���i�[
            dstp[RLDstCount++] = srcp[RLSrcCount];
            RLSrcCount += runLength;
            RLCompFlag = 0;
        }
    }

    // 4�o�C�g���E�A���C�������g
    //   �A���C�������g�p�f�[�^0 �̓f�[�^�T�C�Y�Ɋ܂߂Ȃ�
    i = 0;
    while ((RLDstCount + i) & 0x3)
    {
        dstp[RLDstCount + i] = 0;
        i++;
    }
    return RLDstCount;
}


//===========================================================================
//  �n�t�}��������
//===========================================================================
#define HUFF_END_L  0x80
#define HUFF_END_R  0x40

static void HuffUpdateParentDepth(u16 leftNo, u16 rightNo);
static void HuffMakeCode(u16 nodeNo, u32 paHuffCode);
static u8 HuffCountHWord(u16 nodeNo);
static void HuffMakeHuffTree(u16 rootNo);
static void HuffMakeSubsetHuffTree(u16 huffTreeNo, u8 rightNodeFlag);
static u8 HuffRemainingNodeCanSetOffset(u8 costHWord);
static void HuffSetOneNodeOffset(u16 huffTreeNo, u8 rightNodeFlag);

typedef struct
{
    u32     Freq;                      // �o���p�x
    u16     No;                        // �f�[�^No
    s16     PaNo;                      // �eNo 
    s16     ChNo[2];                   // �qNo (0: �����C 1: �E��)
    u16     PaDepth;                   // �e�m�[�h�̐[��
    u16     LeafDepth;                 // �t�܂ł̐[��
    u32     HuffCode;                  // �n�t�}������
    u8      Bit;                       // �m�[�h�̃r�b�g�f�[�^
    u8      _padding;
    u16     HWord;                     // �e���Ԑߓ_�ɂ����āA���̐ߓ_�����[�g�Ƃ��镔���؂� HuffTree �i�[�ɕK�v�ȃ�������
}
HuffData;                              // �v 24 Byte

static HuffData *HuffTable;            // [512] 12288B
static const HuffData HuffTableInitData = { 0, 0, 0, {-1, -1}, 0, 0, 0, 0, 0 };

static u8 HuffTreeTop;                 // HuffTreeTop �̔ԍ�
static u8 *HuffTree;                   // [256][2] 512B

typedef struct
{
    u8      leftOffsetNeed;            // ���̎q�ߓ_�ւ̃I�t�Z�b�g���K�v�Ȃ�1
    u8      rightOffsetNeed;           // �E�̎q�ߓ_�ւ̃I�t�Z�b�g���K�v�Ȃ�1
    u16     leftNodeNo;                // ���̎q�ߓ_No
    u16     rightNodeNo;               // �E�̎q�ߓ_No
}
HuffTreeCtrlData;                      // �v 6 Byte
static HuffTreeCtrlData *HuffTreeCtrl; // 1536 Byte  [ 256 ]
static const HuffTreeCtrlData HuffTreeCtrlInitData = { 1, 1, 0, 0 };

static u16 sHuffDataNum;               // �����������f�[�^�̎�� 4�r�b�g�������Ȃ�16 8�r�b�g�Ȃ�256

// �n�t�}�����[�N�o�b�t�@�\��
typedef struct
{
    HuffData HuffTable[512];           //    12288B
    u8      HuffTree[256 * 2];         //      512B
    HuffTreeCtrlData HuffTreeCtrl[256]; //     1536B
}
HuffWork;                              // �v 14336B

/*---------------------------------------------------------------------------*
  Name:         MI_CompressHuffman

  Description:  �n�t�}�����k���s�Ȃ��֐�

  Arguments:    srcp            ���k���f�[�^�ւ̃|�C���^
                size            ���k���f�[�^�T�C�Y
                dstp            ���k��f�[�^�ւ̃|�C���^
                                ���k���f�[�^�����傫���T�C�Y�̃o�b�t�@���K�v�ł��B
                huffBitSize     �������r�b�g��
                work            �n�t�}�����k�p�̃��[�N�o�b�t�@

  Returns:      ���k��̃f�[�^�T�C�Y�B
                ���k��̃f�[�^�����k�O�����傫���Ȃ�ꍇ�ɂ͈��k�𒆒f��0��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
u32 MI_CompressHuffman(const u8 *srcp, u32 size, u8 *dstp, u8 huffBitSize, u8 *work)
{
    u32     HuffDstCount;              // ���k�f�[�^�̃o�C�g��
    u8      tmp;
    u16     nodeNum;                   // �L���m�[�h��
    u16     tableTop;                  // �e�[�u���쐬���́A�e�[�u���g�b�vNo
    s32     leftNo, rightNo;           // 2���؍쐬���̃m�[�hNo
    s32     i, ii, iii;
    u8      srcTmp;
    u32     bitStream = 0;
    u32     streamLength = 0;
    u16     rootNo;                    // �񕪖؂̃��[�gNo


    SDK_NULL_ASSERT(srcp);
    SDK_NULL_ASSERT(dstp);
    SDK_ASSERT(huffBitSize == 4 || huffBitSize == 8);
    SDK_NULL_ASSERT(work);
    SDK_ALIGN4_ASSERT(work);

    HuffTable = ((HuffWork *) work)->HuffTable;
    HuffTree = ((HuffWork *) work)->HuffTree;
    HuffTreeCtrl = ((HuffWork *) work)->HuffTreeCtrl;

    sHuffDataNum = (u16)(1 << huffBitSize);     // 8->256, 4->16
    tableTop = sHuffDataNum;

    // �e�[�u��������
    //  �Y���� �F   0  ~ 15(255)    : �������Ώۃf�[�^�̏�� (8�r�b�g������)
    //              16 ~ 31(511)    : �񕪖؍쐬�p�̏�� 
    for (i = 0; i < (u16)(sHuffDataNum * 2); i++)
    {
        HuffTable[i] = HuffTableInitData;
        HuffTable[i].No = (u16)i;
    }

    // �o���p�x�`�F�b�N
    if (huffBitSize == 8)
    {
        for (i = 0; i < size; i++)
        {
            HuffTable[srcp[i]].Freq++; // 8�r�b�g������
        }
    }
    else
    {
        for (i = 0; i < size; i++)
        {                              // 4�r�b�g������        
            tmp = (u8)((srcp[i] & 0xf0) >> 4);  // ���4�r�b�g�����Ɋi�[
            HuffTable[tmp].Freq++;     // ���͕������̂Ƃ�
            tmp = (u8)(srcp[i] & 0x0f);
            HuffTable[tmp].Freq++;
        }
    }

    // �c���[�e�[�u���쐬
    leftNo = rightNo = -1;
    while (1)
    {
        // Freq�̏����������ؒ��_��2�T��  1�͕K��������͂�
        // �q���_(��)�̒T��
        for (i = 0; i < tableTop; i++)
        {
            if ((HuffTable[i].Freq != 0) && (HuffTable[i].PaNo == 0))
            {
                leftNo = i;
                break;
            }
        }
        for (i = (u16)leftNo; i < tableTop; i++)
        {
            if ((HuffTable[i].Freq != 0) &&
                (HuffTable[i].PaNo == 0) && (HuffTable[i].Freq < HuffTable[leftNo].Freq))
            {
                leftNo = i;
            }
        }
        // �q���_(�E)�̒T��
        for (i = 0; i < tableTop; i++)
        {
            if ((HuffTable[i].Freq != 0) && (HuffTable[i].PaNo == 0) && (i != leftNo))
            {
                rightNo = i;
                break;
            }
        }
        for (i = (u16)rightNo; i < tableTop; i++)
        {
            if ((HuffTable[i].Freq != 0) &&
                (HuffTable[i].PaNo == 0) &&
                (HuffTable[i].Freq < HuffTable[rightNo].Freq) && (i != leftNo))
            {
                rightNo = i;
            }
        }
        // 1�����Ȃ�������A�e�[�u���쐬�I��
        if (rightNo < 0)
        {
            if (tableTop == sHuffDataNum)
                // �l�����ނ����Ȃ����݂��Ȃ��ꍇ�ɂ�01�ǂ���������l�ƂȂ�m�[�h���P�쐬����
            {
                HuffTable[tableTop].Freq = HuffTable[leftNo].Freq;
                HuffTable[tableTop].ChNo[0] = (s16)leftNo;
                HuffTable[tableTop].ChNo[1] = (s16)leftNo;
                HuffTable[tableTop].LeafDepth = 1;
                HuffTable[leftNo].PaNo = (s16)tableTop;
                HuffTable[leftNo].Bit = 0;
                HuffTable[leftNo].PaDepth = 1;
            }
            else
            {
                tableTop--;
            }
            rootNo = tableTop;
            nodeNum = (u16)((rootNo - sHuffDataNum + 1) * 2 + 1);
            break;
        }

        // �������؂ƉE�����؂𓝍����钸�_�쐬
        HuffTable[tableTop].Freq = HuffTable[leftNo].Freq + HuffTable[rightNo].Freq;
        HuffTable[tableTop].ChNo[0] = (s16)leftNo;
        HuffTable[tableTop].ChNo[1] = (s16)rightNo;
        if (HuffTable[leftNo].LeafDepth > HuffTable[rightNo].LeafDepth)
        {
            HuffTable[tableTop].LeafDepth = (u16)(HuffTable[leftNo].LeafDepth + 1);
        }
        else
        {
            HuffTable[tableTop].LeafDepth = (u16)(HuffTable[rightNo].LeafDepth + 1);
        }

        HuffTable[leftNo].PaNo = HuffTable[rightNo].PaNo = (s16)tableTop;
        HuffTable[leftNo].Bit = 0;
        HuffTable[rightNo].Bit = 1;

        HuffUpdateParentDepth((u16)leftNo, (u16)rightNo);

        tableTop++;
        leftNo = rightNo = -1;
    }

    // �n�t�}���R�[�h���� (HuffTable[i].HuffCode ��)
    HuffMakeCode(rootNo, 0x00);        // PaDepth�̃r�b�g�������AHuffCode �̉��ʃr�b�g���}�X�N�������̂��n�t�}���R�[�h

    // �e���Ԑߓ_�ɂ����āA���̐ߓ_�����[�g�Ƃ��镔���؂� HuffTree �i�[�ɕK�v�ȃ������ʂ̌v�Z
    (void)HuffCountHWord(rootNo);

    // HuffTree �쐬
    HuffMakeHuffTree(rootNo);
    HuffTree[0] = --HuffTreeTop;

    // �f�[�^�E�w�b�_
    *(u32 *)dstp = size << 8 | MI_COMPRESSION_HUFFMAN | huffBitSize;
    HuffDstCount = 4;

    if (HuffDstCount + (HuffTreeTop + 1) * 2 >= size)   // �\�[�X�����傫�ȃT�C�Y�ɂȂ�ꍇ�̓G���[�I��
    {
        return 0;
    }

    for (i = 0; i < (u16)((HuffTreeTop + 1) * 2); i++)  // �c���[�e�[�u��
    {
        dstp[HuffDstCount++] = ((u8 *)HuffTree)[i];
    }

    // 4�o�C�g���E�A���C�������g
    //   �A���C�������g�p�f�[�^0 �̓f�[�^�T�C�Y�Ɋ܂߂� (�f�R�[�_�̃A���S���Y���ɂ��)
    while (HuffDstCount & 0x3)
    {
        if (HuffDstCount & 0x1)
        {
            HuffTreeTop++;
            dstp[4]++;
        }
        dstp[HuffDstCount++] = 0;
    }

    // �n�t�}��������
    for (i = 0; i < size; i++)         // �f�[�^���k
    {
        u8 val = srcp[i];
        if (huffBitSize == 8)          // 8�r�b�g�n�t�}��
        {
            bitStream = (bitStream << HuffTable[ val ].PaDepth) | HuffTable[ val ].HuffCode;
            streamLength += HuffTable[ val ].PaDepth;

            if (HuffDstCount + (streamLength / 8) >= size)
            {
                // �\�[�X�����傫�ȃT�C�Y�ɂȂ�ꍇ�̓G���[�I��
                return 0;
            }
            for (ii = 0; ii < streamLength / 8; ii++)
            {
                dstp[HuffDstCount++] = (u8)(bitStream >> (streamLength - (ii + 1) * 8));
            }
            streamLength %= 8;
        }
        else                           // 4�r�b�g�n�t�}��
        {
            for (ii = 0; ii < 2; ii++)
            {
                if (ii)
                {
                    srcTmp = (u8)( val >> 4 );        // ���4�r�b�g
                }
                else
                {
                    srcTmp = (u8)( val & 0x0F );      // ����4�r�b�g
                }
                bitStream = (bitStream << HuffTable[srcTmp].PaDepth) | HuffTable[srcTmp].HuffCode;
                streamLength += HuffTable[srcTmp].PaDepth;
                if (HuffDstCount + (streamLength / 8) >= size)
                {
                    // �\�[�X�����傫�ȃT�C�Y�ɂȂ�ꍇ�̓G���[�I��
                    return 0;
                }
                for (iii = 0; iii < streamLength / 8; iii++)
                {
                    dstp[HuffDstCount++] = (u8)(bitStream >> (streamLength - (iii + 1) * 8));
                }
                streamLength %= 8;
            }
        }
    }

    if (streamLength != 0)
    {
        if (HuffDstCount + 1 >= size)
        {
            // �\�[�X�����傫�ȃT�C�Y�ɂȂ�ꍇ�̓G���[�I��
            return 0;
        }
        dstp[HuffDstCount++] = (u8)(bitStream << (8 - streamLength));
    }

    // 4�o�C�g���E�A���C�������g
    //   �A���C�������g�p�f�[�^0 �̓f�[�^�T�C�Y�Ɋ܂߁u��v 
    //   �n�t�}����������������!�@���g���G���f�B�A���ϊ����邽�߁A�A���C�������g���E�f�[�^����Ƀf�[�^���i�[�����
    while (HuffDstCount & 0x3)
    {
        dstp[HuffDstCount++] = 0;
    }

    for (i = 1 + (HuffTreeTop + 1) * 2 / 4; i < (HuffDstCount / 4) + 1; i++)    // ���g���G���f�B�A���ϊ�
    {
        tmp = dstp[i * 4 + 0];
        dstp[i * 4 + 0] = dstp[i * 4 + 3];
        dstp[i * 4 + 3] = tmp;         // �X���b�v
        tmp = dstp[i * 4 + 1];
        dstp[i * 4 + 1] = dstp[i * 4 + 2];
        dstp[i * 4 + 2] = tmp;         // �X���b�v
    }

    return HuffDstCount;
}



//-----------------------------------------------------------------------
// �n�t�}���R�[�h�\�쐬
//-----------------------------------------------------------------------
static void HuffMakeHuffTree(u16 rootNo)
{
    s16     i;
    u16     tmp;
    s16     costHWord, tmpCostHWord;   // �����؂̃R�[�h�\���쐬���Ȃ��������̃R�X�g �ő�l�̐ߓ_�̕����؃R�[�h�\�����
    s16     costOffsetNeed, tmpCostOffsetNeed;
    s16     costMaxKey, costMaxRightFlag;       // �R�X�g�ŏ��̐ߓ_�� HuffTree ������肷�邽�߂̏��
    u8      offsetNeedNum;
    u8      tmpKey, tmpRightFlag;

    // HuffTreeCtrl ������
    for (i = 0; i < 256; i++)
    {
        HuffTree[i * 2] = HuffTree[i * 2 + 1] = 0;
    }
    for (i = 0; i < 256; i++)
    {
        HuffTreeCtrl[i] = HuffTreeCtrlInitData;
    }
    HuffTreeTop = 1;
    costOffsetNeed = 0;

    HuffTreeCtrl[0].leftOffsetNeed = 0; // �g�p���Ȃ� (�e�[�u���T�C�Y�Ƃ��Ďg�p)
    HuffTreeCtrl[0].rightNodeNo = rootNo;

    while (1)                          // return ����܂� 
    {
        // �I�t�Z�b�g��ݒ肷��K�v�̂���m�[�h���̌v�Z
        offsetNeedNum = 0;
        for (i = 0; i < HuffTreeTop; i++)
        {
            if (HuffTreeCtrl[i].leftOffsetNeed)
            {
                offsetNeedNum++;
            }
            if (HuffTreeCtrl[i].rightOffsetNeed)
            {
                offsetNeedNum++;
            }
        }

        // �ő�R�X�g�̐ߓ_������
        costHWord = -1;
        costMaxKey = -1;
        tmpKey = 0;
        tmpRightFlag = 0;

        for (i = 0; i < HuffTreeTop; i++)
        {

            tmpCostOffsetNeed = (u8)(HuffTreeTop - i);

            // ���̎q�ߓ_�̃R�X�g�]��
            if (HuffTreeCtrl[i].leftOffsetNeed)
            {
                tmpCostHWord = (s16)HuffTable[HuffTreeCtrl[i].leftNodeNo].HWord;

                if ((tmpCostHWord + offsetNeedNum) > 64)
                {
                    goto leftCostEvaluationEnd;
                }
                if (!HuffRemainingNodeCanSetOffset((u8)tmpCostHWord))
                {
                    goto leftCostEvaluationEnd;
                }
                if (tmpCostHWord > costHWord)
                {
                    costMaxKey = i;
                    costMaxRightFlag = 0;
                }
                else if ((tmpCostHWord == costHWord) && (tmpCostOffsetNeed > costOffsetNeed))
                {
                    costMaxKey = i;
                    costMaxRightFlag = 0;
                }
            }
          leftCostEvaluationEnd:{
            }

            // �E�̎q�ߓ_�̃R�X�g�]��
            if (HuffTreeCtrl[i].rightOffsetNeed)
            {
                tmpCostHWord = (s16)HuffTable[HuffTreeCtrl[i].rightNodeNo].HWord;

                if ((tmpCostHWord + offsetNeedNum) > 64)
                {
                    goto rightCostEvaluationEnd;
                }
                if (!(HuffRemainingNodeCanSetOffset((u8)tmpCostHWord)))
                {
                    goto rightCostEvaluationEnd;
                }
                if (tmpCostHWord > costHWord)
                {
                    costMaxKey = i;
                    costMaxRightFlag = 1;
                }
                else if ((tmpCostHWord == costHWord) && (tmpCostOffsetNeed > costOffsetNeed))
                {
                    costMaxKey = i;
                    costMaxRightFlag = 1;
                }
            }
          rightCostEvaluationEnd:{
            }
        }

        // �����؂��܂�܂� HuffTree �Ɋi�[
        if (costMaxKey >= 0)
        {
            HuffMakeSubsetHuffTree((u8)costMaxKey, (u8)costMaxRightFlag);
            goto nextTreeMaking;
        }
        else
        {
            // �K�v�I�t�Z�b�g�ő�̃m�[�h������
            for (i = 0; i < HuffTreeTop; i++)
            {
                tmp = 0;
                tmpRightFlag = 0;
                if (HuffTreeCtrl[i].leftOffsetNeed)
                {
                    tmp = HuffTable[HuffTreeCtrl[i].leftNodeNo].HWord;
                }
                if (HuffTreeCtrl[i].rightOffsetNeed)
                {
                    if (HuffTable[HuffTreeCtrl[i].rightNodeNo].HWord > tmp)
                    {
                        tmpRightFlag = 1;
                    }
                }
                if ((tmp != 0) || (tmpRightFlag))
                {
                    HuffSetOneNodeOffset((u8)i, tmpRightFlag);
                    goto nextTreeMaking;
                }
            }
        }
        return;
      nextTreeMaking:{
        }
    }
}

//-----------------------------------------------------------------------
// �����؂��܂�܂� HuffTree �Ɋi�[
//-----------------------------------------------------------------------
static void HuffMakeSubsetHuffTree(u16 huffTreeNo, u8 rightNodeFlag)
{
    u8      i;

    i = HuffTreeTop;
    HuffSetOneNodeOffset(huffTreeNo, rightNodeFlag);

    if (rightNodeFlag)
    {
        HuffTreeCtrl[huffTreeNo].rightOffsetNeed = 0;
    }
    else
    {
        HuffTreeCtrl[huffTreeNo].leftOffsetNeed = 0;
    }

    while (i < HuffTreeTop)
    {
        if (HuffTreeCtrl[i].leftOffsetNeed)
        {
            HuffSetOneNodeOffset(i, 0);
            HuffTreeCtrl[i].leftOffsetNeed = 0;
        }
        if (HuffTreeCtrl[i].rightOffsetNeed)
        {
            HuffSetOneNodeOffset(i, 1);
            HuffTreeCtrl[i].rightOffsetNeed = 0;
        }
        i++;
    }
}

//-----------------------------------------------------------------------
// �^����ꂽ�f�[�^�ʂ̕����؂�W�J���Ă� HuffTree �\�z�Ɏx�Ⴊ�Ȃ������ׂ�
//-----------------------------------------------------------------------
static u8 HuffRemainingNodeCanSetOffset(u8 costHWord)
{
    u8      i;
    s16     capacity;

    capacity = (s16)(64 - costHWord);

    // �I�t�Z�b�g���� i ���������قǑ傫���̂ŁA�\�[�g�����Ai=0 -> HuffTreeTop �Ōv�Z����΂悢
    for (i = 0; i < HuffTreeTop; i++)
    {
        if (HuffTreeCtrl[i].leftOffsetNeed)
        {
            if ((HuffTreeTop - i) <= capacity)
            {
                capacity--;
            }
            else
            {
                return 0;
            }
        }
        if (HuffTreeCtrl[i].rightOffsetNeed)
        {
            if ((HuffTreeTop - i) <= capacity)
            {
                capacity--;
            }
            else
            {
                return 0;
            }
        }
    }

    return 1;
}

//-----------------------------------------------------------------------
// 1�ߓ_���A�n�t�}���R�[�h�\���쐬
//-----------------------------------------------------------------------
static void HuffSetOneNodeOffset(u16 huffTreeNo, u8 rightNodeFlag)
{
    u16     nodeNo;
    u8      offsetData = 0;

    if (rightNodeFlag)
    {
        nodeNo = HuffTreeCtrl[huffTreeNo].rightNodeNo;
        HuffTreeCtrl[huffTreeNo].rightOffsetNeed = 0;
    }
    else
    {
        nodeNo = HuffTreeCtrl[huffTreeNo].leftNodeNo;
        HuffTreeCtrl[huffTreeNo].leftOffsetNeed = 0;
    }

    // ���̎q�ߓ_
    if (HuffTable[HuffTable[nodeNo].ChNo[0]].LeafDepth == 0)
    {
        offsetData |= 0x80;
        HuffTree[HuffTreeTop * 2 + 0] = (u8)HuffTable[nodeNo].ChNo[0];
        HuffTreeCtrl[HuffTreeTop].leftNodeNo = (u8)HuffTable[nodeNo].ChNo[0];
        HuffTreeCtrl[HuffTreeTop].leftOffsetNeed = 0;   // �I�t�Z�b�g�͕K�v�Ȃ��Ȃ�
    }
    else
    {
        HuffTreeCtrl[HuffTreeTop].leftNodeNo = (u16)HuffTable[nodeNo].ChNo[0];  // �I�t�Z�b�g�͕K�v
    }

    // �E�̎q�ߓ_
    if (HuffTable[HuffTable[nodeNo].ChNo[1]].LeafDepth == 0)
    {
        offsetData |= 0x40;
        HuffTree[HuffTreeTop * 2 + 1] = (u8)HuffTable[nodeNo].ChNo[1];
        HuffTreeCtrl[HuffTreeTop].rightNodeNo = (u8)HuffTable[nodeNo].ChNo[1];
        HuffTreeCtrl[HuffTreeTop].rightOffsetNeed = 0;  // �I�t�Z�b�g�͕K�v�Ȃ��Ȃ�
    }
    else
    {
        HuffTreeCtrl[HuffTreeTop].rightNodeNo = (u16)HuffTable[nodeNo].ChNo[1]; // �I�t�Z�b�g�͕K�v
    }

    offsetData |= (u8)(HuffTreeTop - huffTreeNo - 1);
    HuffTree[huffTreeNo * 2 + rightNodeFlag] = offsetData;

    HuffTreeTop++;
}


//-----------------------------------------------------------------------
// 2���؍쐬���ɁA�����؂𓝍������Ƃ��ɁA�����؂̊e�\���m�[�h�̐[�����{1����
//-----------------------------------------------------------------------
static void HuffUpdateParentDepth(u16 leftNo, u16 rightNo)
{
    HuffTable[leftNo].PaDepth++;
    HuffTable[rightNo].PaDepth++;

    if (HuffTable[leftNo].LeafDepth != 0)
    {
        HuffUpdateParentDepth((u16)HuffTable[leftNo].ChNo[0], (u16)HuffTable[leftNo].ChNo[1]);
    }
    if (HuffTable[rightNo].LeafDepth != 0)
    {
        HuffUpdateParentDepth((u16)HuffTable[rightNo].ChNo[0], (u16)HuffTable[rightNo].ChNo[1]);
    }
}

//-----------------------------------------------------------------------
// �n�t�}���R�[�h����
//-----------------------------------------------------------------------
static void HuffMakeCode(u16 nodeNo, u32 paHuffCode)
{
    HuffTable[nodeNo].HuffCode = (paHuffCode << 1) | HuffTable[nodeNo].Bit;

    if (HuffTable[nodeNo].LeafDepth != 0)
    {
        HuffMakeCode((u16)HuffTable[nodeNo].ChNo[0], HuffTable[nodeNo].HuffCode);
        HuffMakeCode((u16)HuffTable[nodeNo].ChNo[1], HuffTable[nodeNo].HuffCode);
    }
}

//-----------------------------------------------------------------------
// ���ԃm�[�h�� HuffTree �쐬�ɕK�v�Ƃ���f�[�^��
//-----------------------------------------------------------------------
static u8 HuffCountHWord(u16 nodeNo)
{
    u8      leftHWord, rightHWord;

    switch (HuffTable[nodeNo].LeafDepth)
    {
    case 0:
        return 0;
    case 1:
        leftHWord = rightHWord = 0;
        break;
    default:
        leftHWord = HuffCountHWord((u16)HuffTable[nodeNo].ChNo[0]);
        rightHWord = HuffCountHWord((u16)HuffTable[nodeNo].ChNo[1]);
        break;
    }

    HuffTable[nodeNo].HWord = (u16)(leftHWord + rightHWord + 1);
    return (u8)(leftHWord + rightHWord + 1);
}
