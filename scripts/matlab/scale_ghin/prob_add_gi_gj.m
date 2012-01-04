function [ prob_vector ] = prob_add_gi_gj(GI,GJ,psi_gi,psi_gj)
%prob_add_gi Function to aid in analyzing map_reduce_GHIN which computes
%probablity of adding Gi..GI objects
%   Compute probablity of adding Gi+1..GI bjects during a map_reduce_GHIN round using a hyper-geometric  
%   distribution
%   Input:
%          Gi: number of objects in subspace of current domain (i)
%          GI: total number of objects in i domain
%          GJ: total number of objects in opposite domain (J)
%          Gj: size of subspace t in opposite domain (j)
%          psi_gi: size of psi_gi
%           
%
    % conditional probablity of adding 0:GI-1 to gi given j objects in j domain
    i_vec=[];
    for j=0:GJ-1 
        i_vec=[i_vec; hygepdf((0:GI-1),GJ,j,psi_gi)];
    end
    size(i_vec)
    % probablity of adding 0:GJ-1 to gj given i objects in i domain
    j_vec=[];
    for i=0:GI-1
       j_vec = [j_vec; hygepdf((0:GJ-1),GI,i,psi_gj)];
    end
    size(j_vec)
    tmp = i_vec' .* j_vec;
    prob_vector=tmp(:);

    
end


