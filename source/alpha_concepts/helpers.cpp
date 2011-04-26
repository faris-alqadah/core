#include "../../headers/alpha_concepts/helpers.h"




void Construct_AlphaSigma_Params(RContext *K,IOSet *query, int s, vector<double> &params){
    assert(params.size() == 1 );
    pair<int,int> dIds = K->GetDomainIds();
    assert(s == dIds.first || s == dIds.second);
    IOSet *labels = K->GetLabels(s);
    for(int i=0; i < labels->Size(); i++)
        if(query->Contains(labels->At(i)))
         params.push_back(K->GetStdDev(s,labels->At(i)));
}


void Construct_MaxSpaceUniform_Params(RContext* K, IOSet *query, int s, vector<double> &params){
    assert(params.size()==1);
    pair<int,int> dIds = K->GetDomainIds();
    assert(s == dIds.first || s == dIds.second);
    for(int i=0; i < query->Size(); i++) params.push_back(query->Size());

}
