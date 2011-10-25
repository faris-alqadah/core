% Author: Faris Alqadah
%
% Compute expected variance of sample variance of a row
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
%           -evsv: expected sample variance
%
%
function evsv = exp_var_sample_var(r,dist)
    if strcmp(dist,'normal')
        mu_2 = var(r);
        mu_4 = 3*mu_2^2;
        m = numel(r);
        lhs =  ((m -1)^2/m^3)*mu_4;
        rhs= ( (m-1)*(m-3)*mu_2^2)/m^3;
        evsv=lhs-rhs;
        
    else
        fprintf('\nUnknown distribution\n');
        evsv=0;
    end
end