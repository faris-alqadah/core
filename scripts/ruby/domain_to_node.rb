require "rubygems"
require "commandline"
require "set"



class App < CommandLine::Application


  def initialize
    author    "Faris Alqadah"
    copyright "Faris Alqadah, 2011"
    synopsis  "domain_file dest_dir"
    short_description "Converts a domain file to several node files"
    long_description "Converts a domain file to several node files"
    options :help



    expected_args :domain_file, :dest_dir

  end


  def main
    begin
      file = File.new(@domain_file,"r")
      while(line = file.gets)
          line.gsub!("\n","")
          line.gsub!("\r","")
          tkns = line.split(" ")
          src = tkns[0]
          dst = tkns[1]
          obj = tkns[2]
          out_file = File.open(@dest_dir+"#{obj}.node","a")
          out_file.puts(line)
          out_file.close
      end
      file.close
    rescue => err
      puts err.to_s
      puts err.backtrace
    end

  end

end