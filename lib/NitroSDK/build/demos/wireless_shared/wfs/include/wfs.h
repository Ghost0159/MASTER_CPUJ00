/*---------------------------------------------------------------------------*
  Project:  NitroSDK - wireless_shared - demos - wfs
  File:     wfs.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wfs.h,v $
  Revision 1.5  2006/05/19 06:42:06  yosizaki
  fix source indent.

  Revision 1.4  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.3  2005/05/09 04:30:58  yosizaki
  add comment of WFS_End().

  Revision 1.2  2005/02/28 05:26:34  yosizaki
  do-indent.

  Revision 1.1  2004/11/16 01:13:04  yosizaki
  moved from /demos/wbt/wbt-fs.


 -[old log at /demos/wbt/wbt-fs/wfs]-

  Revision 1.9  2004/10/25 04:20:14  yosizaki
  add WFS_InitParent and WFS_InitChild.
  add some getter functions.

  Revision 1.8  2004/09/29 07:28:03  yosizaki
  change WFS_Init.

  Revision 1.7  2004/09/29 07:10:11  yosizaki
  merge from branch.

  Revision 1.6  2004/09/15 01:11:12  yosizaki
  add WFS_SetPacketSize().

  Revision 1.5  2004/09/11 04:13:35  yosizaki
  add comments.
  unify MP-size definition.

  Revision 1.4  2004/09/04 13:25:40  yosizaki
  add WFS_GetCurrentDownloadProgress.

  Revision 1.3  2004/09/04 01:45:14  yosizaki
  delete obsolete codes and waste parameters.

  Revision 1.2  2004/09/03 10:10:04  yosizaki
  enable WFS_OBSOLETE.

  Revision 1.1  2004/09/03 01:12:10  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef	NITRO_BUILD_DEMOS_WIRELESSSHARED_WFS_INCLUDE_WFS_H_
#define	NITRO_BUILD_DEMOS_WIRELESSSHARED_WFS_INCLUDE_WFS_H_


#include <nitro.h>

#ifdef __cplusplus
extern  "C" {
#endif


/*****************************************************************************/
/* constant */


/*
 * WFS �̓�����Ԃ������񋓌^�ł�.
 */
typedef enum
{
    WFS_STATE_STOP,
    WFS_STATE_IDLE,
    WFS_STATE_READY,
    WFS_STATE_ERROR
}
WFSState;


/*****************************************************************************/
/* prototype */

/*
 * WFS �̓�����Ԃ��ς�邲�ƂɌĂяo�����R�[���o�b�N�ł�.
 * WFS_STATE_READY ���Ԃ������_�Ŏ����I�� FS �����p�\�ƂȂ�܂�.
 */
typedef void (*WFSStateCallback) (void *);


/*
 * WFS �����̓��I���������蓖�ăR�[���o�b�N�ł�.
 * arg �̓��[�U��`�̈����ł�.
 * ptr �� NULL �Ȃ� size �o�C�g�̃�������Ԃ��Ă�������.
 * ptr �� NULL �łȂ��Ȃ� ptr ��������Ă�������.
 */
typedef void *(*WFSAllocator) (void *arg, u32 size, void *ptr);


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         WFS_InitParent

  Description:  �����t�@�C���V�X�e��(WFS)��e�@�Ƃ��ď��������܂�.
                WM ���C�u�����փ|�[�g�R�[���o�b�N��ݒ肵�Ď����쓮����̂�,
                �K�� WM �� READY �X�e�[�g�ȏ�ŌĂяo���K�v������܂�.

  Arguments:    port             �����ł� MP �ʐM�Ɏg�p������|�[�g�ԍ�.
                callback         �e���Ԓʒm���󂯂�R�[���o�b�N.
                allocator        �����ł̃������m�ۂɎg�p������R�[���o�b�N.
                                 WFS �̎g�p�������ʂ̓A�v���P�[�V�������Ƃ�
                                 ���s���Ɍ��肳��܂�.
                allocator_arg    allocator �ɗ^���郆�[�U��`�̈���.
                parent_packet    �����ݒ肷��e�@�����M�T�C�Y.
                                 ���̒l�� WBT_PACKET_SIZE_MIN �ȏォ��
                                 �e�@�ő呗�M�T�C�Y�ȉ��ł���K�v������܂�.
                                 �q�@�̏ꍇ�͒P�ɖ�������܂�.
                p_rom            �q�@�ɑ΂��Ē񋟂��� FAT/FNT/OVT ���܂�
                                 �v���O�������w���t�@�C���|�C���^.
                                 ����͒ʏ�, ���C�����X�_�E�����[�h�ő��M����
                                 �q�@�v���O�������̂��̂��w�肵�܂�.
                                 �V�[�N�ʒu�͌Ăяo���O�̈ʒu���ێ����܂�.
                                 ���̈����� NULL �̏ꍇ, �e�@���g��
                                 �t�@�C���V�X�e����񋟂��܂�.
                use_parent_fs    TRUE �̏ꍇ, p_rom �Ŏw�肵���v���O�����łȂ�
                                 �e�@���g�� FAT/FNT ��񋟂��܂�.
                                 ���̐ݒ�ɂ��, �Ɨ������e�q�v���O������
                                 �P��t�@�C���V�X�e���̋��L���\�ł�.
                                 p_rom �� NULL �ł���΂��̐ݒ�͖�������,
                                 ��ɋ��L�����ƂȂ�܂�.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WFS_InitParent(int port, WFSStateCallback callback,
                       WFSAllocator allocator, void *allocator_arg, int parent_packet,
                       FSFile *p_rom, BOOL use_parent_fs);

/*---------------------------------------------------------------------------*
  Name:         WFS_InitChild

  Description:  �����t�@�C���V�X�e��(WFS)���q�@�Ƃ��ď��������܂�.
                WM ���C�u�����փ|�[�g�R�[���o�b�N��ݒ肵�Ď����쓮����̂�,
                �K�� WM �� READY �X�e�[�g�ȏ�ŌĂяo���K�v������܂�.

  Arguments:    port             �����ł� MP �ʐM�Ɏg�p������|�[�g�ԍ�.
                callback         �e���Ԓʒm���󂯂�R�[���o�b�N.
                allocator        �����ł̃������m�ۂɎg�p������R�[���o�b�N.
                                 WFS �̎g�p�������ʂ̓A�v���P�[�V�������Ƃ�
                                 ���s���Ɍ��肳��܂�.
                allocator_arg    allocator �ɗ^���郆�[�U��`�̈���.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WFS_InitChild(int port, WFSStateCallback callback,
                      WFSAllocator allocator, void *allocator_arg);

/*---------------------------------------------------------------------------*
  Name:         WFS_Init

  Description:  �����t�@�C���V�X�e��(WFS)�����������܂�.
                �e�@�܂��͎q�@�Ƃ��Ă̓�����Ԃ�������������,
                WM ���C�u�����փ|�[�g�R�[���o�b�N��ݒ肵�Ď����쓮����̂�,
                �K�� WM �� READY �X�e�[�g�ȏ�ŌĂяo���K�v������܂�.

  Arguments:    is_parent        �����e�@�Ȃ� TRUE, �q�Ȃ� FALSE.
                                 ���ۂ̃}���`�u�[�g�ł͂��̈����ɂ�
                                 !MB_IsMultiBootChild() ��^���܂�.
                port             �����ł� MP �ʐM�Ɏg�p������|�[�g�ԍ�.
                parent_packet    �����ݒ肷��e�@�����M�T�C�Y.
                                 ���̒l�� WBT_PACKET_SIZE_MIN �ȏォ��
                                 �e�@�ő呗�M�T�C�Y�ȉ��ł���K�v������܂�.
                                 �q�@�̏ꍇ�͒P�ɖ�������܂�.
                callback         �e���Ԓʒm���󂯂�R�[���o�b�N.
                allocator        �����ł̃������m�ۂɎg�p������R�[���o�b�N.
                                 WFS �̎g�p�������ʂ̓A�v���P�[�V�������Ƃ�
                                 ���s���Ɍ��肳��܂�.
                allocator_arg    allocator �ɗ^���郆�[�U��`�̈���.

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void WFS_Init(BOOL is_parent, int port, int parent_packet,
                         WFSStateCallback callback, WFSAllocator allocator, void *allocator_arg)
{
    if (is_parent)
        WFS_InitParent(port, callback, allocator, allocator_arg, parent_packet, NULL, TRUE);
    else
        WFS_InitChild(port, callback, allocator, allocator_arg);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_Start

  Description:  WFS �ɑ΂�, ������ MP �X�e�[�g�ɂȂ������Ƃ�ʒm���܂�.
                WFS �͂��̒ʒm�ȍ~, WM_SetMPDataToPort() ���g�p����
               �u�D��x:�� (WM_PRIORITY_LOW) �v�Ŏ����쓮���܂�.
                ���̂���, �K�� MP_PARENT �܂��� MP_CHILD �X�e�[�g��
                �Ăяo���K�v������܂�.
                �ʏ�� WM_StartMP() �̃R�[���o�b�N�� state �̗v����ʂ�
                WM_STATECODE_MP_START ���n���ꂽ��ɌĂяo���܂�.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WFS_Start(void);

/*---------------------------------------------------------------------------*
  Name:         WFS_End

  Description:  WFS ���s�v�ɂȂ������_�ŌĂяo���܂�.
                �����Ŋm�ۂ�����������S�ĉ����, �������O�̏�Ԃɖ߂�܂�.
                �ʏ��, ���ׂĂ̖����ʐM���ؒf���ꂽ���_�ŌĂяo���܂�.
                ���̊֐������荞�݃n���h������Ăяo�����Ƃ͂ł��܂���.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WFS_End(void);

/*---------------------------------------------------------------------------*
  Name:         WFS_GetStatus

  Description:  WFS �̌��݂̓�����Ԃ� WFSState �^�Ŏ擾���܂�.

  Arguments:    None.

  Returns:      WFSState �^�̗񋓒l�̂����ꂩ.
 *---------------------------------------------------------------------------*/
WFSState WFS_GetStatus(void);

/*---------------------------------------------------------------------------*
  Name:         WFS_GetCurrentBitmap

  Description:  WBT ���F�����Ă��錻�݂̎q�@�Q�̃r�b�g�}�b�v���擾���܂�.
                ���̊֐��͐e�@���̂݌Ăяo�����Ƃ��ł��܂�.

  Arguments:    None.

  Returns:      ���ݔF������Ă���q�@�Q.
 *---------------------------------------------------------------------------*/
int     WFS_GetCurrentBitmap(void);

/*---------------------------------------------------------------------------*
  Name:         WFS_GetSyncBitmap

  Description:  �A�N�Z�X�������w�肳�ꂽ�q�@�Q�̃r�b�g�}�b�v���擾���܂�.
                ���̊֐��͐e�@���̂݌Ăяo�����Ƃ��ł��܂�.

  Arguments:    None.

  Returns:      �A�N�Z�X���������悤�w�肳�ꂽ�q�@�Q.
 *---------------------------------------------------------------------------*/
int     WFS_GetSyncBitmap(void);

/*---------------------------------------------------------------------------*
  Name:         WFS_GetBusyBitmap

  Description:  ���ݐe�@�ɃA�N�Z�X���̎q�@�Q�̃r�b�g�}�b�v���擾���܂�.
                ���̊֐��͐e�@���̂݌Ăяo�����Ƃ��ł��܂�.

  Arguments:    None.

  Returns:      ���ݐe�@�ɃA�N�Z�X���̎q�@�Q.
 *---------------------------------------------------------------------------*/
int     WFS_GetBusyBitmap(void);

/*---------------------------------------------------------------------------*
  Name:         WFS_IsBusy

  Description:  �w�肵�� aid �̎q�@�����݃A�N�Z�X�������肵�܂�.
                ���̊֐��͐e�@���̂݌Ăяo�����Ƃ��ł��܂�.

  Arguments:    aid              ���肷��q�@�� aid.

  Returns:      �w��̎q�@���A�N�Z�X���Ȃ� TRUE, �����łȂ���� FALSE.
 *---------------------------------------------------------------------------*/
BOOL    WFS_IsBusy(int aid);

/*---------------------------------------------------------------------------*
  Name:         WFS_GetCurrentDownloadProgress

  Description:  �q�@�������� ReadFile ���Ă���t�@�C���̐i�����擾���܂�.
                �P�ʂ� WBT �����̃V�[�P���X�ԍ��ƂȂ�܂�.
                ���̊֐��͎q�@���̂݌Ăяo�����Ƃ��ł��܂�.

  Arguments:    current_count    ���݂̎�M�ς݃V�[�P���X�����i�[����
                                 �ϐ��̃A�h���X.
                total_count      ��M���ׂ��V�[�P���X�������i�[����
                                 �ϐ��̃A�h���X.

  Returns:      ���� ReadFile �̏�Ԃł��������i�����擾�ł���� TRUE,
                �����łȂ���� FALSE ��Ԃ��܂�.
 *---------------------------------------------------------------------------*/
BOOL    WFS_GetCurrentDownloadProgress(int *current_count, int *total_count);

/*---------------------------------------------------------------------------*
  Name:         WFS_GetPacketSize

  Description:  WFS �ɐݒ肳�ꂽ�e�@�� MP �ʐM�p�P�b�g�T�C�Y���擾���܂�.

  Arguments:    None.

  Returns:      WFS �ɐݒ肳�ꂽ�e�@�� MP �ʐM�p�P�b�g�T�C�Y.
 *---------------------------------------------------------------------------*/
int     WFS_GetPacketSize(void);

/*---------------------------------------------------------------------------*
  Name:         WFS_SetPacketSize

  Description:  �e�@�� MP �ʐM�p�P�b�g�T�C�Y��ݒ肵�܂�.
                ���̒l��傫������Ɠ]�����������サ, �����������ꍇ��
                ���� MP �ʐM�Ńf�[�^�V�F�A�����O���̕ʃ|�[�g�ʐM�Ɠ�������,
                �|�[�g�D��x�ɂ�����I�Ȓ�؂�������邱�Ƃ��ł��܂�.
                ���̊֐��͐e�@���̂݌Ăяo�����Ƃ��ł��܂�.

  Arguments:    size             �ݒ肷��e�@�����M�T�C�Y.
                                 ���̒l�� WBT_PACKET_SIZE_MIN �ȏォ��
                                 �e�@�ő呗�M�T�C�Y�ȉ��ł���K�v������܂�.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WFS_SetPacketSize(int size);

/*---------------------------------------------------------------------------*
  Name:         WFS_EnableSync

  Description:  �e�@���ŃA�N�Z�X���������q�@�Q�̃Z�b�g��ݒ肵�܂�.
                ���̊֐���, ����̃t�@�C���Q�𓯂������ŃA�N�Z�X���邱�Ƃ�
                ���炩�ɕۏ؂���Ă���q�@�Q�ɑ΂��ĉ����𓯊����邱�Ƃ�
                WBT ���C�u�����̓������������������I�ȓ]�����������܂�.
                ������, �����J�n�̃^�C�~���O���_���I�Ɉ��S�łȂ��ꍇ
                �q�@�Q�ւ̉���������ăf�b�h���b�N����_�ɒ��ӂ��Ă�������.
                ���̊֐��͐e�@���̂݌Ăяo�����Ƃ��ł��܂�.

  Arguments:    sync_bitmap      �A�N�Z�X���������q�@�Q�� aid �r�b�g�}�b�v.
                                 �e�@���g�������ŉ��ʃr�b�g 1 �͖�������܂�.
                                 ���̒l�� 0 ���w�肷��Ɠ����͍s���܂���.
                                 �f�t�H���g�ł͂��̏�Ԃł�.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WFS_EnableSync(int sync_bitmap);

/*---------------------------------------------------------------------------*
  Name:         WFS_DisableSync

  Description:  �e�@���Ŏq�@�Q�̃A�N�Z�X�������������܂�.
                ���̊֐��� WFS_EnableSync() 0 ���w�肷�邱�ƂƓ����ł�.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void WFS_DisableSync(void)
{
    WFS_EnableSync(0);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_SetDebugMode

  Description:  WFS �����̃f�o�b�O�o�͂�L�� / �����ɐݒ肵�܂�.
                ���̐ݒ�̓f�t�H���g�� FALSE �ɂȂ��Ă��܂�.

  Arguments:    enable_debug     �f�o�b�O�o�͗L���Ȃ� TRUE, �����Ȃ� FALSE.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WFS_SetDebugMode(BOOL enable_debug);


/*****************************************************************************/


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* NITRO_BUILD_DEMOS_WIRELESSSHARED_WFS_INCLUDE_WFS_H_ */
