#!/usr/bin/perl

#�������P�K�v
if( @ARGV != 1 ){
	print( "pl_naix_change.pl need one param\n" );
	exit 1;
}

#�t�@�C���I�[�v��
if( open( FILEIN, $ARGV[0] ) == 0 ){
	print( "can't open ".$ARGV[0]."\n" );
	exit 1;
}

#�f�[�^�R�s�[
@filedata = <FILEIN>;
close( FILEIN );

#�o�̓I�[�v��
if( open( FILEOUT, ">".$ARGV[0] ) == 0 ){
	print( "can't open ".$ARGV[0]."\n" );
	exit 1;
}

foreach $data ( @filedata ){
	#�u�����ďo��	
	$data =~ s/pl_//;
	print( $data );
	print( FILEOUT $data );
};

close( FILEOUT );

exit 0;
