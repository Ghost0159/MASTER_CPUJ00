#=============================================================================================
#
#	�ϓ��V���b�v�A�C�e���Ǘ��\�R���o�[�^
#
#	06.03.06 Satoshi Nohara
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

	file1 = "shop_item.dat"

	print "//====================================================================" > file1
	print "//						�ϓ��V���b�v�A�C�e���f�[�^" >> file1
	print "//" >> file1
	print "// 06.03.06 Satoshi Nohara" >> file1
	print "//" >> file1
	print "// ��shop_item.xls���R���o�[�g���ďo�͂��Ă��܂�" >> file1
	print "//" >> file1
	print "//====================================================================" >> file1
	print "" >> file1

	print "const SHOP_ITEM_DATA shop_item_data[] = {" >> file1
	print "\t//�A�C�e���i���o�[\t\t�i�K" >> file1
}

NR >= 4{

	#���ڐ��`�F�b�N
	if( NF < 2 ) next
	if( $1 == "�}�b�v��" )	next
	if( $1 ~ "�A�C�e��" )	next

	print "\t{ " ItemName[$1] ",\t\t" $2 " }," >> file1
}

END{
	print "};" >> file1
	print "#define SHOP_ITEM_DATA_MAX\t( NELEMS(shop_item_data) )" >> file1
}




