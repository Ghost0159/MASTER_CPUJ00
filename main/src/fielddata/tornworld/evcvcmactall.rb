##################################################################
#	evcvcmactall.rb
#	��Ԃꂽ�������@�C�x���g�f�[�^�A
#	�J�����A�N�V�����f�[�^�t�@�C���S�ẴR���o�[�g�ƈꗗ����
#	evflist.rb ��������ꗗ�t�@�C����
#	�O�� "exec" "system"
##################################################################

#���C�u����
require "evcmactcv.rb"	#�f�[�^�R���o�[�g

#�萔

#=================================================================
#	�C�x���g�W�����v�t�@�C���R���o�[�g
#	list �������ݐ�t�@�C��
#	TW_EVDATALIST_CMACT
#=================================================================
def ev_make_cmactdata( list )
	ret = 0
	count = 1
	convert = "../../../convert/exceltool/excelcommaconv"
	dir = Dir.open( Dir.pwd + "/tw_cmact/" )			# tw_event path
	winpath = dir.path								
	winpath = winpath.sub( /\/cygdrive\/c\//, "c:/" )	# win path
	
	list.printf( "TW_EVDATA_CMACTLIST =" )				# TW_EVDATA_CMACTLIST
	
	dir.each{ |name|									# xls conv write list
		if( name.include?("\.xls") )
			binname = name.sub( /\.xls/, "\.bin" )
			txtname = name.sub( /\.xls/, "\.txt" )
			list.printf( " \\\n" )
			list.printf( "\t%s", winpath+binname )		# �t���p�X
			
			system( convert, winpath+name )				# xls -> txt
			
			ret = evcmactcv_txtfileconv( winpath+txtname )	# �o�C�i���ϊ�
			if( ret == 1 )
				break
			end
			
			count = count + 1
		end
	}
	
	#�f�B���N�g�������
	dir.close
	
	#�t�@�C�����o��
	printf( "tw cmactfile count %d\n", count-1 )
	
	return ret
end

#=================================================================
#	�C�x���g�t�@�C���R���o�[�g
#=================================================================
path = ARGV[0]
if( path == @nil )
	printf( "ERROR evcvcmactall.rb \"ListFileName\"\n" )
	exit 1
end

list = File.open( path, "w" )

ret = ev_make_cmactdata( list )

list.close

if( ret == 1 )			#�G���[
	File.delete( path )
end
