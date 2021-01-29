/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - fnd
  File:     list.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.6 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_FND_LIST_H_
#define NNS_FND_LIST_H_

#include <stddef.h>
#include <nitro/types.h>


#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------*
  Name:         NNSFndLink

  Description:  �o���������N���X�g�̃m�[�h�\���̂ł��B���̍\���̂����X�g�\��
                �Ōq�������\���̂̃����o�Ƃ��Ċi�[���܂��B
 *---------------------------------------------------------------------------*/
typedef struct
{
    void*       prevObject;     // �O�Ɍq����Ă���I�u�W�F�N�g�ւ̃|�C���^�B
    void*       nextObject;     // ���Ɍq����Ă���I�u�W�F�N�g�ւ̃|�C���^�B

} NNSFndLink;


/*---------------------------------------------------------------------------*
  Name:         NNSFndList

  Description:  �o���������N���X�g�\���̂ł��B
 *---------------------------------------------------------------------------*/
typedef struct 
{
    void*       headObject;     // �擪�Ɍq����Ă���I�u�W�F�N�g�ւ̃|�C���^�B
    void*       tailObject;     // ����Ɍq����Ă���I�u�W�F�N�g�ւ̃|�C���^�B
    u16         numObjects;     // ���X�g�Ɍq����Ă���I�u�W�F�N�g�̌��B
    u16         offset;         // NNSFndLink�^�̍\���̃����o�̃I�t�Z�b�g�B

} NNSFndList;


/*---------------------------------------------------------------------------*
  Name:         NNS_FND_INIT_LIST

  Description:  ���X�g�\���̂����������邽�߂̃}�N���ł��B���ۂ̏������ɂ́A
                NNSFndInitList()�֐��ɂ��s���܂��B

                ���̃}�N���ł́A�w�肳�ꂽ�\���̖���NNSFndLink�^�����o�ϐ���
                ����Aoffsetof�}�N�����g���ăI�t�Z�b�g�����߁ANNSFndInitList
                �֐��ɓn���Ă��܂��B

  Arguments:    list:       �����N�\���̂ւ̃|�C���^�B
                structName: ���X�g�Ɍq�������I�u�W�F�N�g�̍\���̖��B
                linkName:   ���̃I�u�W�F�N�g�̃����N�Ɏg�p�����NNSFndLink�^
                            �̃����o�ϐ����B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/

#define NNS_FND_INIT_LIST(list, structName, linkName) \
            NNS_FndInitList(list, offsetof(structName, linkName))


/*---------------------------------------------------------------------------*
    �֐��v���g�^�C�v�B

 *---------------------------------------------------------------------------*/

void    NNS_FndInitList(
                NNSFndList*             list,
                u16                     offset);

void    NNS_FndAppendListObject(
                NNSFndList*             list,
                void*                   object);

void    NNS_FndPrependListObject(
                NNSFndList*             list,
                void*                   object);

void    NNS_FndInsertListObject(
                NNSFndList*             list,
                void*                   target,
                void*                   object);

void    NNS_FndRemoveListObject(
                NNSFndList*             list,
                void*                   object);

void*   NNS_FndGetNextListObject(
                NNSFndList*             list,
                void*                   object);

void*   NNS_FndGetPrevListObject(
                NNSFndList*             list,
                void*                   object);

void*   NNS_FndGetNthListObject(
                NNSFndList*             list,
                u16                     index);


#ifdef __cplusplus
} /* extern "C" */
#endif

/* NNS_FND_LIST_H_ */
#endif
