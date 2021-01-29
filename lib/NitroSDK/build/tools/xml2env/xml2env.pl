#! perl
#---------------------------------------------------------------------------
#  Project:  NitroSDK - xml2env
#  File:     xml2env.pl
#
#  Copyright 2005 Nintendo.  All rights reserved.
#
#  These coded instructions, statements, and computer programs contain
#  proprietary information of Nintendo of America Inc. and/or Nintendo
#  Company Ltd., and are protected by Federal copyright law.  They may
#  not be disclosed to third parties or copied or duplicated in any form,
#  in whole or in part, without the prior written consent of Nintendo.
#
#  $Log: xml2env.pl,v $
#  Revision 1.6  2005/10/27 09:14:50  okubata_ryoma
#  ���C��
#
#  Revision 1.5  2005/10/27 08:21:25  okubata_ryoma
#  ���C��
#
#  Revision 1.4  2005/10/27 07:53:19  okubata_ryoma
#  xml:parser���g�p
#
#  ������
#
#  $NoKeywords: $
#---------------------------------------------------------------------------


use strict;

# XML::Parser���g�p�����ׂ�
if (eval "require XML::Parser; return 1;" != 1){
    printf "cygwin �̃Z�b�g�A�b�v��Perl�̍ŐV�ł�expat���C�u���������āAXML::Parser���g�p�ł�����ɂ��Ă��������x���s���Ă݂Ă��������B\n";
    exit;
}

require Getopt::Std;
require XML::Parser;

# �n���h�����[�`���ւ̎Q�Ƃ��p�����[�^�Ɏw�肵�āA�p�[�T������������
my $parser = XML::Parser -> new(Handlers => {
        Init =>    \&handle_doc_start,
        Final =>   \&handle_doc_end,
        Start =>   \&handle_elem_start,
        End =>     \&handle_elem_end,
        Char =>    \&handle_char_data,
});
   
my (%opts);
my $c = Getopt::Std::getopts('o:h', \%opts);

my ($src, $tmp, $line, $resource_name, $resource_count);

# ���̃I�v�V���������Ȃ������Ƃ��̏o�̓t�@�C����
$tmp = $src . '.result.c';


# ���Ńw���v
if(exists $opts{h} || scalar(@ARGV) < 1){
    print "�R�}���h�̌�Ɉ������P��(���������� .xml �t�@�C��)����͂��Ă��������B\n";
    print "���͗�) perl xml2env.pl d:/test/main.xml \n";
    print "��̗�̓t�@�C���� d:/test/main.xml �ɂ���ꍇ�̗�ł��B\n";
    print "������\"-o\"�����A���̌�Ƀt�@�C��������͂���Ƃ��̃t�@�C���ɏo�͂���܂��B\n";
    print "������\"-h\"������ƁA�ȒP�ȑ���������\������܂��B\n";
    die "$!";
}



# �I�v�V����-o �̂��Ƃɏo�̓t�@�C���̖��O������
if(exists $opts{o}){
    $tmp = $opts{o};
}

# �I�v�V�����Ȃ��ŎQ�Ƃ������t�@�C����������
if(@ARGV){
    $src = $ARGV[$resource_count];
}

open(FO, ">" . $tmp) or die ("cannot open $tmp to write.");

my %default_type = (
 "u8"         => "ENV_U8",
 "s8"         => "ENV_S8",
 "u16"        => "ENV_U16",
 "s16"        => "ENV_S16",
 "u32"        => "ENV_U32",
 "s32"        => "ENV_S32",
 "u64"        => "ENV_U64",
 "s64"        => "ENV_S64",
 "bool"       => "ENV_BOOL",
 "string"     => "ENV_STRING",
 "hexbinary"  => "ENV_BINARY"
);

$line = "/*---------------------------------------------------------------------------*/\n";
$line .= "#include <nitro.h>\n";
$line .= "//================================================================================\n";
$line .= "\n";
print   FO  $line;


# �f�[�^��ǂݍ��݃p�[�T�ŏ�������
my $file = $src;
if($file){
    $parser->parsefile($file);
}
else{
    my $input = "";
    while(<STDIN>){ $input .= $_; }
    $parser->parse($input);
}

close(FO);
exit;

my ($record, $context, $resource_names, $datafile_name);

# �n���h��

# �������n�܂�Ɠ����ɁA�t�@�C���̐擪���o�͂���B
sub handle_doc_start{
    print "";
}
my ($class_name, $class_context);
# �p�[�T���V�����v�f�̊J�n�ɏo����тɌĂ΂��B
sub handle_elem_start{
    my($expat, $name, %atts) = @_;
    $context = $name;
    
    if($name =~ /resource/i){
        $resource_names .= " $atts{'name'},";
        print "ENVResource $atts{'name'}\[\] = {\n";
        print   FO  "ENVResource $atts{'name'}\[\] = {\n";
    }
    elsif($name =~ /class/i){
        $class_name = $atts{'name'};
    }
    else{
        # ���[�v��default_type�ɓ��Ă͂܂郂�m��T���ď���
        foreach my $i(keys %default_type){
            if($name =~ /$i/i){
                $class_context = "\"$class_name\.$atts{'name'}\",\t". "$default_type{$name}";
                if($name =~ /hexbinary/i){
                    if(exists $atts{'src'}){
                        $datafile_name = $atts{'src'};
                        # �t�@�C���ǂݍ��ݗp�̑����l�̏�����
                        $atts{'src'} = undef;
                    }
                }
            }
        }
    }
}

# �v�f���̕����f�[�^���ŐV�v�f�̃o�b�t�@�Ɏ��[����B
sub handle_char_data{
    my($expat, $text) = @_;
    
    $record->{$context} .= $text;
    
}
my ($line, $filedata, $char);
# �p�[�T���V�����v�f�̏I���ɏo����тɌĂ΂��B
sub handle_elem_end{
    my($expat, $name) = @_;
    if($name =~ /resource/i){
        print "    ENV_RESOUCE_END\n};\n\n";
        print   FO  "    ENV_RESOUCE_END\n};\n\n";
    }
    # �v�f����class�łȂ��Ƃ�
    elsif($name ne 'class'){
        if($name =~ /hexbinary/i){
            # �v�f����hexbinary�ł��A�t�@�C����ǂݍ��ޕK�v������Ƃ�
            if($datafile_name){
                open(FP, "<" . $datafile_name) or die ("cannot open $datafile_name to read.");
                while(!eof(FP)) {
                    $char = getc FP;
                    # �������A�X�L�[�R�[�h�ɕϊ�
                    $char = unpack("C", $char);
                    # �A�X�L�[�R�[�h��16�i�ɕϊ�
                    $char = sprintf("%x", $char);
                    $filedata .= '\x' . $char;
                }
                print "    $class_context( \"$filedata\" ),\n";
                print   FO  "    $class_context( \"$filedata\" ),\n";
                # �t�@�C���ǂݍ��݃t���O�̏�����
                $datafile_name = undef;
                $filedata = "";
            }
            else{
                my @ascii;
                my $hexdata;
                # �������A�X�L�[�R�[�h�ɕϊ�
                @ascii = unpack("C*", $record->{$context});
                foreach my $i(@ascii){
                    # �A�X�L�[�R�[�h��16�i�ɕϊ�
                    $hexdata .= '\x' . sprintf("%x", $i);
                }
                print "    $class_context( \"$hexdata\" ),\n";
                print   FO  "    $class_context( \"$hexdata\" ),\n";
                # ����e�L�X�g������������B
                $record->{$context} = "";
            }
        }
        elsif($name =~ /string/i){
            $record->{$context} =~ s/(["\\])/\\$1/g;
            $record->{$context} =~ s/[\n]/\\n/g;
            $record->{$context} =~ s/[\t]/\\t/g;
            print "    $class_context( \"$record->{$context}\" ),\n";
            print   FO  "    $class_context( \"$record->{$context}\" ),\n";
            # ����e�L�X�g������������B
            $record->{$context} = "";
        }
        else{
            # ���[�v��default_type�ɓ��Ă͂܂郂�m��T���ď���
            foreach my $i(keys %default_type){
                if($name =~ /$i/i){
                    print "    $class_context( $record->{$context} ),\n";
                    print   FO  "    $class_context( $record->{$context} ),\n";
                    # ����e�L�X�g������������B
                    $record->{$context} = "";
                }
            }
        }
        # �e�L�X�g�̏�����
        $class_context = "";
        $context = "";
    }
}

# �Ō�Ƀt�@�C���̏I�����o�͂���B
sub handle_doc_end{
    print "ENVResource* resourceArray\[\]={";
    print   FO  "ENVResource* resourceArray\[\]={";
    print "$resource_names";
    print   FO  "$resource_names";
    print " NULL };";
    print   FO  " NULL };";
}

