% Author: Faris Alqadah
%
% Compute expected sample variance of a row
% 
% See "A Pattern-Based Approach to Multi-way Clustering" by Alqadah et al.
%   for details
%
%  Inputs:
%         -r: the row
%         -dist: the assumed distribution of the row (currently only
%               suppots normal)
%               options are:
%                        - 'normal'
%          
%  Returns: 
%           -esv: expected sample variance
%
%
function esv = exp_sample_var(r,dist)
    if strcmp(dist,'normal')
        mu_2 = var(r);
       %add other cases (other distributions here later) 
        esv = ( (numel(r)-1)/numel(r) ) * mu_2;
        
    else
        fprintf('\nUnknown distribution\n');
        esv=0;
    end
end