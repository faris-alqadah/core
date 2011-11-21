require "rubygems"
require "commandline"
require "set"
class App < CommandLine::Application


  def initialize
    author    "Faris Alqadah"
    copyright "Faris Alqadah, 2010"
    synopsis  "sparse_file"
    short_description ""
    long_description "Converts full matrix format to sparse"
    options :help


    expected_args :full_file
  end

  def main
    begin
      arr = File.open(@full_file,"r").readlines
      out_str = File.open(@full_file+".sparse","w")
      for a in arr
        a.gsub!("\n","")
        a.gsub!("\r","")
        tkns = a.split(",")
        col_cnt=0
        comma_cnt=0
        for t in tkns
          unless t == "NaN"
            if comma_cnt > 0
              out_str.write ","
            end
            out_str.write "#{col_cnt} #{t}"
            comma_cnt = comma_cnt + 1
          end
          col_cnt = col_cnt + 1
        end
        out_str.write("\n")
      end
      out_str.write("###")
      out_str.close
    rescue => err
      puts err.to_s
      puts err.backtrace
    end

  end

end
