% Author: Faris Alqadah
%
%
%  Plots bi-cluster with lowese MSE from concept files in the given
%  directory
%
%   file: n cluster file
%   full_matrix: the original data matrix (to compute MSE)
%
function plot_lowest_mse_bicluster(file,full_matrix)

   min_idx=1;
   min_mse=9999999;
   if fopen(file) > 0
        [clusters cluster_name] = read_nclusters(file,file);
        for i=1:numel(clusters)
                        matrix = full_matrix(clusters{i}{1},clusters{i}{2});
                        mm=mean_square_error_sparse(matrix);
                        if mm < min_mse
                            min_mse=mm;
                            min_idx=i;
                        end

        end
        plot_bicluster(full_matrix(clusters{min_idx}{1},clusters{min_idx}{2}));
       full_matrix(clusters{min_idx}{1},clusters{min_idx}{2})
   else
       fprintf('\nCould not open file!\n');
       
   end
end