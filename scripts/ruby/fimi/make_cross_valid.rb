# Create cross validation datasets from fimi files
# For fimi file a Training, Validation, and Test set will be created,
# one for each round of cross validation. The test and cross validation datasets
# will have one single item per row
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
  version "fimi_to_pairs 2012 Faris Alqadah"
  banner <<-EOS
Creates cross validation datasets from FIMI files. For the FIMI file a training, testing and validation
data set will be created
Usage:
       fimi_to_pairs [options] <fimi_file> <output_dir> <num_rounds>
where [options] are:
EOS


end

  begin
      @fimi_file = ARGV[0]
      output_dir = ARGV[1]
      @num_rounds = ARGV[2].to_i
      file_arr = File.open(@fimi_file,"r").readlines
      for i in 0...@num_rounds
        train_file_name=output_dir+"Train"+i.to_s+".fimi"
        valid_file_name=output_dir+"Valid"+i.to_s+".fimi"
        test_file_name=output_dir+"Test"+i.to_s+".fimi"
        train_out = File.open(train_file_name,"w")
        valid_out = File.open(valid_file_name,"w")
        test_out = File.open(test_file_name,"w")
        input_out = File.open(output_dir+"Train"+i.to_s+".input","w")
        row_cnt=0
        col_max=0
        for row in file_arr
          row.gsub!("\n","")
          row.gsub!("\r","")
          tkns = row.split(" ")
          if tkns.count > 2
            row_cnt += 1
            v_obj = rand(tkns.count)
            t_obj =rand(tkns.count)
            train_ctr=0
            for i in 0..tkns.count
               col_val = tkns[i].to_i
               if col_val > col_max
                 col_max=col_val
               end
              if i == v_obj
                valid_out.write(tkns[i])
              elsif i == t_obj
                test_out.write(tkns[i])
              else
                unless train_ctr == 0
                  train_out.write(" ")
                end
                train_out.write(tkns[i])
                train_ctr += 1
              end
             end
           train_out.write("\n")
           valid_out.write("\n")
           test_out.write("\n")
           end
          end
          col_max += 1
          train_out.write("###")
          valid_out.write("###")
          test_out.write("###")
          train_out.close
          valid_out.close
          test_out.close
          # write the input file
          input_out.write("2\n1")
          input_out.write("\nusers;"+row_cnt.to_s+"\nuser_names")
          input_out.write("\nitems;"+col_max.to_s+"\nitem_names")
          input_out.write("\nusers--items")
          input_out.write("\n"+train_file_name)
          input_out.close
        end
  rescue => err
    puts err.to_s
    puts err.backtrace
  end