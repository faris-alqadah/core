function xReturn = get_glycans_by_pathway(obj,pathway_id)
%get_glycans_by_pathway(obj,pathway_id)
%
%     Input:
%       pathway_id = (string)
%   
%     Output:
%       return{:} = (string)

% Build up the argument lists.
values = { ...
   pathway_id, ...
   };
names = { ...
   'pathway_id', ...
   };
types = { ...
   '{http://www.w3.org/2001/XMLSchema}string', ...
   };

% Create the message, make the call, and convert the response into a variable.
soapMessage = createSoapMessage( ...
    'SOAP/KEGG', ...
    'get_glycans_by_pathway', ...
    values,names,types,'rpc');
response = callSoapService( ...
    obj.endpoint, ...
    'SOAP/KEGG#get_glycans_by_pathway', ...
    soapMessage);
xReturn = parseSoapResponse(response);
