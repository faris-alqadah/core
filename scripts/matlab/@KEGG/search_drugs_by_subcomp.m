function xReturn = search_drugs_by_subcomp(obj,mol,offset,limit)
%search_drugs_by_subcomp(obj,mol,offset,limit)
%
%     Input:
%       mol = (string)
%       offset = (int)
%       limit = (int)
%   
%     Output:
%       return{:} = (StructureAlignment)

% Build up the argument lists.
values = { ...
   mol, ...
   offset, ...
   limit, ...
   };
names = { ...
   'mol', ...
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
    'search_drugs_by_subcomp', ...
    values,names,types,'rpc');
response = callSoapService( ...
    obj.endpoint, ...
    'SOAP/KEGG#search_drugs_by_subcomp', ...
    soapMessage);
xReturn = parseSoapResponse(response);
