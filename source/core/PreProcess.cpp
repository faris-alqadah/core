#include "../../headers/core/PreProcess.h"


void Tokenize(const string& str, vector<string>& tokens,
                      const string& delimiters){
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)  {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
void Error(string &message){
    cerr<<"\n"<<message<<"\n";
    exit(-1);
}

RelationGraph* MakeRelationGraph(string &inputFile){
          ifstream myfile(inputFile.c_str());
          if (myfile.is_open()){
              string line;
              getline (myfile,line);
              int numDomains = atoi(line.c_str()); //get the number of domains
              getline (myfile,line);
              int numContexts = atoi(line.c_str()); //get the number of contexts
              if(numContexts + 1 != numDomains) {
                  string errMsg = "Number of domains and number of contexts are not consistent";
                  Error(errMsg);
              }
              vector<string> domainNames;
              map<string,int> domainName_id_map;
              vector<NameMap*> nameMaps;
              map<int,int> domainId_size_map; //map domain number to number of elements
              //get set names and thier name files
              for(int i=0; i < numDomains; i++){
                 getline(myfile,line); //this is the domain name
                 
                 vector<string> tkns;
                 Tokenize(line, tkns, ";");
                 //first token is name of domain
                 domainNames.push_back(tkns[0]);
                 //second token is number of elements
                 domainId_size_map[i+1] = atoi(tkns[1].c_str());
                 domainName_id_map[tkns[0]]=i+1;
                 getline(myfile,line); //this is the path to the namefile
                 NameMap *nmp = new NameMap(line,atoi(tkns[1].c_str()));
                 nmp->SetId(i+1);
                 nameMaps.push_back(nmp);
               }
               //now get contexts and relation graph
              RelationGraph *grph = new RelationGraph();
              for(int i=0; i < numContexts; i++){
                  getline(myfile,line); //this line specifies the two domains
                  vector<string> currDomainNames;
                  string ctxName;
                  Tokenize(line,currDomainNames,"--");
                  int dId1 = domainName_id_map[currDomainNames[0]];
                  //check if a new element was inserted indicating an error
                  if(domainName_id_map.size() > numDomains) {
                      string errMsg = "Error specifying context..."+currDomainNames[0]+" does not match any previoulsy defined domain";
                      Error(errMsg);
                  }
                  int dId2 = domainName_id_map[currDomainNames[1]];
                  //check if a new element was inserted indicating an error
                  if(domainName_id_map.size() > numDomains) {
                      string errMsg = "Error specifying context..."+currDomainNames[1]+" does not match any previoulsy defined domain";
                      Error(errMsg);
                  }
                  ctxName = currDomainNames[0]+"__"+currDomainNames[1];
                  getline(myfile,line);  //this line specifies the fimi file
                  grph->AddContext(MakeContext(line,dId1,dId2,ctxName,i+1,nameMaps[dId1-1],nameMaps[dId2-1],domainId_size_map[dId1],domainId_size_map[dId2]));
              }
	      myfile.close();
              return grph;
          }
	  else{
              string errMsg="Could not open the input file: "+inputFile;
	  	Error(errMsg);
	  }

}


RelationGraphR* MakeRelationGraphR(string &inputFile){
          ifstream myfile(inputFile.c_str());
          if (myfile.is_open()){
              string line;
              getline (myfile,line);
              int numDomains = atoi(line.c_str()); //get the number of domains
              getline (myfile,line);
              int numContexts = atoi(line.c_str()); //get the number of contexts
              if(numContexts + 1 != numDomains) {
                  string errMsg = "Number of domains and number of contexts are not consistent";
                  Error(errMsg);
              }
              vector<string> domainNames;
              map<string,int> domainName_id_map;
              map<int,int> domainId_size_map; //map domain number to number of elements
              vector<NameMap*> nameMaps;
              //get set names and thier name files
              for(int i=0; i < numDomains; i++){
                 getline(myfile,line); //this is the domain name
                 //the line is split into name and number of elements
                 vector<string> tkns;
                 Tokenize(line, tkns, ";");
                 //first token is name of domain
                 domainNames.push_back(tkns[0]);
                 //second token is number of elements
                 domainId_size_map[i+1] = atoi(tkns[1].c_str());
                 domainName_id_map[tkns[0]]=i+1;
                 getline(myfile,line); //this is the path to the namefile
                NameMap *nmp = new NameMap(line,atoi(tkns[1].c_str()));
                 nmp->SetId(i+1);
                 nameMaps.push_back(nmp);
               }
               //now get contexts and relation graph
              RelationGraphR *grph = new RelationGraphR();
              for(int i=0; i < numContexts; i++){
                  getline(myfile,line); //this line specifies the two domains
                  vector<string> currDomainNames;
                  string ctxName;
                  Tokenize(line,currDomainNames,"--");
                  int dId1 = domainName_id_map[currDomainNames[0]];
                  //check if a new element was inserted indicating an error
                  if(domainName_id_map.size() > numDomains) {
                      string errMsg = "Error specifying context..."+currDomainNames[0]+" does not match any previoulsy defined domain";
                      Error(errMsg);
                  }
                  int dId2 = domainName_id_map[currDomainNames[1]];
                  //check if a new element was inserted indicating an error
                  if(domainName_id_map.size() > numDomains) {
                      string errMsg = "Error specifying context..."+currDomainNames[1]+" does not match any previoulsy defined domain";
                      Error(errMsg);
                  }
                  ctxName = currDomainNames[0]+"__"+currDomainNames[1];
                  getline(myfile,line);  //this line specifies the fimi file
                  grph->AddRContext(MakeRContext(line,dId1,dId2,ctxName,i+1,nameMaps[dId1-1],nameMaps[dId2-1], domainId_size_map[dId1],domainId_size_map[dId2]));
              }
	      myfile.close();
              return grph;
          }
	  else{
              string errMsg="Could not open the input file: "+inputFile;
              Error(errMsg);
	  }
}



Context * MakeContext(string &inputFile,int dId1, int dId2, string &name, int ctxId, NameMap *nm1, NameMap *nm2,int sz1, int sz2){
    //first make the nclusters from the fimis
    NCluster *dmn1 = MakeNClusterFromFimi(inputFile,sz1);
    NCluster *dmn2 = TransposeFimi(dmn1,sz1,sz2);
    dmn1->SetId(dId1);
    dmn2->SetId(dId2);
    Context *ret = new Context(dmn1,dmn2);
    ret->SetName(name);
    ret->SetId(ctxId);
    ret->SetNameMap(dId1,nm1);
    ret->SetNameMap(dId2,nm2);
    ret->SetDomainId(0,dId1);
    ret->SetDomainId(1,dId2);
    return ret;

}

RContext * MakeRContext(string &inputFile,int dId1, int dId2, string &name, int ctxId, NameMap *nm1, NameMap *nm2,int sz1, int sz2){
    NRCluster *dmn1 = MakeNRClusterFromSparseFile(inputFile,sz1);
    
    NRCluster *dmn2 = TransposeSparse(dmn1,sz1,sz2);
    dmn1->SetId(dId1);
    dmn2->SetId(dId2);
    RContext *ret = new RContext(dmn1,dmn2);
    ret->SetName(name);
    ret->SetId(ctxId);
    ret->SetNameMap(dId1,nm1);
    ret->SetNameMap(dId2,nm2);
    ret->SetDomainId(0,dId1);
    ret->SetDomainId(1,dId2);
    return ret;
}

NCluster *MakeNClusterFromFimi(string &inputFile, int sz){
     ifstream myfile(inputFile.c_str());
      if (myfile.is_open()){
          vector<IOSet *> sets;
          int cnt=0;
          for (string line; getline(myfile, line);) {
                if(line == "###" ) break;
               // if(cnt >= sz) break;
                vector<string> entries;
                Tokenize(line,entries," ");
                IOSet *t = new IOSet;
                for(int i=0; i < entries.size(); i++) t->Add(atoi(entries[i].c_str()));
                t->SetId(cnt);
                sets.push_back(t);
                cnt++;
          }
        return new NCluster(sets.size(),sets);
  }else{
          string errMsg = "Could not open the FIMI file: "+inputFile;
          Error(errMsg);
  }
}

NRCluster *MakeNRClusterFromSparseFile(string &inputFile, int sz){
    ifstream myfile(inputFile.c_str());
      if (myfile.is_open()){
          vector<RSet *> sets;
          int cnt=0;
          string line;
          while(line != "###" && !myfile.eof() && cnt < sz ) {
              getline(myfile,line);
              if (line == "###") break;
              vector<string> firstEntries; //these are the comma seperated values
                Tokenize(line,firstEntries,",");
                RSet *t = new RSet;
                for(int i=0; i < firstEntries.size(); i++){
                    vector<string> pairEntry; //this is the index value pair
                    Tokenize(firstEntries[i],pairEntry," ");
                    pair<int,double> ee;
                    ee.first = atoi(pairEntry[0].c_str());
                    ee.second = atof(pairEntry[1].c_str());
                    t->Add(ee);
                }
                t->SetId(cnt);
                sets.push_back(t);
                cnt++;
          }
        return new NRCluster(sets.size(),sets);
  }else{
          string errMsg = "Could not open the sparse file: "+inputFile;
          Error(errMsg);
  }
}