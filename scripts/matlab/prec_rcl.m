% Faris Alqadah
% Johns Hopkins University
%
% Computes precision, recall, false postive rate, true positive rate
%
%   Input:
%           actual: binary vector of predicted values
%           predict: binary vector of actual values
%    Returns:
%           prec,rcl,fpr,tpr
%
%
function [prec,rcl,fpr,tpr] = prec_rcl(actual,predict)
    predict = logical(predict);
    tp= numel(find(actual(predict)==1));
    fp = numel(find(actual(predict)==0));
    %tn = numel(find(actual(~predict)==0));
    %fn =  numel(find(actual(~predict)==1));
    P = numel(find(actual == 1));%tp+fn;
    N = numel(find(actual==0));%fp+tn;
    prec = tp/(tp+fp);
    rcl = tp/(P);
    tpr =tp/(P);
    fpr = fp/(N);
end