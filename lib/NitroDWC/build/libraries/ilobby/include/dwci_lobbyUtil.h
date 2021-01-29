/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./build/lobby/include/dwci_lobbyUtil.h

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
 * @brief Wi-Fi ���r�[���C�u���� ���[�e�B���e�B�w�b�_
 */

#ifndef DWCi_LOBBY_UTIL_H_
#define DWCi_LOBBY_UTIL_H_

#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <string.h>
#include "dwci_lobbyNonport.h"
#include "dwci_lobbyBase.h"

#define DWC_SAFE_FREE(ptr) if(ptr){ DWC_Free((DWCAllocType)0, ptr, 0); ptr=NULL; }
#define DWC_SAFE_DELETE(ptr) if(ptr){ delete ptr; ptr = NULL; }

// ���C�u���������Ŏg�p����A���P�[�^
template <class T> class DWCi_Allocator;

// ���C�u���������Ŏg�p���镶����
typedef std::basic_string<char, std::char_traits<char>, DWCi_Allocator<char> > DWCi_String;

// �ÓI�A�T�[�g
// �^������false�ɂȂ�ƃR���p�C���G���[�ɂȂ�
template<bool>
class DWCi_StaticAssert;
template<>
class DWCi_StaticAssert<true>{};
#define DWCi_STATIC_ASSERT(exp) DWCi_StaticAssert<(exp)>()


// vector�̃f�[�^�o�b�t�@���擾����Bvector::data()
template <class T, class AllocatorT>
T* DWCi_GetVectorBuffer(const std::vector<T, AllocatorT>& vec)
{
    if(vec.empty())
    {
        return NULL;
    }
    return const_cast<T*>(&vec[0]);
}

// DWCi_String��啶���������𖳎����Ĕ�r����
template <class T>
s32 DWCi_Stricmp(T lhs, T rhs)
{
    // �֐��^�̒�`�B(char (*)(char))�ȂǓ����B
    typedef typename T::value_type (*ValueTypeToLower)(typename T::value_type);
    
    // �������ɕϊ����Ă����r
    // std::tolower�̓V�O�l�`�����L���X�g���Ȃ���warning���o��B
    std::transform(lhs.begin(), lhs.end(), rhs.begin(), (ValueTypeToLower)std::tolower);
    std::transform(rhs.begin(), rhs.end(), rhs.begin(), (ValueTypeToLower)std::tolower);
    return lhs.compare(rhs);
}

// DWCi_String��啶���������𖳎������������w�肵�Ĕ�r����
template <class T>
s32 DWCi_Strnicmp(T lhs, T rhs, std::size_t n)
{
    // �֐��^�̒�`�B(char (*)(char))�ȂǓ����B
    typedef typename T::value_type (*ValueTypeToLower)(typename T::value_type);
    
    // �������ɕϊ����Ă����r
    std::transform(lhs.begin(), lhs.end(), lhs.begin(), (ValueTypeToLower)std::tolower);
    std::transform(rhs.begin(), rhs.end(), rhs.begin(), (ValueTypeToLower)std::tolower);
    return lhs.compare(0, n, rhs);
}

inline s32 DWCi_StrnicmpChar(const char* lhs, const char* rhs, std::size_t n)
{
    return DWCi_Strnicmp(DWCi_String(lhs), DWCi_String(rhs), n);
}

// DWCi_String�Ȃǂ�char������p��sprintf
// T��std::string�݊��̌^
template <typename T>
T DWCi_SPrintf(const char* format, ...)
{
    va_list           arguments;
    std::vector<char, typename T::allocator_type > buffer;

    DWC_ASSERTMSG(format, "format is NULL");
    va_start(arguments, format);

    int length = DWCi_Np_VSNPrintf(NULL, 0, format, arguments) + 1;
    DWC_ASSERTMSG(length > 0, format);
    buffer.resize((std::size_t)length);
    int result = DWCi_Np_VSNPrintf(DWCi_GetVectorBuffer(buffer), (std::size_t)length, format, arguments);
    DWC_ASSERTMSG(result > 0, format);
    buffer[(std::size_t)length-1]    = '\0';    // �ی�

    va_end(arguments);
    return T(DWCi_GetVectorBuffer(buffer));
}

// DWCi_String�Ȃǂ�char������p��snprintf
// size�ɂ�snprintf���l�o�b�t�@�̃T�C�Y��^����Bsize-1�Ԗڂɂ͕K��'\0'���������܂��B
template <class T>
T DWCi_SNPrintf(std::size_t size, const char* format, ...)
{
    va_list           arguments;
    std::vector<char, typename T::allocator_type> buffer(size);

    DWC_ASSERTMSG(format, "format is NULL");
    va_start(arguments, format);
    DWCi_Np_VSNPrintf(DWCi_GetVectorBuffer(buffer), size, format, arguments);
    va_end(arguments);
    return T(DWCi_GetVectorBuffer(buffer));
}

/**
 * split�֐�
 *
 * @param inStr ����������������
 * @param delims �f���~�^(�����̔z��)
 * @param limit ��������ő吔�B�擪����limit��������B1�Ȃ番�����ꂸ���̂܂܂�string������B0�ȉ��܂��͏ȗ�����ƑS�ĕ�������B
 * 
 * @retval �������ꂽ������
*/
template <class T>
std::vector<T, DWCi_Allocator<T> > DWCi_SplitByChars(const T& inStr, const T& delims, std::size_t limit=0)
{
    std::vector<T, DWCi_Allocator<T> > result;
    std::size_t cutAt;
    std::size_t splitCount = 1;
    T str(inStr);
    
    while( splitCount != limit && (cutAt = str.find_first_of(delims)) != str.npos )
    {
        if(cutAt > 0)
        {
            result.push_back(str.substr(0, cutAt));
            splitCount++;
        }
        str = str.substr(cutAt + 1);
    }
    if(str.length() > 0)
    {
        result.push_back(str);
    }
    return result;
}

/**
 * split�֐�
 *
 * @param inStr ����������������
 * @param delim �f���~�^(������)
 * @param limit ��������ő吔�B�擪����limit��������B1�Ȃ番�����ꂸ���̂܂܂�string������B0�ȉ��܂��͏ȗ�����ƑS�ĕ�������B
 * 
 * @retval �������ꂽ������
*/
template <class T>
std::vector<T, DWCi_Allocator<T> > DWCi_SplitByStr(const T& inStr, const T& delim, std::size_t limit=0)
{
    std::vector<T, DWCi_Allocator<T> > result;
    std::size_t cutAt;
    std::size_t splitCount = 1;
    T str(inStr);
    
    while( splitCount != limit && (cutAt = str.find(delim)) != str.npos )
    {
        if(cutAt > 0)
        {
            result.push_back(str.substr(0, cutAt));
            splitCount++;
        }
        str = str.substr(cutAt + delim.length());
    }
    if(str.length() > 0)
    {
        result.push_back(str);
    }
    return result;
}

// Base64�f�R�[�h
template <class charT, class AllocatorT>  // charT�ɂ�1byte�̌^���w�肵�Ă��������B
BOOL DWCi_Base64Decode(const char* value, std::vector<charT, AllocatorT >& buf)
{
    DWCi_STATIC_ASSERT(sizeof(charT) == 1);
    
    u32 size = strlen(value);
    buf.resize(size);
    if(size == 0)
    {
        // �T�C�Y0�̕����񂪗^����ꂽ�Ƃ��̓T�C�Y0��vector��Ԃ�
        return TRUE;
    }
    int retSize = DWC_Base64Decode(value, strlen(value), (char*)DWCi_GetVectorBuffer(buf), buf.size());
    if(retSize == -1)
    {
        return FALSE;
    }
    buf.resize((std::size_t)retSize);
    return TRUE;
}

// �ύX�����m����N���X
// �l���ύX���ꂽ�̂����m���AConfirm()���Ă΂ꂽ�Ƃ��Ɋm�F�������̂Ƃ���
// T�͒l���ύX���ꂽ�����`�F�b�N���邽�߂�==���Z�q���I�[�o�[���[�h����K�v������B
template <class T>
class DWCi_ChangeDetectable : public DWCi_Base
{
    T value;
    mutable BOOL changed;
    
public:
    typedef T value_type;
    
    DWCi_ChangeDetectable()
        : value()
        , changed(TRUE)
    {}
    
    // value_type����̃R���X�g���N�^
    DWCi_ChangeDetectable(const T& _value)
        : value(_value)
        , changed(TRUE)
    {}
    
    // �R�s�[�R���X�g���N�^
    DWCi_ChangeDetectable(const DWCi_ChangeDetectable& src)
    {
        *this = src;
    }
    
    // ���
    DWCi_ChangeDetectable& operator=(const DWCi_ChangeDetectable& rhs)
    {
        if(!(value == rhs.value))
        {
            value = rhs.value;
            changed = TRUE;
        }
        return *this;
    }
    
    // value_type����̑��
    DWCi_ChangeDetectable& operator=(const T& _value)
    {
        return *this = DWCi_ChangeDetectable<T>(_value);
    }
    
    // ����Ɠ�������force��TRUE�̂Ƃ��͋����I�ɕύX����
    const DWCi_ChangeDetectable& Change(const DWCi_ChangeDetectable& src, BOOL force)
    {
        if(force)
        {
            changed = TRUE;
        }
        return *this = src;
    }
    
    // value_type�����Change
    const DWCi_ChangeDetectable& Change(const T& _value, BOOL force)
    {
        return this->Change(DWCi_ChangeDetectable<T>(_value), force);
    }
    
    // value_type�̉��Z�q���G�~�����[�g
    bool operator==(const T& _value) const
    {
        return value == _value;
    }
    bool operator<(const T& _value) const
    {
        return value < _value;
    }
    
    // �ύX���ꂽ�����ׂ�
    BOOL IsChanged() const
    {
        return changed;
    }
    
    // �ύX���m�F���Ď擾����
    const T& Confirm() const
    {
        changed = FALSE;
        return value;
    }
    
    // �l���擾����
    const T& Get() const
    {
        return value;
    }
};

#endif
