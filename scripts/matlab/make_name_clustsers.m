% Faris Alqadah
% Johns Hopkins University
%
% Maps n-cluster ids to names for given domain and given name-map
%
%   Input:
%           cluster: the cluster with object ids (cell array)
%           name_map: array with each entry corresponding to id and name
%           (cell array)
%         
%    Returns:
%            cluster_names
%
%

function clusters_names = make_name_clustsers(clusters,domain_id,name_map)
    clusters_names ={};
    for i=1:numel(clusters)
        lcl_cluster = clusters{i}{domain_id};
        clusters_names{i} = {};
         for j=1:numel(lcl_cluster)
            clusters_names{i}{j} = ['sce:' name_map{lcl_cluster{j}}];
         end
    end

end