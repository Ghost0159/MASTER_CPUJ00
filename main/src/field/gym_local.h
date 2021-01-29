//=============================================================================
/**
 * @file	gym_local.h
 * @bfief	�W���d�|���Ƃ�
 * @author	Nozomu Saito
 *
 */
//=============================================================================
#ifndef __GYM_LOCAL_H__
#define __GYM_LOCAL_H__

//���W��
//swtich�p
#define GYM_WATER_LV1	(0)
#define GYM_WATER_LV2	(1)
#define GYM_WATER_LV3	(2)

//�S�[�X�g�W��
//swtich�p
#define GYM_GHOST_LIFT_LV1	(0)
#define GYM_GHOST_LIFT_LV2	(1)

#define GHOST_LIFT_ROOM_START_Z		(22)	//���t�g�̕����ɏ��߂ē������Ƃ��́A���@Z���W

//�|�W��
#define STEEL_LIFT_MAX	(24)

#if 0	//DP
//�i���W��
#define COMBAT_WALL_MAX	(12)
#endif

//�d�C�W��
#define GEAR_ROT_ST_0	(0)			//0��
#define GEAR_ROT_ST_90	(1)			//90��
#define GEAR_ROT_ST_180	(2)			//180��
#define GEAR_ROT_ST_270	(3)			//270��
#define GEAR_ROT_ST_MAX	(4)

#define ELEC_ROOM1_START_Z	(14)
#define ELEC_ROOM2_START_Z	(21)
#define ELEC_ROOM3_START_Z	(25)

#define ELEC_ROOM_MAX	(3)


///////�W�����[�N///////
//���W�����[�N---------------------
typedef struct WATER_GYM_WORK_tag
{
	u8 Water;//���W���̐���
}WATER_GYM_WORK;

//�S�[�X�g�W�����[�N-------------------
typedef struct GHOST_GYM_WORK_tag
{
	u8 Lift;	//���t�g�̈ʒu�i�ォ�����j
}GHOST_GYM_WORK;

//�|�W�����[�N---------------------
typedef struct STEEL_GYM_WORK_tag
{
	int LiftPosBit;	//���t�g�̈ʒu���i�[
}STEEL_GYM_WORK;

#if 0	//DP
//�i���W�����[�N--------------------
typedef struct COMBAT_GYM_WORK_tag
{
	int WallPosBit;	//������̈ʒu���i�[
}COMBAT_GYM_WORK;
#endif

//�d�C�W�����[�N--------------------
typedef struct ELEC_GYM_WORK_tag
{
	int GearRotateCode;	//�M�A��]��
	int RoomNo;		//�����ԍ�0�`2
}ELEC_GYM_WORK;

//PL ���W�����[�N-------------------
typedef struct PL_GRASS_GYM_WORK_tag
{
	u32 time_seqflag;
}PL_GRASS_GYM_WORK;

//PL �i���W�����[�N-----------------
typedef struct PLFIGHT_GYM_WORK_tag
{
	int init_flag;	//�������t���O
}PLFIGHT_GYM_WORK;

//PL �S�[�X�g�W�����[�N
typedef struct PLGHOST_GYM_WORK_tag
{
	s16 init_flag;
	s16 hint_no;
	s16 hint_gx;
	s16 hint_gz;
}PLGHOST_GYM_WORK;

//PL �ʑ�
typedef struct
{
	int dmy;
}FLD_VILLA_WORK;

//PL ��Ԃꂽ������
#define TW_SP_GROUND_BIT (4) //0-15
#define TW_SP_GROUND_MAX (1<<TW_SP_GROUND_BIT)

typedef struct
{
	u32 init_flag:1;			//�������t���O
	u32 fstone_flag:24;			//��ѐΏo���t���O
	u32 sp_ground_no:TW_SP_GROUND_BIT; //���@���݂̓���n�`�ԍ�
	u32 dmy:3;					//4 �r�b�g�]��
	
	u16 cm_ox;
	u16 cm_oy;					//8
	u16 cm_oz;						
	u16 zlink_mflag;			//12
	
	u32 event_flag;				//16
	
	u8 free[16];				//��
}FLD_TORNWORLD_WORK;

#endif	//__GYM_LOCAL_H__
