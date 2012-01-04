function [ markov_mat ] = make_markov_mat(GI,GJ)
%Create a tranisition matrix from markov model modeling add rounds in
%map_reduce_ghin
%
%   Create a tranisition matrix from markov model modeling add rounds in
%   map_reduce_ghin by pairing all possible add states into a single matrix
%   and computing their transisiton probablity with prob_add_gi
%
markov_mat = zeros(GI*GJ,GI*GJ); % pair all states together

prob_add_gi(Gi,GI,GJ,Gj,psi_gi)
    
end