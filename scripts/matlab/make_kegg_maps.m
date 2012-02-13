% Faris Alqadah
% Johns Hopkins University
%
% Make kegg maps
% 
%   Input:
%           gene_list: list of gene names assumed to be in correct kegg
%                        naming format
%           orgCode: the organisim code in KEGG
%         
%    Returns:
%           gene_pathway_map: map of genes to pathways
%           pathway_gene_map: map of pathways to genes
%
%
function [gene_pathway_map, pathway_gene_map] = make_kegg_maps(gene_list,orgCode)
    gene_pathway_map = containers.Map();
    pathway_gene_map = containers.Map();
    wsdlURL = 'http://soap.genome.jp/KEGG.wsdl';
    className = createClassFromWsdl(wsdlURL);
    kegg = KEGG;
    all_pathways = {};
    % construct the gene pathway map and
    % store all pathways that pretain to these genes
    for i=1:numel(gene_list)
        if mod(i,100) == 0
            fprintf('\n%d',i);
        end
        str = [orgCode ':' gene_list{i}];
        gene_pathways = get_pathways_by_genes(kegg,{str});
         if length(gene_pathways) > 0
           % add the pathways to the gene->pathways map  
            gene_pathway_map(str)=gene_pathways;
            % add this gene to pathway map
            if ~iscell(gene_pathways)
                gene_pathways = {gene_pathways};
            end
            for j=1:numel(gene_pathways)
                if ~pathway_gene_map.isKey(gene_pathways{j})
                    newAdd = {};
                    newAdd{1} = str;
                    pathway_gene_map(gene_pathways{j}) = newAdd;
                else
                   gg = pathway_gene_map(gene_pathways{j});
                   gg{numel(gg)+1} = str;
                   pathway_gene_map(gene_pathways{j}) = gg;
                end
            end
        end
    end
    fprintf('\n');

end