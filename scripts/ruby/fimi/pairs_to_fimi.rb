#Reads a file that consists of pairs of objects (these represent edges of a graph) and converts the graph into a fimi file graph representation
# Additionally outputs a names file that maps each identifier to the relevant name of the node
#
#
#
#
# Author::    Faris Alqadah  (mailto:faris.alqadah@gmail.com)
#


$LOAD_PATH << '../lib'
require "rubygems"
require "commandline"
require "set"
require "fimi"


class App < CommandLine::Application


  def initialize
    author    "Faris Alqadah"
    copyright "Faris Alqadah, 2012"
    synopsis  "pairs_file output_file name_file [--weights_matrix]"
    short_description "Reads a file that consists of pairs of objects (these represent edges of a graph) and converts the graph into a fimi file graph representation"
    long_description  "Reads a file that consists of pairs of objects (these represent edges of a graph) and converts the graph into a fimi file graph representation"
    options :help

    option :flag,
      :names => "--weights_matrix",
      :opt_found => get_args,
      :opt_not_found => false,
      :opt_description => "Pairs have weights associated with them. Setting this flag will read these weights and output the corresponding matrix to the fimi file with weights"

    expected_args :pairs_file, :out_file, :name_file

  end

def main
  begin
      file_arr = File.open(@pairs_file,"r").readlines
      @fimi_map = Hash.new
      @id_node_map = Hash.new
      @node_id_map = Hash.new
      @id_ctr=0
      if opt["--weights_matrix"]
        puts "weights matrix option enabled"
        @fimi_weights_map = Hash.new
      end
      puts "reading pairs file..."
      for f in file_arr
        f.gsub!("\n","")
        f.gsub!("\r","")
        tkns = f.split(" ")
        g1 = tkns[0]
        g2 = tkns[1]
        if opt["--weights_matrix"]
          weight = tkns[2]
        end
        if !@node_id_map.has_key?(g1)
          @node_id_map[g1] = @id_ctr
          @id_node_map[@id_ctr] = g1
          @id_ctr = @id_ctr+1
        end
        if !@node_id_map.has_key?(g2)
          @node_id_map[g2] = @id_ctr
          @id_node_map[@id_ctr] = g2
          @id_ctr = @id_ctr+1
        end
        if !@fimi_map.has_key?(g1)
          @fimi_map[g1] = Array.new
          if opt["--weights_matrix"]
              @fimi_weights_map[g1] = Array.new
          end
        end
        if !@fimi_map.has_key?(g2)
          @fimi_map[g2] = Array.new
          if opt["--weights_matrix"]
              @fimi_weights_map[g2] = Array.new
          end
        end
        a1 = @fimi_map[g1]
        a2 = @fimi_map[g2]
        if opt["--weights_matrix"]
            a1_weights = @fimi_weights_map[g1]
            a2_weights = @fimi_weights_map[g2]
            a1_lcl = Array.new
            a1_lcl[0] = @node_id_map[g2]
            a1_lcl[1] = weight
            a2_lcl = Array.new
            a2_lcl[0] = @node_id_map[g1]
            a2_lcl[1] = weight
            a1_weights << a1_lcl
            a2_weights << a2_lcl
            @fimi_weights_map[g1] = a1_weights
            @fimi_weights_map[g2] = a2_weights
        else
           a1 << @node_id_map[g2]
           a2 << @node_id_map[g1]
        end
        @fimi_map[g1] = a1
        @fimi_map[g2] = a2
      end
      puts "writing fimi file..."
      Fimi.wirte_fimi_from_hash(@out_file,@fimi_map,@id_node_map)
      puts "writing name file..."
      name_out = File.open(@name_file,"w")
      for i in 0..@id_ctr-1
        name_out.puts @id_node_map[i]
      end
      name_out.close
      if opt["--weights_matrix"]
        puts "wiriting weights matrix"
        Fimi.write_fimi_as_matrix(@name_file+".weights_matrix",@fimi_weights_map,@id_node_map)
      end
      puts "done!"
  rescue => err
    puts err.to_s
    puts err.backtrace
  end

end





end