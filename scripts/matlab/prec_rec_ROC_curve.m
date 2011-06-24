% Faris Alqadah
% Johns Hopkins University
%
% Plots precision-recall curve and ROC curve given prec,rcl,fpr, and tpr
% vectors
%
%
function prec_rec_ROC_curve(prec,rcl,tpr,fpr)
    
    figure(1);
    hold on;
    plot(rcl,prec,'-x','MarkerSize',10);
    ylabel('Precision');
    xlabel('Recall');
    axis([0 1 0 1]);
    figure(2);
    hold on;
    plot(fpr,tpr,'-x',[0 1],[0 1],'k','MarkerSize',10);
    xlabel('FPR');
    ylabel('TPR');
    axis([0 1 0 1])
end