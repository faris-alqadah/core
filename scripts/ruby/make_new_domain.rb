require "rubygems"
require "commandline"
require "set"



class App < CommandLine::Application


  def initialize
    author    "Faris Alqadah"
    copyright "Faris Alqadah, 2011"
    synopsis  "node_file_dir output_file"
    short_description "Converts node files to the new domain file format"
    long_description "Converts node files to the new domain file format"
    options :help



    expected_args :node_file_dir, :output_file

  end


  def main
    begin
      out_file = File.new(@output_file,"w")
      @obj_id_hash = Hash.new
      Dir.foreach(@node_file_dir) do |f|
        if f != ".." and f != "."
          arr = File.open(@node_file_dir+f,"r").readlines
          for a in arr
            a.gsub!("\n","")
            a.gsub!("\r","")
            tkns = a.split(" ")
            src = tkns[0].to_i
            dst = tkns[1]
            obj = tkns[2].to_i
            if !@obj_id_hash.has_key?(obj)
              @obj_id_hash[obj] = Array.new
              @obj_id_hash[obj] << a
            else
               @obj_id_hash[obj] <<";#{a}"
            end
          end
        end
      end
      keys = @obj_id_hash.keys
      keys.sort!
      for k in keys
        arr = @obj_id_hash[k]
        str = ''
        #puts "new str"
        for a in arr
         # puts a
          str = str+a
        end
        out_file.puts str
      end
      out_file.close
    end
    rescue => err
      puts err.to_s
      puts err.backtrace
    end

  end
