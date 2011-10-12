% Author: Faris Alqadah
%
% Create a smaller real-valued bi-clustering dataset from a bigger one by
% randomly sampling the rows of the dataset, then removing columns that
% contain only zeros .
%
%  Inputs:
%         
%           -data: the original dataset
%           -rownames: names of the rows
%           -colnames: names of the columns
%           -n: number of rows to sample (n < size(data)(1))
%           -base_name: the base_name of the file to output to
%           -add_noise: set to 1 to add noise to the dataset to make up for
%                       sparsity
%
%  Returns:
%           -s_data: the new dataset
%           -s_rownames: the new rownames
%           -s_colnames: the new column names
%
%
%   Outputs: s_data, s_rownames, s_colnames as base_name + .csv,
%   .s_rownames ,.s_colnames respectivley
%
%

function [s_data s_rownames s_colnames] = sample_dataset(data,rownames,colnames,n,base_name,add_noise)
    [x y] = size(data);
    rand_idxs=unique(ceil(x.*rand(n,1)));
    s_rownames = rownames(rand_idxs);
    col_idxs = find(sum(data(rand_idxs,:)) > 0);
    s_data = data(rand_idxs,col_idxs);
    s_colnames = colnames(col_idxs);
    % make zeros nans
    s_data(find(s_data == 0)) = NaN;
    % add noise?
    if (add_noise == 1)
        s_data = make_sparse_noisy(s_data);
    end
    % output to files
    dlmwrite([base_name '.csv'],s_data,'delimiter',',');
    fid=fopen([base_name '.s_rownames'],'w');
    for i=1:numel(s_rownames) fprintf(fid,'%s\n',s_rownames{i}); end
    fclose(fid);
    fid=fopen([base_name '.s_colnames'],'w');
    for i=1:numel(s_colnames) fprintf(fid,'%s\n',s_colnames{i}); end
    fclose(fid);
end