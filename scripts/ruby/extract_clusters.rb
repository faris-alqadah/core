require "rubygems"
require "commandline"
require "set"
require "ncluster"



class App < CommandLine::Application


  def initialize
    author    "Faris Alqadah"
    copyright "Faris Alqadah, 2011"
    synopsis  "cluster_file output_base_name [--domains][d1 d2 ... dn][--lines][--sfiles]"
    short_description "Read an n-cluster file produce by DMRC package and split it into n files, one file per cluster. Option to extract only specific domains in each cluster"
    long_description "Read an n-cluster file produce by DMRC package and split it into n files, one file per cluster. Option to extract only specific domains in each cluster"
    options :help

   option   :names => "--domains",
              :arity => [1,-1],
             :opt_found => get_args,
             :opt_not_found => false,
             :opt_description => "Only extract these domains"

     option :flag,
      :names => "--lines",
      :opt_found => get_args,
      :opt_not_found => false,
      :opt_description => "Place extracted objects one per line"

    option :flag,
      :names => "--sfiles",
      :opt_found => get_args,
      :opt_not_found => false,
      :opt_description => "Objects of each domain will be placed in seperate file"

    expected_args :cluster_file, :output_base_name

  end


  def main
    begin
      arr = File.open(@cluster_file,"r").readlines
      cluster_ctr=1
      ctr=0
      if opt.domains
        while ctr < arr.size
           ctr,clus = get_single_cluster(arr,ctr)

           output_cluster(clus,cluster_ctr)
           cluster_ctr = cluster_ctr+1
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



  ##
  #Outputs an ncluster, according the selected options
  #
  # Input:
  #     hsh: a hash of arrays representing the n-cluster
  #     cluster_cnt: the number cluster to be output
  # Output: output of the n-cluster to a file or multiple files
  #
  # Returns: none
  #
  def output_cluster(hsh,cluster_cnt)

      if opt.sfiles #make several output files on per domain
        outs = Hash.new
        for k in hsh.keys
            outs[k] = File.open(@output_base_name+"#{cluster_cnt}_#{k}","w")
        end
      else
        outs = File.open(@output_base_name+"#{cluster_cnt}","w")
      end

      for k in hsh.keys
        if opt.domains.include?(k)
            if opt.sfiles && opt.lines
                output_domain_lines(outs[k],k,hsh[k])
            elsif opt.sfiles && !opt.lines
              output_domain_ncluster(outs[k],k,hsh[k])
            elsif !opt.sfiles && opt.lines
                output_domain_lines(outs,k,hsh[k])
            else
              output_domain_ncluster(outs,k,hsh[k])
            end
        end
      end
      # close file stream(s)
      if opt.sfiles
        for k in hsh.keys
            outs[k].close
        end
      else
        outs.close
      end
  end

  ##
  # Output a domain to a file as an object per line
  #
  def output_domain_lines(out,domain_num,arr)
      for i in 0..arr.size-1
        out.write("#{arr[i]}")
        unless i == arr.size-1
          out.write("\n")
        end
      end
  end

  ##
  # Output a domain to a file in the usual way
  #
  def output_domain_ncluster(out,domain_num,arr)
    out.write("[#{domain_num}]\t")
    for i in 0..arr.size-1
        unless i == 0
          out.write(" ")
        end
        out.write("#{i}")
    end
    out.write("\n")
  end

end