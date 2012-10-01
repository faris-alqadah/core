#include "../../headers/core/MiscOps.h"


struct PairCmp {
    bool operator()(const pair<int,double> &lhs, const pair<int,double> &rhs) {
        return lhs.second < rhs.second;
    }
};

vector< pair<int,double> > * Sort_Map_By_Value( map<int,double> &theMap){
    vector<pair<int,double> > *pairsVector  = new vector< pair<int,double> >(theMap.size());
    map<int,double>::iterator it = theMap.begin();
    int i=0;
    while(it != theMap.end()){
        (*pairsVector)[i++] = (*it);
        it++;
    }
   sort(pairsVector->begin(), pairsVector->end(), PairCmp());
   return pairsVector;
}
