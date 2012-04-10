% Author: Faris Alqadah
% JHU
% Compute perecentage of clusters that are significantly GO-annotated.
% 
% Inputs:
%         -GO: the GO object
%         -go_map: a hash map that maps genes to GO terms
%         - all_gene_counts: back groudn vector with counts of GO terms in
%               background
%         -clusters: cell array of clusters
%         -domain: the domain within the cluster to check for annotation
%         -p_cutoff: pvalue cut off to determine siginifcance
%  Returns: 
%           -annos: an array of structs containing 
%                       -term (the GO term)
%                       -pval  (the p-value)
%                       -raw count (raw count of number of genes annotated)
%                       
%                       -raw total
%                       
%
function [percent] = clusters_GO_annotation(GO,go_map,all_gene_counts,clusters,domain,p_cutoff)
    anno_vec=[];
    for i=1:numel(clusters)
        cluster= clusters{i};
        curr_count= get_GO_counts(GO,go_map,cluster);
        annos = GO_annotation(all_gene_counts,curr_count,GO);
        fprintf('\ncluster %d ',i);
        for i=1:numel(annos)
            fprintf('\n\t%s\t%s',annos{i}.term,annos{i}.descrip);
        end
        anno_vec=[anno_vec is_annotated(annos,p_cutoff)];
    end
    percent = numel(find(anno_vec))/numel(anno_vec);    
    fprintf('\n');
end

function [annotated] = is_annotated(annos,p_cutoff)
    annotated=0;
    for i=1:numel(annos)
        if annos{i}.pval <= p_cutoff && annos{i}.raw_count >= 2
            annotated=1;
            break
        end
    end
end