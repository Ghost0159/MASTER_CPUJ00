#=============================================================================================
#
#	�Œ�O�b�Y�A�C�e���Ǘ��\�R���o�[�^
#
#	06.05.13 Satoshi Nohara
#
#=============================================================================================
BEGIN{

	#=================================================================
	#
	#�A�C�e����`�t�@�C���ǂݍ���
	#
	#=================================================================
	ITEMSYM_H = "../../src/fielddata/base_goods/goods_id_def.h"

	#getline = �P�s�ǂݍ���
	#�P�͖߂�l
	while ( 1 == getline < ITEMSYM_H ) {
		if ( NF == 5 && $1 =="#define" ) {
			ItemName[$5] = $2
		}
	}
	close(ITEMSYM_H)

	file1 = "fs_goods.dat"
	file2 = "fs_goods_def.h"

	#�f�[�^�t�@�C��
	print "//====================================================================" > file1
	print "//						�Œ�O�b�Y�A�C�e���f�[�^" >> file1
	print "//" >> file1
	print "// 06.05.13 Satoshi Nohara" >> file1
	print "//" >> file1
	print "// ��fs_goods.xls���R���o�[�g���ďo�͂��Ă��܂�" >> file1
	print "//" >> file1
	print "//====================================================================" >> file1
	print "" >> file1
	print "typedef u16 FS_GOODS_DATA;" >> file1
	print "" >> file1

	#��`�t�@�C��
	print "//====================================================================" > file2
	print "//					�Œ�O�b�Y�A�C�e���Ăяo���i���o�[" >> file2
	print "//" >> file2
	print "// 06.05.13 Satoshi Nohara" >> file2
	print "//" >> file2
	print "// ��fs_goods.xls���R���o�[�g���ďo�͂��Ă��܂�" >> file2
	print "//" >> file2
	print "//====================================================================" >> file2
	print "#ifndef FS_GOODS_DEF_H" >> file2
	print "#define FS_GOODS_DEF_H" >> file2
	print "" >> file2

	label[256]	#�]�[��ID�i�[
	count = 0	#���������f�[�^��
}

NR >= 4{

	#���ڐ��`�F�b�N
	if( NF < 3 ) next
	if( $1 == "�}�b�v��" )	next
	if( $1 ~ "�A�C�e��" )	next

	#�����ݒ�
	data = $0
	max = NF
	label[count] = $2
	#print max
	print "//"$1 >> file1
	print "const FS_GOODS_DATA fs_goods_" $2 "[] = {" >> file1

	#����Ȃ��f�[�^���폜
	sub( /^[^\t]*/, "", data )		#"\t"�ƈ�v����܂ō폜
	sub( /[\t]/, "", data )			#"\t"�ƈ�v������폜
	sub( /^[^\t]*/, "", data )		#"\t"�ƈ�v����܂ō폜
	sub( /[\t]/, "", data )			#"\t"�ƈ�v������폜

	for( i=2; i < max ;i++ ){

		#���̃A�C�e���f�[�^�𔲂��o��
		tmp = data 
		sub( /\t.*$/, "", tmp )	#"\t"�ȍ~���폜
		print "\t" ItemName[tmp] "," >> file1
		#printf( "%s, ", ItemName[tmp] ) >> file1
		#print tmp
		#print ItemName[tmp]

		#�A�C�e���f�[�^��i�߂�
		sub( /^[^\t]*/, "", data )	#"\t"�ƈ�v����܂ō폜
		sub( /[\t]/, "", data )		#"\t"�ƈ�v������폜
	}
	print "\t0xffff," >> file1
	print "};" >> file1
	tmp = toupper( $2 )
	print "#define FS_GOODS_" tmp "_MAX\t( NELEMS(fs_goods_" $2 ") )\n" >> file1
	print "#define ID_SHOP_GOODS_" tmp "\t(" count ")" >> file2
	count++
}

END{
	#�f�[�^�t�@�C��
	print "//�f�[�^�e�[�u��" >> file1
	print "const FS_GOODS_DATA* fs_goods_data[] = {" >> file1

	for( i=0; i < count ;i++ ){
		label[i]
		print "\tfs_goods_" label[i] "," >> file1
	}
	print "};" >> file1

	print "#define FS_GOODS_DATA_MAX\t( NELEMS(fs_goods_data) )\n" >> file1

	#��`�t�@�C��
	print "\n#endif /* FS_GOODS_DEF_H */" >> file2
}


