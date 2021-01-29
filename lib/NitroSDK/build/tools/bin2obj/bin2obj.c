/*---------------------------------------------------------------------------*
  Project:  NitroSDK - bin2obj
  File:     bin2obj.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: bin2obj.c,v $
  Revision 1.7  2006/07/21 07:57:19  yasu
  PowerPC �Ή�

  Revision 1.6  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.5  2005/06/14 00:57:07  yasu
  -s �I�v�V�����̒ǉ�

  Revision 1.4  2005/06/13 08:52:26  yasu
  %f ���V���{�����ɑΉ�

  Revision 1.3  2005/06/13 02:56:34  yasu
  �I�v�V�����̏C���A�e�X�g�쐬�ɂ�铮��̊m�F

  Revision 1.2  2005/06/10 12:55:42  yasu
  1st ���t�@�N�^�����O

  Revision 1.1  2005/06/10 00:43:01  yasu
  ������

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include "bin2obj.h"

/*---------------------------------------------------------------------------*
  Name:         main

  Description:  bin2obj ���C��
 *---------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    Bin2ObjArgs args;
    BOOL    result;

    cook_args(&args, argc, argv);
    result = bin2obj(&args);
    free_args(&args);

    return result ? 0 : 1;
}

/*---------------------------------------------------------------------------*
  Name:         bin2obj

  Description:  �I�u�W�F�N�g�����o�͂���

  Arguments:    t     ������� bin2obj ����
  
  Returns:      TRUE ����  FALSE ���s
 *---------------------------------------------------------------------------*/
BOOL bin2obj(const Bin2ObjArgs * t)
{
    Object  object;

    //
    // �I�u�W�F�N�g�f�[�^�쐬
    //
    object_init(&object, t->machine, t->endian);

    if (!add_datasec(&object,
                     t->section_rodata, t->section_rwdata,
                     t->symbol_begin, t->symbol_end, t->binary_filename, t->writable, t->align))
    {
        return FALSE;
    }

    map_section(&object);

    //
    // �I�u�W�F�N�g�o��
    //
    if (!output_object(&object, t->object_filename))
    {
        return FALSE;
    }

    return TRUE;
}
