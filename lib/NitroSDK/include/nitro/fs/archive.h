/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - libraries
  File:     archive.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/


#if	!defined(NITRO_FS_ARCHIVE_H_)
#define NITRO_FS_ARCHIVE_H_

#include <nitro/misc.h>
#include <nitro/types.h>
#include <nitro/os/common/thread.h>


#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************/
/* constant */

enum
{
    /* �A�[�J�C�u���̍ő咷 */
    FS_ARCHIVE_NAME_LEN_MAX = 3
};

/*
 * �A�[�J�C�u������ԃt���O (�C�����C���֐��p)
 * ���[�U�͂����̒萔�𒼐ڎg�p���邱�Ƃ͂���܂���.
 */
#define	FS_ARCHIVE_FLAG_REGISTER	0x00000001
#define	FS_ARCHIVE_FLAG_LOADED		0x00000002
#define	FS_ARCHIVE_FLAG_TABLE_LOAD	0x00000004
#define	FS_ARCHIVE_FLAG_SUSPEND		0x00000008
#define	FS_ARCHIVE_FLAG_RUNNING		0x00000010
#define	FS_ARCHIVE_FLAG_CANCELING	0x00000020
#define	FS_ARCHIVE_FLAG_SUSPENDING	0x00000040
#define	FS_ARCHIVE_FLAG_UNLOADING	0x00000080
#define	FS_ARCHIVE_FLAG_IS_ASYNC	0x00000100
#define	FS_ARCHIVE_FLAG_IS_SYNC		0x00000200

/*
 * FSArchive::flag ��, �A�[�J�C�u�����̂��߂�
 * �ȉ��̃r�b�g�̈�����R�ɒ�`���Ďg�����Ƃ��ł��܂�.
 */
#define	FS_ARCHIVE_FLAG_USER_RESERVED_BIT	0x00010000
#define	FS_ARCHIVE_FLAG_USER_RESERVED_MASK	0xFFFF0000

/*
 * �t�@�C���R�}���h����уA�[�J�C�u���b�Z�[�W.
 * �A�[�J�C�u�������Ƀ��[�U�v���V�[�W���Ŏg�p���܂�.
 */
typedef enum
{
    /* �񓯊��n�R�}���h. */
    FS_COMMAND_ASYNC_BEGIN = 0,
    FS_COMMAND_READFILE = FS_COMMAND_ASYNC_BEGIN,
    FS_COMMAND_WRITEFILE,
    FS_COMMAND_ASYNC_END,

    /* �����n�R�}���h. */
    FS_COMMAND_SYNC_BEGIN = FS_COMMAND_ASYNC_END,
    FS_COMMAND_SEEKDIR = FS_COMMAND_SYNC_BEGIN,
    FS_COMMAND_READDIR,
    FS_COMMAND_FINDPATH,
    FS_COMMAND_GETPATH,
    FS_COMMAND_OPENFILEFAST,
    FS_COMMAND_OPENFILEDIRECT,
    FS_COMMAND_CLOSEFILE,
    FS_COMMAND_SYNC_END,

    /* �A�[�J�C�u���b�Z�[�W. */
    FS_COMMAND_STATUS_BEGIN = FS_COMMAND_SYNC_END,
    FS_COMMAND_ACTIVATE = FS_COMMAND_STATUS_BEGIN,
    FS_COMMAND_IDLE,
    FS_COMMAND_SUSPEND,
    FS_COMMAND_RESUME,
    FS_COMMAND_STATUS_END,

    FS_COMMAND_INVALID
}
FSCommandType;

/*
 * ���[�U�v���V�[�W���ݒ�t���O.
 * FS_SetArchiveProc() �ň��� flags �Ɏw�肵�܂�.
 * �R�[���o�b�N��v��������̂����r�b�g��L���ɂ��܂�.
 */

/* �񓯊��n�R�}���h */
#define	FS_ARCHIVE_PROC_READFILE		(1 << FS_COMMAND_READFILE)
#define	FS_ARCHIVE_PROC_WRITEFILE		(1 << FS_COMMAND_WRITEFILE)
        /* �񓯊��n�̑S�R�}���h */
#define	FS_ARCHIVE_PROC_ASYNC	\
	(FS_ARCHIVE_PROC_READFILE | FS_ARCHIVE_PROC_WRITEFILE)

/* �����n�̃R�}���h */
#define	FS_ARCHIVE_PROC_SEEKDIR			(1 << FS_COMMAND_SEEKDIR)
#define	FS_ARCHIVE_PROC_READDIR			(1 << FS_COMMAND_READDIR)
#define	FS_ARCHIVE_PROC_FINDPATH		(1 << FS_COMMAND_FINDPATH)
#define	FS_ARCHIVE_PROC_GETPATH			(1 << FS_COMMAND_GETPATH)
#define	FS_ARCHIVE_PROC_OPENFILEFAST	(1 << FS_COMMAND_OPENFILEFAST)
#define	FS_ARCHIVE_PROC_OPENFILEDIRECT	(1 << FS_COMMAND_OPENFILEDIRECT)
#define	FS_ARCHIVE_PROC_CLOSEFILE		(1 << FS_COMMAND_CLOSEFILE)
        /* �����n�̑S�R�}���h */
#define	FS_ARCHIVE_PROC_SYNC	\
	(FS_ARCHIVE_PROC_SEEKDIR | FS_ARCHIVE_PROC_READDIR |	\
	 FS_ARCHIVE_PROC_FINDPATH | FS_ARCHIVE_PROC_GETPATH |	\
	FS_ARCHIVE_PROC_OPENFILEFAST | FS_ARCHIVE_PROC_OPENFILEDIRECT | FS_ARCHIVE_PROC_CLOSEFILE)

/* ��ԕω����̃��b�Z�[�W */
#define	FS_ARCHIVE_PROC_ACTIVATE		(1 << FS_COMMAND_ACTIVATE)
#define	FS_ARCHIVE_PROC_IDLE			(1 << FS_COMMAND_IDLE)
#define	FS_ARCHIVE_PROC_SUSPENDING		(1 << FS_COMMAND_SUSPEND)
#define	FS_ARCHIVE_PROC_RESUME			(1 << FS_COMMAND_RESUME)
        /* ��ԕω����̑S���b�Z�[�W */
#define	FS_ARCHIVE_PROC_STATUS	\
	(FS_ARCHIVE_PROC_ACTIVATE | FS_ARCHIVE_PROC_IDLE |	\
	 FS_ARCHIVE_PROC_SUSPENDING | FS_ARCHIVE_PROC_RESUME)

/* ���ׂẴ��b�Z�[�W */
#define	FS_ARCHIVE_PROC_ALL	(~0)

typedef enum
{
    /****************************************************************
	 * ���[�U�G���[�R�[�h
	 * FS_GetErrorCode() �Ŏ擾���܂�.
	 ****************************************************************/

    /*
     * �����������������ʂƂ��Ă̐����������܂�.
     * �Ⴆ�� 100 BYTE ���[�h���悤�Ƃ�����
     * �I�[�ɒB���� 50 BYTE �����ǂ߂Ȃ������ꍇ��
     * �u�����v�Ƃ��Ĉ����܂�.
     */
    FS_RESULT_SUCCESS = 0,

    /*
     * �����������������ʂƂ��Ă̎��s�������܂�.
     * ReadDir �ŏI�[��ǂ񂾂Ƃ��Ȃ�,
     * �����Ƃ��ĔF�߂��Ă���z����́u���s�v���w���܂�.
     */
    FS_RESULT_FAILURE,

    /*
     * ���݃R�}���h�������ł��邱�Ƃ������܂�.
     * �����Ȃ̂�, �����ɂ̓G���[�ł͂���܂���.
     */
    FS_RESULT_BUSY,

    /*
     * �R�}���h���L�����Z�����ꂽ���Ƃ������܂�.
     * ���[�U���g�ɂ�閾���I�ȃR�}���h�L�����Z����,
     * �A�[�J�C�u�A�����[�h���̋����L�����Z��������܂�.
     */
    FS_RESULT_CANCELED,
    FS_RESULT_CANCELLED = FS_RESULT_CANCELED,

    /*
     * �A�[�J�C�u���w��R�}���h�ɑΉ����Ȃ��������Ƃ������܂�.
     * ���̃G���[�̓R�}���h�������Ė�������A�[�J�C�u�����s��,
     * ���[�U�͑ΏۃA�[�J�C�u�̓���d�l��c�����Ă���K�v������܂�.
     */
    FS_RESULT_UNSUPPORTED,

    /*
     * �t�@�C���V�X�e���܂��̓A�[�J�C�u�̃G���[�ɂ�鎸�s�������܂�.
     */
    FS_RESULT_ERROR,

    /****************************************************************
	 * �v���V�[�W�����x�� �G���[�R�[�h
	 * ���[�U�v���V�[�W�����Ԃ�, FS_GetErrorCode() �ɂ͊܂܂�܂���.
	 ****************************************************************/

    /*
     * �R�}���h�̏�����񓯊��I�ɍs������, �ЂƂ܂������Ԃ��܂�.
     * ���̒l��Ԃ����ꍇ��, ���[�h�E���C�g�R�[���o�b�N�Ɠ��l��
     * ������ FS_NotifyArchiveAsyncEnd() �Œʒm����K�v������܂�.
     */
    FS_RESULT_PROC_ASYNC,

    /*
     * ���Ȃ��Ƃ����񂾂��̓f�t�H���g�����ɔC���܂�.
     * ��������Y�R�}���h�ւ̃��[�U�v���V�[�W���͌Ăяo����܂�.
     * �P�ɃR�}���h���t�b�N�������ꍇ�Ȃǂ͖��񂱂��Ԃ��܂�.
     */
    FS_RESULT_PROC_DEFAULT,

    /*
     * ����ȍ~�̂��̃R�}���h��S�ăf�t�H���g�����ɔC���܂�.
     * ���Y�R�}���h�ւ̃v���V�[�W���Ăяo���t���O�͉�������,
     * ���񂩂�Ăяo����܂���.
     */
    FS_RESULT_PROC_UNKNOWN
}
FSResult;


/*****************************************************************************/
/* declaration */

struct FSFile;
struct FSFileLink;
struct FSArchiveFAT;
struct FSArchiveFNT;
struct FSArchive;

/*
 * ���[�U�v���V�[�W��
 * �A�[�J�C�u����ꉻ���邽�߂Ƀ��[�U���ݒ肵�܂�.
 */
typedef FSResult (*FS_ARCHIVE_PROC_FUNC) (struct FSFile *, FSCommandType);

/*
 * Read/Write �A�N�Z�X�p�R�[���o�b�N
 *
 * �Ăяo���̒��ŃA�N�Z�X����������� FS_RESULT_SUCCESS ��Ԃ��K�v������܂�.
 * �񓯊������Ȃǂŏ������p�������� FS_RESULT_PROC_ASYNC ��Ԃ�,
 * �������� FS_NotifyArchiveAsyncEnd() ���R�[������K�v������܂�.
 *
 * �S�R�}���h�̃f�t�H���g�����͂��̃R�[���o�b�N���g�p����
 * FAT / FNT / �t�@�C���C���[�W�փA�N�Z�X���邱�ƂŎ������܂�.
 * �t��, �K�v�ȑS�ẴR�}���h�����[�U�v���V�[�W�����̏�����
 * �K�؂ɃI�[�o�[���C�h����΂����̃R�[���o�b�N�͌Ă΂�܂���.
 */
typedef FSResult (*FS_ARCHIVE_READ_FUNC) (struct FSArchive *p, void *dst, u32 pos, u32 size);
typedef FSResult (*FS_ARCHIVE_WRITE_FUNC) (struct FSArchive *p, const void *src, u32 pos, u32 size);

/*
 * �ȉ��̍\���̂̓A�[�J�C�u�����҂̗��֐��̂��߂Ɍ��J����Ă��܂�.
 * file system API �Œ��ڂ�����K�v�Ƃ��邱�Ƃ͂���܂���.
 */

typedef struct FSFileLink
{
    struct FSFile *prev;
    struct FSFile *next;
}
FSFileLink;

typedef struct FSArchiveFAT
{
    u32     top;
    u32     bottom;
}
FSArchiveFAT;

typedef struct FSArchiveFNT
{
    u32     start;
    u16     index;
    u16     parent;
}
FSArchiveFNT;

/*
 * �A�[�J�C�u�\����
 * �A�[�J�C�u�����҂͂��̍\���̂̎��̂��ЂƂp�ӂ�,
 *
 * (1) FS_InitArchive() �ŏ�����.
 * (2) FS_RegisterArchiveName() �Ŗ��O��o�^.
 * (3) FS_LoadArchive() �ŃV�X�e���Ƀ��[�h.
 *
 * �̎菇�Ńt�@�C���V�X�e���ɃA�[�J�C�u��ǉ����邱�Ƃ��ł��܂�.
 * �A�[�J�C�u�����I�Ȃ��̂ł���Ȃ��
 *
 * (4) FS_UnloadArchive() �ŃV�X�e������A�����[�h.
 *
 * �ɂ���ăA�[�J�C�u�����, �ė��p���邱�Ƃ��ł��܂�.
 * �A�[�J�C�u�������I�ɕύX�������ꍇ�ɂ�
 *
 * (5) FS_ReleaseArchiveName() �Ŗ��O�����.
 *
 * �̂̂��� (2) �����蒼�����Ƃ��ł��܂�.
 */

typedef struct FSArchive
{
/* private: */

    /* �t�@�C���V�X�e���̊Ǘ��p�ϐ� */
    union
    {
        /* �A�[�J�C�u�����ʂ����ӂ̖��O.
           �p���� 1�`3 �����ő召��������ʂ��Ȃ� */
        char    ptr[FS_ARCHIVE_NAME_LEN_MAX + 1];
        u32     pack;
    }
    name;
    /* �V�X�e�����̃A�[�J�C�u�A�����X�g */
    struct FSArchive *next;
    struct FSArchive *prev;

    /* �R�}���h������Ԃ̊Ǘ��p�ϐ� */
    OSThreadQueue sync_q;              /* �������� (SeekDir ��) */
    OSThreadQueue stat_q;              /* ��ԕύX (Suspend ��) */
    u32     flag;                      /* ������ԃt���O (FS_ARCHIVE_FLAG_*) */

    FSFileLink list;                   /* �����҂��̃R�}���h���X�g */

    /* ROM �t�@�C���t�H�[�}�b�g�ɏ�������e��I�t�Z�b�g */
    u32     base;                      /* �x�[�X�I�t�Z�b�g */
    u32     fat;                       /* FAT �I�t�Z�b�g */
    u32     fat_size;                  /* FAT �T�C�Y */
    u32     fnt;                       /* FNT �I�t�Z�b�g */
    u32     fnt_size;                  /* FNT �T�C�Y */
    u32     fat_bak;                   /* �v�����[�h�O�� FAT �x�[�X�I�t�Z�b�g */
    u32     fnt_bak;                   /* �v�����[�h�O�� FNT �x�[�X�I�t�Z�b�g */
    void   *load_mem;                  /* ���[�h���ꂽ�e�[�u���̃����� */

    /* ��{�I�y���[�V�����R�[���o�b�N */
    FS_ARCHIVE_READ_FUNC read_func;
    FS_ARCHIVE_WRITE_FUNC write_func;

    /* �e�[�u���փA�N�Z�X����ۂ̓����R�[���o�b�N.
       (�����ł������� FS_IsArchiveTableLoaded() ���g��Ȃ�����) */
    FS_ARCHIVE_READ_FUNC table_func;

    /* ���[�U�v���V�[�W�� (FS_ARCHIVE_PROC_*) */
    FS_ARCHIVE_PROC_FUNC proc;
    u32     proc_flag;

}
FSArchive;


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         FS_InitArchive

  Description:  �A�[�J�C�u�\���̂�������.

  Arguments:    p_arc            ����������A�[�J�C�u.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FS_InitArchive(FSArchive *p_arc);

/*---------------------------------------------------------------------------*
  Name:         FS_GetArchiveName

  Description:  �A�[�J�C�u�����擾.

  Arguments:    p_arc            ���O���擾����A�[�J�C�u.

  Returns:      �t�@�C���V�X�e���ɓo�^���ꂽ�A�[�J�C�u��.
 *---------------------------------------------------------------------------*/
static inline const char *FS_GetArchiveName(const FSArchive *p_arc)
{
    return p_arc->name.ptr;
}

/*---------------------------------------------------------------------------*
  Name:         FS_GetArchiveBase

  Description:  �A�[�J�C�u�̃x�[�X�I�t�Z�b�g���擾.

  Arguments:    p_arc            �x�[�X�I�t�Z�b�g���擾����A�[�J�C�u.

  Returns:      �A�[�J�C�u�̃x�[�X�I�t�Z�b�g.
 *---------------------------------------------------------------------------*/
static inline u32 FS_GetArchiveBase(const FSArchive *p_arc)
{
    return p_arc->base;
}

/*---------------------------------------------------------------------------*
  Name:         FS_GetArchiveFAT

  Description:  �A�[�J�C�u�� FAT �I�t�Z�b�g���擾.

  Arguments:    p_arc            FAT �I�t�Z�b�g���擾����A�[�J�C�u.

  Returns:      �A�[�J�C�u�� FAT �I�t�Z�b�g.
 *---------------------------------------------------------------------------*/
static inline u32 FS_GetArchiveFAT(const FSArchive *p_arc)
{
    return p_arc->fat;
}

/*---------------------------------------------------------------------------*
  Name:         FS_GetArchiveFNT

  Description:  �A�[�J�C�u�� FNT �I�t�Z�b�g���擾.

  Arguments:    p_arc            FNT �I�t�Z�b�g���擾����A�[�J�C�u.

  Returns:      �A�[�J�C�u�� FNT �I�t�Z�b�g.
 *---------------------------------------------------------------------------*/
static inline u32 FS_GetArchiveFNT(const FSArchive *p_arc)
{
    return p_arc->fnt;
}

/* �A�[�J�C�u�̃x�[�X����̎w��ʒu�I�t�Z�b�g���擾 */
/*---------------------------------------------------------------------------*
  Name:         FS_GetArchiveOffset

  Description:  �A�[�J�C�u�̃x�[�X����̎w��ʒu�I�t�Z�b�g���擾.

  Arguments:    p_arc            �A�[�J�C�u.

  Returns:      �x�[�X�����Z�����w��ʒu�I�t�Z�b�g.
 *---------------------------------------------------------------------------*/
static inline u32 FS_GetArchiveOffset(const FSArchive *p_arc, u32 pos)
{
    return p_arc->base + pos;
}

/*---------------------------------------------------------------------------*
  Name:         FS_IsArchiveLoaded

  Description:  �A�[�J�C�u�����݃t�@�C���V�X�e���Ƀ��[�h�ς݂�����.

  Arguments:    p_arc            ���肷��A�[�J�C�u.

  Returns:      �t�@�C���V�X�e���Ƀ��[�h�ς݂Ȃ� TRUE.
 *---------------------------------------------------------------------------*/
static inline BOOL FS_IsArchiveLoaded(volatile const FSArchive *p_arc)
{
    return (p_arc->flag & FS_ARCHIVE_FLAG_LOADED) ? TRUE : FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         FS_IsArchiveTableLoaded

  Description:  �A�[�J�C�u�����݃e�[�u�����v�����[�h�ς݂�����.

  Arguments:    p_arc            ���肷��A�[�J�C�u.

  Returns:      �e�[�u�����v�����[�h�ς݂Ȃ� TRUE.
 *---------------------------------------------------------------------------*/
static inline BOOL FS_IsArchiveTableLoaded(volatile const FSArchive *p_arc)
{
    return (p_arc->flag & FS_ARCHIVE_FLAG_TABLE_LOAD) ? TRUE : FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         FS_IsArchiveSuspended

  Description:  �A�[�J�C�u�����݃T�X�y���h��������.

  Arguments:    p_arc            ���肷��A�[�J�C�u.

  Returns:      �T�X�y���h���Ȃ� TRUE.
 *---------------------------------------------------------------------------*/
static inline BOOL FS_IsArchiveSuspended(volatile const FSArchive *p_arc)
{
    return (p_arc->flag & FS_ARCHIVE_FLAG_SUSPEND) ? TRUE : FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         FS_FindArchive

  Description:  �A�[�J�C�u������������.
                ��v���閼�O��������� NULL ��Ԃ�.

  Arguments:    name             ��������A�[�J�C�u���̕�����.
                name_len         name �̕�����.

  Returns:      �������Č��������A�[�J�C�u�̃|�C���^�� NULL.
 *---------------------------------------------------------------------------*/
FSArchive *FS_FindArchive(const char *name, int name_len);

/*---------------------------------------------------------------------------*
  Name:         FS_RegisterArchiveName

  Description:  �A�[�J�C�u�����t�@�C���V�X�e���֓o�^��, �֘A�t����.
                �A�[�J�C�u���̂͂܂��t�@�C���V�X�e���Ƀ��[�h����Ȃ�.
                �A�[�J�C�u�� "rom" �̓t�@�C���V�X�e���ɗ\��ς�.

  Arguments:    p_arc            ���O���֘A�t����A�[�J�C�u.
                name             �o�^���閼�O�̕�����.
                name_len         name �̕�����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
BOOL    FS_RegisterArchiveName(FSArchive *p_arc, const char *name, u32 name_len);

/*---------------------------------------------------------------------------*
  Name:         FS_ReleaseArchiveName

  Description:  �o�^�ς݂̃A�[�J�C�u�����������.
                �t�@�C���V�X�e������A�����[�h����Ă���K�v������.

  Arguments:    p_arc            ���O���������A�[�J�C�u.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FS_ReleaseArchiveName(FSArchive *p_arc);

/*---------------------------------------------------------------------------*
  Name:         FS_LoadArchive

  Description:  �A�[�J�C�u���t�@�C���V�X�e���Ƀ��[�h����.
                ���łɃA�[�J�C�u���X�g�ɖ��O���o�^����Ă���K�v������.

  Arguments:    p_arc            ���[�h����A�[�J�C�u.
                base             �Ǝ��Ɏg�p�\�ȔC�ӂ� u32 �l.
                fat              FAT �e�[�u���̐擪�I�t�Z�b�g.
                fat_size         FAT �e�[�u���̃T�C�Y.
                fnt              FNT �e�[�u���̐擪�I�t�Z�b�g.
                fnt_size         FNT �e�[�u���̃T�C�Y.
                read_func        ���[�h�A�N�Z�X�R�[���o�b�N.
                write_func       ���C�g�A�N�Z�X�R�[���o�b�N.

  Returns:      �A�[�J�C�u�����������[�h������ TRUE.
 *---------------------------------------------------------------------------*/
BOOL    FS_LoadArchive(FSArchive *p_arc, u32 base,
                       u32 fat, u32 fat_size, u32 fnt, u32 fnt_size,
                       FS_ARCHIVE_READ_FUNC read_func, FS_ARCHIVE_WRITE_FUNC write_func);

/*---------------------------------------------------------------------------*
  Name:         FS_UnloadArchive

  Description:  �A�[�J�C�u���t�@�C���V�X�e������A�����[�h����.
                ���ݏ������̃^�X�N���S�Ċ�������܂Ńu���b�L���O����.

  Arguments:    p_arc            �A�����[�h����A�[�J�C�u.

  Returns:      �A�[�J�C�u���������A�����[�h������ TRUE.
 *---------------------------------------------------------------------------*/
BOOL    FS_UnloadArchive(FSArchive *p_arc);

/*---------------------------------------------------------------------------*
  Name:         FS_LoadArchiveTables

  Description:  �A�[�J�C�u�� FAT + FNT ����������Ƀv�����[�h����.
                �w��T�C�Y�ȓ��̏ꍇ�̂ݓǂݍ��݂����s��, �K�v�T�C�Y��Ԃ�.

  Arguments:    p_arc            �e�[�u�����v�����[�h����A�[�J�C�u.
                p_mem            �e�[�u���f�[�^�̊i�[��o�b�t�@.
                max_size         p_mem �̃T�C�Y.

  Returns:      ���, ���v�̃e�[�u���T�C�Y��Ԃ�.
 *---------------------------------------------------------------------------*/
u32     FS_LoadArchiveTables(FSArchive *p_arc, void *p_mem, u32 max_size);

/*---------------------------------------------------------------------------*
  Name:         FS_UnloadArchiveTables

  Description:  �A�[�J�C�u�̃v�����[�h���������������.

  Arguments:    p_arc            �v�����[�h���������������A�[�J�C�u.

  Returns:      �v�����[�h�������Ƃ��ă��[�U����^�����Ă����o�b�t�@.
 *---------------------------------------------------------------------------*/
void   *FS_UnloadArchiveTables(FSArchive *p_arc);

/*---------------------------------------------------------------------------*
  Name:         FS_SuspendArchive

  Description:  �A�[�J�C�u�̏����@�\���̂��~����.
                ���ݎ��s���̏����������, ���̊�����ҋ@.

  Arguments:    p_arc            ��~����A�[�J�C�u.

  Returns:      �Ăяo���ȑO�ɃT�X�y���h��ԂłȂ���� TRUE.
 *---------------------------------------------------------------------------*/
BOOL    FS_SuspendArchive(FSArchive *p_arc);

/*---------------------------------------------------------------------------*
  Name:         FS_ResumeArchive

  Description:  ��~���Ă����A�[�J�C�u�̏������ĊJ����.

  Arguments:    p_arc            �ĊJ����A�[�J�C�u.

  Returns:      �Ăяo���ȑO�ɃT�X�y���h��ԂłȂ���� TRUE.
 *---------------------------------------------------------------------------*/
BOOL    FS_ResumeArchive(FSArchive *p_arc);

/*---------------------------------------------------------------------------*
  Name:         FS_SetArchiveProc

  Description:  �A�[�J�C�u�̃��[�U�v���V�[�W����ݒ肷��.
                proc == NULL �܂��� flags = 0 �Ȃ�
                �P�Ƀ��[�U�v���V�[�W���𖳌��ɂ���.

  Arguments:    p_arc            ���[�U�v���V�[�W����ݒ肷��A�[�J�C�u.
                proc             ���[�U�v���V�[�W��.
                flags            �v���V�[�W���փt�b�N����R�}���h�̃r�b�g�W��.

  Returns:      ���, ���v�̃e�[�u���T�C�Y��Ԃ�.
 *---------------------------------------------------------------------------*/
void    FS_SetArchiveProc(struct FSArchive *p_arc, FS_ARCHIVE_PROC_FUNC proc, u32 flags);

/*---------------------------------------------------------------------------*
  Name:         FS_NotifyArchiveAsyncEnd

  Description:  �񓯊��Ŏ��s���Ă����A�[�J�C�u�����̊�����ʒm���邽�߂�
                �A�[�J�C�u����������Ăяo��.

  Arguments:    p_arc            ������ʒm����A�[�J�C�u.
                ret              ��������.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FS_NotifyArchiveAsyncEnd(FSArchive *p_arc, FSResult ret);

/*---------------------------------------------------------------------------*
  Name:         FSi_EndArchive

  Description:  �S�ẴA�[�J�C�u���I�������ĉ������.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FSi_EndArchive(void);


/*****************************************************************************/


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* NITRO_FS_ARCHIVE_H_ */

/*---------------------------------------------------------------------------*
  $Log: archive.h,v $
  Revision 1.17  2007/08/24 04:34:29  okubata_ryoma
  fix about headerfiles.

  Revision 1.16  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.15  2005/09/02 08:28:29  yosizaki
  add compatible symbol 'FS_RESULT_CANCELLED'

  Revision 1.14  2005/05/30 04:20:42  yosizaki
  add FSi_EndArchive().
  add comments.

  Revision 1.13  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.12  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.11  2004/11/02 10:07:49  yosizaki
  fix typo.

  Revision 1.10  2004/07/23 08:32:27  yosizaki
  fix command operation.

  Revision 1.9  2004/07/19 13:12:59  yosizaki
  add all commands.

  Revision 1.8  2004/07/08 13:45:54  yosizaki
  change archive commands.

  Revision 1.7  2004/06/30 10:28:08  yosizaki
  add comments.

  Revision 1.6  2004/06/30 04:30:46  yosizaki
  implement user-procedure system.

  Revision 1.5  2004/06/22 01:49:37  yosizaki
  add user-procedure system (reserved).

  Revision 1.4  2004/06/03 13:32:53  yosizaki
  fix around archive asyncronous operation.

  Revision 1.3  2004/05/20 12:37:38  yosizaki
  fix FSFileLink definition.

  Revision 1.2  2004/05/20 06:30:33  yosizaki
  add FS_ConvertPathToFileID.
  add some structure for archive.

  Revision 1.1  2004/05/11 04:32:58  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
