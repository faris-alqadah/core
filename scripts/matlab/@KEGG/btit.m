function xReturn = btit(obj,string)
%btit(obj,string)
%
%     Input:
%       string = (string)
%   
%     Output:
%       return = (string)

% Build up the argument lists.
values = { ...
   string, ...
   };
names = { ...
   'string', ...
   };
types = { ...
   '{http://www.w3.org/2001/XMLSchema}string', ...
   };

% Create the message, make the call, and convert the response into a variable.
soapMessage = createSoapMessage( ...
    'SOAP/KEGG', ...
    'btit', ...
    values,names,types,'rpc');
response = callSoapService( ...
    obj.endpoint, ...
    'SOAP/KEGG#btit', ...
    soapMessage);
xReturn = parseSoapResponse(response);
