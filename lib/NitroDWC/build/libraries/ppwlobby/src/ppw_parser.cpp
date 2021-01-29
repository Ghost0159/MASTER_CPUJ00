/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./src/lobby/lobby.c

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
 * @brief ���r�[���C�u�����̃��b�p�[���[�e�B���e�B�[ �\�[�X
 */

#include "ppw_parser.h"


BOOL PPW_LobbyMessage::Parse(const DWCi_String& message, Result& result)
{
    // �w�b�_�ƃ{�f�B�𕪂���
    std::vector<DWCi_String, DWCi_Allocator<DWCi_String> > headerBody = DWCi_SplitByStr<DWCi_String>(message, PPW_LOBBY_MESSAGE_HEADER_BODY_SEPALATOR, 2);

    if(headerBody.size() != 2)
    {
        return FALSE;
    }

    // �w�b�_���K����傫�������疳������
    if(headerBody[0].length() > MAX_MESSAGE_HEADER_SIZE)
    {
        return FALSE;
    }
    
    // �{�f�B���K����傫�������疳������
    if(headerBody[1].length() > PPW_LOBBY_MAX_STRING_SIZE)
    {
        return FALSE;
    }

    // �w�b�_�𕪉����Z�b�g����
    {
        std::vector<DWCi_String, DWCi_Allocator<DWCi_String> > headerBlocks
            = DWCi_SplitByStr<DWCi_String>(headerBody[0], PPW_LOBBY_MESSAGE_HEADER_BLOCK_SEPALATOR);
        if(headerBlocks.size() < CURRENT_HEADER_BLOCK_NUM)
        {
            return FALSE;
        }
        char* endptr;
        result.version = strtoul(headerBlocks[0].c_str(), &endptr, 16);
        result.blockNum = strtoul(headerBlocks[1].c_str(), &endptr, 16);    // ���̂Ƃ���u���b�N�̐��͋C�ɂ��Ȃ�
        result.format = DWCi_StrnicmpChar(headerBlocks[2].c_str(), "B", 1) == 0 ? FORMAT_BASE64 : FORMAT_STRING;
        result.target = DWCi_StrnicmpChar(headerBlocks[3].c_str(), "S", 1) == 0 ? TARGET_SYSTEM : TARGET_APPLICATION;
        
        result.type = strtol(headerBlocks[4].c_str(), &endptr, 16);
    }
    
    // �{�f�B���Z�b�g����
    if(result.format == FORMAT_BASE64)
    {
        if(!DWCi_Base64Decode(headerBody[1].c_str(), result.data))
        {
            DWC_Printf(DWC_REPORTFLAG_ERROR, "PPW_LobbyMessage::Parse: DWC_Base64Decode failed\n");
            return FALSE;
        }
    }
    else // result.format == FORMAT_STRING
    {
        result.data.assign((const u8*)headerBody[1].c_str(), (const u8*)headerBody[1].c_str() + headerBody[1].length());
        result.data.push_back('\0');    // FORMAT_STRING�̏ꍇ��NULL�I�[��ۏ؂���
    }
    
    return TRUE;
}

BOOL PPW_LobbyMessage::Build(FORMAT format, TARGET target, s32 type, const char* message, std::size_t size, DWCi_String& dstStr)
{
    if(format == FORMAT_STRING && size > PPW_LOBBY_MAX_STRING_SIZE)
    {
        return FALSE;
    }
    if(format == FORMAT_BASE64 && size > PPW_LOBBY_MAX_BINARY_SIZE)
    {
        return FALSE;
    }

    dstStr = "";

    dstStr += DWCi_SNPrintf<DWCi_String>(3, "%x", CURRENT_MESSAGE_VERSION);    // �o�[�W����
    dstStr += PPW_LOBBY_MESSAGE_HEADER_BLOCK_SEPALATOR;
    dstStr += DWCi_SNPrintf<DWCi_String>(2, "%x", CURRENT_HEADER_BLOCK_NUM);   // �u���b�N�̌�
    dstStr += PPW_LOBBY_MESSAGE_HEADER_BLOCK_SEPALATOR;

    // ������t�H�[�}�b�g
    switch(format)
    {
    case FORMAT_STRING:
        dstStr += "S";
        break;
    case FORMAT_BASE64:
        dstStr += "B";
        break;
    default:
        DWC_ASSERTMSG( FALSE, "PPW_LobbyMessage::Build: Unknown format." );
        return FALSE;;
    }

    dstStr += PPW_LOBBY_MESSAGE_HEADER_BLOCK_SEPALATOR;

    // �Ώ�
    switch(target)
    {
    case PPW_LobbyMessage::TARGET_APPLICATION:
        dstStr += "A";
        break;
    case PPW_LobbyMessage::TARGET_SYSTEM:
        dstStr += "S";
        break;
    default:
        DWC_ASSERTMSG( FALSE, "PPW_LobbyMessage::Build: Unknown target." );
        return FALSE;
    }
    
    dstStr += PPW_LOBBY_MESSAGE_HEADER_BLOCK_SEPALATOR;
    
    // �^�C�v
    dstStr += DWCi_SNPrintf<DWCi_String>(8, "%x", type);   // �u���b�N�̌�
    dstStr += PPW_LOBBY_MESSAGE_HEADER_BLOCK_SEPALATOR;
    
    // �I�v�V����
    dstStr += PPW_LOBBY_MESSAGE_NO_OPTION;    // �I�v�V��������

    dstStr += PPW_LOBBY_MESSAGE_HEADER_BODY_SEPALATOR;


    // �{�f�B���\�z����
    switch(format)
    {
    case FORMAT_STRING:
        dstStr += DWCi_SNPrintf<DWCi_String>(size, "%s", message);
        break;
    case FORMAT_BASE64:
        {
            char buf[PPW_LOBBY_MAX_STRING_SIZE + 1];
            DWCi_Np_CpuClear8(buf, sizeof(buf));
    
            int writtenSize = DWC_Base64Encode(message, size, buf, sizeof(buf));
            if(writtenSize == -1)
            {
                DWC_ASSERTMSG( FALSE, "PPW_LobbyMessage::Build: Internal error. DWC_Base64Encode failed" );
                return FALSE;
            }
            dstStr += buf;
        }
        break;
    default:
        DWC_ASSERTMSG( FALSE, "PPW_LobbyMessage::Build: Unknown format." );
        return FALSE;;
    }
    return TRUE;
}
