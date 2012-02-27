function data_logical_idxs = get_ppi_pairs_sym(row_names,edges,data_mat)
             
    % create map of row names to idxs
     map = containers.Map;
     data_logical_idxs=zeros(numel(row_names),numel(row_names));
     for i=1:numel(row_names)
         map(row_names{i}) = i;
     end
     % iterate through all pairs and if both pairs appear in map mark it in
     % the matrix
     for i=1:numel(edges)
         [v1,v2] = strtok(edges{i},',');
         v2 = v2(2:length(v2));
         if map.isKey(v1) && map.isKey(v2)
             data_logical_idxs(map(v1),map(v2)) =1;
             data_logical_idxs(map(v2),map(v1)) =1;
         end
         
     end
     % mark all NaNs
    nan_idxs = find(isnan(data_mat));
    data_logical_idxs(nan_idxs)=NaN;
end