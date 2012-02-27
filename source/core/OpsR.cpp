#include "../../headers/core/OpsR.h"


NRCluster *TransposeSparse(NRCluster *a,int sz1,int sz2){
    assert(a != NULL && a->GetN() == sz1);
    NRCluster *ret = new NRCluster(sz2);
    for(int i=0; i < a->GetN(); i++){
        IOSet *idxs = a->GetSet(i)->GetIdxs();
        for(int j=0; j < idxs->Size(); j++){
            pair<int,double> val = a->GetSet(i)->At(idxs->At(j));
            val.first=i;
            ret->GetSet(a->GetSet(i)->At(idxs->At(j)).first)->Add( val );
        }
        delete idxs;
    }
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

RSet *Intersect(RSet *a, IOSet *b){
    RSet *ret = new RSet;
    for(int aPtr=0,bPtr=0; aPtr < a->Size() && bPtr < b->Size();){
        if(a->At(aPtr).first == b->At(bPtr)){
            ret->Add(a->At(aPtr));
            aPtr++;
            bPtr++;
        }else if(a->At(aPtr).first > b->At(bPtr)) bPtr++;
        else aPtr++;
    }
    return ret;
}

RSet *Union(RSet *a, RSet *b){
    RSet *ret = new RSet;
    for(int aPtr=0,bPtr=0; aPtr < a->Size() && bPtr < b->Size();){
        
    }
}


//!Compute the mean of an RSet
double Mean(RSet *a);