require "rubygems"
require "commandline"
require "set"



class App < CommandLine::Application


  def initialize
    author    "Faris Alqadah"
    copyright "Faris Alqadah, 2011"
    synopsis  "cluster_file output_base_name [--domains][d1 d2 ... dn]"
    short_description "Read an n-cluster file produce by DMRC package and split it into n files, one file per cluster. Option to extract only specific domains in each cluster"
    long_description "Read an n-cluster file produce by DMRC package and split it into n files, one file per cluster. Option to extract only specific domains in each cluster"
    options :help

   option   :names => "--domains",
              :arity => [1,-1],
             :opt_found => get_args,
             :opt_not_found => false,
             :opt_description => "Only extract these domains"

    expected_args :cluster_file, :output_base_name

  end


  def main
    begin
      arr = File.open(@cluster_file,"r").readlines
      cluster_ctr=1
      if opt.domains
        first_line=true

        for a in arr
          a.gsub!("\n","")
          a.gsub!("\r","")
          if first_line == true
               curr_out = File.open(@output_base_name+"#{cluster_ctr}","w")
               first_line=false
          end
          if a != ";;;"
            tkns =  a.split("\t")
            domain = get_domain_num(tkns[0])
            if opt.domains.include?(domain)
              curr_out.write("[#{domain}]\t#{tkns[1]}\n")
            end
          else
            curr_out.write(";;;")
            cluster_ctr = cluster_ctr+1
            first_line=true
            curr_out.close
          end
        end

      else
        puts "Please specify domains!!"
        exit
      end
 rescue => err
        puts err.to_s
        puts err.backtrace
    end
  end

  def get_domain_num(str)
     str.gsub!("[","")
     str.gsub!("]","")
     return str
  end

end