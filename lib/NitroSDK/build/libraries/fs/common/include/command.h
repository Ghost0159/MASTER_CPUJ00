/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - libraries
  File:     command.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/


#if	!defined(NITRO_FS_COMMAND_H_)
#define NITRO_FS_COMMAND_H_

#include <nitro/misc.h>
#include <nitro/types.h>
#include <nitro/fs/file.h>


#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************/
/* constant */

extern FSResult (*const (fsi_default_command[])) (FSFile *);


/*****************************************************************************/
/* variable */

extern FSDirPos current_dir_pos;


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         FSi_StrNICmp

  Description:  �����֐�.
                �召��������ʂ����Ɏw��o�C�g�������������r.

  Arguments:    str1             ��r��������.
                str2             ��r�敶����.
                len              ��r�o�C�g��.

  Returns:      (str1 - str2) �̔�r����.
 *---------------------------------------------------------------------------*/
int     FSi_StrNICmp(const char *str1, const char *str2, u32 len);

/*---------------------------------------------------------------------------*
  Name:         FSi_SendCommand

  Description:  �����֐�.
                �A�[�J�C�u�փR�}���h�𔭍s����.
                �N���^�C�~���O�̒����ƂƂ���, �����n�Ȃ炱���Ńu���b�L���O.

  Arguments:    p_file           �R�}���h�������w�肳�ꂽ FSFile �\����.
                command          �R�}���h ID.

  Returns:      �R�}���h����������� TRUE.
 *---------------------------------------------------------------------------*/
BOOL    FSi_SendCommand(FSFile *p_file, FSCommandType command);

/*---------------------------------------------------------------------------*
  Name:         FSi_TranslateCommand

  Description:  �����֐�.
                ���[�U�v���V�[�W���܂��̓f�t�H���g�������Ăяo��, ���ʂ�Ԃ�.
                �����n�R�}���h���񓯊��I������Ԃ����ꍇ�͓����Ŋ�����҂�.
                �񓯊��n�R�}���h���񓯊��I������Ԃ����ꍇ�͂��̂܂ܕԂ�.

  Arguments:    p_file           ���s����R�}���h���i�[���� FSFile �\����.
                command          �R�}���h ID.

  Returns:      �R�}���h�̏�������.
 *---------------------------------------------------------------------------*/
FSResult FSi_TranslateCommand(FSFile *p_file, FSCommandType command);

/*---------------------------------------------------------------------------*
  Name:         FSi_ReleaseCommand

  Description:  �����֐�.
                �R�}���h���������ҋ@�X���b�h������Ε��A����.

  Arguments:    p_file           ���������R�}���h���i�[���� FSFile �\����.
                ret              �R�}���h�̌��ʒl.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FSi_ReleaseCommand(FSFile *p_file, FSResult ret);

/*---------------------------------------------------------------------------*
  Name:         FSi_NextCommand

  Description:  �����֐�.
                ���ɏ������ׂ��R�}���h��I������.
                �񓯊��R�}���h���I�����ꂽ�炻�̃|�C���^��Ԃ�.
                NULL �ȊO���Ԃ��ꂽ�ꍇ�͂��̏�ŏ������K�v.

  Arguments:    p_arc            ���̃R�}���h���擾����A�[�J�C�u.

  Returns:      ���̏�ŏ�����K�v�Ƃ��鎟�̃R�}���h.
 *---------------------------------------------------------------------------*/
FSFile *FSi_NextCommand(FSArchive *p_arc);

/*---------------------------------------------------------------------------*
  Name:         FSi_ExecuteAsyncCommand

  Description:  �����֐�.
                �񓯊��n�R�}���h�̎��s.
                �ŏ��� 1 ��̓��[�U�X���b�h���犄�荞�݋��ŌĂ΂��.
                �A�[�J�C�u�������I�ɓ��삷����肱���ŃR�}���h�������J��Ԃ�,
                1 ��ł��񓯊������ɂȂ�Α����� NotifyAsyncEnd() �ōs��.

                �����, �A�[�J�C�u���������� / �񓯊��Ő؂�ւ��ꍇ��
                NotifyAsyncEnd() �̌Ăяo�����ɒ��ӂ���K�v������.

  Arguments:    p_file           ���s����񓯊��R�}���h���i�[���� FSFile �\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FSi_ExecuteAsyncCommand(FSFile *p_file);

/*---------------------------------------------------------------------------*
  Name:         FSi_ExecuteSyncCommand

  Description:  �����֐�.
                �������[�h�ɂ�����R�}���h���s.

  Arguments:    p_file           ���s���铯���R�}���h���i�[���� FSFile �\����.

  Returns:      �R�}���h����������� TRUE.
 *---------------------------------------------------------------------------*/
BOOL    FSi_ExecuteSyncCommand(FSFile *p_file);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* NITRO_FS_COMMAND_H_ */

/*---------------------------------------------------------------------------*
  $Log: command.h,v $
  Revision 1.6  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.5  2005/05/30 04:13:26  yosizaki
  add comments.

  Revision 1.4  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.3  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.2  2004/07/23 08:30:26  yosizaki
  fix command operation.

  Revision 1.1  2004/07/08 13:40:41  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
