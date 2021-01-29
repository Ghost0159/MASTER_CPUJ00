//==============================================================================
/**
 * @file	gds_rap_response.h
 * @brief	gds_rap.c�����M�f�[�^�̔����擾������Ɨ�����������
 * @author	matsuda
 * @date	2008.01.13(��)
 */
//==============================================================================
#ifndef __GDS_RAP_RESPONSE_H__
#define __GDS_RAP_RESPONSE_H__


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern int GDS_RAP_RESPONSE_DressupShot_Upload(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res);
extern int GDS_RAP_RESPONSE_DressupShot_Download(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res);
extern int GDS_RAP_RESPONSE_Boxshot_Upload(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res);
extern int GDS_RAP_RESPONSE_Boxshot_Download(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res);
extern int GDS_RAP_RESPONSE_RankingType_Download(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res);
extern int GDS_RAP_RESPONSE_RankingUpdate_Download(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res);
extern int GDS_RAP_RESPONSE_BattleVideo_Upload(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res);
extern int GDS_RAP_RESPONSE_BattleVideo_Search_Download(GDS_RAP_WORK *gdsrap, 
	POKE_NET_RESPONSE *res);
extern int GDS_RAP_RESPONSE_BattleVideo_Data_Download(GDS_RAP_WORK *gdsrap, 
	POKE_NET_RESPONSE *res);
extern int GDS_RAP_RESPONSE_BattleVideo_Favorite_Upload(GDS_RAP_WORK *gdsrap, 
	POKE_NET_RESPONSE *res);

//--------------------------------------------------------------
//	��M�o�b�t�@����f�[�^�擾
//--------------------------------------------------------------
extern int GDS_RAP_RESPONSE_DressupShot_Download_RecvPtr_Set(GDS_RAP_WORK *gdsrap, 
	GT_GDS_DRESS_RECV **dress_array, int array_max);
extern int GDS_RAP_RESPONSE_Boxshot_Download_RecvPtr_Set(GDS_RAP_WORK *gdsrap, 
	GT_BOX_SHOT_RECV **box_array, int array_max);
extern void GDS_RAP_RESPONSE_RankingType_Download_RecvPtr_Set(GDS_RAP_WORK *gdsrap, 
	GT_RANKING_TYPE_RECV **ranking_type_recv);
extern void GDS_RAP_RESPONSE_RankingUpdate_Download_RecvPtr_Set(GDS_RAP_WORK *gdsrap, 
	GT_LAST_WEEK_RANKING_ALL_RECV **last_week, GT_THIS_WEEK_RANKING_DATA_ALL_RECV **this_week);
extern u64 GDS_RAP_RESPONSE_BattleVideo_Upload_DataGet(GDS_RAP_WORK *gdsrap);
extern int GDS_RAP_RESPONSE_BattleVideoSearch_Download_RecvPtr_Set(GDS_RAP_WORK *gdsrap, 
	GT_BATTLE_REC_OUTLINE_RECV **outline_array, int array_max);
extern int GDS_RAP_RESPONSE_BattleVideoSearch_Download_DataCopy(GDS_RAP_WORK *gdsrap, 
	GT_BATTLE_REC_OUTLINE_RECV *outline_array, int array_max);
extern int GDS_RAP_RESPONSE_BattleVideoData_Download_RecvPtr_Set(GDS_RAP_WORK *gdsrap, 
	GT_BATTLE_REC_RECV **rec);
extern void GDS_RAP_RESPONSE_BattleVideoFavorite_Upload_RecvPtr_Set(GDS_RAP_WORK *gdsrap);


#endif	//__GDS_RAP_RESPONSE_H__
