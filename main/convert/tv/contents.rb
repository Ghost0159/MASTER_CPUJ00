infile = ARGV[0]
outfile = ARGV[1]

inf = File.open(infile, "r")
outf = File.open(outfile, "w")
puts "\>\>#{outf.path}"
while line = inf.gets
	cl = line.split ","
	if cl[2] == "" then break end
	if cl[1].to_i == 0 then next end
	#puts "#{cl}"

	#[�X�^�[�g(5����), ���������邩(9��������)]		DP
	#newcl = cl[5, 9].map{|str| str.to_i}

	#��[�X�^�[�g(5����), ���������邩(10��������)]	PL
	#��[�X�^�[�g(5����), ���������邩(12��������)]	PL
	#newcl = cl[5, 10].map{|str| str.to_i}
	newcl = cl[5, 12].map{|str| str.to_i}

	#puts "#{newcl}"
	#pstr = newcl.pack("c*")
	outf.syswrite newcl.pack("c*")
end
inf.close
outf.close
