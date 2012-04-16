# Module for basic and utility operations involving fimi files.
# A fimi file is a file that typically represents a graph as adjacency lists
# where the ith line corresponds to the vertex with id i-1. Ids are simply
# integers starting at 0 upto n-1, where n-1 is the number of vertices in the graph. ON each line is
# a space sperated list of integers that represent the neighbors nodes with respect to node i-1.
#
#
#
#
#
# Author::    Faris Alqadah  (mailto:faris.alqadah@gmail.com)
#
require "set"

module Fimi


  # Writes the contents of the array 'arr' to the file 'file' as a fimi file
  # *arr is an array of arrays in the correct order for the fimi file
  # *file is the name of the file to output the fimi file to
  #
  def Fimi.wirte_fimi(file,arr)
    begin
      out_str = File.open(file,"w")
      for i in 0..arr.length-1
         aa = arr[i]
         for j in 0..aa.length-1
           unless j==0
              out_str.write(" ")
           end
           out_str.write("#{aa[j]}")
         end
         unless i == arr.length-1
           out_str.write("\n")
         end
      end
      out_str.close
    rescue => err
      puts err.to_s
      puts err.backtrace
    end
  end

  # Writes the contents of the hash map hsh as a fimi file. In hsh, a string
  # is associated with an integer list (representing neighboring nodes). node_map
  # is a hash map that associates integers 0...n-1 with their string equivalent
  #
  def Fimi.wirte_fimi_from_hash(file,hsh,node_map)
    begin
      out_str = File.open(file,"w")
      for i in 0..node_map.length-1
         aa = hsh[node_map[i]]
         aa.sort!
         for j in 0..aa.length-1
           unless j==0
              out_str.write(" ")
           end
           out_str.write("#{aa[j]}")
         end
         unless i == node_map.length-1
           out_str.write("\n")
         end
      end
      out_str.close
    rescue => err
      puts err.to_s
      puts err.backtrace
    end
  end


  # Writes the contents of the hash map hsh as a binary matrix. In hsh, a string
  # is associated with an integer list (representing neighboring nodes). node_map
  # is a hash map that associates integers 0...n-1 with their string equivalent
  #
  def Fimi.write_fimi_as_bin_matrix(file,hsh,node_map)
    begin
      out_str = File.open(file,"w")
      for i in 0..node_map.length-1
         aa = hsh[node_map[i]]
         aa.sort!
         for j in 0..node_map.length-1
           unless j==0
              out_str.write(" ")
           end
           if aa.include?(j)
             out_str.write("1")
           else
             out_str.write("0")
           end
         end
         unless i == node_map.length-1
           out_str.write("\n")
         end
      end
      out_str.close
    rescue => err
      puts err.to_s
      puts err.backtrace
    end
  end


  # Writes the contents of the hash map hsh as a matrix. In hsh, a string
  # is associated with a list of integer and real valued pairs (representing neighboring nodes and their weights). node_map
  # is a hash map that associates integers 0...n-1 with their string equivalent
  #
  def Fimi.write_fimi_as_matrix(file,hsh,node_map)
    begin
      out_str = File.open(file,"w")
      for i in 0..node_map.length-1
         aa = hsh[node_map[i]]
         idxs_val_map = Hash.new
         for a in aa
           idxs_val_map[a[0]] = a[1]
         end
         aa.sort! {|x,y| x[0] <=> y[0] }
         for j in 0..node_map.length-1
           unless j==0
              out_str.write(" ")
           end
           if idxs_val_map.has_key?(j)
            out_str.write("#{idxs_val_map[j]}")
           else
             out_str.write("0")
            end
         end
         unless i == node_map.length-1
           out_str.write("\n")
         end
      end
      out_str.close
    rescue => err
      puts err.to_s
      puts err.backtrace
    end

  end

end