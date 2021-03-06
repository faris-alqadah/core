% Author: Faris Alqadah
%
% Create precision recall curve given predicitoins, and labels
%
%  Inputs:
%         -predict: a matrix of predictions where each column is the
%           prediction for a certain 'threshold level'
%         -label: the class labels
%         -curve_num: which number curve to plot as (this is in case
%         multiple lines will be on same figure
%          
%  Returns: prec : vector of precision
%           rcl: vector of recall
%
%  Outputs: graph of precis
%
%
function [prec rcl] = make_prec_rec_curve(predict,label,curve_num)
    if curve_num > 1
        hold on;
    end
    [x y] = size(predict);
    for i=1:y
         [prec(i) rcl(i) fpr tpr] = prec_rcl(predict(:,i),label);      
    end
    plot(rcl,prec,get_plot_style(curve_num),'MarkerSize',10,'LineWidth',2);
    xlabel('Recall');
    ylabel('Precision');
    grid on;
end