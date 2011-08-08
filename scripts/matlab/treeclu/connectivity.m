% Author: Faris Alqadah
%
% Given a binary matrix and indices of a submatrix, computes connectivity,
% self-connectivity, expected connectivity, and expected self-connectivity
%
%  Inputs:
%         ctx: binary matrix
%         si: subspace i
%         sj: subspace j
%         alpha: the alpha factor to be applied to expected connectivity
%         delta: the delta 
%   Returns:
%           conn: vector with connectvity and self connectivity
%           econn: vector with expected connectivity and expected self connectivity
%           
%
function [conn,econn]= connectivity(ctx,si,sj,alpha,delta)
   sub = ctx(si,sj);
   conn(1) = connect(sub);
   conn(2) = conn(1);
   conn(3) = self_conn(sub);
   conn(4) = self_conn(sub');
   econn(1) = alpha*exp_conn(ctx,si,sj);
   econn(2) = alpha*exp_conn(ctx',sj,si);
   econn(3) = delta*exp_self_conn(ctx,si,sj);
   econn(4) = delta*exp_self_conn(ctx',sj,si);   
end

function sc = self_conn(sub)
    [x y] = size(sub);
    nrml = (nchoosek(x,2)*y)
    c=0;
    for i=1:x
        for j=i+1:x
            c = c+ numel(find(bitand(sub(i,:),sub(j,:))));
        end
    end
    sc = c/nrml;
    
end

function c = connect(sub)
   c= numel(find(sub == 1)) / numel(sub);
end

function ec = exp_conn(full,gi,gj)
    [x y] = size(full);
    sub = full(gi,:);
    ones = numel(find(sub == 1));
    ec = (numel(gj)/y)*ones;
    ec = ec/(numel(gi)*numel(gj));
end

function esc = exp_self_conn(full,gi,gj)
    [x y] = size(full);
    nrml = (nchoosek(numel(gi),2)*y^2);
    sub = full(gi,:);
    % first compute number of ones in each row
    ones_cnt = zeros(numel(gi),1);
    for i=1:numel(gi)
        ones_cnt(i) = numel(find(sub(i,:)));
    end
    ctr=0;
    max = numel(ones_cnt);
    % now compute directly from formula
    for i=1:max-1
        ctr = ctr+(ones_cnt(i)*(sum(ones_cnt(i+1:max))));
    end
    esc = ctr/nrml;
end