function obj = KEGG

obj.endpoint = 'http://soap.genome.jp/keggapi/request_v6.2.cgi';
obj.wsdl = 'http://soap.genome.jp/KEGG.wsdl';

obj = class(obj,'KEGG');

