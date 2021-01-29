#!perl
#=========================================================================
#
#	�t�@�C����
#	pokemon_pl/convert/eventdata/elfallpickup2.pl
#
#	���s
#	pokemon_pl/src/fielddata/eventdata/makefile
#	elfallpickup2.pl flist all_data.elf
#
#	flist
#	zone_dummy zone_ug zone_c01 ... �S�Ẵ]�[�����o�͂���Ă���
#
#	07.03.14 Satoshi Nohara
#	CW2.0�ŁAall_data.elf�̏o�͌`�����ύX�ɂȂ��Ă����̂őΉ�
#
#=========================================================================

#all_data.elf
$totalname = $ARGV[1];

open(FLIST, $ARGV[0]);
$flist = <FLIST>;
close(FLIST);

#all_data.elf���J��
open(FILE, $totalname);

$size;
$value;
$value2;
@tbl;
$tbl_num = 0;
$time;
@tbl2;
@tbl2_num;

$time = localtime(time);
print "elfallpickup2.pl start ", $time, "\n";

#�K�v�ȃf�[�^�𔲂��o���ăe�[�u���쐬
tbl_make();
tbl_make2();

#FObj,Connect,BGTALK,PosEv�ɕ����ăe�[�u�����쐬
tbl_make3();
tbl_make4();

#-------------------------------------------------------------------------
#
#	�󔒂ŋ�؂�(zone_dummy zone_c01�Ȃǂ����Ԃɏ���)
#
#-------------------------------------------------------------------------
foreach $basename( split(/\s+/, $flist) ){

	#�w�胉�x���̃f�[�^���t�@�C�����甲���o��
    &createbin();

	#zone_c01_total.bin�t�@�C�����J���A�o��
    $outfile = $basename . "_total.bin";
    open(WFILE, "> $outfile");
	binmode(WFILE);

	#������𐔎��Ƃ��ĉ��Z
	#print sprintf("pack(\"C*\", %s);", $data);
    print WFILE eval( sprintf("pack(\"C*\", %s);", $data) );
    close(WFILE);
}
close(FILE);

$time = localtime(time);
print "elfallpickup2.pl end   ", $time, "\n";

#-------------------------------------------------------------------------
#
#	�w�胉�x���̃f�[�^���t�@�C�����甲���o��
#
#	BGTALK_zone_c01_num�Ȃ�
#
#-------------------------------------------------------------------------
sub createbin{

    $data = '';

	#$time = localtime(time);
	#print "1�]�[���J�n����", $time, "\n";

	#evb
	&pickupbin2ex( sprintf("BGTALK_%s_num:", $basename), 2 );
	&pickupbin2ex( sprintf("BGTALK_%s:", $basename), 2 );
	#&pickupbin2( sprintf("BGTALK_%s_num:", $basename) );
	#&pickupbin2( sprintf("BGTALK_%s:", $basename) );

	#evc
    &pickupbin2ex( sprintf("FObj_%s_num:", $basename), 0 );
    &pickupbin2ex( sprintf("FObj_%s:", $basename), 0 );
	#&pickupbin2( sprintf("FObj_%s_num:", $basename) );
	#&pickupbin2( sprintf("FObj_%s:", $basename) );

	#evd
    &pickupbin2ex( sprintf("Connect_%s_num:", $basename), 1 );
    &pickupbin2ex( sprintf("Connect_%s:", $basename), 1 );
	#&pickupbin2( sprintf("Connect_%s_num:", $basename) );
	#&pickupbin2( sprintf("Connect_%s:", $basename) );

	#evp
    &pickupbin2ex( sprintf("PosEv_%s_num:", $basename), 3 );
    &pickupbin2ex( sprintf("PosEv_%s:", $basename), 3 );
	#&pickupbin2( sprintf("PosEv_%s_num:", $basename) );
	#&pickupbin2( sprintf("PosEv_%s:", $basename) );
}

#-------------------------------------------------------------------------
#
#	�K�v�ȃf�[�^�𔲂��o���ăe�[�u���쐬
#
#-------------------------------------------------------------------------
sub tbl_make{

    seek(FILE, 0, 0);

	#FILE = all_data.elf
	while( <FILE> ){

		#�f�[�^�L�q������T��
		#if( $_ =~ /DATA \(\.rodata\)/ ){				#test
		if( $_ =~ /INITIALIZED DATA \(\.rodata\)/ ){
			#$mode = 1;
			return;
		}
	}
}
#-------------------------------------------------------------------------
#
#	�K�v�ȃf�[�^�𔲂��o���ăe�[�u���쐬2
#
#-------------------------------------------------------------------------
sub tbl_make2{

	#FILE = all_data.elf
	while( <FILE> ){

		#���x���łȂ�������T��
		if( $_ =~ /'/ ){
			$tbl[ $tbl_num ] = $_;
			$tbl_num+=1;
		}
	}
}

#-------------------------------------------------------------------------
#
#	�t�@�C������w��̃��x���f�[�^�𔲂��o��
#	���f�[�^�̓O���[�o���ϐ�$data�֒~�ς����
#
#-------------------------------------------------------------------------
sub pickupbin2{

    my $temp = $_[0];							#BGTALK_zone_c01_num
	my @temp3;
	#print "�m�F��\t", $temp, "\n";				#BGTALK_zone_c01_num

    seek(FILE, 0, 0);
	$size = 0;

	#�R�����Ȃ�
	$temp =~ s/\://;

	#FILE = all_data.elf
	while( <FILE> ){

		#�A�h���X�ƃT�C�Y���擾

		@temp3 = split( /\s+/, $_ );

		if( $temp eq $temp3[7] ){				#test

			$value = $temp3[1];

			#10�i���l�ɕϊ�
			$value2 = hex( $value ) & 0xfffffff0;

			#16�i������ɕϊ������l�ɕϊ�
			$value2 = hex( sprintf("%x",    $value2) );

			#�啶���ɂ���
			#$value =~ tr/a-f/A-F/;
			#$value2 =~ tr/a-f/A-F/;

			$size = hex($temp3[2]);

			#print "�f�[�^�̃A�h���X\t", $value, "\n";			#�A�h���X
			#print "�f�[�^�̂��鋫�E��\t", $value2, "\n";		#�A�h���X(����̂����l)
			#print "size\t", $size, "\n";		#�T�C�Y

			search_code();
			return;
		}
    }
}

#-------------------------------------------------------------------------
#
#	�f�[�^������ʒu��T���Ď擾
#
#-------------------------------------------------------------------------
sub search_code{

	my $code;
	my $count = $value2 >> 4;			# / 16

	$code = $tbl[ $count ];
	$count+=1;

	$code =~ ( s/^0x........: // );
	$code =~ s/'.*'//;

	my @temp99 = split( /\s+/, $code );
	my $i = 0;
	my $j = 0;
	my $no = hex( $value ) - $value2;

	#�T�C�Y���擾����
	while( $i < $size ){

		if( ($no + $j) >= 16 ){

			$code = $tbl[ $count ];
			$count+=1;
			@temp99 = split( /\s+/, $code );
			$j = 0;
			$no = 0;
		}

		$data .= "0x";									#��������
		$data .= $temp99[ $no + $j + 1 ];				#��������
		$data .= ",";									#��������

		$i += 1;
		$j += 1;
	}

	return;
}


#=========================================================================
#=========================================================================
#=========================================================================
#=========================================================================

#-------------------------------------------------------------------------
#
#	�K�v�ȃf�[�^�𔲂��o���ăe�[�u���쐬3
#
#-------------------------------------------------------------------------
sub tbl_make3{

    seek(FILE, 0, 0);

	#FILE = all_data.elf
	while( <FILE> ){

		#�f�[�^�L�q������T��
		if( $_ =~ /\(\.symtab\)/ ){
			return;
		}
	}
}

#-------------------------------------------------------------------------
#
#	�K�v�ȃf�[�^�𔲂��o���ăe�[�u���쐬4
#
#-------------------------------------------------------------------------
sub tbl_make4{

	my $pos;

	#FILE = all_data.elf
	while( <FILE> ){

		#�K�v�ȃf�[�^�͂����܂łȂ̂ŏI��
		if( $_ =~ /strtab/ ){
			return;
		}

		#�J�e�S���[�ŕ����Ċi�[
		if( $_ =~ /FObj/ ){
			$pos = 0;
		}elsif( $_ =~ /Connect/ ){
			$pos = 1;
		}elsif( $_ =~ /BGTALK/ ){
			$pos = 2;
		}else{
			$pos = 3;
		}

		$tbl2[$pos][ $tbl2_num[$pos] ] = $_;
		$tbl2_num[$pos]+=1;
	}
}

#-------------------------------------------------------------------------
#
#	�t�@�C������w��̃��x���f�[�^�𔲂��o��
#	���f�[�^�̓O���[�o���ϐ�$data�֒~�ς����
#
#-------------------------------------------------------------------------
sub pickupbin2ex{

	my $i = 0;
    my $temp = $_[0];							#BGTALK_zone_c01_num
    my $num = $_[1];							#FOBJ,CONNECT,BG,POS�ǂꂩ
	my @temp3;

	$size = 0;

	#�R�����Ȃ�
	$temp =~ s/\://;

	while( $i < $tbl2_num[ $num ] ){

		#�A�h���X�ƃT�C�Y���擾

		@temp3 = split( /\s+/, $tbl2[ $num ][ $i ] );
		$i+=1;

		if( $temp eq $temp3[7] ){				#test

			$value = $temp3[1];

			#10�i���l�ɕϊ�
			$value2 = hex( $value ) & 0xfffffff0;

			#16�i������ɕϊ������l�ɕϊ�
			$value2 = hex( sprintf("%x",    $value2) );

			#�啶���ɂ���
			#$value =~ tr/a-f/A-F/;
			#$value2 =~ tr/a-f/A-F/;

			$size = hex($temp3[2]);

			#print "�f�[�^�̃A�h���X\t", $value, "\n";			#�A�h���X
			#print "�f�[�^�̂��鋫�E��\t", $value2, "\n";		#�A�h���X(����̂����l)
			#print "size\t", $size, "\n";		#�T�C�Y

			search_code();
			return;
		}
    }
}


