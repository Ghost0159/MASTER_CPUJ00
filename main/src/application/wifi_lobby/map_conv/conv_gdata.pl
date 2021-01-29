##################################################
#		
#	wflby_gdata.txt����O���b�h�f�[�^�̃��X�g���쐬����
#
##################################################
@GDATA_FILE = undef;	#�t�@�C���f�[�^
@MAPDATA_FILE = undef;	#�t�@�C���f�[�^
@MAPOBJID	= undef;	#�}�b�v�I�u�W�FID
@EVID		= undef;	#�C�x���gID
%GDATANO	= undef;	#�O���b�h�f�[�^NO�̃n�b�V��
$MAPGRID_X	= undef;	#�}�b�v�̑傫��
$MAPGRID_Y	= undef;	#�}�b�v�̑傫��
@MAPGRID_DT = undef;	#�}�b�v�O���b�h�f�[�^

######�ǂݍ��ރt�@�C��
$INPUT_MAPDATA		= "wflby_map.txt";
$INPUT_GDATA		= "wflby_gdata.txt";
$OUTPUT_MAPH		= "wflby_mapdata.h";
$OUTPUT_GDATANO		= "wflby_map.dat";

######�\���`
$OBJID_YOYAKU		= "NONE";
$EVID_YOYAKU		= "NONE";
$OBJID_DEF			= "WFLBY_MAPOBJID_";
$EVID_DEF			= "WFLBY_MAPEVID_";

#�t�@�C���I�[�v��
&file_open();

#�f�[�^�쐬
$return = &dataid_make();
if( $return == -1 ){
	exit(1);	#err
}

#�}�b�v�f�[�^���쐬����
$return = &map_data_make();
if( $return == -1 ){
	exit(1);	#err
}

#�S�f�[�^�������o��
#�w�b�_�[
&write_header();
#�}�b�v�f�[�^
&write_map();

exit(0);





##################################################
#		
#	�֐�
#
##################################################

#�t�@�C���ǂݍ���
sub file_open{
	open( FILEIN, $INPUT_MAPDATA );
	@MAPDATA_FILE = <FILEIN>;
	close( FILEIN );
	open( FILEIN, $INPUT_GDATA );
	@GDATA_FILE = <FILEIN>;
	close( FILEIN );
}


#GDATA���ォ�猟�����āAMAPOBJID,EVID,GDATANO�𐶐�
sub dataid_make{
	my( $obj_num, $ev_num, $gdatano, $gdata_obj, $gdata_ev, $zyufuku, $hit );

	$MAPOBJID[ 0 ]	= $OBJID_YOYAKU;
	$EVID[ 0 ]		= $EVID_YOYAKU;
	$obj_num = 1;
	$ev_num = 1;
	
	foreach $one ( @GDATA_FILE ){

		$one =~ s/\r\n//g;
			
		if( $one =~ /([A-Z][^,]*),[^,]*,([0-9]),([^,]*)/ ){	#�A���t�@�x�b�g�̂��邬�傤�Ȃ�f�[�^

			#OBJID
			$zyufuku = &double_check( \@MAPOBJID, $1 );
			if( $zyufuku == -1 ){		
				$MAPOBJID[$obj_num] = $1;
				$gdata_obj = $obj_num;
				$obj_num++;
			}else{
				print( "mapobjid  zyuufuku $1\n" );
				return -1;	#�G���[
			}

			#EVID
			if( $3 ne "0" ){
				$zyufuku = &double_check( \@EVID, $3 );
				if( $zyufuku == -1 ){
					$EVID[$ev_num] = $3;
					$gdata_ev = $ev_num;
					$ev_num ++;
				}else{
					$gdata_ev = $zyufuku;
				}
			}else{
				$gdata_ev = 0;	#�Ȃ���Ȃ��Ƃ�
			}

			#HIT BIT
			if( $2 eq "1" ){
				$hit = 1;
			}else{
				$hit = 0;
			}

			#GDATANO
			$gdatano = $hit | ($gdata_ev << 1) | ($gdata_obj << 16);
			$GDATANO{ $MAPOBJID[$gdata_obj] } = $gdatano;
		}
	}
}

#�d���`�F�b�N
sub double_check{
	my( $arry, $check ) = @_;
	my( $count );

	$count  = 0;

	for( $count = 0; $count <(@$arry+0); $count++ ){
		if( $$arry[$count] eq $check ){
			return $count;	#�d��
		}
	}
	return -1;			#�d���Ȃ�
}



#�}�b�v�f�[�^���쐬����
sub map_data_make{
	
	my( @d_line, $line_c, $gdata );
	
	$MAPGRID_Y = 0;
	$MAPGRID_X = 0;


	foreach $one (@MAPDATA_FILE){
		
		if( $one =~ /[A-Z].*/ ){	#�A���t�@�x�b�g�������Ă��OK
			
			#�����̕����𔲂�
			$one =~ s/^[0-9]+,//g;
		
			#�Ō�ɂ��Ă�\n�𔲂�
			$one =~ s/\r\n//g;
			
			@d_line = split( /,/, $one );
			
			if( $MAPGRID_X == 0 ){
				$MAPGRID_X = @d_line;		#�����i�[
			}else{
				if( $MAPGRID_X != @d_line ){
					print( "mapgrid_siz x diff  line".$MAPGRID_Y."\n" );
					return -1;
				}
			}

			for( $line_c=0; $line_c<$MAPGRID_X; $line_c++ ){
				$gdata = &get_gdata( $d_line[ $line_c ] );
				if( $gdata == -1 ){
					print( "mapobj none ".$d_line[ $line_c ]."\n" );
					return -1;
				}
				$MAPGRID_DT[ ($MAPGRID_Y*$MAPGRID_X)+$line_c ] = $gdata;
			}


			$MAPGRID_Y ++;
		}
	}

	return 0;
}

#GDATANO����w���GDATA���擾����
sub get_gdata{

	my( $sarch ) = @_;
	my( $key, $gdata, $retgdata );

	$retgdata = -1;
	
	while( ($key, $gdata) = each( %GDATANO ) ){
#		print( "hash ".$key."=$gdata  sarch=$sarch\n" );
		if( $key eq $sarch ){
			$retgdata = $gdata;
		}
	}
	return $retgdata;
}


#�w�b�_�[�������o��
sub write_header{

	my( $i );
	
	open( FILEOUT, ">$OUTPUT_MAPH" );
	
	print( FILEOUT "#ifndef __WFLBY_MAP_H__\r\n" );
	print( FILEOUT "#define __WFLBY_MAP_H__\r\n" );

	#�}�b�v�f�[�^�̑傫��
	print( FILEOUT "\r\n\r\n\r\n" );
	print( FILEOUT "// �}�b�v�̑傫��\r\n" );
	print( FILEOUT "#define WFLBY_MAPSIZE_X		($MAPGRID_X)\r\n" );
	print( FILEOUT "#define WFLBY_MAPSIZE_Y		($MAPGRID_Y)\r\n" );
	
	#�I�u�W�F�N�gID
	print( FILEOUT "\r\n\r\n\r\n" );
	print( FILEOUT "// �}�b�v�I�u�W�F�N�gID\r\n" );
	for( $i=0; $i<@MAPOBJID; $i++ ){
		print( FILEOUT "#define $OBJID_DEF".$MAPOBJID[$i]."		($i)\r\n" );
	}

	#�C�x���gID
	print( FILEOUT "\r\n\r\n\r\n" );
	print( FILEOUT "// �C�x���gID\r\n" );
	for( $i=0; $i<@EVID; $i++ ){
		print( FILEOUT "#define $EVID_DEF".$EVID[$i]."		($i)\r\n" );
	}

	print( FILEOUT "\r\n\r\n\r\n" );
	print( FILEOUT "#endif //__WFLBY_MAP_H__\r\n" );
	
	close( FILEOUT );
}

#�}�b�v�f�[�^�������o���܂�
sub write_map{

	my( $i );
	
	open( FILEOUT, ">$OUTPUT_GDATANO" );
	binmode( FILEOUT );

	

	for( $i=0; $i<@MAPGRID_DT; $i++ ){
		print( FILEOUT pack("I", $MAPGRID_DT[$i]) );
	}
	
	close( FILEOUT );
}
