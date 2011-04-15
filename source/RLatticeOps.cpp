#include "../headers/RLatticeOps.h"


RSet *Prime_Alpha_Naive(NRCluster *a, RContext *K, int s, int t, double(*d)(NRCluster*,vector<double> &),double(*f)(NRCluster*,vector<double> &),
                    vector<double> &paramsD, vector<double> &paramsF){
    pair<int,int> dIds = K->GetDomainIds();
    assert( (dIds.first == s || dIds.first == t) && (dIds.second == s || dIds.second == t));
    RSet *qSet = a->GetSetById(s);
    IOSet *rslt = new RSet;
    for(int i=0; i < K->GetNumSets(t); i++){
        //get the subspace
        RSet *row = K->GetSet(t,i);
        IOSet *rowSubspace = Intersect(qSet,row);
        if( d(rowSubspace,paramsD) < f(row,paramsF)){
            rslt->Add(i);
        }
        delete rowSubspace;

    }
    return rslt;
}