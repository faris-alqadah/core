% Author: Faris Alqadah
%
% Given a binary matrix and indices of a submatrix, computes connectivity,
% self-connectivity, expected connectivity, and expected self-connectivity
%
%  Inputs:
%         ctx: binary matrix
%         si: subspace i
%         sj: subspace j
%   Returns:
%           conn: vector with connectvity and self connectivity
%           econn: vector with expected connectivity and expected self connectivity
%           
%
function [conn,econn]= connectivity(ctx,si,sj)
   sub = ctx(si,sj);
   conn(1) = 
   conn(2) = conn(1);
 
   
end

function sc = self_conn(sub)
    [x y] = size(sub);
    c=0;
    for i=1:x
        for j=i+1:x
            c = c+ numel(find(bitand(sub(x(i),:),sub(x(j),:))));
        end
    end
    
end

function c = conn(sub)
    numel(find(sub == 1)) / numel(sub);
end

function ec = exp_conn(sub)

end

function esc = exp_self_conn(sub)

end