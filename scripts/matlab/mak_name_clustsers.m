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

function cluster_names = make_name_clustsers(cluster,name_map)
    cluster_name ={};
    for i=1:numel(cluster)
        cluster_name{i} = name_map{cluster{i}};
    end

end