% Author: Faris Alqadah
% JHU
% 
%  Computes vectors of GO annotation counts for a list of genes
%
% Inputs:
%         -GO: the GO object
%          -go_map: a map mapping go_ids to a go term
%         -gene_list: the list of genes (using sgd)  
%  Returns: 
%          gene_count: the vector of counts
%                       
%
function [ gene_counts] = get_GO_counts(GO,go_map,gene_list)
        gene_counts =  zeros(GO.Terms(end).id,1);
        for i = 1:numel(gene_list)
            if isKey(go_map,gene_list{i})
                go_ids = getrelatives(GO,go_map(gene_list{i}));
                gene_counts(go_ids) = gene_counts(go_ids)+1;
            end
        end
end

