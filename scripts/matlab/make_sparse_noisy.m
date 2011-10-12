% Author: Faris Alqadah
%
% Fill a sparse matrix (NaN filled) with random noise generated over the
% interval of [max...min] where max and min are the maximum and minimum
% values of the matrix
%
%  Inputs:
%         
%           -data: the original dataset (matrix)
%  Returns:
%           -noisy_data: data filled in with noise
%
function noisy_data = make_sparse_noisy(data)
    max_v = max(max(data));
    min_v = min(min(data));
    noisy_data =min_v + (max_v-min_v).*rand(size(data));
    non_nan_idxs = find(~isnan(data));
    noisy_data(non_nan_idxs) = data(non_nan_idxs);
end