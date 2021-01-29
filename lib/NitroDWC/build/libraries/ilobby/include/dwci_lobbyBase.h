/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./build/lobby/include/dwci_base.h

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
 * @brief Wi-Fi ���r�[���C�u���� �A���P�[�^�w�b�_
 */

#ifndef DWCi_LOBBY_BASE_H_
#define DWCi_LOBBY_BASE_H_

#pragma exceptions off
#ifndef NITRO_DEBUG
    #pragma RTTI off
#endif

//#include <new>
namespace std
{
    typedef void (*new_handler)();
}

#include <dwc.h>
#include <functional>

#ifdef max  // std::numeric<T>::max���g���Ȃ��Ȃ�̂�undef
    #undef max
#endif
#ifdef min  // std::numeric<T>::min���g���Ȃ��Ȃ�̂�undef
    #undef min
#endif
#include <limits>



// new, delete, allocator ��`�N���X�B���̃��C�u�����Ŏg��(���I�ɐ�������)�\���́A�N���X�͑S�Ă��̃N���X���p������
// �R���e�i�ɓ����Ƃ��͕K���A���P�[�^�� DWCi_Allocator<T> ���w�肷��B������T�̓R���e�i�ɓ����N���X�B
// ��1: std::vector<DWCi_Channel, DWCi_Allocator<DWCi_Channel> > channel;
// ��2: std::set<DWCi_Channel, std::less<DWCi_Channel>, DWCi_Allocator<DWCi_Channel> > testset;
class DWCi_Base
{
protected:
    DWCi_Base(){}
    // ���̃N���X�̃|�C���^���g�p���Ĕh���N���X��delete���s��Ȃ��̂Ŕ�virtual��protected�ɂ���
    ~DWCi_Base(){}
public:
    static void* operator new(std::size_t size) throw();
    
    // �z�unew
    static void* operator new (std::size_t size, void* p) throw()
    {
        (void)size;
        return p;
    }
    
    static void* operator new[](size_t size) throw()
    {
        return operator new(size);
    }
    
    static void operator delete(void *rawMemory)
    {
        if(rawMemory == NULL)
        {
            return;
        }
        //DWC_Printf(DWC_REPORTFLAG_ERROR, "DWCi_Base::operator delete: deleted at %p\n", rawMemory);
        DWC_Free((DWCAllocType)0, rawMemory, 0);
    }
    
    static void operator delete(void *rawMemory, void* p)
    {
        (void)rawMemory;
        (void)p;
        return;
    }
    
    static void operator delete[](void *rawMemory)
    {
        operator delete(rawMemory);
    }
    
    // �Ǝ���new_handler���`����
private:
    static std::new_handler s_currentNewHandler;
public:
    static std::new_handler set_new_handler(std::new_handler handler) throw()
    {
        std::new_handler oldNewHandler = s_currentNewHandler;
        s_currentNewHandler = handler;
        return oldNewHandler;
    }
    
public:
    // �|�C���^�̎w���Ă������r����֐��I�u�W�F�N�g
    template<class T>
    struct ptr_less : std::binary_function<T*, T*, bool>
    {
        bool operator()(const T* x, const T* y) const
        {
            return *x < *y;
        }
    };
};

// �J�X�^���A���P�[�^
// DWCi_Base��new, delete���g�p����
template <class T>
class DWCi_Allocator
{
public:
    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;

    // �^U�ւ�rebind
    template <class U>
    struct rebind
    {
        typedef DWCi_Allocator<U> other;
    };

    // �l�̃A�h���X���Ƃ�
    pointer address (reference value) const
    {
        return &value;
    }
    const_pointer address (const_reference value) const
    {
        return &value;
    }

    // �R���X�g���N�^�ƃf�X�g���N�^
    // �A���P�[�^�̓X�e�[�g�������Ȃ��̂ŉ������Ȃ�
    DWCi_Allocator() throw()
    {
    }
    DWCi_Allocator(const DWCi_Allocator&) throw()
    {
    }
    template <class U>
    DWCi_Allocator (const DWCi_Allocator<U>&) throw()
    {
    }
    ~DWCi_Allocator() throw()
    {
    }

    // �A���P�[�g�ł���ő吔��Ԃ�
    size_type max_size () const throw()
    {
        return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }

    // �A���P�[�g���邪�������͂��Ȃ�
    pointer allocate (size_type num, const void* = 0)
    {
        // DWCi_Base��new���g��
        pointer ret = (pointer)(DWCi_Base::operator new(num*sizeof(T)));
        return ret;
    }

    // �z�unew�ŏ���������
    void construct (pointer p, const T& value)
    {
        // DWCi_Base���p�����Ă��Ȃ�T�ɑ΂��Ă�new�w�b�_�̔z�unew���g����
        new((void*)p) T(value);
    }

    // �f�X�g���N�^�Ŕj������
    void destroy (pointer p)
    {
        p->~T();
    }

    // �j�����ꂽ�I�u�W�F�N�g�̗̈���������
    void deallocate (pointer p, size_type num)
    {
        (void)num;
        DWCi_Base::operator delete((void*)p);
    }
};

// ���̃A���P�[�^���琶�������^�͑S�ē��������̂Ƃ���
template <class T1, class T2>
bool operator== (const DWCi_Allocator<T1>&,
                 const DWCi_Allocator<T2>&) throw()
{
    return true;
}
template <class T1, class T2>
bool operator!= (const DWCi_Allocator<T1>&,
                 const DWCi_Allocator<T2>&) throw()
{
    return false;
}

#endif
