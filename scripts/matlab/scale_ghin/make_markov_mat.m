function [ markov_mat ] = make_markov_mat(GI,GJ,psi_gi,psi_gj)
%Create a tranisition matrix from markov model modeling add rounds in
%map_reduce_ghin
%
%   Create a tranisition matrix from markov model modeling add rounds in
%   map_reduce_ghin by pairing all possible add states into a single matrix
%   and computing their transisiton probablity with prob_add_gi
%
markov_mat = []; % pair all states together
cnt=1;
for i=1:1
    p_a_k_Gi = prob_add_k_Gj(GJ,GI,psi_gj,i) % probablity add k objects to GI given size of Gj=1...GJ
    for j=1:1
         p_a_k_Gj = prob_add_k_Gj(GI,GJ,psi_gi,j) % probablity add k objects to GJ given size of Gi=1...GI
         p_a_ki_a_kj = p_a_k_Gi .* p_a_k_Gj';                              % joint probablities is product
         markov_mat(cnt,:) = p_a_ki_a_kj(:);
         cnt = cnt +1;
    end
    fprintf('\n %d of %d',i,GI);
end
   fprintf('\n');
end