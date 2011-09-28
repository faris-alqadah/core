require "rubygems"
require "commandline"
require "set"



class App < CommandLine::Application


  def initialize
    author    "Faris Alqadah"
    copyright "Faris Alqadah, 2011"
    synopsis  "cluster_file_dir input_file"
    short_description "Read cluster files in a directory and covert them to query files for qbbc"
    long_description "Read cluster files in a directory and covert them to query files for qbbc"
    options :help


    expected_args :cluster_file_dir, :input_file
  end


  def main
    begin
      arr = File.open(@input_file).readlines
      for a in arr
        a.gsub!("\n","")
        a.gsub!("\r","")
      end
      Dir.foreach(@cluster_file_dir) do |item|
         next if item == '.' or item == '..'
            arr1 = File.open(@cluster_file_dir+item).readlines
            for a in arr1
              a.gsub!("\n","")
              a.gsub!("\r","")
              if a != ";;;"
                tkns1 = a.split("\t")
                tkns2 = tkns1[1].split(" ")
                curr_out = File.open(@cluster_file_dir+item+".query","w")
                for t in tkns2
                  curr_out.puts arr[Integer(t)]
                end
                curr_out.close
              end
            end
      end
  
      
     rescue => err
        puts err.to_s
        puts err.backtrace
    end
  end
end