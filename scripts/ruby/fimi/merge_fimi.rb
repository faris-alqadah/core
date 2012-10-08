# Horizontal merge two imi files into one. For vertical merge, use 'cat'
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
  version "fimi_to_pairs 2012 Faris Alqadah"
  banner <<-EOS
Horizontal merge two fimi files into one.
Usage:
       fimi_to_pairs [options] <fimi_file> <fimi_file_2> <output_file>
where [options] are:
EOS


end

  begin
      fimi_file = ARGV[0]
      fimi_file2 = ARGV[1]
      out_file = ARGV[2]
      # assumes same legnth or stops when end of fimi file1 is reached

      file_arr = File.open(fimi_file,"r").readlines
      file_arr2 = File.open(fimi_file2,"r").readlines
      out_stream = File.open(out_file,"w")
      for i in 0..file_arr.count
        row_1 = file_arr[i]
        if row_1 == "###"
          out_stream.write("###")
          break
        end
        row_2 = file_arr2[i]
        row_1.gsub!("\n","")
        row_1.gsub!("\r","")
        row_2.gsub!("\n","")
        row_2.gsub!("\r","")
        tkns_1 =row_1.split(" ")
        tkns_2 = row_2.split(" ")
        tt_1 = Array.new
        for t in tkns_1
          tt_1 << t.to_i
        end
        tt_2 = Array.new
        for t in tkns_2
          tt_2 << t.to_i
        end
        all_tkns = tt_1+tt_2
        all_tkns.sort!
        lcl_cnt=0
        for t in all_tkns
          unless lcl_cnt == 0
            out_stream.write(" ")
          end
          out_stream.write(t)
          lcl_cnt += 1
        end
        out_stream.write("\n")
      end
  rescue => err
    puts err.to_s
    puts err.backtrace
  end