require "rubygems"
require "commandline"
require "set"
require "ncluster"



class App < CommandLine::Application


  def initialize
    author    "Faris Alqadah"
    copyright "Faris Alqadah, 2011"
    synopsis  "fimi_file size_a size_b"
    short_description "Read a fimi file and writs its transpose to fimi_file.transpose"
    long_description "Read a fimi file and writs its transpose to fimi_file.transpose"
    options :help

    expected_args :fimi_file, :size_a, :size_b

  end


  def main
    begin
      arr = File.open(@fimi_file,"r").readlines
      out_file = File.open(@fimi_file+".transpose","w")
      transpose_hash = Hash.new # store each object with identifier
      line_cnt=0
      # read file and associate each object per line with its line number
      for a in arr
        a.gsub!("\n","")
        a.gsub!("\r","")
        tkns = a.split(" ")
        for t in tkns
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
      sz_b = @size_b.to_i
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
        unless i == sz_b -1
          out_file.write("\n")
        end
      end
      out_file.close

 rescue => err
        puts err.to_s
        puts err.backtrace
    end
  end





end