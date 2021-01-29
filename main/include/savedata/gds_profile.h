//==============================================================================
/**
 * @file	gds_profile.h
 * @brief	GDS�v���t�B�[���̃w�b�_
 * @author	matsuda
 * @date	2007.07.24(��)
 */
//==============================================================================
#ifndef __PROFILE_H__
#define __PROFILE_H__

//==============================================================================
//	�^��`
//==============================================================================
///GDS�v���t�B�[���̕s��`�|�C���^
typedef struct _GDS_PROFILE * GDS_PROFILE_PTR;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern GDS_PROFILE_PTR GDS_Profile_AllocMemory(int heap_id);
extern void GDS_Profile_FreeMemory(GDS_PROFILE_PTR gpp);
extern void GDS_Profile_MyDataSet(GDS_PROFILE_PTR gpp, SAVEDATA *sv);
extern STRBUF * GDS_Profile_CreateNameString(const GDS_PROFILE_PTR gpp, int heapID);
extern u32 GDS_Profile_GetID(const GDS_PROFILE_PTR gpp);
extern u32 GDS_Profile_GetSex(const GDS_PROFILE_PTR gpp);
extern int GDS_Profile_GetMonsNo(const GDS_PROFILE_PTR gpp);
extern int GDS_Profile_GetFormNo(const GDS_PROFILE_PTR gpp);
extern int GDS_Profile_GetNation(const GDS_PROFILE_PTR gpp);
extern int GDS_Profile_GetArea(const GDS_PROFILE_PTR gpp);
extern int GDS_Profile_GetMonthBirthday(const GDS_PROFILE_PTR gpp);
extern int GDS_Profile_GetTrainerView(const GDS_PROFILE_PTR gpp);
extern int GDS_Profile_GetRomCode(const GDS_PROFILE_PTR gpp);
extern int GDS_Profile_GetEggFlag(const GDS_PROFILE_PTR gpp);
extern int GDS_Profile_GetLanguage(const GDS_PROFILE_PTR gpp);
extern STRBUF * GDS_Profile_GetSelfIntroduction(const GDS_PROFILE_PTR gpp, PMS_DATA *pms_dest, int heap_id);


#endif	//__PROFILE_H__
