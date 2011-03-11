
#include "../Headers/PostProcess.h"

const int OBJECTS=0;
const int ATTRIBUTES=1;

void OutputClusters(int n,ofstream &out){
 HTable::iterator it = H.hashTbl.begin();
    while (it != H.hashTbl.end()){
        for(int i=0; i < n; i++){
            (*it).second->a->GetSet(i)->Output(out);
            if( i != n-1)
                out<<"-";
        }   
        out<<"\n";
        it++;

    }
 out<<"####";
}
void OutputClustersNames(int n, ofstream &out){
     HTable::iterator it = H.hashTbl.begin();
    while (it != H.hashTbl.end()){
       // for(int i=0; i < n; i++){
            (*it).second->a->Output(out, nameMapVector);
      //  }
        out<<"\n";
        it++;

    }
 out<<"####";
}