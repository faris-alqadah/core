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
require "trollop"
require "set"
require "fimi"



opts = Trollop::options do
  version "pairs_to_fimi 2012 Faris Alqadah"
  banner <<-EOS
Converts a pairs or triples file representing a sparse matrix to the 'fimi' sparse matrix representation. Also creates a 'name' file for the rows and columns

Usage:
       pairs_to_fimi [options] <pairs_file> <out_file>
where [options] are:
EOS

  opt :weights, "Pairs file containts triples, so the matrix is non-binary"

end

  begin
      @pairs_file = ARGV[0]
      @out_file = ARGV[1]
      file_arr = File.open(@pairs_file,"r").readlines
      @fimi_map = Hash.new # store {A_id => [list of B_ids]}, A_id corresponds to line counter
      if opts[:weights] == true
        puts "weights matrix option enabled"
        @fimi_weights_map = Hash.new
      end
      puts "reading pairs file..."
      max_row_id=0
      for f in file_arr
        f.gsub!("\n","")
        f.gsub!("\r","")
        tkns = f.split("\t")
        g1 = tkns[0].to_i
        g2 = tkns[1].to_i
        if opts[:weights] == true
          weight = tkns[2].to_f
        end
        if !@fimi_map.has_key?(g1)
          @fimi_map[g1] = Array.new
          if opts[:weights]
              @fimi_weights_map[g1] = Hash.new
          end
        end
        @fimi_map[g1] <<g2
        if opts[:weights] == true
           @fimi_weights_map[g1][g2] = weight
        end
        if g1 > max_row_id
          max_row_id = g1
        end
      end
      puts "writing fimi file..."
      if opts[:weights] == true
        Fimi.write_weighted_fimi_from_hash(@out_file,@fimi_map,@fimi_weights_map,max_row_id)
      else
         Fimi.wirte_fimi_from_hash(@out_file,@fimi_map,max_row_id)
      end
      puts "done!"
  rescue => err
    puts err.to_s
    puts err.backtrace
  end






#end