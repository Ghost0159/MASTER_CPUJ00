/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MATH - 
  File:     math/crc.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: crc.h,v $
  Revision 1.5  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.4  2005/04/13 11:58:43  seiki_masashi
  GetDigest -> GetHash

  Revision 1.3  2005/04/11 10:34:54  seiki_masashi
  ���[�e�B���e�B�֐��̒ǉ�

  Revision 1.2  2005/04/11 08:24:39  seiki_masashi
  small fix

  Revision 1.1  2005/04/11 01:49:56  seiki_masashi
  crc.h �̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_MATH_CRC_H_
#define NITRO_MATH_CRC_H_

#include <nitro/misc.h>
#include <nitro/types.h>

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// Constant definition
//----------------------------------------------------------------------------

#define MATH_CRC8_STANDARD_POLY     0x07
#define MATH_CRC8_STANDARD_INIT     0

#define MATH_CRC16_STANDARD_POLY    0xa001      // �r�b�g���]������̂͐��������������]
#define MATH_CRC16_STANDARD_INIT    0
#define MATH_CRC16_CCITT_POLY       0x1021
#define MATH_CRC16_CCITT_INIT       0xffff

#define MATH_CRC32_STANDARD_POLY    0xedb88320  // �r�b�g���]������̂͐��������������]
#define MATH_CRC32_STANDARD_INIT    0xffffffff
#define MATH_CRC32_POSIX_POLY       0x04c11db7
#define MATH_CRC32_POSIX_INIT       0

//----------------------------------------------------------------------------
// Type definition
//----------------------------------------------------------------------------

typedef u8 MATHCRC8Context;
typedef u16 MATHCRC16Context;
typedef u32 MATHCRC32Context;

typedef struct MATHCRC8Table
{
    u8      table[256];
}
MATHCRC8Table;

typedef struct MATHCRC16Table
{
    u16     table[256];
}
MATHCRC16Table;

typedef struct MATHCRC32Table
{
    u32     table[256];
}
MATHCRC32Table;

//----------------------------------------------------------------------------
// Declaration of function
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Implementation of inline function
//----------------------------------------------------------------------------

/*****************************************************************************/
/* CRC-8 ����                                                               */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC8InitTable

  Description:  CRC-8 �l�����߂邽�߂Ɏg�� MATHCRC8Table �\���̂�����������B
  
  Arguments:    table MATHCRC8Table �\����
                poly  �����������̃r�b�g�\������ŏ�ʃr�b�g����������
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MATHi_CRC8InitTable(MATHCRC8Table * table, u8 poly);

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC8InitTableRev

  Description:  CRC-8 �l�����߂邽�߂Ɏg�� MATHCRC8Table �\���̂�����������B
                ���ʃr�b�g����ʂ̌��Ƃ��Ĉ����B
  
  Arguments:    table MATHCRC8Table �\����
                poly  �����������̃r�b�g�\������ŏ�ʃr�b�g������������
                      �㉺���]����������
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MATHi_CRC8InitTableRev(MATHCRC8Table * table, u8 poly);

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC8Init

  Description:  CRC-8 �l�����߂邽�߂Ɏg�� MATHCRC8Context �\���̂�����������B
  
  Arguments:    context MATHCRC8Context �\����
                init    CRC �̏����l
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATHi_CRC8Init(MATHCRC8Context * context, u8 init)
{
    *context = init;
}

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC8Update

  Description:  CRC-8 �l��^�����f�[�^�ōX�V����B
  
  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC8Table �ւ̃|�C���^
                context MATHCRC8Context �\����
                input   ���̓f�[�^�ւ̃|�C���^
                length  ���̓f�[�^��
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void
 
 MATHi_CRC8Update(const MATHCRC8Table * table, MATHCRC8Context * context, const void *input,
                  u32 length);

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC8UpdateRev

  Description:  CRC-8 �l��^�����f�[�^�ōX�V����B
                ���ʃr�b�g����ʂ̌��Ƃ��Ĉ����B
  
  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC8Table �ւ̃|�C���^
                context MATHCRC8Context �\����
                input   ���̓f�[�^�ւ̃|�C���^
                length  ���̓f�[�^��
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void
MATHi_CRC8UpdateRev(const MATHCRC8Table * table, MATHCRC8Context * context, const void *input,
                    u32 length)
{
    MATHi_CRC8Update(table, context, input, length);
}

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC8GetHash

  Description:  �ŏI�I�� CRC-8 �l�𓾂�B
  
  Arguments:    context MATHCRC8Context �\����
  
  Returns:      �v�Z����.
 *---------------------------------------------------------------------------*/
static inline u8 MATHi_CRC8GetHash(MATHCRC8Context * context)
{
    return (u8)*context;
}

/*****************************************************************************/
/* CRC-8                                                                    */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC8InitTable

  Description:  CRC-8 �l�����߂邽�߂Ɏg�� MATHCRC8Table �\���̂�����������B
  
  Arguments:    table MATHCRC8Table �\����
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATH_CRC8InitTable(MATHCRC8Table * table)
{
    MATHi_CRC8InitTable(table, MATH_CRC8_STANDARD_POLY);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC8Init

  Description:  CRC-8 �l�����߂邽�߂Ɏg�� MATHCRC8Context �\���̂�����������B
  
  Arguments:    context MATHCRC8Context �\����
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATH_CRC8Init(MATHCRC8Context * context)
{
    MATHi_CRC8Init(context, MATH_CRC8_STANDARD_INIT);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC8Update

  Description:  CRC-8 �l��^�����f�[�^�ōX�V����B
  
  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC8Table �ւ̃|�C���^
                context MATHCRC8Context �\����
                input   ���̓f�[�^�ւ̃|�C���^
                length  ���̓f�[�^��
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void
MATH_CRC8Update(const MATHCRC8Table * table, MATHCRC8Context * context, const void *input,
                u32 length)
{
    MATHi_CRC8Update(table, context, input, length);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC8GetHash

  Description:  �ŏI�I�� CRC-8 �l�𓾂�B
  
  Arguments:    context MATHCRC8Context �\����
  
  Returns:      �v�Z����.
 *---------------------------------------------------------------------------*/
static inline u8 MATH_CRC8GetHash(MATHCRC8Context * context)
{
    return MATHi_CRC8GetHash(context);
}

/*****************************************************************************/
/* CRC-16 ����                                                               */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC16InitTable

  Description:  CRC-16 �l�����߂邽�߂Ɏg�� MATHCRC16Table �\���̂�����������B
  
  Arguments:    table MATHCRC16Table �\����
                poly  �����������̃r�b�g�\������ŏ�ʃr�b�g����������
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MATHi_CRC16InitTable(MATHCRC16Table * table, u16 poly);

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC16InitTableRev

  Description:  CRC-16 �l�����߂邽�߂Ɏg�� MATHCRC16Table �\���̂�����������B
                ���ʃr�b�g����ʂ̌��Ƃ��Ĉ����B
  
  Arguments:    table MATHCRC16Table �\����
                poly  �����������̃r�b�g�\������ŏ�ʃr�b�g������������
                      �㉺���]����������
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MATHi_CRC16InitTableRev(MATHCRC16Table * table, u16 poly);

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC16Init

  Description:  CRC-16 �l�����߂邽�߂Ɏg�� MATHCRC16Context �\���̂�����������B
  
  Arguments:    context MATHCRC16Context �\����
                init    CRC �̏����l
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATHi_CRC16Init(MATHCRC16Context * context, u16 init)
{
    *context = init;
}

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC16Update

  Description:  CRC-16 �l��^�����f�[�^�ōX�V����B
  
  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC16Table �ւ̃|�C���^
                context MATHCRC16Context �\����
                input   ���̓f�[�^�ւ̃|�C���^
                length  ���̓f�[�^��
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void
 
 MATHi_CRC16Update(const MATHCRC16Table * table, MATHCRC16Context * context, const void *input,
                   u32 length);

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC16UpdateRev

  Description:  CRC-16 �l��^�����f�[�^�ōX�V����B
                ���ʃr�b�g����ʂ̌��Ƃ��Ĉ����B
  
  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC16Table �ւ̃|�C���^
                context MATHCRC16Context �\����
                input   ���̓f�[�^�ւ̃|�C���^
                length  ���̓f�[�^��
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void
 
 MATHi_CRC16UpdateRev(const MATHCRC16Table * table, MATHCRC16Context * context, const void *input,
                      u32 length);

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC16GetHash

  Description:  �ŏI�I�� CRC-16 �l�𓾂�B
  
  Arguments:    context MATHCRC16Context �\����
  
  Returns:      �v�Z����.
 *---------------------------------------------------------------------------*/
static inline u16 MATHi_CRC16GetHash(MATHCRC16Context * context)
{
    return (u16)*context;
}

/*****************************************************************************/
/* CRC-16/CCITT(X.25)                                                        */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC16CCITTInitTable

  Description:  CRC-16/CCITT �l�����߂邽�߂Ɏg�� MATHCRC16Table �\���̂�����������B
  
  Arguments:    table MATHCRC16Table �\����
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATH_CRC16CCITTInitTable(MATHCRC16Table * table)
{
    MATHi_CRC16InitTable(table, MATH_CRC16_CCITT_POLY);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC16CCITTInit

  Description:  CRC-16/CCITT �l�����߂邽�߂Ɏg�� MATHCRC16Context �\���̂�����������B
  
  Arguments:    context MATHCRC16Context �\����
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATH_CRC16CCITTInit(MATHCRC16Context * context)
{
    MATHi_CRC16Init(context, MATH_CRC16_CCITT_INIT);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC16CCITTUpdate

  Description:  CRC-16/CCITT �l��^�����f�[�^�ōX�V����B
  
  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC16Table �ւ̃|�C���^
                context MATHCRC16Context �\����
                input   ���̓f�[�^�ւ̃|�C���^
                length  ���̓f�[�^��
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void
MATH_CRC16CCITTUpdate(const MATHCRC16Table * table, MATHCRC16Context * context, const void *input,
                      u32 length)
{
    MATHi_CRC16Update(table, context, input, length);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC16CCITTGetHash

  Description:  �ŏI�I�� CRC-16/CCITT �l�𓾂�B
  
  Arguments:    context MATHCRC16Context �\����
  
  Returns:      �v�Z����.
 *---------------------------------------------------------------------------*/
static inline u16 MATH_CRC16CCITTGetHash(MATHCRC16Context * context)
{
    return MATHi_CRC16GetHash(context);
}

/*****************************************************************************/
/* CRC-16                                                                    */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC16ARCInitTable

  Description:  CRC-16 �l�����߂邽�߂Ɏg�� MATHCRC16Table �\���̂�����������B
  
  Arguments:    table MATHCRC16Table �\����
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATH_CRC16InitTable(MATHCRC16Table * table)
{
    MATHi_CRC16InitTableRev(table, MATH_CRC16_STANDARD_POLY);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC16ARCInit

  Description:  CRC-16 �l�����߂邽�߂Ɏg�� MATHCRC16Context �\���̂�����������B
  
  Arguments:    context MATHCRC16Context �\����
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATH_CRC16Init(MATHCRC16Context * context)
{
    MATHi_CRC16Init(context, MATH_CRC16_STANDARD_INIT);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC16ARCUpdate

  Description:  CRC-16 �l��^�����f�[�^�ōX�V����B
  
  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC16Table �ւ̃|�C���^
                context MATHCRC16Context �\����
                input   ���̓f�[�^�ւ̃|�C���^
                length  ���̓f�[�^��
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void
MATH_CRC16Update(const MATHCRC16Table * table, MATHCRC16Context * context, const void *input,
                 u32 length)
{
    MATHi_CRC16UpdateRev(table, context, input, length);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC16ARCGetHash

  Description:  �ŏI�I�� CRC-16 �l�𓾂�B
  
  Arguments:    context MATHCRC16Context �\����
  
  Returns:      �v�Z����.
 *---------------------------------------------------------------------------*/
static inline u16 MATH_CRC16GetHash(MATHCRC16Context * context)
{
    return MATHi_CRC16GetHash(context);
}

/*****************************************************************************/
/* CRC-32 ����                                                               */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC32InitTable

  Description:  CRC-32 �l�����߂邽�߂Ɏg�� MATHCRC32Table �\���̂�����������B
  
  Arguments:    table MATHCRC32Table �\����
                poly  �����������̃r�b�g�\������ŏ�ʃr�b�g����������
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MATHi_CRC32InitTable(MATHCRC32Table * table, u32 poly);

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC16InitTableRev

  Description:  CRC-32 �l�����߂邽�߂Ɏg�� MATHCRC16Table �\���̂�����������B
                ���ʃr�b�g����ʂ̌��Ƃ��Ĉ����B
  
  Arguments:    table MATHCRC16Table �\����
                poly  �����������̃r�b�g�\������ŏ�ʃr�b�g������������
                      �㉺���]����������
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MATHi_CRC32InitTableRev(MATHCRC32Table * table, u32 poly);

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC32Init

  Description:  CRC-32 �l�����߂邽�߂Ɏg�� MATHCRC32Context �\���̂�����������B
  
  Arguments:    context MATHCRC32Context �\����
                init    CRC �̏����l
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATHi_CRC32Init(MATHCRC32Context * context, u32 init)
{
    *context = init;
}

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC32Update

  Description:  CRC-32 �l��^�����f�[�^�ōX�V����B
  
  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC32Table �ւ̃|�C���^
                context MATHCRC32Context �\����
                input   ���̓f�[�^�ւ̃|�C���^
                length  ���̓f�[�^��
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void
 
 MATHi_CRC32Update(const MATHCRC32Table * table, MATHCRC32Context * context, const void *input,
                   u32 length);

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC32UpdateRev

  Description:  CRC-32 �l��^�����f�[�^�ōX�V����B
                ���ʃr�b�g����ʂ̌��Ƃ��Ĉ����B
  
  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC32Table �ւ̃|�C���^
                context MATHCRC32Context �\����
                input   ���̓f�[�^�ւ̃|�C���^
                length  ���̓f�[�^��
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void
 
 MATHi_CRC32UpdateRev(const MATHCRC32Table * table, MATHCRC32Context * context, const void *input,
                      u32 length);

/*---------------------------------------------------------------------------*
  Name:         MATHi_CRC32GetHash

  Description:  �ŏI�I�� CRC-32 �l�𓾂�B
  
  Arguments:    context MATHCRC32Context �\����
  
  Returns:      �v�Z����.
 *---------------------------------------------------------------------------*/
static inline u32 MATHi_CRC32GetHash(MATHCRC32Context * context)
{
    return (u32)*context;
}

/*****************************************************************************/
/* CRC-32                                                                    */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC32InitTable

  Description:  CRC-32 �l�����߂邽�߂Ɏg�� MATHCRC32Table �\���̂�����������B
  
  Arguments:    table MATHCRC32Table �\����
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATH_CRC32InitTable(MATHCRC32Table * table)
{
    MATHi_CRC32InitTableRev(table, MATH_CRC32_STANDARD_POLY);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC32Init

  Description:  CRC-32 �l�����߂邽�߂Ɏg�� MATHCRC32Context �\���̂�����������B
  
  Arguments:    context MATHCRC32Context �\����
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATH_CRC32Init(MATHCRC32Context * context)
{
    MATHi_CRC32Init(context, MATH_CRC32_STANDARD_INIT);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC32Update

  Description:  CRC-32 �l��^�����f�[�^�ōX�V����B
  
  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC32Table �ւ̃|�C���^
                context MATHCRC32Context �\����
                input   ���̓f�[�^�ւ̃|�C���^
                length  ���̓f�[�^��
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void
MATH_CRC32Update(const MATHCRC32Table * table, MATHCRC32Context * context, const void *input,
                 u32 length)
{
    MATHi_CRC32UpdateRev(table, context, input, length);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC32GetHash

  Description:  �ŏI�I�� CRC-32 �l�𓾂�B
  
  Arguments:    context MATHCRC32Context �\����
  
  Returns:      �v�Z����..
 *---------------------------------------------------------------------------*/
static inline u32 MATH_CRC32GetHash(MATHCRC32Context * context)
{
    return (u32)(~MATHi_CRC32GetHash(context));
}

/*****************************************************************************/
/* CRC-32/POSIX 1003.2                                                       */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC32POSIXInitTable

  Description:  CRC-32/POSIX �l�����߂邽�߂Ɏg�� MATHCRC32Table �\���̂�����������B
  
  Arguments:    table MATHCRC32Table �\����
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATH_CRC32POSIXInitTable(MATHCRC32Table * table)
{
    MATHi_CRC32InitTable(table, MATH_CRC32_POSIX_POLY);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC32POSIXInit

  Description:  CRC-32/POSIX �l�����߂邽�߂Ɏg�� MATHCRC32Context �\���̂�����������B
  
  Arguments:    context MATHCRC32Context �\����
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATH_CRC32POSIXInit(MATHCRC32Context * context)
{
    MATHi_CRC32Init(context, MATH_CRC32_POSIX_INIT);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC32POSIXUpdate

  Description:  CRC-32/POSIX �l��^�����f�[�^�ōX�V����B
  
  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC32Table �ւ̃|�C���^
                context MATHCRC32Context �\����
                input   ���̓f�[�^�ւ̃|�C���^
                length  ���̓f�[�^��
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void
MATH_CRC32POSIXUpdate(const MATHCRC32Table * table, MATHCRC32Context * context, const void *input,
                      u32 length)
{
    MATHi_CRC32Update(table, context, input, length);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CRC32POSIXGetHash

  Description:  �ŏI�I�� CRC-32/POSIX �l�𓾂�B
  
  Arguments:    context MATHCRC32Context �\����
  
  Returns:      �v�Z����.
 *---------------------------------------------------------------------------*/
static inline u32 MATH_CRC32POSIXGetHash(MATHCRC32Context * context)
{
    return (u32)(~MATHi_CRC32GetHash(context));
}


/*****************************************************************************/
/* ���[�e�B���e�B�֐�                                                        */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         MATH_CalcCRC8

  Description:  CRC-8 �l���v�Z����B

  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC8Table �ւ̃|�C���^
                data    ���̓f�[�^�̃|�C���^
                dataLength  ���̓f�[�^��

  Returns:      �v�Z����.
 *---------------------------------------------------------------------------*/
u8      MATH_CalcCRC8(const MATHCRC8Table * table, const void *data, u32 dataLength);

/*---------------------------------------------------------------------------*
  Name:         MATH_CalcCRC16

  Description:  CRC-16 �l���v�Z����B

  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC16Table �ւ̃|�C���^
                data    ���̓f�[�^�̃|�C���^
                dataLength  ���̓f�[�^��

  Returns:      �v�Z����.
 *---------------------------------------------------------------------------*/
u16     MATH_CalcCRC16(const MATHCRC16Table * table, const void *data, u32 dataLength);

/*---------------------------------------------------------------------------*
  Name:         MATH_CalcCRC16CCITT

  Description:  CRC-16/CCITT �l���v�Z����B

  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC16Table �ւ̃|�C���^
                data    ���̓f�[�^�̃|�C���^
  Arguments:    data    ���̓f�[�^�̃|�C���^
                dataLength  ���̓f�[�^��

  Returns:      �v�Z����.
 *---------------------------------------------------------------------------*/
u16     MATH_CalcCRC16CCITT(const MATHCRC16Table * table, const void *data, u32 dataLength);

/*---------------------------------------------------------------------------*
  Name:         MATH_CalcCRC32

  Description:  CRC-32 �l���v�Z����B

  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC32Table �ւ̃|�C���^
                data    ���̓f�[�^�̃|�C���^
                dataLength  ���̓f�[�^��

  Returns:      �v�Z����.
 *---------------------------------------------------------------------------*/
u32     MATH_CalcCRC32(const MATHCRC32Table * table, const void *data, u32 dataLength);

/*---------------------------------------------------------------------------*
  Name:         MATH_CalcCRC32POSIX

  Description:  CRC-32/POSIX �l���v�Z����B

  Arguments:    table   �v�Z�p�̃e�[�u�� MATHCRC32Table �ւ̃|�C���^
                data    ���̓f�[�^�̃|�C���^
                dataLength  ���̓f�[�^��

  Returns:      �v�Z����.
 *---------------------------------------------------------------------------*/
u32     MATH_CalcCRC32POSIX(const MATHCRC32Table * table, const void *data, u32 dataLength);





#ifdef __cplusplus
}/* extern "C" */
#endif

/* NITRO_MATH_CRC_H_ */
#endif
