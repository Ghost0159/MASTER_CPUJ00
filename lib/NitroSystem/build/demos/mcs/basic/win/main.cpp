/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - mcs - basic - win
  File:     main.cpp

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/

#include "stdafx.h"
#include <nnsys/mcs.h>

static const WORD MCS_CHANNEL0 = 0;
static const WORD MCS_CHANNEL1 = 1;

struct CommColor
{
    BYTE    red;
    BYTE    green;
    BYTE    blue;
    BYTE    dummy;
};

namespace
{

NNSMcsPFOpenStream      pfOpenStream;
NNSMcsPFOpenStreamEx    pfOpenStreamEx;

void
PrintWin32Error(DWORD errNum)
{
    _TCHAR* buffer;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errNum,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // ����̌���
        reinterpret_cast<LPTSTR>(&buffer),
        0,
        NULL);
    fprintf(stderr, "Win32 error [%d] %s\n", errNum, buffer);
    LocalFree(buffer);
}

void
MainLoop()
{
    // �ڑ�
    HANDLE hStream = pfOpenStream(MCS_CHANNEL0, 0);

    if (hStream == INVALID_HANDLE_VALUE)
    {
        // �`�����l���� MCS_CHANNEL1 �ɕς��āA������x�ڑ������݂�
        NNSMcsStreamInfo streamInfo = { sizeof(streamInfo) };
        hStream = pfOpenStreamEx(MCS_CHANNEL1, 0, &streamInfo);

        if (hStream == INVALID_HANDLE_VALUE)
        {
            PrintWin32Error(GetLastError());
            return;
        }

        printf("device type %d\n", streamInfo.deviceType);
    }

    LONG pos = 0;
    LONG val = 1;
    while (true)
    {
        pos += val;

        DWORD cbWritten;
        BOOL bSuccess = WriteFile(
            hStream,
            &pos,                   // �������ރf�[�^�̃o�b�t�@�ւ̃|�C���^
            sizeof(pos),            // �������ރf�[�^�̃o�C�g��
            &cbWritten,             // ���ۂɏ������܂ꂽ�o�C�g��
            NULL);
        if (! bSuccess)
        {
            PrintWin32Error(GetLastError());
            break;
        }

        Sleep(100);

        DWORD totalBytesAvail;
        bSuccess = PeekNamedPipe(
            hStream,
            NULL,
            0,
            NULL,
            &totalBytesAvail,   // ���p�\�ȃo�C�g��
            NULL);
        if (! bSuccess)
        {
            PrintWin32Error(GetLastError());
            break;
        }

        if (totalBytesAvail > 0)
        {
            LONG wkVal;
            DWORD readBytes;
            bSuccess = ReadFile(
                hStream,
                &wkVal,             // �ǂݍ��ރf�[�^���i�[����o�b�t�@�ւ̃|�C���^
                sizeof(wkVal),      // �ǂݍ��ރf�[�^�̃o�C�g��
                &readBytes,         // ���ۂɓǂݍ��񂾃o�C�g��
                NULL);
            if (! bSuccess)
            {
                PrintWin32Error(GetLastError());
                break;
            }

            if (readBytes == sizeof(wkVal))
            {
                val = wkVal;
            }
        }
    }

    // �X�g���[�������
    CloseHandle(hStream);
}

}   // namespace

int
_tmain(int argc, _TCHAR* argv[])
{
    _TCHAR modulePath[MAX_PATH];

    // ���W���[���̃p�X��W�J
    const DWORD writtenChars = ExpandEnvironmentStrings(
        _T("%NITROSYSTEM_ROOT%\\tools\\win\\mcsserver\\nnsmcs.dll"),
        modulePath,
        MAX_PATH);

    if (writtenChars > MAX_PATH)
    {
        fprintf(stderr, "module name too long.\n");
        return 1;
    }

    // ���W���[���̃��[�h
    const HMODULE hModule = LoadLibrary(modulePath);
    if (! hModule)
    {
        fprintf(stderr, "nnsmcs.dll not found.\n");
        return 1;
    }

    pfOpenStream   = reinterpret_cast<NNSMcsPFOpenStream  >(GetProcAddress(hModule, NNS_MCS_API_OPENSTREAM  ));
    pfOpenStreamEx = reinterpret_cast<NNSMcsPFOpenStreamEx>(GetProcAddress(hModule, NNS_MCS_API_OPENSTREAMEX));

    if (pfOpenStream && pfOpenStreamEx)
    {
        MainLoop();
    }

    FreeLibrary(hModule);

    return 0;
}
