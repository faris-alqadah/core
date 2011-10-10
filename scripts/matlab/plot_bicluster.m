% Author: Faris Alqadah
%
% Display a bi-cluster (real valued) as both a heat map and
% "expresssion" plot
%
%  Inputs:
%         
%           -bicluster: a cell array of the row idxs and col idxs of the
%                   bi-cluster in the original data matrix
%           -rownames: names of the rows
%           -colnames: names of the columns
%           -matrix: the original data matrix
%
function plot_bicluster(bicluster,rownames,colnames,matrix)
    plot(matrix(bicluster{1},bicluster{2})','-x','MarkerSize',16);
   % legend(rownames(bicluster{1}));
   % set(gca,'XTick',1:length(bicluster{2}));
   % set(gca,'XTickLabel',colnames(bicluster{2}));
    

end