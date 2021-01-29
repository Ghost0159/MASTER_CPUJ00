#=============================================================================================
#
#	�Œ�V���b�v�A�C�e���Ǘ��\�R���o�[�^
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
	ITEMSYM_H = "../../include/itemtool/itemsym.h"

	#getline = �P�s�ǂݍ���
	#�P�͖߂�l
	while ( 1 == getline < ITEMSYM_H ) {
		if ( NF == 7 && $1 =="#define" ) {
			ItemName[$7] = $2
		}
	}
	close(ITEMSYM_H)

	file1 = "fs_item.dat"
	file2 = "fs_item_def.h"

	#�f�[�^�t�@�C��
	print "//====================================================================" > file1
	print "//						�Œ�V���b�v�A�C�e���f�[�^" >> file1
	print "//" >> file1
	print "// 06.05.13 Satoshi Nohara" >> file1
	print "//" >> file1
	print "// ��fs_item.xls���R���o�[�g���ďo�͂��Ă��܂�" >> file1
	print "//" >> file1
	print "//====================================================================" >> file1
	print "" >> file1
	print "typedef u16 FS_ITEM_DATA;" >> file1
	print "" >> file1

	#��`�t�@�C��
	print "//====================================================================" > file2
	print "//					�Œ�V���b�v�A�C�e���Ăяo���i���o�[" >> file2
	print "//" >> file2
	print "// 06.05.13 Satoshi Nohara" >> file2
	print "//" >> file2
	print "// ��fs_item.xls���R���o�[�g���ďo�͂��Ă��܂�" >> file2
	print "//" >> file2
	print "//====================================================================" >> file2
	print "#ifndef FS_ITEM_DEF_H" >> file2
	print "#define FS_ITEM_DEF_H" >> file2
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
	print "const FS_ITEM_DATA fs_item_" $2 "[] = {" >> file1

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
	print "#define FS_ITEM_" tmp "_MAX\t( NELEMS(fs_item_" $2 ") )\n" >> file1
	print "#define ID_SHOP_ITEM_" tmp "\t(" count ")" >> file2
	count++
}

END{
	#�f�[�^�t�@�C��
	print "//�f�[�^�e�[�u��" >> file1
	print "const FS_ITEM_DATA* fs_item_data[] = {" >> file1

	for( i=0; i < count ;i++ ){
		label[i]
		print "\tfs_item_" label[i] "," >> file1
	}
	print "};" >> file1

	print "#define FS_ITEM_DATA_MAX\t( NELEMS(fs_item_data) )\n" >> file1

	#��`�t�@�C��
	print "\n#endif /* FS_ITEM_DEF_H */" >> file2
}


