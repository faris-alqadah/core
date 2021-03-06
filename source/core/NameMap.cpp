#include <fstream>

#include "../../headers/core/NameMap.h"

NameMap::NameMap(){}



NameMap::NameMap(string &_fileName){
 fileName = _fileName;
 numEntries=0;
 ifstream myfile;
 id = 0;
  myfile.open(fileName.c_str());
  //assert(myfile.is_open());
  if (myfile.is_open()){
    int lineNum = 0;
    string line;
    while (!myfile.eof()){
        getline(myfile, line);
        if(line == "###") break;
        mapping.push_back(line);
        lineNum++;
        numEntries++;
        
    }
    //make rev map
    for(int i=0; i < mapping.size(); i++)  revMap[mapping[i]] = i;

  }
  else{
      cerr<<"\nCoult not open NAME file!! "<<fileName<<endl<<"\n";
      exit(-1);
  }
}
NameMap::NameMap(string & _fileName, unsigned int n){
    fileName = _fileName;
 numEntries=n;
 ifstream myfile;
 id = 0;
  myfile.open(fileName.c_str());
  //assert(myfile.is_open());
  if (myfile.is_open()){
    int lineNum = 0;
    string line;
    while (!myfile.eof()){
        if(lineNum >= numEntries) break;
        getline(myfile, line);
        if(line == "###") break;
        mapping.push_back(line);
        lineNum++;
    }
    if(lineNum != numEntries){
        cerr<<"\nNumber of names does not match number of domains specified ("<<numEntries<<") in "<<fileName<<"\n";

        exit(-1);
    }
    //make rev map
    for(int i=0; i < mapping.size(); i++)  revMap[mapping[i]] = i;

  }
  else{
      cerr<<"\nCoult not open NAME file!! "<<fileName<<endl<<"\n";
      exit(-1);
  }
}
string NameMap::GetName(unsigned int x){
//    cout<<"\nx: "<<x<<"\t"<<mapping.size()<<"\t"<<fileName;
//    cout.flush();
//    cout<<"\ntest";
    assert ( x < mapping.size());
        return mapping[x];


}

int NameMap::NameToId(string& n){
    if (revMap.count(n)>0){
        return revMap[n];
    }
    return -1;
}

string NameMap::GetFileName(){return fileName;}
int NameMap::GetNumEntries(){return numEntries;}

void NameMap::SetId(int idd){id=idd;}
int  NameMap::GetId(){return id;}

 

