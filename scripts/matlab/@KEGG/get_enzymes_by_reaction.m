function xReturn = get_enzymes_by_reaction(obj,reaction_id)
%get_enzymes_by_reaction(obj,reaction_id)
%
%     Input:
%       reaction_id = (string)
%   
%     Output:
%       return{:} = (string)

% Build up the argument lists.
values = { ...
   reaction_id, ...
   };
names = { ...
   'reaction_id', ...
   };
types = { ...
   '{http://www.w3.org/2001/XMLSchema}string', ...
   };

% Create the message, make the call, and convert the response into a variable.
soapMessage = createSoapMessage( ...
    'SOAP/KEGG', ...
    'get_enzymes_by_reaction', ...
    values,names,types,'rpc');
response = callSoapService( ...
    obj.endpoint, ...
    'SOAP/KEGG#get_enzymes_by_reaction', ...
    soapMessage);
xReturn = parseSoapResponse(response);
