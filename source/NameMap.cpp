#include <fstream>

#include "../Headers/NameMap.h"

NameMap::NameMap(){}

NameMap::NameMap(string _fileName, unsigned int _numEntries){
  fileName = _fileName;
  numEntries=_numEntries;
  ifstream myfile;
  mapping.resize(numEntries);

  myfile.open(fileName.c_str());
  assert(myfile.is_open() && numEntries > 0);
  if (myfile.is_open()){
    int lineNum = 0;
    string line;
    while (! myfile.eof() ){
        if(lineNum == numEntries) break;
        getline (myfile,line);
        mapping[lineNum] = line;
        lineNum++;
    }
  }
  else{
      cerr<<"\nCoult not open NAME file!! "<<fileName<<endl;
      cerr<<"\nERROR!!\n";
  }
    
}
string NameMap::GetName(unsigned int x){
    assert ( x < numEntries);
        return mapping[x];


}

string NameMap::GetFileName(){return fileName;}
int NameMap::GetNumEntries(){return numEntries;}

 

