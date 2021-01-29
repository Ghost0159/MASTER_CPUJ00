/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./src/lobby/dwci_lobbyBase.cpp

  Copyright 2005-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/
/**
 * @file
 *
 * @brief �A���P�[�^�\�[�X
 */

#include "dwci_lobbyCallback.h"


/** 
 * @brief �w�肵���I�y���[�V����ID�̃R�[���o�b�N���폜���܂��B
 * 
 * @param[in] operationId �I�y���[�V����ID�B
 * 
 * @retval TRUE �����B
 * @retval FALSE ������Ȃ������B
 */
BOOL DWCi_CallbackManager::RemoveCallback(u32 operationId)
{
    DWCi_AbstractCallback findObj(operationId, NULL);
    CallbackContainer::iterator ret = callbacks.find(&findObj);
    if(ret == callbacks.end())
    {
        return FALSE;
    }
    delete *ret;
    callbacks.erase(ret);
    return TRUE;
}

/** 
 * @brief �R�[���o�b�N��S�č폜���܂��B
 */
void DWCi_CallbackManager::ClearCallback()
{
    for(CallbackContainer::iterator it = callbacks.begin(); it != callbacks.end();)
    {
        delete *it;
        callbacks.erase(it++);
    }
}
