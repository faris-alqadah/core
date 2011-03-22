#include "../../headers/nclusters/bordat.h"

vector<IOSet*>* MaxMod_Partition(Context *ctx, NCluster *c, int s, int t) {
    IOSet *sSet= c->GetSetById(s);
    IOSet *tSet = c->GetSetById(t);
    list<IOSet*> *partition = new list<IOSet*>;
    IOSet *initial =  ctx->GetLabels(s);
    IOSet *tmp = initial;
    initial = Difference(initial, sSet); //reprsent sub-relation
    delete tmp;
    partition->push_back(initial);
    for (int i = 0; i < tSet->Size(); i++) {
        int currSz = partition->size();
        list<IOSet*>::iterator it = partition->begin();
        for (int j = 0; j < currSz; j++) {
            if ((*it)->Size() > 1) {
                IOSet *prime = Difference(ctx->GetSet(t,tSet->At(i)), sSet); 
                IOSet * k1 = Intersect((*it), prime);
                IOSet * k11 = Difference((*it), prime);
                if (k1->Size() > 0 && k11->Size() > 0) {
                    k1->Sort();
                    k11->Sort();
                    partition->insert(it, k1);
                    partition->insert(it, k11);
                    delete (*it);
                    it = partition->erase(it);
                } else {
                    delete k1;
                    delete k11;
                    it++;
                }
                delete prime;
            } else it++;
        }
    }
    //copy to vector
    //list was used for more efficeint insertion and deletion
    vector<IOSet*> * partitionV = new vector<IOSet*>;
    for (list<IOSet*>::iterator it = partition->begin(); it != partition->end(); it++)
        partitionV->push_back(*it);

    partition = NULL;
    return partitionV;
}

list<IOSet*>* NonDominating_MaxMods(Context *ctx, NCluster * c, int s, int t,
        vector<IOSet*>* maxmods,
        vector<IOSet*>* primes, vector<IOSet*>* domInfo) {

    IOSet *sSet = c->GetSetById(s);
    IOSet *tSet = c->GetSetById(t);
    //calculate all the primes first
    for (int i = 0; i < maxmods->size(); i++) {
        maxmods->at(i)->SetId(i);
        primes->at(i) = Intersect(ctx->GetSet(s,maxmods->at(i)->At(0)), tSet);
        domInfo->at(i) = new IOSet;
    }
    list<IOSet*> * nd = new list<IOSet*>;
    for (int i = 0; i < maxmods->size(); i++) {
        if (!maxmods->at(i)->GetMarked()) {
            nd->push_back(maxmods->at(i));
            //remove maxmods which dominate X
            for (int j = i + 1; j < maxmods->size(); j++) {
                if (!maxmods->at(j)->GetMarked()) {
                    if (ProperSubSet(primes->at(j), primes->at(i))) {
                        domInfo->at(i)->Add(j); //add to domination info
                        maxmods->at(j)->SetMarked(true);
                    }
                }
            }
        }//added
    }
    return nd;
}

void RemoveMarked(list<IOSet*> * ndMaxMods, IOSet *marked) {
    list<IOSet*>::iterator it = ndMaxMods->begin();
    while (it != ndMaxMods->end()) {
        bool removed = false;
        for (int i = 0; i < marked->Size(); i++) {
            if ((*it)->Contains(marked->At(i))) {
                //delete (*it);
                //it = ndMaxMods->erase(it);
                (*it)->SetMarked(true);
                removed = true;
                break;
            }
        }
        it++;
    }
}

void Enum_NConcepts_Bordat(NCluster *a, RelationGraph *g, IOSet *marked, int s, int t){
    srchLvl++;
    Context *ctx = g->GetContext(s,t);
    //1. Compute partition of the (s,t) pair into maxmods of the s set
    vector<IOSet*>* maxmods = MaxMod_Partition(ctx,a,s,t);
    //2. Find the set of non-dominating maxmods
    vector<IOSet*>* primes = new vector<IOSet*>(maxmods->size());
    vector<IOSet*>* domInfo = new vector<IOSet*>(maxmods->size());
    list<IOSet*> * ndMaxMods = NonDominating_MaxMods(ctx,a,s,t,maxmods,primes,domInfo);
    //3. Compute NEW which is ndMaxMods minus any maxmod containnig an element of MARKED
    RemoveMarked(ndMaxMods, marked);
    int ctr=1;
    for (list<IOSet*>::iterator it = ndMaxMods->begin(); it != ndMaxMods->end(); it++) {
        if(dispProgress) DispProgress(ctr,ndMaxMods->size());
           if (!(*it)->GetMarked()) {
                NCluster * lrnrConcept = new NCluster;
                lrnrConcept->AddSet(Union(a->GetSetById(s),*it));
                lrnrConcept->GetSet(0)->SetId(s);
                lrnrConcept->AddSet(Intersect(a->GetSetById(t),primes->at((*it)->Id())));
                lrnrConcept->GetSet(1)->SetId(t);
                //enumerated a concept in the learnere
                //pruning can only occur if the t set does not match size
                //if s set does not match size then
                //check other contexts for support, if possibe then continue
                //other wise prune
                if(lrnrConcept->GetSetById(t)->Size() >= PRUNE_SIZE_VECTOR[t]){
                    bool sSat = lrnrConcept->GetSetById(s)->Size() >= PRUNE_SIZE_VECTOR[s];
                    NCluster *nCluster = MakeMatch(lrnrConcept,g,s,t);
                    bool simPrune = false;
                    if (nCluster == NULL) simPrune = true;
                    else if( sSat && enumerationMode == ENUM_MEM) StoreCluster(nCluster);
                    else if( sSat && enumerationMode == ENUM_FILE) OutputCluster(nCluster,g);

                    if(!simPrune)
                         Enum_NConcepts_Bordat(nCluster,g,new IOSet,s,t);
                    srchLvl--;
                } // end if t sat
                delete lrnrConcept;
                //update marked
                IOSet *tmp = marked;
                marked = Union(marked, (*it));
                delete tmp;
                IOSet *lclDomInfo = domInfo->at((*it)->Id());
                int numDom = lclDomInfo->Size();
                for (int i = 0; i < numDom; i++) {
                 IOSet *domMaxmod = maxmods->at(lclDomInfo->At(i));
                 IOSet *tmp1 = marked;
                 marked = Union(domMaxmod, marked);
                 delete tmp1;
                }
             } // end if marked
             (*it) = NULL;
             ctr++;
    } //end for
    //clean up
    DstryVector(maxmods);
    DstryVector(primes);
    DstryVector(domInfo);
    delete ndMaxMods;
    delete marked;
}

NCluster * MakeMatch(NCluster *lrnrConcept, RelationGraph *g, int s, int t){

}

void OutputCluster(NCluster *c, RelationGraph *g){
    c->Output(OUT2,*g->GetNameMaps());
    c->Output(OUT1);
}

void StoreCluster(NCluster *c){
    CONCEPTS.push_back(c);
}

void DispProgress(int counter, int total){
    if (srchLvl == 1)
                cout << "\n" << counter << " of " << total;
}

//void EnumCands(NCluster* currConcept, IOSet* marked, int attSetId, int objSetId, int ctxNum, vector<int> &neighborCtx) {
//    srchLvl++;
//    //1. Compute partition of currConcept->attributes into maxmods
//    IOSet *currAtts = currConcept->GetSet(ATTRIBUTES);
//    list<IOSet*> * maxmods = MaxMod_Partition(contextVector[ctxNum], currAtts, currConcept->GetSet(OBJECTS));
//    vector<IOSet*> *maxmodsV = new vector<IOSet*>(maxmods->size());
//    int cnt = 0;
//    for (list<IOSet*>::iterator it = maxmods->begin(); it != maxmods->end(); it++) {
//        maxmodsV->at(cnt) = *it;
//        cnt++;
//        *it = NULL;
//    }
//    delete maxmods;
//    //2. Find the set of non-dominating maxmods
//    vector<IOSet*>* primes = new vector<IOSet*>;
//    vector<IOSet*>* domInfo = new vector<IOSet*>;
//    domInfo->resize(maxmodsV->size());
//    primes->resize(maxmodsV->size());
//    list<IOSet*> * ndMaxMods = NonDominating_MaxMods(contextVector[ctxNum],
//            currAtts, currConcept->GetSet(OBJECTS),
//            maxmodsV,
//            primes, domInfo);
//    if (computeCover)Cover(currConcept, ndMaxMods, primes);
//    //3. Compute NEW which is ndMaxMods minus any maxmod containnig an element of MARKED
//    RemoveMarked(ndMaxMods, marked);
//    // possible to reverse the order of steps 2 and 3 for possible faster execution
//    int counter = 1;
//    for (list<IOSet*>::iterator it = ndMaxMods->begin(); it != ndMaxMods->end(); it++) {
//        if (dispProgress) {
//            if (srchLvl == 1)
//                cout << "\n" << counter << " of " << ndMaxMods->size();
//        }
//
//        if (!(*it)->GetMarked()) {
//            NCluster * newConcept = new NCluster(2, false);
//            IOSet *atts = Union(currAtts, *it);
//            IOSet *objs = Intersect(currConcept->GetSet(OBJECTS), primes->at((*it)->Id()));
//            newConcept->InitalizeSet(ATTRIBUTES, atts);
//            newConcept->InitalizeSet(OBJECTS, objs);
//            newConcept->ComputeHeight();
//            totalNumConcepts++;
//            bool objsSize;
//            bool attsSize;
//            bool contSim = true;
//            if (objs->Size() >= minCard[objSetId]) objsSize = true;
//            else objsSize = false;
//
//            if (atts->Size() >= minCard[attSetId]) attsSize = true;
//            else attsSize = false;
//            if (objsSize) {
//                //check other contexts
//                NCluster *cluster = new NCluster(N);
//                cluster->GetSet(attSetId)->DeepCopy(newConcept->GetSet(ATTRIBUTES));
//                cluster->GetSet(attSetId)->SetId(attSetId);
//                cluster->GetSet(objSetId)->DeepCopy(newConcept->GetSet(OBJECTS));
//                cluster->GetSet(objSetId)->SetId(objSetId);
//
//                for (int i = 0; i < neighborCtx.size(); i++) {
//
//                    int setToQuery;
//                    if (contextVector[neighborCtx[i]]->GetRowsId() == contextVector[ctxNum]->GetRowsId() ||
//                            contextVector[neighborCtx[i]]->GetColsId() == contextVector[ctxNum]->GetRowsId()) {
//                        setToQuery = contextVector[ctxNum]->GetRowsId();
//                    } else {
//                        setToQuery = contextVector[ctxNum]->GetColsId();
//                    }
//
//                    int otherId;
//                    if (setToQuery == contextVector[ neighborCtx[i]]->GetColsId())
//                        otherId = contextVector[ neighborCtx[i]]->GetRowsId();
//                    else
//                        otherId = contextVector[ neighborCtx[i]]->GetColsId();
//                    IOSet *set_i;
//                    if (setToQuery == attSetId)
//                        set_i = CheckMinPrime(atts, neighborCtx[i], setToQuery, otherId, minCard[otherId]);
//                    else
//                        set_i = CheckMinPrime(objs, neighborCtx[i], setToQuery, otherId, minCard[otherId]);
//
//
//                    if (set_i == NULL) {
//                        contSim=false;
//                        delete cluster;
//                        break;
//                    }else{
//                        cluster->GetSet(otherId)->DeepCopy(set_i);
//                        cluster->GetSet(otherId)->SetId(otherId);
//                        delete set_i;
//                    }
//                }
//
//                if (contSim) {
//                    if (attsSize) {
//                        numConcepts++;
//                        if(outputConceptsNames)
//                            cluster->Output( outConceptsNamesStrm,nameMapVector);
//                        //add to all objs clustered
//                        for(int i=0; i < N; i++)
//                            allObjsClustered->GetSet(i)->DeepCopy(Union(cluster->GetSet(i),allObjsClustered->GetSet(i)));
//
//                    }
//                    IOSet *newMarked = new IOSet(marked);
//                    EnumCands(newConcept, newMarked, attSetId, objSetId, ctxNum, neighborCtx);
//                    srchLvl--;
//                }
//
//            }
//            delete newConcept;
//            //update marked
//            IOSet *tmp = marked;
//            marked = Union(marked, (*it));
//            delete tmp;
//            IOSet *lclDomInfo = domInfo->at((*it)->Id());
//            int numDom = lclDomInfo->Size();
//
//            for (int i = 0; i < numDom; i++) {
//                IOSet *domMaxmod = maxmodsV->at(lclDomInfo->At(i));
//                IOSet *tmp1 = marked;
//                marked = Union(domMaxmod, marked);
//                delete tmp1;
//            }
//        } else {
//        }
//        (*it) = NULL;
//        counter++;
//    }
//    DstryVector(maxmodsV);
//    DstryVector(primes);
//    DstryVector(domInfo);
//    delete ndMaxMods;
//    delete marked;
//
//}
