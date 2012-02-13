% Faris Alqadah
% Johns Hopkins University
%
% Plots precision-recall curve and ROC curve given prec,rcl,fpr, and tpr
% vectors
% 
%
%  Inputs:
%         -prec: vector of precision values
%         -rec: vector of recall values
%         -tpr: vector of true positive rate
%         -fpr: vector of false positive rate
%         -curve_num: what number curve this (for mulitple curves on same
%         plot)
%          
%  Returns: none
%
%  Outputs: precision recall curve, ROC cure
function prec_rec_ROC_curve(prec,rcl,tpr,fpr,curve_num)
    
    figure(1);
    hold on;
    plot(rcl,prec,get_plot_style(curve_num),'MarkerSize',10,'LineWidth',2);
    ylabel('Precision');
    xlabel('Recall');
    %axis([0 1 0 1]);
    figure(2);
    hold on;
    plot(fpr,tpr,get_plot_style(curve_num),'MarkerSize',5,'LineWidth',2)
    hold on;
    %plot([0 1],[0 1],'k','LineWidth',3);
    xlabel('FPR');
    ylabel('TPR');
    axis([0 0.1 0 1])
end