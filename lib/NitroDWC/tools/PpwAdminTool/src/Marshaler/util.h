#pragma once

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