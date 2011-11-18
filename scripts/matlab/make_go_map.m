% Author: Faris Alqadah
% JHU
% 
%  Make a map that maps genes to GO terms
%
% Inputs:
%         -GO: the GO object
%          sgd_file: the file contaiing the sgd (or any other gene
%          association file)
%  Returns: 
%          go_map: the map
%                       
%
function [ go_map] = make_go_map(GO,sgd_file)
      ann = goannotread(sgd_file,'Aspect','F','Fields',{'DB_Object_Symbol','GOid'});
      go_map = Construct_GO_map(ann);
end

%Construct a map that maps genes to GO terms 
%
%Input: 
%   -ann: struct of GO terms
% Returns:
%       - a map indexed by genes of gene -> go terms
function [go_map] = Construct_GO_map(ann)
        go_map = containers.Map();
        for i=1:numel(ann)
            key = ann(i).DB_Object_Symbol;
            if isKey(go_map,key)
                go_map(key) = [go_map(key) ann(i).GOid];
            else
                go_map(key) = ann(i).GOid;
            end
        end
end