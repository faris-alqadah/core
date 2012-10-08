# Transpose a sparse binary matrix represented as fimi file
#
#
#
#
# Author::    Faris Alqadah  (mailto:faris.alqadah@gmail.com)
#


$LOAD_PATH << './lib'
require "rubygems"
require "trollop"
require "set"
require "fimi"
require "ncluster"

opts = Trollop::options do
  version "pairs_to_fimi 2012 Faris Alqadah"
  banner <<-EOS
Transpose a sparse binary matrix represented as fimi file

Usage:
       transpose_fimi.rb [options] <fimi_file>
where [options] are:
EOS


end
    begin
      @fimi_file = ARGV[0]
      arr = File.open(@fimi_file,"r").readlines
      out_file = File.open(@fimi_file+".transpose","w")
      transpose_hash = Hash.new # store each object with identifier
      line_cnt=0
      max_col=0
      # read file and associate each object per line with its line number
      for a in arr
        a.gsub!("\n","")
        a.gsub!("\r","")
        tkns = a.split(" ")
        for t in tkns
          col_id = t.to_i
          if col_id > max_col
            max_col=col_id
          end
          if transpose_hash.has_key?(t)
           # puts "putting #{t} -> #{line_cnt}"
            transpose_hash[t] << line_cnt
          else
            transpose_hash[t] = Array.new
            transpose_hash[t] << line_cnt
            #puts "putting #{t} -> #{line_cnt}"
          end
        end
        line_cnt = line_cnt+1
      end
      sz_b = max_col
      for i in 0..sz_b
        key = i.to_s
        lcl_cnt=1
        if transpose_hash[key]
          for a in transpose_hash[key]
            unless lcl_cnt == 1
              out_file.write(" ")
            end
            out_file.write(a)
            lcl_cnt = lcl_cnt+1
          end
        end
        #unless i == sz_b -1
          out_file.write("\n")
        #end
      end
      out_file.write("###")
      out_file.close

 rescue => err
        puts err.to_s
        puts err.backtrace
    end