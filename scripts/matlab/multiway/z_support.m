% Author: Faris Alqadah
%
% Compute z-support of a subspace in a data matrix. Assumes K is a full
% matrix.
% 
% See "A Pattern-Based Approach to Multi-way Clustering" by Alqadah et al.
%   for details
%
%  Inputs:
%         -K: the data matrix
%         -sub: an array {G,M} that contains two vectors representing the
%               subpsace
%         -dist: the assumed row distribution (currently only
%                supports 'normal'
%               options are:
%                        - 'normal'
%          
%  Returns: 
%           -z_sup: the z support of the subspace
%
%
function z_sup = z_support(K,sub,dist)
    [x,y] = size(K);
    if strcmp(dist,'normal')
        G = sub{1};
        M = sub{2};
        esv=[];
        evsv=[];
        for i=1:numel(G)
            r(i) = range(K(G(i),M));
            esv(i) = exp_sample_var(K(G(i),1:y),dist);
            evsv(i) = exp_var_sample_var(K(G(i),1:y),dist);
        end
        v = sum(r)
        ev = sum(esv)
        stddev = sum(evsv)
        if v <= ev
            z_sup = abs(v-ev)/stddev;
        else
            z_sup = 0;
        end 
    else
        fprintf('\nUnknown distribution\n');
        z_sup=0;
    end
   
end