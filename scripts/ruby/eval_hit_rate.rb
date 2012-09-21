# Computes hit rate for evaluation of top-n recommender systems (see recommender system lit for details)
# Input is two files:
#   -first file is list of top-n recommendations
#   -second file is the 'leave-out' file
# Both files should have same number of line numbers, as each line number corresponds to a 'user'
# Both files should be space seperated with integer indices
# 
# Author::    Faris Alqadah  (mailto:faris.alqadah@gmail.com)
#


$LOAD_PATH << '../lib'
require "rubygems"
require "trollop"
require "set"


def make_set(line)
   begin
        line.gsub!("\n","")
        line.gusb!("\r","")
        tkns = line.split(" ")
        ret = Set.new
        for t in tkns
          ret.add(t.to_i())
        end
        return ret
    rescue => err
      puts err.to_s
      puts err.backtrace
    end
end

opts = Trollop::options do
  version "eval_hit_rate 2012 Faris Alqadah"
  banner <<-EOS
Computes hit rate for evaluation of top-n recommender systems (see recommender system lit for details)

Usage:
       eval_hit_rate [options] <top_n_file> <leave_out_file>
where [options] are:
EOS

end

  begin
      @top_n_file = ARGV[0]
      @leave_out_file = ARGV[1]
      top_n_arr = File.open(@top_n_file,"r").readlines
      leave_out_arr = File.open(@leave_out_file,"r").readlines
      if top_n_arr.length != leave_out_arr.length
        puts 'ERROR: Files not same length!'
        exit(0)
      end
      num_hits =0
      for i in 0..top_n_arr.length-1
        top_n_set = make_set(top_n_arr[i])
        leave_out_set = make_set(leave_out_arr[i])
        intersect = top_n_set & leave_out_set
        if intersect.length > 0
          num_hits += 1
        end   
      end
      hit_rate = Float(num_hits) / Float(top_n_arr.length)
      puts 'Hit rate: #{hit_rate}'
  rescue => err
    puts err.to_s
    puts err.backtrace
  end
