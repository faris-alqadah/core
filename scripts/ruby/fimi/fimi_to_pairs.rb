# Convert fimi file to pairs file
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
  version "fimi_to_pairs 2012 Faris Alqadah"
  banner <<-EOS
Converts a sparse binary matrix in fimi format to a pairs file, where each 'link' is represented by a pair of integer indices

Usage:
       fimi_to_pairs [options] <fimi_file> <out_file>
where [options] are:
EOS

  opt :zero_idx, "Add 1 to ids generate non-zero indexing"
end

  begin
      @fimi_file = ARGV[0]
      @out_file = ARGV[1]
      file_arr = File.open(@fimi_file,"r").readlines
      out_str = File.open(@out_file,"w")
      row_cnt=0
      max_col_id=0
      t_cnt=0
      for row in file_arr
        if row == '###'
          break
        end
        row.gsub!("\n","")
        row.gsub!("\r","")
        tkns = row.split(" ")
        row_id = row_cnt
        if opts[:zero_idx] == true
          row_id += 1
        end
        for t in tkns
          unless t_cnt == 0
            out_str.write("\n")
          end
          col_id = t.to_i
          if opts[:zero_idx] == true
            col_id = col_id+1
          end
          if col_id > max_col_id
            max_col_id = col_id
          end
          out_str.write(row_id.to_s+"\t"+col_id.to_s)
          t_cnt += 1
        end
        row_cnt += 1
      end
      row_cnt += 1
      puts 'row count: '+row_cnt.to_s
      puts 'max col id: '+max_col_id.to_s
      puts 'transactions coutn: '+t_cnt.to_s
      out_str.close
      
  rescue => err
    puts err.to_s
    puts err.backtrace
  end