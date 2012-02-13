function xReturn = get_pathways_by_genes(obj,genes_id_list)
%get_pathways_by_genes(obj,genes_id_list)
%
%     Input:
%       genes_id_list{:} = (string)
%   
%     Output:
%       return{:} = (string)

% Build up the argument lists.
values = { ...
   genes_id_list, ...
   };
names = { ...
   'genes_id_list', ...
   };
types = { ...
   '{http://www.w3.org/2001/XMLSchema}string[]', ...
   };

% Create the message, make the call, and convert the response into a variable.
soapMessage = createSoapMessage( ...
    'SOAP/KEGG', ...
    'get_pathways_by_genes', ...
    values,names,types,'rpc');
response = callSoapService( ...
    obj.endpoint, ...
    'SOAP/KEGG#get_pathways_by_genes', ...
    soapMessage);
xReturn = parseSoapResponse(response);