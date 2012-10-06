# Computes hit rate for evaluation of top-n recommender systems (see recommender system lit for details)
# Input is two files:
#   -first file is list of top-n recommendations
#   -second file is the 'leave-out' file
# Both files should have same number of line numbers, as each line number corresponds to a 'user'
# Both files should be space seperated with integer indices
# 
# Author::    Faris Alqadah  (mailto:faris.alqadah@gmail.com)
#


require "rubygems"
require "trollop"
require "set"

module HitRate
  
def HitRate.make_arr(line,top_n)
   begin
        line.gsub!("\n","")
        line.gsub!("\r","")
        tkns = line.split(" ")
        ret = Array.new
        cntr=0
        for t in tkns
          if cntr >= top_n
            break
          end
          ret << t.to_i
          cntr += 1
        end
        return ret
    rescue => err
      puts err.to_s
      puts err.backtrace
    end
end

def HitRate.get_hit_pos(rec_arr,test_item)
  pos=1
  for item in rec_arr
    if item == test_item
      return pos
    end
    pos += 1
  end
  return -1

end

def HitRate.get_hit_rate(top_n_file,leave_out_file,nn)
      top_n_arr = File.open(top_n_file,"r").readlines
      leave_out_arr = File.open(leave_out_file,"r").readlines
      num_hits =0
      inv_hit=0.0
      for i in 0..top_n_arr.length-1
        top_n_set = make_arr(top_n_arr[i],nn)
        leave_out_set = make_arr(leave_out_arr[i],nn)
        leave_out_item = leave_out_set[0]
        hit_pos = get_hit_pos(top_n_set,leave_out_item)
        if hit_pos > 0
          num_hits += 1
          inv_hit += 1.0/Float(hit_pos)
        end
      end
      hit_rate = Float(num_hits) / Float(leave_out_arr.length)
      arhr = Float(inv_hit) / Float(leave_out_arr.length)
      return hit_rate,arhr
 end

end
