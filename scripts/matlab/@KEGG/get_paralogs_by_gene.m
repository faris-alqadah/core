function xReturn = get_paralogs_by_gene(obj,genes_id,offset,limit)
%get_paralogs_by_gene(obj,genes_id,offset,limit)
%
%     Input:
%       genes_id = (string)
%       offset = (int)
%       limit = (int)
%   
%     Output:
%       return{:} = (SSDBRelation)

% Build up the argument lists.
values = { ...
   genes_id, ...
   offset, ...
   limit, ...
   };
names = { ...
   'genes_id', ...
   'offset', ...
   'limit', ...
   };
types = { ...
   '{http://www.w3.org/2001/XMLSchema}string', ...
   '{http://www.w3.org/2001/XMLSchema}int', ...
   '{http://www.w3.org/2001/XMLSchema}int', ...
   };

% Create the message, make the call, and convert the response into a variable.
soapMessage = createSoapMessage( ...
    'SOAP/KEGG', ...
    'get_paralogs_by_gene', ...
    values,names,types,'rpc');
response = callSoapService( ...
    obj.endpoint, ...
    'SOAP/KEGG#get_paralogs_by_gene', ...
    soapMessage);
xReturn = parseSoapResponse(response);
