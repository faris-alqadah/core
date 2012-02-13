function xReturn = get_motifs_by_gene(obj,genes_id,db)
%get_motifs_by_gene(obj,genes_id,db)
%
%     Input:
%       genes_id = (string)
%       db = (string)
%   
%     Output:
%       return{:} = (MotifResult)

% Build up the argument lists.
values = { ...
   genes_id, ...
   db, ...
   };
names = { ...
   'genes_id', ...
   'db', ...
   };
types = { ...
   '{http://www.w3.org/2001/XMLSchema}string', ...
   '{http://www.w3.org/2001/XMLSchema}string', ...
   };

% Create the message, make the call, and convert the response into a variable.
soapMessage = createSoapMessage( ...
    'SOAP/KEGG', ...
    'get_motifs_by_gene', ...
    values,names,types,'rpc');
response = callSoapService( ...
    obj.endpoint, ...
    'SOAP/KEGG#get_motifs_by_gene', ...
    soapMessage);
xReturn = parseSoapResponse(response);