#include "../headers/PreProcess.h"


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
              //get set names and thier name files
              for(int i=0; i < numDomains; i++){
                 getline(myfile,line); //this is the domain name
                 domainNames.push_back(line);
                 domainName_id_map[line]=i+1;
                 getline(myfile,line); //this is the path to the namefile
                 NameMap *nmp = new NameMap(line);
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
                  grph->AddContext(MakeContext(line,dId1,dId2,ctxName,i+1,nameMaps[dId1-1],nameMaps[dId2-1]));
              }
	      myfile.close();
              return grph;
          }
	  else{
              string errMsg="Could not open the input file: "+inputFile;
	  	Error(errMsg);
	  }

}

Context * MakeContext(string &inputFile,int dId1, int dId2, string &name, int ctxId, NameMap *nm1, NameMap *nm2){
    //first make the nclusters from the fimis
    NCluster *dmn1 = MakeNClusterFromFimi(inputFile);
    NCluster *dmn2 = TransposeFimi(dmn1);
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

NCluster *MakeNClusterFromFimi(string &inputFile){
     ifstream myfile(inputFile.c_str());
      if (myfile.is_open()){
          vector<IOSet *> sets;
          int cnt=0;
          string line;
          getline(myfile,line);
         while (! myfile.eof() ){
             vector<string> entries;
             Tokenize(line,entries," ");
             IOSet *t = new IOSet;
             for(int i=0; i < entries.size(); i++) t->Add(atoi(entries[i].c_str()));
             t->SetId(cnt);
             sets.push_back(t);
             cout<<"\n";
             t->Output();
             cnt++;
             getline(myfile,line);

        }
          cout<<"\nsets size: "<<sets.size();
        return new NCluster(sets.size(),sets);
  }else{
          string errMsg = "Could not open the FIMI file: "+inputFile;
          Error(errMsg);
  }
}