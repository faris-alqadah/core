% Author: Faris Alqadah
%
% Produce box plots of mean square error for different queries
%
%  Inputs:
%         
%           -file_dir: directory containing n files (one for each query)
%                      and each directory corresponding to a different method
%           -n: number of queries
%           -method_names: cell array with name of each method
%           -full_matrix: the full matrix
%                          
%  Output: box plot comparing all the methods
function make_boxplots_mse(file_dirs,n,method_names,full_matrix)
    mse = [];
    for i=1:numel(file_dir)
        curr_mse = [];
        for j=1:n
            cluster_file_name = [file_dir{i} j '.concepts'];
            cluster_name_file_name=[cluster_file_name '.names'];
            [cluster cluster_name] = read_nclusters(cluster_file_name,cluster_name_file_name);
            matrix = full_matrix(cluster{1},cluster{2}];
            curr_mse = [curr_mse mean_square_error_sparse(matrix)];
        end
        mse = [mse; curr_mse];
    end
    boxplot(mse,'labels',method_names);
    ylabel('MSE');

end