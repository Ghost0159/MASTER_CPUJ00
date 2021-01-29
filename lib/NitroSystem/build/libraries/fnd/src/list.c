/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - fnd
  File:     list.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.8 $
 *---------------------------------------------------------------------------*/

#include <nnsys/misc.h>
#include <nnsys/fnd/list.h>

#define OBJ_TO_LINK(list,obj)   ((NNSFndLink*)(((u32)(obj))+(list)->offset))


/*---------------------------------------------------------------------------*
  Name:         NNS_FndInitList

  Description:  ���X�g�\���̂����������܂��B

  Arguments:    list:   ���X�g�\���̂ւ̃|�C���^�B
                offset: ���X�g�Ɍq�������\���̂̒��ɑ��݂���ANNSFndLink�^��
                        �����o�ϐ��̍\���̓��ł̃I�t�Z�b�g���w�肵�܂��B
                        stddef.h�Œ�`����Ă���offsetof�}�N�����g�p����ƁA
                        �֗��ł��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndInitList(NNSFndList* list, u16 offset)
{
    NNS_NULL_ASSERT(list);

    list->headObject = NULL;
    list->tailObject = NULL;
    list->numObjects = 0;
    list->offset     = offset;
}

/*---------------------------------------------------------------------------*
  Name:         SetFirstObject                                      [static]

  Description:  �ŏ��̃I�u�W�F�N�g�����X�g�ɉ����܂��B

  Arguments:    list:   ���X�g�\���̂ւ̃|�C���^�B
                object: ���X�g�Ɍq�������I�u�W�F�N�g�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
SetFirstObject(NNSFndList* list, void* object)
{
    NNSFndLink* link;

    NNS_NULL_ASSERT(list  );
    NNS_NULL_ASSERT(object);

    link = OBJ_TO_LINK(list, object);

    link->nextObject = NULL;
    link->prevObject = NULL;
    list->headObject = object;
    list->tailObject = object;
    list->numObjects++;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndAppendListObject

  Description:  �I�u�W�F�N�g�����X�g�̍Ō�ɒǉ����܂��B

  Arguments:    list:   ���X�g�\���̂ւ̃|�C���^�B
                object: ���X�g�Ɍq�������I�u�W�F�N�g�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndAppendListObject(NNSFndList* list, void* object)
{
    NNS_NULL_ASSERT(list  );
    NNS_NULL_ASSERT(object);

    if (list->headObject == NULL)
    {
        // ���X�g����̎��B
        SetFirstObject(list, object);
    }
    else
    {
        NNSFndLink* link = OBJ_TO_LINK(list, object);

        link->prevObject = list->tailObject;
        link->nextObject = NULL;

        OBJ_TO_LINK(list, list->tailObject)->nextObject = object;
        list->tailObject = object;
        list->numObjects++;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndPrependListObject

  Description:  �I�u�W�F�N�g�����X�g�̐擪�ɑ}�����܂��B

  Arguments:    list:   ���X�g�\���̂ւ̃|�C���^�B
                object: ���X�g�Ɍq�������I�u�W�F�N�g�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndPrependListObject(NNSFndList* list, void* object)
{
    NNS_NULL_ASSERT(list  );
    NNS_NULL_ASSERT(object);

    if (list->headObject == NULL)
    {
        // ���X�g����̎��B
        SetFirstObject(list, object);
    }
    else
    {
        NNSFndLink* link = OBJ_TO_LINK(list, object);

        link->prevObject = NULL;
        link->nextObject = list->headObject;

        OBJ_TO_LINK(list, list->headObject)->prevObject = object;
        list->headObject = object;
        list->numObjects++;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndInsertListObject

  Description:  �I�u�W�F�N�g���w�肳�ꂽ�ʒu�ɑ}�����܂��B�I�u�W�F�N�g�́A
                target�Ŏw�肳�ꂽ�I�u�W�F�N�g�̑O�ɑ}������܂��B�}���悪�w
                �肳��Ă��Ȃ��ꍇ�itarget��NULL�̏ꍇ�j�A�I�u�W�F�N�g�̓��X
                �g�̍Ō�ɒǉ�����܂��B

  Arguments:    list:   ���X�g�\���̂ւ̃|�C���^�B
                target: �}���������ʒu�ɂ���I�u�W�F�N�g�ւ̃|�C���^�B
                object: ���X�g�Ɍq�������I�u�W�F�N�g�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndInsertListObject(NNSFndList* list, void* target, void* object)
{
    NNS_NULL_ASSERT(list  );
    NNS_NULL_ASSERT(object);

    if (target == NULL)
    {
        // target���w�肳��Ă��Ȃ��ꍇ�́ANNS_FndAppendListObject()�Ɠ����B
        NNS_FndAppendListObject(list, object);
    }
    else if (target == list->headObject)
    {
        // target�����X�g�̐擪�ł���ꍇ��NNS_FndPrependListObject()�Ɠ����B
        NNS_FndPrependListObject(list, object);
    }
    else
    {
        NNSFndLink* link    = OBJ_TO_LINK(list, object);
        void*       prevObj = OBJ_TO_LINK(list, target)->prevObject;
        NNSFndLink* prevLnk = OBJ_TO_LINK(list, prevObj);

        link->prevObject    = prevObj;
        link->nextObject    = target;
        prevLnk->nextObject = object;
        OBJ_TO_LINK(list, target)->prevObject = object;
        list->numObjects++;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndRemoveListObject

  Description:  �I�u�W�F�N�g�����X�g����폜���܂��B

  Arguments:    list:   ���X�g�\���̂ւ̃|�C���^�B
                object: ���X�g����폜�������I�u�W�F�N�g�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndRemoveListObject(NNSFndList* list, void* object)
{
    NNSFndLink* link;

    NNS_NULL_ASSERT(list  );
    NNS_NULL_ASSERT(object);

    link = OBJ_TO_LINK(list, object);

    if (link->prevObject == NULL)
    {
        list->headObject = link->nextObject;
    }
    else
    {
        OBJ_TO_LINK(list, link->prevObject)->nextObject = link->nextObject;
    }
    if (link->nextObject == NULL)
    {
        list->tailObject = link->prevObject;
    }
    else
    {
        OBJ_TO_LINK(list, link->nextObject)->prevObject = link->prevObject;
    }
    link->prevObject = NULL;
    link->nextObject = NULL;
    list->numObjects--;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetNextListObject

  Description:  object�Ŏw�肳�ꂽ�I�u�W�F�N�g�̎��Ɍq����Ă���I�u�W�F�N�g
                ��Ԃ��܂��Bobject��NULL���w�肳��Ă����ꍇ�ɂ́A���X�g�̐�
                ���Ɍq����Ă���I�u�W�F�N�g��Ԃ��܂��B

  Arguments:    list:   ���X�g�\���̂ւ̃|�C���^�B
                object: ���X�g���̃I�u�W�F�N�g�ւ̃|�C���^�B

  Returns:      �w�肳�ꂽ�I�u�W�F�N�g�̎��̃I�u�W�F�N�g�ւ̃|�C���^��Ԃ���
                ���B�����A���̃I�u�W�F�N�g��������΁ANULL��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void*
NNS_FndGetNextListObject(NNSFndList* list, void* object)
{
    NNS_NULL_ASSERT(list);

    if (object == NULL)
    {
        return list->headObject;
    }
    return OBJ_TO_LINK(list, object)->nextObject;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetPrevListObject

  Description:  object�Ŏw�肳�ꂽ�I�u�W�F�N�g�̑O�Ɍq����Ă���I�u�W�F�N�g
                ��Ԃ��܂��Bobject��NULL���w�肳��Ă����ꍇ�ɂ́A���X�g�̌�
                ���Ɍq����Ă���I�u�W�F�N�g��Ԃ��܂��B

  Arguments:    list:   ���X�g�\���̂ւ̃|�C���^�B
                object: ���X�g���̃I�u�W�F�N�g�ւ̃|�C���^�B

  Returns:      �w�肳�ꂽ�I�u�W�F�N�g�̑O�̃I�u�W�F�N�g�ւ̃|�C���^��Ԃ���
                ���B�����A�O�̃I�u�W�F�N�g��������΁ANULL��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void*
NNS_FndGetPrevListObject(NNSFndList* list, void* object)
{
    NNS_NULL_ASSERT(list);

    if (object == NULL)
    {
        return list->tailObject;
    }
    return OBJ_TO_LINK(list, object)->prevObject;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetNthListObject

  Description:  ���X�g�̂m�ԖڂɌq����Ă���I�u�W�F�N�g�ւ̃|�C���^��Ԃ���
                ���B�擪���珇�ԂɃ��X�g�����ǂ�ׁA���X�g�̌㕔�Ɍq����Ă�
                �I�u�W�F�N�g���A���Ԃ�������܂��B

  Arguments:    index:  �I�u�W�F�N�g�̃C���f�b�N�X�B

  Returns:      �I�u�W�F�N�g�ւ̃|�C���^��Ԃ��܂��B�����A�w�肳�ꂽ�C���f�b
                �N�X�̃I�u�W�F�N�g�����������ꍇ�ɂ́ANULL���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
void*
NNS_FndGetNthListObject(NNSFndList* list, u16 index)
{
    int         count  = 0;
    NNSFndLink* object = NULL;

    NNS_NULL_ASSERT(list);

    while ((object = NNS_FndGetNextListObject(list, object)) != NULL)
    {
        if (index == count)
        {
            return object;
        }
        count++;
    }
    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_UTCountListNode

  Description:  ���X�g�Ɍq����Ă���I�u�W�F�N�g�̐����J�E���g���܂��B�擪��
                �珇�ԂɃ��X�g���J�E���g����ׁA���X�g�������Ǝ��Ԃ��������
                ���B

  Arguments:    list:   ���X�g�\���̂ւ̃|�C���^�B
 
  Returns:      ���X�g�Ɍq����Ă���I�u�W�F�N�g�̐��B
 *---------------------------------------------------------------------------*/
#if 0
int
NNS_UTCountListNode(NNS_UTList* list)
{
    int             count  = 0;
    NNS_UTListNode* object = NULL;

    NNS_NULL_ASSERT(list);

    while ((object = NNS_UTGetNextListObject(list, object)) != NULL)
    {
        count++;
    }
    return count;
}
#endif
