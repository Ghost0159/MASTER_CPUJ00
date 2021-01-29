//==============================================================================
//
//	GDS�p�����[�^��MIN,MAX��`
//
//==============================================================================
#ifndef __GDS_MIN_MAX_H__
#define __GDS_MIN_MAX_H__


//==============================================================================
//	gds_boxshot.h
//==============================================================================
//--------------------------------------------------------------
//	GT_BOX_SHOT�\����
//--------------------------------------------------------------
//GT_BOX_SHOT.monsno�@����
#define GT_BOXSHOT_MONSNO_MIN			(0)
//GT_BOX_SHOT.monsno�@���
#define GT_BOXSHOT_MONSNO_MAX			(493)

//GT_BOX_SHOT.personal_rnd�@����
#define GT_BOXSHOT_PERSONAL_RND_MIN		(0)
//GT_BOX_SHOT.personal_rnd�@���
#define GT_BOXSHOT_PERSONAL_RND_MAX		(0xffffffff)

//GT_BOX_SHOT.id_no�@����
#define GT_BOXSHOT_IDNO_MIN				(0)
//GT_BOX_SHOT.id_no�@���
#define GT_BOXSHOT_IDNO_MAX				(0xffffffff)

//GT_BOX_SHOT.egg_bit�@����
#define GT_BOXSHOT_EGG_BIT_MIN			(0)
//GT_BOX_SHOT.egg_bit�@���
#define GT_BOXSHOT_EGG_BIT_MAX			(0x3fffffff)

//GT_BOX_SHOT.form_no�@����
#define GT_BOXSHOT_FORMNO_MIN			(0)
//GT_BOX_SHOT.form_no�@���
#define GT_BOXSHOT_FORMNO_MAX			(0xff)

//GT_BOX_SHOT.wallPaper�@����
#define GT_BOXSHOT_WALLPAPER_MIN		(0)
//GT_BOX_SHOT.wallPaper�@���
#define GT_BOXSHOT_WALLPAPER_MAX		(31)

//GT_BOX_SHOT.category_no�@����
#define GT_BOXSHOT_CATEGORY_NO_MIN		(0)
//GT_BOX_SHOT.category_no�@���
#define GT_BOXSHOT_CATEGORY_NO_MAX		(5)


//==============================================================================
//	gds_dressup.h
//==============================================================================
//--------------------------------------------------------------
//	GT_GDS_DRESS�\����
//--------------------------------------------------------------
//GT_GDS_DRESS.init_flag�@���K�f�[�^�͕K�����̐����ɂȂ��Ă��܂�
#define GT_DRESS_INIT_FLAG_NUMBER		(0x2345)

//GT_GDS_DRESS.accessory_set_msk�@����
#define GT_DRESS_ACCESSORY_SET_MSK_MIN	(0)
//GT_GDS_DRESS.accessory_set_msk�@���
#define GT_DRESS_ACCESSORY_SET_MSK_MAX	(0x3ff)

//GT_GDS_DRESS.bg_id�@����
#define GT_DRESS_BGID_MIN				(0)
//GT_GDS_DRESS.bg_id�@���
#define GT_DRESS_BGID_MAX				(17)

//GT_GDS_DRESS.country�@����
#define GT_DRESS_COUNTRY_MIN			(0)
//GT_GDS_DRESS.country�@���
#define GT_DRESS_COUNTRY_MAX			(233)

//--------------------------------------------------------------
//	GT_IMC_SAVEDATA_POKEMON�\����
//--------------------------------------------------------------
//GT_IMC_SAVEDATA_POKEMON.personal_rnd�@����
#define GT_IMC_SAVEDATA_POKEMON_PERSONAL_RND_MIN	(0)
//GT_IMC_SAVEDATA_POKEMON.personal_rnd�@���
#define GT_IMC_SAVEDATA_POKEMON_PERSONAL_RND_MAX	(0xffffffff)

//GT_IMC_SAVEDATA_POKEMON.idno�@����
#define GT_IMC_SAVEDATA_POKEMON_IDNO_MIN			(0)
//GT_IMC_SAVEDATA_POKEMON.idno�@���
#define GT_IMC_SAVEDATA_POKEMON_IDNO_MAX			(0xffffffff)

//GT_IMC_SAVEDATA_POKEMON.monsno�@����
#define GT_IMC_SAVEDATA_POKEMON_MONSNO_MIN			(0)
//GT_IMC_SAVEDATA_POKEMON.monsno�@���
#define GT_IMC_SAVEDATA_POKEMON_MONSNO_MAX			(493)

//GT_IMC_SAVEDATA_POKEMON.poke_pri�@����
#define GT_IMC_SAVEDATA_POKEMON_POKE_PRI_MIN		(-128)
//GT_IMC_SAVEDATA_POKEMON.poke_pri�@���
#define GT_IMC_SAVEDATA_POKEMON_POKE_PRI_MAX		(127)

//GT_IMC_SAVEDATA_POKEMON.poke_x�@����
#define GT_IMC_SAVEDATA_POKEMON_POKE_X_MIN			(0)
//GT_IMC_SAVEDATA_POKEMON.poke_x�@���
#define GT_IMC_SAVEDATA_POKEMON_POKE_X_MAX			(255)

//GT_IMC_SAVEDATA_POKEMON.poke_y�@����
#define GT_IMC_SAVEDATA_POKEMON_POKE_Y_MIN			(0)
//GT_IMC_SAVEDATA_POKEMON.poke_y�@���
#define GT_IMC_SAVEDATA_POKEMON_POKE_Y_MAX			(192)

//GT_IMC_SAVEDATA_POKEMON.form_id�@����
#define GT_IMC_SAVEDATA_POKEMON_FORM_ID_MIN			(0)
//GT_IMC_SAVEDATA_POKEMON.form_id�@���
#define GT_IMC_SAVEDATA_POKEMON_FORM_ID_MAX			(0xff)

//GT_IMC_SAVEDATA_POKEMON.oya_sex�@����
#define GT_IMC_SAVEDATA_POKEMON_OYA_SEX_MIN			(0)
//GT_IMC_SAVEDATA_POKEMON.oya_sex�@���
#define GT_IMC_SAVEDATA_POKEMON_OYA_SEX_MAX			(0xff)

//--------------------------------------------------------------
//	GT_IMC_SAVEDATA_ACCESSORIE�\����
//--------------------------------------------------------------
//GT_IMC_SAVEDATA_ACCESSORIE.accessory_no�@����
#define GT_IMC_SAVEDATA_ACCESSORY_NO_MIN			(0)
//GT_IMC_SAVEDATA_ACCESSORIE.accessory_no�@���
#define GT_IMC_SAVEDATA_ACCESSORY_NO_MAX			(99)

//GT_IMC_SAVEDATA_ACCESSORIE.accessory_x�@����
#define GT_IMC_SAVEDATA_ACCESSORY_X_MIN				(0)
//GT_IMC_SAVEDATA_ACCESSORIE.accessory_x�@���
#define GT_IMC_SAVEDATA_ACCESSORY_X_MAX				(255)

//GT_IMC_SAVEDATA_ACCESSORIE.accessory_y�@����
#define GT_IMC_SAVEDATA_ACCESSORY_Y_MIN				(0)
//GT_IMC_SAVEDATA_ACCESSORIE.accessory_y�@���
#define GT_IMC_SAVEDATA_ACCESSORY_Y_MAX				(192)

//GT_IMC_SAVEDATA_ACCESSORIE.accessory_pri�@����
#define GT_IMC_SAVEDATA_ACCESSORY_PRI_MIN			(-128)
//GT_IMC_SAVEDATA_ACCESSORIE.accessory_pri�@���
#define GT_IMC_SAVEDATA_ACCESSORY_PRI_MAX			(127)

//--------------------------------------------------------------
//	GT_PMS_DATA�\����
//--------------------------------------------------------------
//GT_PMS_DATA.sentence_type�@����
#define GT_PMS_DATA_SENTENCE_TYPE_MIN				(0)
//GT_PMS_DATA.sentence_type�@���
#define GT_PMS_DATA_SENTENCE_TYPE_MAX				(4)

//GT_PMS_DATA.sentence_id�@����
#define GT_PMS_DATA_SENTENCE_ID_MIN					(0)
//GT_PMS_DATA.sentence_id�@���
#define GT_PMS_DATA_SENTENCE_ID_MAX					(19)

//GT_PMS_DATA.word�@����
#define GT_PMS_DATA_WORD_MIN						(0)
//GT_PMS_DATA.word�@���
#define GT_PMS_DATA_WORD_MAX						(0xffff)


//==============================================================================
//	gds_ranking.h
//==============================================================================
//--------------------------------------------------------------
//	GT_RANKING_MYDATA�\����
//--------------------------------------------------------------
//GT_RANKING_MYDATA.ranking_type�@����
#define GT_RANKING_MYDATA_RANKING_TYPE_MIN			(1)
//GT_RANKING_MYDATA.ranking_type�@���
#define GT_RANKING_MYDATA_RANKING_TYPE_MAX			(87)

//GT_RANKING_MYDATA.score�@����
#define GT_RANKING_MYDATA_SCORE_MIN					(0)
//GT_RANKING_MYDATA.score�@���
#define GT_RANKING_MYDATA_SCORE_MAX					(0xffffffff)

//--------------------------------------------------------------
//	GT_LAST_WEEK_RANKING_DATA�\����
//--------------------------------------------------------------
//GT_LAST_WEEK_RANKING_DATA.ranking_type�@����
#define GT_LAST_WEEK_RANKING_DATA_RANKING_TYPE_MIN				(1)
//GT_LAST_WEEK_RANKING_DATA.ranking_type�@���
#define GT_LAST_WEEK_RANKING_DATA_RANKING_TYPE_MAX				(87)

//GT_LAST_WEEK_RANKING_DATA.trainer_type_ranking�@����
#define GT_LAST_WEEK_RANKING_DATA_TRAINER_TYPE_RANKING_MIN		(0)
//GT_LAST_WEEK_RANKING_DATA.trainer_type_ranking�@���
#define GT_LAST_WEEK_RANKING_DATA_TRAINER_TYPE_RANKING_MAX		(15)

//GT_LAST_WEEK_RANKING_DATA.trainer_type_score�@����
#define GT_LAST_WEEK_RANKING_DATA_TRAINER_TYPE_SCORE_MIN		(0)
//GT_LAST_WEEK_RANKING_DATA.trainer_type_score�@���
#define GT_LAST_WEEK_RANKING_DATA_TRAINER_TYPE_SCORE_MAX		(0xffffffff)

//GT_LAST_WEEK_RANKING_DATA.birthday_month_ranking�@����
#define GT_LAST_WEEK_RANKING_DATA_BIRTHDAY_MONTH_RANKING_MIN	(1)
//GT_LAST_WEEK_RANKING_DATA.birthday_month_ranking�@���
#define GT_LAST_WEEK_RANKING_DATA_BIRTHDAY_MONTH_RANKING_MAX	(12)

//GT_LAST_WEEK_RANKING_DATA.birthday_month_score�@����
#define GT_LAST_WEEK_RANKING_DATA_BIRTHDAY_MONTH_SCORE_MIN		(0)
//GT_LAST_WEEK_RANKING_DATA.birthday_month_score�@���
#define GT_LAST_WEEK_RANKING_DATA_BIRTHDAY_MONTH_SCORE_MAX		(0xffffffff)

//GT_LAST_WEEK_RANKING_DATA.monsno_ranking�@����
#define GT_LAST_WEEK_RANKING_DATA_MONSNO_RANKING_MIN			(1)
//GT_LAST_WEEK_RANKING_DATA.monsno_ranking�@���
#define GT_LAST_WEEK_RANKING_DATA_MONSNO_RANKING_MAX			(493)

//GT_LAST_WEEK_RANKING_DATA.monsno_score�@����
#define GT_LAST_WEEK_RANKING_DATA_MONSNO_SCORE_MIN				(0)
//GT_LAST_WEEK_RANKING_DATA.monsno_score�@���
#define GT_LAST_WEEK_RANKING_DATA_MONSNO_SCORE_MAX				(0xffffffff)

//--------------------------------------------------------------
//	GT_RANKING_TYPE_RECV�\����
//--------------------------------------------------------------
//GT_RANKING_TYPE_RECV.ranking_type�@����
#define GT_RANKING_TYPE_RECV_RANKING_TYPE_MIN					(0)
//GT_RANKING_TYPE_RECV.ranking_type�@���
#define GT_RANKING_TYPE_RECV_RANKING_TYPE_MAX					(87)

//--------------------------------------------------------------
//	GT_PLAYTIME�\����
//--------------------------------------------------------------
//GT_PLAYTIME.hour�@����
#define GT_PLAYTIME_HOUR_MIN			(0)
//GT_PLAYTIME.hour�@���
#define GT_PLAYTIME_HOUR_MAX			(999)

//GT_PLAYTIME.minute�@����
#define GT_PLAYTIME_MINUTE_MIN			(0)
//GT_PLAYTIME.minute�@���
#define GT_PLAYTIME_MINUTE_MAX			(59)

//GT_PLAYTIME.second�@����
#define GT_PLAYTIME_SECOND_MIN			(0)
//GT_PLAYTIME.second�@���
#define GT_PLAYTIME_SECOND_MAX			(59)

//--------------------------------------------------------------
//	GT_RANKING_PROFILE�\����
//--------------------------------------------------------------
//GT_RANKING_PROFILE.birthday_month�@����
#define GT_RANKING_PROFILE_BIRTHDAY_MONTH_MIN		(1)
//GT_RANKING_PROFILE.birthday_month�@���
#define GT_RANKING_PROFILE_BIRTHDAY_MONTH_MAX		(12)

//GT_RANKING_PROFILE.trainer_view�@����
#define GT_RANKING_PROFILE_TRAINER_VIEW_MIN			(0)
//GT_RANKING_PROFILE.trainer_view�@���
#define GT_RANKING_PROFILE_TRAINER_VIEW_MAX			(15)

//GT_RANKING_PROFILE.monsno�@����
#define GT_RANKING_PROFILE_MONSNO_MIN				(0)
//GT_RANKING_PROFILE.monsno�@���
#define GT_RANKING_PROFILE_MONSNO_MAX				(493)


#endif	//__GDS_MIN_MAX_H__
