function [ output_args ] = make_local_markov_mat(GI,GJ,Gj,psi_gi)
%make_local_markov_mat creates a sub-matrix of the markov mat, by fixing
%one of the domains
%
%        
%          GI: total number of objects in i domain
%          GJ: total number of objects in opposite domain (J)
%          Gj: size of subspace t in opposite domain (j)
%          psi_gi: size of psi_gi   
%
%
%
%
    markov_mat = zeros(GI,GI);
    for i=1:GI
        markov_mat(i,:) = prob_add_gi(i,GI,GJ,Gj,psi_gi); % fill in the rows with everything fixed except gi
    end
end

