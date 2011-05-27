#include "../../headers/alpha_concepts/helpers.h"




void Construct_AlphaSigma_Params(RContext *K,IOSet* query, int s,int t, int tRow, vector<double> &params){
    assert(params.size() == 1 );
    pair<int,int> dIds = K->GetDomainIds();
    assert( (s == dIds.first && t == dIds.second) || (s == dIds.second && t == dIds.first));
    params.push_back(K->GetStdDev(t,tRow)); //std dev of full row/column <b>M</b>
         
}


void Construct_MaxSpaceUniform_Params(RContext* K, IOSet* query, int s, int t, int tRow, vector<double> &params){
    assert(params.size()==1);
    pair<int,int> dIds = K->GetDomainIds();
    assert(s == dIds.first || s == dIds.second);
    params.push_back((double)K->GetNumSets(t)); //size of <b>M</b>
    vector<double> rangeParams;
    RSet *sset = K->GetSet(t,tRow);
    double rr = Range(sset,rangeParams);
    params.push_back(rr); //range of full row/column <b>M</b>
    params.push_back(query->GetQuality());
}

IOSet *Naive_Range_Intersect(IOSet *supSet1, IOSet *supSet2){

}