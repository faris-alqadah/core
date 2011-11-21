% Author: Faris Alqadah
% JHU
% Compute p-values for GO functional annotation of n-cluster. 
% Upto this point assumes SGD (Yeast) genes....
% 
% Inputs:
%         -all_gene_counts: vector with counts of GO annos indexed by GO id
%         for backgroudn genes
%         -cluster_gene_count: vector with counts of GO annos indexed by GO id
%         for backgroudn genes
%         -GO: the GO object
%  Returns: 
%           -annos: an array of structs containing 
%                       -term (the GO term)
%                       -pval  (the p-value)
%                       -raw count (raw count of number of genes annotated)
%                       
%                       -raw total
%                       -descrip
%                       
%

function [ annos] = GO_annotation(all_gene_counts,cluster_gene_count,GO)
        annos = {};
        pvals = hygepdf(cluster_gene_count,max(all_gene_counts),max(cluster_gene_count),all_gene_counts);
        [dummy idx] = sort(pvals);
        for i=1:10
            s = struct;
            s.term = char(num2goid(idx(i)));
            s.pval = pvals(idx(i));
            s.raw_count = cluster_gene_count(idx(i));
            s.raw_total = all_gene_counts(idx(i));
            s.descrip='';% GO(idx(i)).Term.definition(2:min(end,60));
            annos{i} = s;
        end
end



