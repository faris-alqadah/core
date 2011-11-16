% Author: Faris Alqadah
% JHU
% Compute p-values for GO functional annotation of n-cluster. 
% Upto this point assumes SGD (Yeast) genes....
% 
% Inputs:
%         -sgd_file: path to latest sgd file
%         -GO: the GO object
%         -gene_list: the list of genes (using sgd)  
%         -all_genes: list of all genes in the dataset 
%  Returns: 
%           -annos: an array of structs containing 
%                       -term (the GO term)
%                       -pval  (the p-value)
%                       -raw count (raw count of number of genes annotated)
%                       
%                       -raw total
%                       
%
function [ annos] = GO_annotation(sgd_file,GO,gene_list,all_genes)
        ann = goannotread(sgd_file,'Aspect','F','Fields',{'DB_Object_Symbol','GOid'});
        go_map = Construct_GO_map(ann);
        annos = {};
        % compute counts for all terms, w.r.t all genes and cluster genes
        all_gene_counts =  zeros(GO.Terms(end).id,1);
        cluster_gene_counts = zeros(GO.Terms(end).id,1);
        for i = 1:numel(all_genes)
            if isKey(go_map,all_genes{i})
                go_ids = getrelatives(GO,go_map(all_genes{i}));
                all_gene_counts(go_ids) = all_gene_counts(go_ids)+1;
                if (any(strcmp(all_genes{i},gene_list)))
                    cluster_gene_counts(go_ids) = cluster_gene_counts(go_ids)+1;
                end
            end
        end
        % compute p-values
        pvals = hygepdf(cluster_gene_count,max(all_gene_counts),max(cluster_gene_count),all_gene_counts);
        [dummy idx] = sort(pvalues);
        for i=1:10
            s = struct;
            s.term = char(num2goid(idx(i)));
            s.pval = pvalues(idx(i));
            s.raw_count = cluster_gene_count(idx(i));
            s.raw_total = cluster_gene_count(idx(i));
            annos{i} = s;
        end
end

%Construct a map of go terms given a struct of GO terms 
%
%Input: 
%   -ann: struct of GO terms
% Returns:
%       - a map indexed by genes of gene -> go terms
function [go_map] = Construct_GO_map(ann)
        go_map = containers.Map();
        for i=1:numel(ann)
            key = ann(i).DB_Object_Symbol;
            if isKey(go_map,key)
                go_map(key) = [go_map(key) ann(i).GOid];
            else
                go_map(key) = ann(i).GOid;
            end
        end
end

