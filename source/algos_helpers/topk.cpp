#include "../../headers/algos_helpers/topk.h"


void RetainTopK_Overlap(vector<NCluster*> &v, NCluster *c,double (*Ovlp)(NCluster*,NCluster*), double ovlpThresh, int k){
    assert(c != NULL && k > 0 && (ovlpThresh >= 0 && ovlpThresh <= 1));
    if( v.size() < k){
        v.push_back(new NCluster(*c));
    }
    else{
        for(int i=0; i < v.size(); i++){
            if (Ovlp(c,v[i]) >= ovlpThresh && c->GetQuality() > v[i]->GetQuality()){
                SwapDelete(v,c,i);
                sort(v.begin(),v.end(),Compare_Quality);
                return;
            }
        }
        int limit = v.size() < k ? v.size()-1:k-1;
        if(c->GetQuality() > v[limit]->GetQuality()){
            SwapDelete(v,c,limit);
            sort(v.begin(),v.end(),Compare_Quality);
        }
    }

}

