# Remove duplicate values in a rows of fimi file
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
  version "remove_dups 2012 Faris Alqadah"
  banner <<-EOS
Remove duplicate values in a rows of fimi file
Usage:
       remove_dups [options] <fimi_file> <output_file>
where [options] are:
EOS


end

  begin
      fimi_file = ARGV[0]
      out_file = ARGV[1]
      # assumes same legnth or stops when end of fimi file1 is reached

      file_arr = File.open(fimi_file,"r").readlines
      out_stream = File.open(out_file,"w")
      for row in file_arr
	row.gsub!("\n","")
	row.gsub!("\r","")
        if row == "###"
          out_stream.write("###")
          break
        end
	tkns = row.split(" ")
	row_set = Set.new
	col_cnt=0
	for t in tkns
	  if not row_set.include?(t)
	    unless col_cnt == 0
	      out_stream.write(" ")
	    end
	    out_stream.write(t)
	    row_set.add(t)
	    col_cnt += 1
	  end
	 end
        out_stream.write("\n")
       end
    rescue => err
      puts err.to_s
      puts err.backtrace
     end