#Reads a fimi (binary matrix) file and outputs it as a csr file
# Additionally outputs a names file that maps each identifier to the relevant name of the node
#
#
#
#
# Author::    Faris Alqadah  (mailto:faris.alqadah@gmail.com)
#


$LOAD_PATH << '../lib'
require "rubygems"
require "trollop"
require "set"
require "fimi"

opts = Trollop::options do
  version "pairs_to_fimi 2012 Faris Alqadah"
  banner <<-EOS
Converts a fimi (binary matrix) file that represents sparse matrix to the csr file format

Usage:
       fimi_to_csr [options] <fimi_file> <out_file>
where [options] are:
EOS


end

  begin
      @fimi_file = ARGV[0]
      @out_file = ARGV[1]
      file_arr = File.open(@fimi_file,"r").readlines
      out_stream = File.open(@out_file,"w")
      ctr=0
      for row in file_arr
	   unless ctr == 0
	    out_stream.write("\n")
	  end
	  row.gsub!("\n","")
	  row.gsub!("\r","")
	  tkns = row.split(" ")
	  col_cntr=0
	  for t in tkns
	    t_int = t.to_i
	    t_int = t_int +1
	    unless col_cntr == 0
	      out_stream.write(" ")
	    end
	    out_stream.write(t_int.to_s+" 1")
	    col_cntr += 1
	  end
	  ctr += 1
      end
      out_stream.close
      
  rescue => err
    puts err.to_s
    puts err.backtrace
  end
