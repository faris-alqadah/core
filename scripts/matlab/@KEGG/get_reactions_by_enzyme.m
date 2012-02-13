function xReturn = get_reactions_by_enzyme(obj,enzyme_id)
%get_reactions_by_enzyme(obj,enzyme_id)
%
%     Input:
%       enzyme_id = (string)
%   
%     Output:
%       return{:} = (string)

% Build up the argument lists.
values = { ...
   enzyme_id, ...
   };
names = { ...
   'enzyme_id', ...
   };
types = { ...
   '{http://www.w3.org/2001/XMLSchema}string', ...
   };

% Create the message, make the call, and convert the response into a variable.
soapMessage = createSoapMessage( ...
    'SOAP/KEGG', ...
    'get_reactions_by_enzyme', ...
    values,names,types,'rpc');
response = callSoapService( ...
    obj.endpoint, ...
    'SOAP/KEGG#get_reactions_by_enzyme', ...
    soapMessage);
xReturn = parseSoapResponse(response);
