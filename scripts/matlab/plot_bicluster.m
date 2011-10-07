% Author: Faris Alqadah
%
% Display a bi-cluster (real valued) as both a heat map and
% "expresssion" plot
%
%  Inputs:
%         
%           -bicluster: a matrix representing the bi-cluster
%
%                          
function plot_bicluster(bicluster)
    figure(1)
    plot(bicluster','-x','MarkerSize',16);
    figure(2)
    image(bicluster*100);
    

end