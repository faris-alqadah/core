% Author: Faris Alqadah
%
%  Creates network (s) (bi-partite adjacency matrix) from star shaped
%  n-clusters, assuming the first domain read in is central node. Network
%  consists of edges of those nodes that are linked in an n-cluster.
%
%  Inputs:
%         -ncluster_file: the file containing nclusters
%         -domain_sizes: sizes of each domain 
%         
%          
%  Returns: 
%           -networks: cell array of matrices representing networks.
%
%  
%
%
function [networks] = make_networks_from_ncluster(ncluster_file,domain_sizes)

    networks = cell(1,numel(domain_sizes)-1);
    for i=2:numel(domain_sizes)
        networks{i-1} = zeros(domain_sizes(1),domain_sizes(i));
    end
    [clusters names] = read_nclusters(ncluster_file,ncluster_file);
    for i=1:numel(clusters)
        for j=1:numel(domain_sizes)-1
            networks{j}(cell2mat(clusters{i}{1}),cell2mat(clusters{i}{j+1}))=1;
        end
    end    
end