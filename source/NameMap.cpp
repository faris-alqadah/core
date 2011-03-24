#include <fstream>

#include "../headers/NameMap.h"

NameMap::NameMap(){}

NameMap::NameMap(string _fileName, unsigned int _numEntries){
  fileName = _fileName;
  numEntries=_numEntries;
  ifstream myfile;
  mapping.resize(numEntries);
  id = 0;
  myfile.open(fileName.c_str());
  assert(myfile.is_open() && numEntries > 0);
  if (myfile.is_open()){
    int lineNum = 0;
    for (string line; getline(myfile, line);) {
        if(lineNum == numEntries) break;
        getline (myfile,line);
        mapping[lineNum] = line;
        lineNum++;
    }
  }
  else{
      cerr<<"\nCoult not open NAME file!! "<<fileName<<endl<<"\n";
      exit(-1);
  }    
}

NameMap::NameMap(string &_fileName){
 fileName = _fileName;
 numEntries=0;
 ifstream myfile;
 id = 0;
  myfile.open(fileName.c_str());
  assert(myfile.is_open());
  if (myfile.is_open()){
    int lineNum = 0;
    for (string line; getline(myfile, line);) {
        mapping.push_back(line);
        lineNum++;
        numEntries++;
    }
  }
  else{
      cerr<<"\nCoult not open NAME file!! "<<fileName<<endl<<"\n";
      exit(-1);
  }
}
string NameMap::GetName(unsigned int x){
    assert ( x < numEntries);
        return mapping[x];


}

string NameMap::GetFileName(){return fileName;}
int NameMap::GetNumEntries(){return numEntries;}

void NameMap::SetId(int idd){id=idd;}
int  NameMap::GetId(){return id;}

 

