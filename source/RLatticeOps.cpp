#include "../headers/RLatticeOps.h"

IOSet *Prime_Alpha_Naive(RContext *K, IOSet *query, int s, int t,double alpha, 
                         double(*d)(RSet*,vector<double> &),double(*f)(RSet*,vector<double> &),
                         void(*p)(RContext *,IOSet *, int, int,int, vector<double> &)){

    pair<int,int> dIds = K->GetDomainIds();
    assert( (dIds.first == s || dIds.first == t) && (dIds.second == s || dIds.second == t));
    IOSet *rslt = new IOSet;
    cout<<"\nalpha: "<<alpha;
    for(int i=0; i < K->GetNumSets(t); i++){
        //get the subspace
        RSet *row = K->GetSet(t,i);
        RSet *rowSubspace = Intersect(row,query);
        vector<double> lclParamsF; //construct parameter vector for the consistency function, make alpha the first element though
        vector<double> rangeParams;
        lclParamsF.push_back(alpha);  //assign the variance for this particlar row / column
        rowSubspace->SetQuality(d(rowSubspace,rangeParams));
        query->SetQuality(rowSubspace->GetQuality()); //set the quality on both the indices and the RSet
        p(K,query,s,t,row->Id(),lclParamsF);
        if( rowSubspace->GetQuality() < f(row,lclParamsF))
            rslt->Add(i);
        delete rowSubspace;

    }
    query->SetQuality(0); //reset the quality on query...
    return rslt;
}