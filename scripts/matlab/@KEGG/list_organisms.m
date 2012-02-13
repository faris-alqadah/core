function xReturn = list_organisms(obj)
%list_organisms(obj)
%
%     Input:
%   
%     Output:
%       return{:} = (Definition)

% Build up the argument lists.
values = { ...
   };
names = { ...
   };
types = { ...
   };

% Create the message, make the call, and convert the response into a variable.
soapMessage = createSoapMessage( ...
    'SOAP/KEGG', ...
    'list_organisms', ...
    values,names,types,'rpc');
response = callSoapService( ...
    obj.endpoint, ...
    'SOAP/KEGG#list_organisms', ...
    soapMessage);
xReturn = parseSoapResponse(response);
