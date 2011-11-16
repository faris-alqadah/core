% Faris Alqadah
% Johns Hopkins University
%
% Reads in ncluster files produced by the "Data Mining Research Code"
% project (https://github.com/faris-alqadah/Data-Mining-Research-Code)
%
%   Input:
%           file: the ncluster file
%           name_file: ncluster file with names of clusters
%    Returns:
%           clusters:
%               a cell-array of cell arrays representing each n-cluster
%           names:
%               a cell-array of the names of the objects for all clusters
%
%
function [clusters,names] = read_nclusters(file,name_file)
    fid(1) = fopen(file,'r');
    fid(2) = fopen(name_file,'r');
    clusters = {};
    names={};
    
    for i=1:2
        lcl_cluster = {}; % store local clusters
        lcl_cnt=1;
        total_cnt=1;
        line = fgetl(fid(i));
        while ischar(line)
            if strcmp(line,';;;') % end of a cluster add it
                if i==1
                    clusters{total_cnt} = lcl_cluster;
                else
                    names{total_cnt} = lcl_cluster;
                end
                total_cnt = total_cnt+1;
                lcl_cnt=1;
                lcl_cluster={};
            else
                lcl_cluster{lcl_cnt} = get_curr_row(line,i);
                lcl_cnt = lcl_cnt+1;
            end
            line = fgetl(fid(i));
        end
        fclose(fid(i));
    end    
end


%%%utility functions%%%

%returns the numbers from the curr row
function row = get_curr_row(line,ii)
    row={};
    i=1;
    [str, remain] = strtok(line); % this is first character, just the index so ignore
    while true
        [tok, remain] = strtok(remain);
        if isempty(tok)  
            break;  
        end
        if ii == 1
            row{i} = str2num(tok)+1;
        else
            row{i} = tok;
        end
        
        i=i+1;  
    end

end
