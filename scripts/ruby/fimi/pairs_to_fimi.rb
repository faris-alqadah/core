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


def write_name_ids(file_name,max_id,zero_idx)
   begin
      out_str = File.open(file_name,"w")
      if zero_idx == true
        strt=0
      else
        strt=1
      end
      for i in strt..max_id
       out_str.write(i)
       unless i == max_id
         out_str.write("\n")
       end
      end
      out_str.close
    rescue => err
      puts err.to_s
      puts err.backtrace
    end
end

opts = Trollop::options do
  version "pairs_to_fimi 2012 Faris Alqadah"
  banner <<-EOS
Converts a pairs or triples file representing a sparse matrix to the 'fimi' sparse matrix representation. Also creates a 'name' file for the rows and columns

Usage:
       pairs_to_fimi [options] <pairs_file> <out_file>
where [options] are:
EOS

  opt :weights, "Pairs file containts triples, so the matrix is non-binary"
  opt :id_names, "Generate name files using id numbers as the names"
  opt :zero_idx, "Subtract 1 from ids to generate 0 index"
  opt :reverse, "Use the second id as the rows and first id cols"
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
      if opts[:zero_idx] == true
        puts 'zero indexing set'
      end
      if opts[:id_names] == true
        puts 'print id names set'
      end
      if opts[:reverse] == true
        puts 'reverse option set'
      end
      puts "reading pairs file..."
      max_row_id=0
      max_col_id=0
      for f in file_arr
        f.gsub!("\n","")
        f.gsub!("\r","")
        tkns = f.split("\t")
        if opts[:reverse] == false
          g1 = tkns[0].to_i
          g2 = tkns[1].to_i
        else
          g1 = tkns[1].to_i
          g2 = tkns[0].to_i
        end
        if opts[:zero_idx] == true
          g1 = g1-1
          g2 = g2-1
        end
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
        if g2 > max_col_id
          max_col_id = g2
        end
      end
      puts "writing fimi file..."
      if opts[:weights] == true
        Fimi.write_weighted_fimi_from_hash(@out_file,@fimi_map,@fimi_weights_map,max_row_id)
      else
         Fimi.wirte_fimi_from_hash(@out_file,@fimi_map,max_row_id)
      end
      if opts[:id_names] == true
        write_name_ids(@out_file+'.row.names',max_row_id,opts[:zero_idx])
        write_name_ids(@out_file+'.col.names',max_col_id,opts[:zero_idx])
      end
      puts "done!"
      puts "largest row id #{max_row_id}"
      puts "largest cold id #{max_col_id}"
  rescue => err
    puts err.to_s
    puts err.backtrace
  end
