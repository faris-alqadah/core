function xReturn = search_glycans_by_kcam(obj,kcf,program,option,offset,limit)
%search_glycans_by_kcam(obj,kcf,program,option,offset,limit)
%
%     Input:
%       kcf = (string)
%       program = (string)
%       option = (string)
%       offset = (int)
%       limit = (int)
%   
%     Output:
%       return{:} = (StructureAlignment)

% Build up the argument lists.
values = { ...
   kcf, ...
   program, ...
   option, ...
   offset, ...
   limit, ...
   };
names = { ...
   'kcf', ...
   'program', ...
   'option', ...
   'offset', ...
   'limit', ...
   };
types = { ...
   '{http://www.w3.org/2001/XMLSchema}string', ...
   '{http://www.w3.org/2001/XMLSchema}string', ...
   '{http://www.w3.org/2001/XMLSchema}string', ...
   '{http://www.w3.org/2001/XMLSchema}int', ...
   '{http://www.w3.org/2001/XMLSchema}int', ...
   };

% Create the message, make the call, and convert the response into a variable.
soapMessage = createSoapMessage( ...
    'SOAP/KEGG', ...
    'search_glycans_by_kcam', ...
    values,names,types,'rpc');
response = callSoapService( ...
    obj.endpoint, ...
    'SOAP/KEGG#search_glycans_by_kcam', ...
    soapMessage);
xReturn = parseSoapResponse(response);
