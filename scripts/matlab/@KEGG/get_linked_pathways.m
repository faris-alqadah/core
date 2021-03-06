function xReturn = get_linked_pathways(obj,pathway_id)
%get_linked_pathways(obj,pathway_id)
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
    'get_linked_pathways', ...
    values,names,types,'rpc');
response = callSoapService( ...
    obj.endpoint, ...
    'SOAP/KEGG#get_linked_pathways', ...
    soapMessage);
xReturn = parseSoapResponse(response);
