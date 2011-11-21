module Ncluster

  ##
  #
  #returns clusters as an array of hashes.
  #Each hash is map of domain_id -> array of objects
  #
  def read_clusters(file)
    begin
      arr = File.open(file,"r").readlines
      ctr=1
      ret_arry = Array.new
      while ctr < arr.size
           ctr,clus = get_single_cluster(arr,ctr)
           ret_arry << clus
           cluster_ctr = cluster_ctr+1
        end
    rescue => err
      puts err.to_s
      puts err.backtrace
    end
    
    return ret_arry
  end


  ##
  #return a single cluster as hash of arrays. Hash key is domain number
  #
  # Input:
  #     arr: the array containing entire n-cluster file
  #     ctr: the starting poisition of where to seek next n-cluster
  #
  # Output: none
  #
  # Returns:
  #     new_ctr : the new ctr after finding next n-cluster
  #     nclu: the ncluster as a hash of arrays
  def get_single_cluster(arr,ctr)
      nclu = Hash.new
      lcl_ctr=ctr
      while true
        arr[lcl_ctr].gsub!("\n","")
        arr[lcl_ctr].gsub!("\r","")
        if arr[lcl_ctr] == ";;;"
          return lcl_ctr+1, nclu
        else
           tkns =  arr[lcl_ctr].split("\t")
           domain = get_domain_num(tkns[0])
           lcl_arr = Array.new
           for i in 1..tkns.size-1
             lcl_arr << tkns[i]
           end
           nclu[domain] = lcl_arr
        end
        lcl_ctr = lcl_ctr + 1
      end
      return lcl_ctr,nclu
  end

  ##
  # returns the domain number of a line assumed to contain the domain number
  # in an ncluster
  #
  def get_domain_num(str)
     str.gsub!("[","")
     str.gsub!("]","")
     return str
  end


  

end