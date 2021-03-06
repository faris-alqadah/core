% Faris Alqadah
% Johns Hopkins University
%
% Count number of clusters that have at least one KEGG pathway associated
% and returns their idxs
%
%   Input:
%      
%         
%    Returns:
%         
%
%

function  [pathways,pvals,cnt,cluster_idxs]=clusters_kegg_enrichment(clusters,domain,orgCode,totalGenes,gene_pathway_map,pathway_gene_map,p_cutoff)
    cnt = 0;
    pathways={};
    pvals = {};
    cluster_idxs=[];
    for i=1:numel(clusters)
        [l_pathways, l_pvals] = kegg_enrichment(clusters{i},domain,orgCode,totalGenes,gene_pathway_map,pathway_gene_map);
        if numel(l_pathways) > 0
            c_idxs = find(l_pvals <= p_cutoff);
            if numel(c_idxs) >= 1
                cnt = cnt +1;
                pathways{cnt} = l_pathways;
                pvals{cnt} = {l_pvals};
                cluster_idxs = [cluster_idxs i];
            end
        end
    end
end