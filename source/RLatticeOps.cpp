#include "../headers/RLatticeOps.h"

IOSet *Prime_Alpha_Naive(RContext *K, IOSet *query, int s, int t, double(*d)(RSet*,vector<double> &),double(*f)(RSet*,vector<double> &),
                    vector<double> &paramsD, vector<double> &paramsF){
    pair<int,int> dIds = K->GetDomainIds();
    assert( (dIds.first == s || dIds.first == t) && (dIds.second == s || dIds.second == t));
    IOSet *rslt = new IOSet;
     vector<double> lclParamsF(2); //only pass in the variance for current particular row or column
     lclParamsF[0] = paramsF[0]; //this is the alpha value
    for(int i=0; i < K->GetNumSets(t); i++){
        //get the subspace
        RSet *row = K->GetSet(t,i);
        RSet *rowSubspace = Intersect(row,query);
        lclParamsF[1] = paramsF[i+1];  //assign the variance for this particlar row / column
        if( d(rowSubspace,paramsD) < f(row,paramsF)){
            rslt->Add(i);
        }
        delete rowSubspace;

    }
    return rslt;
}