infile = ARGV[0]
outfile = ARGV[1]

inf = File.open(infile, "r")
outf = File.open(outfile, "w")

puts "\>\>#{outf.path}"
inf.gets	#��s�ړǂݔ�΂�

while line = inf.gets
	cl = line.split ","

	#2����A7����
	newcl = cl[2, 7].map{|str| str.to_i}

	#puts "#{newcl}"
	outf.syswrite newcl.pack("c*")
end
inf.close
outf.close
