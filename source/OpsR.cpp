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

RSet *Intersect(RSet *a, RSet *b){
    //assumes indices are sorted
    RSet *ret = new RSet;
    for(int aPtr=0,bPtr=0; aPtr < a->Size() && bPtr < b->Size();){
        if(a->At(aPtr).first == b->At(bPtr).first){
            ret->Add(a->At(aPtr));
            aPtr++;
            bPtr++;
        }else if(a->At(aPtr).first > b->At(bPtr).first) bPtr++;
        else aPtr++;
    }
    return ret;
}

RSet *Union(RSet *a, RSet *b){
    RSet *ret = new RSet;
    for(int aPtr=0,bPtr=0; aPtr < a->Size() && bPtr < b->Size();){
        
    }
}