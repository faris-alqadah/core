function [ prob_vector ] = prob_add_k_Gj(GI,GJ,psi_gi,Gi)
%Computes matrix of probablities for adding k objects to a domain given
% that opposite subsace as Gj objects
%probablity of adding Gi..GI objects

  
    priors = 1-hygepdf(0,GJ,1:GJ,psi_gi); % condition probablity p(find one link | size(Gj))
  
    for i=1:numel(priors)
        prob_vector(i,1) = binopdf(0,GI-Gi,priors(i));
        prob_vector(i,2:GI) = binopdf(1:GI-1,GI-Gi,priors(i));
    end
    %prob_vector = prob_vector;
    prob_vector(find(isnan(prob_vector))) = 0;

end


