require "rubygems"
require "commandline"
require "set"



class App < CommandLine::Application


  def initialize
    author    "Faris Alqadah"
    copyright "Faris Alqadah, 2011"
    synopsis  "mat_file fimi_file"
    short_description "Convert a sparse matrix file (mat) to a binary fimi file"
    long_description "Convert a sparse matrix file (mat) to a binary fimi file"
    options :help

 

    expected_args :mat_file, :fimi_file

  end


  def main
    begin
      arr = File.open(@mat_file,"r").readlines
      out_str = File.open(@fimi_file,"w")
      for a in arr
        a.gsub!("\n","")
        a.gsub!("\r","")
        tkns = a.split(",")
        first=true
        for t in tkns
          tkns2 = t.split(" ")
          if first == false
            out_str.write(" ")
          else
            first=false
          end
          out_str.write(tkns2[0])
        end
        out_str.write("\n")
      end
      out_str.close
    rescue => err
      puts err.to_s
      puts err.backtrace
    end

  end

end