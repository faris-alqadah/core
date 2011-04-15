#include "../headers/OpsR.h"


NRCluster *TransposeSparse(NRCluster *a){
    assert(a != NULL);
    NRCluster *ret = new NRCluster(a->GetMaxIdx()+1);
    for(int i=0; i < a->GetN(); i++)
        for(int j=0; j < a->GetSet(i)->Size(); j++)
            ret->GetSet(a->GetSet(i)->At(j).first)->Add( a->GetSet(i)->At(j) );
     //set ids
     for(int i=0; i < ret->GetN(); i++) ret->GetSet(i)->SetId(i);
    return ret;
}
