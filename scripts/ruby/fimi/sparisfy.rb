# Sparsify a fimi by a sparsity factor
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
  version "sparsify 2012 Faris Alqadah"
  banner <<-EOS
Sparsify a fimi by sparsity factor
Usage:
       sparsify [options] <fimi_file> <out_file> <sparse factor (0-1)>
where [options] are:
EOS


end

  begin
      fimi_file = ARGV[0]
      sparse_factor= ARGV[1].to_f
      out_file = ARGV[2]
      # assumes same legnth or stops when end of fimi file1 is reached

      file_arr = File.open(fimi_file,"r").readlines
      out_stream = File.open(out_file,"w")
      edge_cnt=0
      # count the edges
      for row in file_arr
        row.gsub!("\n","")
        row.gsub!("\r","")
        if row == "###"
          out_stream.write("###")
          break
        end
        tkns = row.split(" ")
        col_cnt = 0
        for t in tkns
          s_rand = rand()
          if s_rand > sparse_factor
            unless col_cnt == 0
              out_stream.write(" ")
            end
            out_stream.write(t)
            col_cnt += 1
          end
         edge_cnt += tkns.count
        end
        out_stream.write("\n")
      end
      out_stream.close
  rescue => err
    puts err.to_s
    puts err.backtrace
  end