% Author: Faris Alqadah
%
% Produces a scatter plot of mse of original query vs actual result
%
%  Inputs:
%         
%           -file_dir: directory containing n files (one for each query)
%                      and each directory corresponding to a different method
%               
%                   these should be listed as q1_1,q1_2,....,q_n_x
%                       where q_i corresponds to the query file q_i (see
%                       next)
%           -query_file_dir: directory of original query terms (idxs)
%
%               these should be listed q1....qn
%               
%           -n: the total number of queries
%           -x: vector of number of files per query
%           -method_names: cell array with name of each method
%           -full_matrix: the full matrix
%                          
%  Output: scatter plot of original mse score vs result mse score
function make_scatterplot_mse(file_dirs,query_file_dir,n,x,method_names,full_matrix)
    markers = ['+';'o';'*';'.';'x';'s';'d';'^';'v';'>';'<';'p';'h'];
    colors = ['r';'g';'b';'c';'m';'y';'k'];
    for i=1:numel(file_dirs)
        orig_mse = [];
        query_mse = [];
        for j=1:n
            % read the query file
            query_file = [query_file_dir num2str(j) '.concepts'];
            query_name_file = [query_file '.names'];
            [query_c query_c_name] = read_nclusters(query_file,query_name_file);
            % compute the original mse
            q_matrix = full_matrix(:,query_c{1}{1})
            o_mse = mean_square_error(q_matrix);
            for k=1:x(j)
                cluster_file_name = [file_dirs{i} num2str(j) '_' num2str(k) '.concepts'];
                cluster_name_file_name=[cluster_file_name '.names'];
                [cluster cluster_name] = read_nclusters(cluster_file_name,cluster_name_file_name);
                matrix = full_matrix(cluster{1}{2},cluster{1}{1});
                curr_mse = [curr_mse mean_square_error_sparse(matrix)];
                orig_mse=[orig_mse o_mse];
                query_mse = [query_mse curr_mse];     
           end
        end
        % make the plot
        plot(orig_mse,query_mse,[colors(mod(i,numel(colors))) markers( mod(i,numel(markers))+1)]);
    end
    xlabel('Seed MSE');
    ylabel('Bi-cluster MSE');
    legend(method_names);
   
end