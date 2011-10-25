% Author: Faris Alqadah
%
% Utility function for plotting different lines on same plot. Given an
% integer will vary marker style and line color accordingly
%
%  Inputs:
%         plot_num: integer
%          
%  Returns: a plot style
%
%  Outputs: graph of precis
%
%
function plot_style = get_plot_style(plot_num)
    markers = ['+';'o';'*';'.';'x';'s';'d';'^';'v';'>';'<';'p';'h'];
    colors = ['r';'g';'b';'c';'m';'y';'k'];
    plot_style = ['-' colors(mod(plot_num,numel(colors))+1) markers( mod(plot_num,numel(markers))+1)];
end