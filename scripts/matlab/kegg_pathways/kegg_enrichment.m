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

function  [pathways,pvals]=kegg_enrichment(cluster,domain,orgCode,totalGenes,gene_pathway_map,pathway_gene_map)
    % get all pathways encompassing cluster
    cluster_pathways = {};
    pathways={};
    pvals=[];
    %make kegg names
    cluster_c = cluster{domain};
    for i=1:numel(cluster_c)
       cluster_c{i} = [orgCode ':' cluster{domain}{i}]; 
    end
    for i=1:numel(cluster_c)
        if gene_pathway_map.isKey(cluster_c{i})
            cluster_pathways = union(cluster_pathways,gene_pathway_map(cluster_c{i}));
        end
    end
    % for each pathway compute number of genes it encompasses
    pathway_cnt=1;
    for i=1:numel(cluster_pathways)
       genes_all_path = pathway_gene_map(cluster_pathways{i});
       cluster_genes = intersect(genes_all_path,cluster_c);
       % compute hypergeometric p-value
       if numel(cluster_genes) > 0
        %fprintf('\n%s\tkegg fraction %d of %d\tcluster fraction %d of %d',cluster_pathways{i},numel(genes_all_path),totalGenes,numel(cluster_genes),numel(cluster));
        p=hygepdf(numel(cluster_genes),totalGenes,numel(genes_all_path),numel(cluster_c));
        pathways{pathway_cnt} = cluster_pathways{i};
        pvals = [pvals p];
        pathway_cnt = pathway_cnt+1;
       end
    end
end