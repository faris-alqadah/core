% Faris Alqadah
% Johns Hopkins University
%
% Count number of clusters that have at least one KEGG pathway associated
% and returns their idxs
%
%   Input:
%           clusters: the cluster array assumed to have corr KEGG names etc

%         
%    Returns:
%            cnt,idxs
%
%

function [cnt,idxs] = count_clusters_pathways(clusters)
    wsdlURL = 'http://soap.genome.jp/KEGG.wsdl';
    className = createClassFromWsdl(wsdlURL);
    kegg = KEGG;
    cnt =0;
    idxs=[];
    for i=1:numel(clusters)
        if mod(i,50) == 0
            fprintf('\n%d\t%d',i,cnt);
            idxs
        end
        pathways = get_pathways_by_genes(kegg, clusters{i});
        if length(pathways) > 0
            cnt = cnt+1;
            idxs = [idxs i];
        end
    end
    fprintf('\n');
end