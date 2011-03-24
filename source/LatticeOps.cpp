/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2008
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  Represent a context from Formal Concept Analysis
 *
 *
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/
#include "../headers/LatticeOps.h"





NCluster *GetTop(Context *c){
    NCluster *ret = new NCluster(2);
    pair<int,int> domainIds = c->GetDomainIds();
    ret->GetSet(0)->SetId(domainIds.first);
    ret->GetSet(1)->SetId(domainIds.second);
    //add all objects from domain2
    for(int i=0; i < c->GetNumSets(domainIds.second); i++) ret->GetSet(1)->Add(i);
    //find objects in domain1 that contain all objects of domain2
    for(int i=0; i < c->GetNumSets(domainIds.first);i++)
        if(c->GetSet(domainIds.first,i)->Size() ==  c->GetNumSets(domainIds.second))
            ret->GetSet(0)->Add(i);
     return ret;
}

NCluster * GetBottom(Context *c) {
     NCluster *ret = new NCluster(2);
    pair<int,int> domainIds = c->GetDomainIds();
    ret->GetSet(0)->SetId(domainIds.first);
    ret->GetSet(1)->SetId(domainIds.second);
    //add all objects from domain2
    for(int i=0; i < c->GetNumSets(domainIds.first); i++) ret->GetSet(0)->Add(i);
    //find objects in domain1 that contain all objects of domain2
    for(int i=0; i < c->GetNumSets(domainIds.second);i++)
        if(c->GetSet(domainIds.second,i)->Size() ==  c->GetNumSets(domainIds.first))
            ret->GetSet(1)->Add(i);
     return ret;
}


IOSet *Prime(NCluster *a, RelationGraph *g, int s, int t,int min){
    assert(g->IsEdge(s,t));    
    IOSet *sSet = a->GetSetById(s);
    if(min < 0) min=0;
    Context *ctx = g->GetContext(s,t);
    if(a->GetSetById(s)->Size() == 0) return NULL; //since 0 return as is
    else{                            //compute the prime as set intersections (this is a fairly naive method)
    
        IOSet *rslt = new IOSet(ctx->GetSet(s,sSet->At(0)));
        if(rslt->Size() < min){  //if sSet only has a single object then perform min check here
            delete rslt;
            return NULL;
        }
        for(int i=1; i < sSet->Size(); i++){
            if(rslt->Size() < min ||ctx->GetSet(s,sSet->At(i))->Size() < min ){ // no point in performing intersection if size < min
                delete rslt;
                return NULL;
            }
            IOSet *tmp = rslt;
            rslt = Intersect(rslt,ctx->GetSet(s,sSet->At(i)));
            delete tmp;
            tmp = NULL;
        }
        if( rslt->Size() >= min) {//final check for satasfaction
            rslt->SetId(t);
            return rslt;
        }else{
            delete rslt;
            return NULL;
        }
    }
}






