function xReturn = get_genes_by_motifs(obj,motif_id_list,offset,limit)
%get_genes_by_motifs(obj,motif_id_list,offset,limit)
%
%     Input:
%       motif_id_list{:} = (string)
%       offset = (int)
%       limit = (int)
%   
%     Output:
%       return{:} = (Definition)

% Build up the argument lists.
values = { ...
   motif_id_list, ...
   offset, ...
   limit, ...
   };
names = { ...
   'motif_id_list', ...
   'offset', ...
   'limit', ...
   };
types = { ...
   '{http://www.w3.org/2001/XMLSchema}string[]', ...
   '{http://www.w3.org/2001/XMLSchema}int', ...
   '{http://www.w3.org/2001/XMLSchema}int', ...
   };

% Create the message, make the call, and convert the response into a variable.
soapMessage = createSoapMessage( ...
    'SOAP/KEGG', ...
    'get_genes_by_motifs', ...
    values,names,types,'rpc');
response = callSoapService( ...
    obj.endpoint, ...
    'SOAP/KEGG#get_genes_by_motifs', ...
    soapMessage);
xReturn = parseSoapResponse(response);
