/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./build/lobby/include/ppw_profile.h

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
 * @brief �|�P�������r�[���C�u���� �������J�w�b�_
 */

#ifndef PPW_PROFILE_H_
#define PPW_PROFILE_H_

#include "dwci_lobbyData.h"
class PPW_Lobby;

// TODO: �����̏��Ƒ��l�̏���ʃN���X�ɂ���
class PPW_LobbyInnerProfile : public DWCi_Base
{
public:
    // �f�[�^�^��`
    // �V�X�e����`�v���t�B�[��
    struct SystemProfile
    {
        // �����I�ɑ���M����V�X�e���Ǘ����
        // �Q�����Ă���T�u�`�����l��
        PPW_LOBBY_CHANNEL_KIND subChannelKind;
        
        // ���r�[�ɓ���������
        s64 enterTime;
        
        // ���C���`�����l���ɓ��鏀��������������
        BOOL ready;
        
        SystemProfile()
            : subChannelKind(PPW_LOBBY_CHANNEL_KIND_INVALID)
            , enterTime(PPW_LOBBY_INVALID_TIME)
            , ready(FALSE)
        {
            enterTime = DWCi_Np_GetCurrentSecondByUTC();
        }
        
        // ��r���Z�q
        bool operator==(const SystemProfile& rhs) const
        {
            return memcmp(this, &rhs, sizeof(*this)) == 0;
        }
        
        PPW_LobbySystemProfile ToPublicSystemProfile() const
        {
            PPW_LobbySystemProfile profile;
            profile.subChannelKind = subChannelKind;
            profile.enterTime = enterTime;
            return profile;
        }
    };
    
    // ���[�U��`�f�[�^
    struct UserProfile
    {
        typedef std::vector<u8, DWCi_Allocator<u8> > DataType;
        DataType data;
        
        UserProfile()
            : data()
        {}
        
        // �o�C�g�񂩂�̐���
        UserProfile(const u8* _userProfile, u32 _userProfileSize)
        {
            data.assign(_userProfile, _userProfile + _userProfileSize);
        }
        
        // ��r���Z�q
        bool operator==(const UserProfile& rhs) const
        {
            return data.size() == rhs.data.size() && (data.empty() || memcmp(DWCi_GetVectorBuffer(data), DWCi_GetVectorBuffer(rhs.data), data.size()) == 0);
        }
    };
    // �v���t�B�[���̏��
    enum STATE
    {
        STATE_PREPARING,    // ������
        STATE_DATARECVED,   // �f�[�^�擾�ς�
        STATE_READY,        // �f�[�^�擾�ς݂�����������M�ς�
        STATE_ACTIVATED     // �ڑ������R�[���o�b�N�R�[���ς�
    };
private:
    // �����I�ɑ���M����V�X�e����`�v���t�B�[��
    DWCi_ChangeDetectable<SystemProfile> systemProfile;
    
    // �����I�ɑ���M���郆�[�U��`�v���t�B�[��
    DWCi_ChangeDetectable<UserProfile> userProfile;
    
    
    // ���������p�f�[�^
    
    // �v���t�B�[�����o�^��
    STATE state;
    
    // ���j�[�NID
    s32 pid;
    
    // ���������ꂽ���`�F�b�N�p
    BOOL systemProfileInitialized;
    BOOL userProfileInitialized;
    BOOL activated;
public:
    PPW_LobbyInnerProfile()
        : pid(PPW_LOBBY_INVALID_USER_ID){}
    PPW_LobbyInnerProfile(s32 _pid)
        : systemProfile()
        , userProfile()
        , state(STATE_PREPARING)
        , pid(_pid)
        , systemProfileInitialized(FALSE)
        , userProfileInitialized(FALSE)
        , activated(FALSE)
    {
    }
    
    virtual ~PPW_LobbyInnerProfile(){};
    
    // ��r���Z�q
    bool operator<(const PPW_LobbyInnerProfile& rhs) const
    {
        return pid < rhs.pid;
    }
    
    // find_if�p�֐��I�u�W�F�N�g
    struct Equal : public std::unary_function<PPW_LobbyInnerProfile*, bool>
    {
        const s32 pid;
        Equal(s32 _pid)
            : pid(_pid)
        {}
        
        bool operator() (const PPW_LobbyInnerProfile* rhs) const
        {
            return pid == rhs->pid;
        }
    };

    /** 
     * @brief �A�v���P�[�V�����y�уV�X�e���������Z�b�g���܂��B
     * 
     * @param[in] _userProfile ���[�U��`�v���t�B�[���f�[�^�B
     * @param[in] _userProfileSize ���[�U��`�v���t�B�[���̌��B
     * @param[in] force �����I�ɑ��M���邩�B
     */
    void SetUserProfile(const u8* _userProfile, u32 _userProfileSize, BOOL force=FALSE)
    {
        UserProfile newUserProfile(_userProfile, _userProfileSize);
        userProfile.Change(newUserProfile, force);
        userProfileInitialized = TRUE;
    }
    
    /** 
     * @brief �A�v���P�[�V�����������擾���܂��B
     * 
     * @param[in] _userProfile ���[�U��`�v���t�B�[���f�[�^�B
     * @param[in] _userProfileSize ���[�U��`�v���t�B�[���̌��B
     * 
     * @retval TRUE�B
     * @retval FALSE�B
     */
    BOOL GetUserProfile(u8* _userProfile, u32* _userProfileSize) const
    {
        UserProfile::DataType data = userProfile.Get().data;
        u32 orgSize = *_userProfileSize;
        
        // �������T�C�Y��`����
        *_userProfileSize = data.size();
        
        if(*_userProfileSize == 0)
        {
            return TRUE;
        }
        
        if(orgSize < data.size() || _userProfile == NULL)
        {
            return FALSE;
        }
        DWCi_Np_CpuCopy8(DWCi_GetVectorBuffer(data), _userProfile, data.size());
        return TRUE;
    }
    
    /** 
     * @brief ���[�U��`�v���t�B�[����ύX�m�F�����Ɏ擾���܂��B
     * 
     * @retval ���[�U��`�v���t�B�[���B
     */
    const UserProfile& GetUserProfile() const
    {
        return userProfile.Get();
    }
    
    /** 
     * @brief  ���[�U�f�[�^��ύX�m�F�A�擾���܂��B
     * 
     * @retval ���[�U��`�v���t�B�[���B
     */
    const UserProfile& GetUserProfileConfirm() const
    {
        return userProfile.Confirm();
    }
    
    /** 
     * @brief  ���[�U�f�[�^���X�V���ꂽ���𒲂ׂ܂��B
     * 
     * @retval TRUE �X�V����Ă���B
     * @retval FALSE �X�V����Ă��Ȃ��B
     */
    BOOL IsUserProfileUpdated() const
    {
        return userProfile.IsChanged();
    }
    
    /** 
     * @brief  �V�X�e����`�v���t�B�[����S�ăZ�b�g���܂��B
     * 
     * @param _systemProfile �V�X�e����`�v���t�B�[��
     */
    void SetSystemProfile(const SystemProfile& _systemProfile)
    {
        systemProfile = _systemProfile;
        systemProfileInitialized = TRUE;
    }
    
    /** 
     * @brief �V�X�e����`�v���t�B�[����ύX�m�F�����Ɏ擾���܂��B
     * 
     * @retval �V�X�e����`�v���t�B�[���B
     */
    const SystemProfile& GetSystemProfile() const
    {
        return systemProfile.Get();
    }
    
    /** 
     * @brief �V�X�e����`�v���t�B�[����ύX�m�F�A�擾���܂��B
     * 
     * @retval �V�X�e����`�v���t�B�[���B
     */
    const SystemProfile& GetSystemProfileConfirm() const
    {
        return systemProfile.Confirm();
    }
    
    /** 
     * @brief �V�X�e���f�[�^���X�V���ꂽ���𒲂ׂ܂��B
     * 
     * @retval TRUE �X�V����Ă���B
     * @retval FALSE �X�V����Ă��Ȃ��B
     */
    BOOL IsSystemProfileUpdated() const
    {
        return systemProfile.IsChanged();
    }
    
    /** 
     * @brief ���[�U�ڑ��R�[���o�b�N���Ă񂾂��Ƃ��L�^���܂��B
     */
    void Activate()
    {
        activated = TRUE;
    }
    
    /** 
     * @brief ���[�U�ڑ��R�[���o�b�N���R�[���ς݂��𒲂ׂ܂��B
     * 
     * @retval TRUE �Ă񂾁B
     * @retval FALSE �܂��B
     */
    BOOL IsActivated() const
    {
        return activated;
    }
    
    /** 
     * @brief ��Ԃ��擾���܂��B
     * 
     * @retval �X�e�[�g�B
     */
    STATE GetState() const
    {
        return state;
    }
    
    /** 
     * @brief pid���擾���܂��B
     * 
     * @retval pid�B
     */
    s32 GetPid() const
    {
        return pid;
    }
    
    /** 
     * @brief ���[�U�f�[�^�ƃV�X�e���f�[�^�����łɎ�M�ς݂��B
     * 
     * @retval TRUE ��M�ς݁B
     * @retval FALSE �܂��B
     */
    bool IsDataRecved() const
    {
        return state >= STATE_DATARECVED;
    }
    
    void Process();
};

class PPW_LobbyProfileManager : public DWCi_Base
{
    typedef std::set<PPW_LobbyInnerProfile*, DWCi_Base::ptr_less<PPW_LobbyInnerProfile>, DWCi_Allocator<PPW_LobbyInnerProfile*> > PPW_ProfileContainer;
    PPW_ProfileContainer profiles;
    PPW_LobbyInnerProfile myProfile;
public:
    PPW_LobbyProfileManager()
        : profiles()
        , myProfile()
    {}
    virtual ~PPW_LobbyProfileManager()
    {
        ClearProfile();
    }
    
    /** 
     * @brief �w�肵�����[�U��Activate����Ă��邩���ׂ܂��B
     * 
     * @retval TRUE �A�N�e�B�x�[�g�ς݁B
     * @retval FALSE �܂��B
     */
    BOOL IsActivatedProfile(s32 id)
    {
        PPW_LobbyInnerProfile* profile = FindProfile(id);
        return profile && profile->IsActivated();
    }
    
    /** 
     * @brief �����̃v���t�B�[�����擾����B
     * 
     * @retval �����̃v���t�B�[���B
     */
    PPW_LobbyInnerProfile& GetMyProfile()
    {
        return myProfile;
    }
    
    BOOL                    AddProfile(s32 id);
    BOOL                    RemoveProfile(s32 id);
    PPW_LobbyInnerProfile*  FindProfile(s32 id);
    void                    ClearProfile();
    
    BOOL                                    IsFull() const;
    std::vector<s32, DWCi_Allocator<s32> >  EnumId() const;
    void    InitializeMyProfile(s32 id, const u8* userProfile, u32 userProfileSize);
    BOOL    CheckAllProfilesRecved() const;
    void    Process();
};

#endif
