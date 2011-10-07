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
    box_mse = ones(numel(file_dirs),n*max(x));
    box_mse(find(box_mse)) = NaN;
    for i=1:numel(file_dirs)
        orig_mse = [];
        query_mse = [];
        i
        box_ctr=1;
        for j=1:n
            % read the query file
            query_file = [query_file_dir num2str(j)];
            query_name_file = [query_file '.query'];
            [query_c query_c_name] = read_nclusters(query_file,query_file);
            % compute the original mse
            if numel(query_c{1}{1}) > 1
                
                q_matrix = full_matrix(:,query_c{1}{1});
                o_mse = mean_square_error(q_matrix);
                for k=1:x(i)
                    cluster_file_name = [file_dirs{i} num2str(j) '_' num2str(k) '.concepts'];
                    if fopen(cluster_file_name) > 0

                        cluster_name_file_name=[cluster_file_name '.names'];
                        [cluster cluster_name] = read_nclusters(cluster_file_name,cluster_file_name);
                        matrix = full_matrix(cluster{1}{2},cluster{1}{1});
                        orig_mse=[orig_mse o_mse];
                        mm=mean_square_error(matrix);
                        query_mse = [query_mse mm];
                        box_mse(i,box_ctr)=mm;
                        box_ctr=box_ctr+1;
                    end
                end
            end
        end
        % make the plot
        figure(1)
        fprintf('\nPlotting...\n');
        semilogy(orig_mse,query_mse,[colors(i) markers( i)],'MarkerSize',10);
        hold on;
        
    end
    xlabel('Seed MSE','FontSize',16);
    ylabel('Bi-cluster MSE','FontSize',16);
    legend(method_names);
    figure(2)
    boxplot(box_mse','labels',method_names);
    ylabel('MSE','FontSize',16);
    set(gca,'YScale','log');
   
end