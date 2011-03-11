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
#include "../Headers/LatticeOps.h"

void OutputMaxModList(list<IOSet*> *a) {
    list<IOSet*>::iterator it = a->begin();
    while (it != a->end()) {
        (*it)->Output();
        cout << ", ";
        it++;
    }

}

NCluster * GetTop(Context *ctx) {
    vector<IOSet*> v(2);
    v[OBJECTS] = new IOSet;
    v[ATTRIBUTES] = new IOSet;
    for (int i = 0; i < ctx->GetNumRows(); i++) v[OBJECTS]->Add(i);
    return new NCluster(2, v);
}

NCluster * GetBottom(Context *ctx) {
    vector<IOSet*> v(2);
    v[OBJECTS] = new IOSet;
    v[ATTRIBUTES] = new IOSet;
    for (int i = 0; i < ctx->GetNumCols(); i++) v[ATTRIBUTES]->Add(i);
    return new NCluster(2, v);
}

NCluster * GetMatch(IOSet *cand, int contextNum, int firstSetId, int secondId) {
    NCluster *ret = new NCluster(2, false);
    IOSet *a = PrimeId(cand, contextNum, firstSetId);
    if (a == NULL) return NULL;
    else {
        ret->InitalizeSet(0, a);
        ret->InitalizeSet(1, PrimeId(a, contextNum, secondId));
        return ret;
    }
}

IOSet *PrimeId(IOSet *set, int contextNum, int setId) {
    //cout<<"\n***\ngot set : < "; set->Output(); cout<<" >";
    //cout<<"\ncontext num: "<<contextNum<<"\tset id: "<<setId;
    int otherId;
    if (contextVector[contextNum]->GetColsId() == setId) otherId = contextVector[contextNum]->GetRowsId();
    else otherId = contextVector[contextNum]->GetColsId();

    if (set->Size() == 0) {
        return NULL;
    } else if (set->Size() == 1) {
        IOSet *t = new IOSet(contextVector[contextNum]->GetSet(setId, set->At(0)));
        if (t->Size() < minCard[otherId]) {
            return NULL;
        } else return t;
    } else {
        IOSet *ret = new IOSet;
        ret->DeepCopy(contextVector[contextNum]->GetSet(setId, set->At(0)));
        if (ret->Size() < minCard[otherId]) {
            return NULL;
        }
        for (int i = 1; i < set->Size(); i++) {
            IOSet * r = contextVector[contextNum]->GetSet(setId, set->At(i));
            if (r->Size() < minCard[otherId]) {
                r = NULL;
                return NULL;
            }
            IOSet *tmp = ret;
            ret = Intersect(ret, r);
            delete tmp;
            if (ret->Size() < minCard[otherId]) {
                r = NULL;
                return NULL;
            }
        }
        return ret;
    }


}

IOSet* CheckMinPrime(IOSet *set, int contextNum, int firstSetId, int secondId, int min) {
    if (set->Size() == 0) return NULL;//false;
    else if (set->Size() == 1) {
        IOSet *t = contextVector[contextNum]->GetSet(firstSetId, set->At(0));
        if (t->Size() >= min) {
            //t = NULL;
            return new IOSet(*t);
        } else {
            t = NULL;
            return NULL;//false;
        }
    } else {
        IOSet *ret = new IOSet;
        ret->DeepCopy(contextVector[contextNum]->GetSet(firstSetId, set->At(0)));
        if (ret->Size() < min) {
            delete ret;
            return NULL;//false;
        }
        for (int i = 1; i < set->Size(); i++) {
            IOSet * r = contextVector[contextNum]->GetSet(firstSetId, set->At(i));
            if (r->Size() < min) {
                r = NULL;
                delete ret;
                return NULL;//false;
            }
            IOSet *tmp = ret;
            ret = Intersect(ret, r);
            delete tmp;
            if (ret->Size() < min) {
                r = NULL;
                delete ret;
                return NULL;//false;
            }
        }
        // cout<<"\nend of check "<<ret->Size()<<" compared to min: "<<min;
        //delete ret;
        return ret;//true;
    }
    //return true;
}

IOSet* Prime(IOSet* set, Context *ctx, int rowCols) {
    if (set->Size() == 0) return NULL;
    else if (set->Size() == 1) {
        if (rowCols == ATTRIBUTES) return new IOSet(*ctx->GetCol(set->At(0)));
        else return new IOSet(*ctx->GetRow(set->At(0)));
    } else {
        IOSet *ret = new IOSet;
        if (rowCols == ATTRIBUTES) ret->DeepCopy(ctx->GetCol(set->At(0)));
        else ret->DeepCopy(ctx->GetRow(set->At(0)));
        for (int i = 1; i < set->Size(); i++) {
            IOSet *tmp;
            if (rowCols == ATTRIBUTES) tmp = Intersect(ret, ctx->GetCol(set->At(i)));
            else tmp = Intersect(ret, ctx->GetRow(set->At(i)));
            IOSet *tmp2 = ret;
            ret = tmp;
            delete tmp2;
            tmp = NULL;
            tmp2 = NULL;
        }
        return ret;
    }
}

void AddConcept(NCluster *a) {

    //iterate through all clusters
    //if find similarity too close, then keep concept wtih greater mutual information
    /* if (H.Size() < 1 || !clusterConcepts) {
         NCluster *cpy = new NCluster(*a);
         H.Add(cpy);
         return;

     }
     HTable::iterator it = H.hashTbl.begin();
     while (it != H.hashTbl.end()){
         double sim;
         if(simMeasure == JACCARD) sim = Weighted_Jaccard(a, (*it).second->a,0.5);
         else if(simMeasure == SORENSEN) sim = Weighted_Sorensen(a, (*it).second->a,0.5);
         else if(simMeasure == ZEROS_INDUCED) sim = Zeros_Induced(a, (*it).second->a);

         if (sim >= simThreshold){
             IOSet *atts =Union(a->GetSet(ATTRIBUTES), (*it).second->a->GetSet(ATTRIBUTES));
             IOSet *objs = Union(a->GetSet(OBJECTS), (*it).second->a->GetSet(OBJECTS));
              NCluster * newConcept = new NCluster(2,false);
              newConcept->InitalizeSet(ATTRIBUTES,atts);
              newConcept->InitalizeSet(OBJECTS,objs);
              H.Remove((*it).second);
              H.Add(newConcept);
              return;

         }
         it++;
     }
     NCluster *cpy = new NCluster(*a);
     H.Add(cpy);*/
}

/*void DFConcepts(NCluster* currConcept, IOSet* marked, int ctxNum) {
    //1. Compute partition of currConcept->attributes into maxmods
    IOSet *currAtts = currConcept->GetSet(ATTRIBUTES);
    list<IOSet*> * maxmods = MaxMod_Partition(contextVector[ctxNum], currAtts, currConcept->GetSet(OBJECTS));
    vector<IOSet*> *maxmodsV = new vector<IOSet*>(maxmods->size());
    // cout<<"\nafter maxmods: \n"; currConcept->Output();
    int cnt = 0;
    for (list<IOSet*>::iterator it = maxmods->begin(); it != maxmods->end(); it++) {
        maxmodsV->at(cnt) = *it;
        cnt++;
        *it = NULL;
    }
    delete maxmods;
    //2. Find the set of non-dominating maxmods
    vector<IOSet*>* primes = new vector<IOSet*>;
    vector<IOSet*>* domInfo = new vector<IOSet*>;
    domInfo->resize(maxmodsV->size());
    primes->resize(maxmodsV->size());
    list<IOSet*> * ndMaxMods = NonDominating_MaxMods(contextVector[ctxNum],
            currAtts, currConcept->GetSet(OBJECTS),
            maxmodsV,
            primes, domInfo);
    if (computeCover)Cover(currConcept, ndMaxMods, primes);
    //3. Compute NEW which is ndMaxMods minus any maxmod containnig an element of MARKED
    RemoveMarked(ndMaxMods, marked);
    // possible to reverse the order of steps 2 and 3 for possible faster execution
    for (list<IOSet*>::iterator it = ndMaxMods->begin(); it != ndMaxMods->end(); it++) {
        if (!(*it)->GetMarked()) {
            NCluster * newConcept = new NCluster(2, false);
            IOSet *atts = Union(currAtts, *it);
            IOSet *objs = Intersect(currConcept->GetSet(OBJECTS), primes->at((*it)->Id()));
            newConcept->InitalizeSet(ATTRIBUTES, atts);
            newConcept->InitalizeSet(OBJECTS, objs);
            newConcept->ComputeHeight();
            if (objs->Size() >= minHeight && atts->Size() >= minWidth) {
                numConcepts++;
                // AddConcept(newConcept);
                // cout<<"\nGot concept: \n";
                //newConcept->Output();
                // cout<<"\nexpected index of connectivity: "<<contextVector[ctxNum]->ExpectedIdxConn(newConcept->GetSet(0),newConcept->GetSet(1));
                if (dispProgress) {
                    if (numConcepts % 1000 == 1)
                        cout << "\nGot: " << numConcepts << " concepts";
                }

            }
            if (newConcept->GetHeight() > minHeight) {
                IOSet *newMarked = new IOSet(marked);
                DFConcepts(newConcept, newMarked, ctxNum);
                //delete newMarked;
            }
            delete newConcept;
            //update marked
            IOSet *tmp = marked;
            marked = Union(marked, (*it));
            delete tmp;
            tmp = NULL;
            IOSet *lclDomInfo = domInfo->at((*it)->Id());
            int numDom = lclDomInfo->Size();
            for (int i = 0; i < numDom; i++) {
                IOSet *domMaxmod = maxmodsV->at(lclDomInfo->At(i));
                IOSet *tmp1 = marked;
                marked = Union(domMaxmod, marked);
                delete tmp1;
            }
        } else {
        }
        (*it) = NULL;
    }

    DstryVector(maxmodsV);
    DstryVector(primes);
    DstryVector(domInfo);
    delete marked;
    delete ndMaxMods;
}*/

void EnumCands(NCluster* currConcept, IOSet* marked, int attSetId, int objSetId, int ctxNum, vector<int> &neighborCtx) {
    srchLvl++;
    //1. Compute partition of currConcept->attributes into maxmods
    IOSet *currAtts = currConcept->GetSet(ATTRIBUTES);
    list<IOSet*> * maxmods = MaxMod_Partition(contextVector[ctxNum], currAtts, currConcept->GetSet(OBJECTS));
    vector<IOSet*> *maxmodsV = new vector<IOSet*>(maxmods->size());
    int cnt = 0;
    for (list<IOSet*>::iterator it = maxmods->begin(); it != maxmods->end(); it++) {
        maxmodsV->at(cnt) = *it;
        cnt++;
        *it = NULL;
    }
    delete maxmods;
    //2. Find the set of non-dominating maxmods
    vector<IOSet*>* primes = new vector<IOSet*>;
    vector<IOSet*>* domInfo = new vector<IOSet*>;
    domInfo->resize(maxmodsV->size());
    primes->resize(maxmodsV->size());
    list<IOSet*> * ndMaxMods = NonDominating_MaxMods(contextVector[ctxNum],
            currAtts, currConcept->GetSet(OBJECTS),
            maxmodsV,
            primes, domInfo);
    if (computeCover)Cover(currConcept, ndMaxMods, primes);
    //3. Compute NEW which is ndMaxMods minus any maxmod containnig an element of MARKED
    RemoveMarked(ndMaxMods, marked);
    // possible to reverse the order of steps 2 and 3 for possible faster execution
    int counter = 1;
    for (list<IOSet*>::iterator it = ndMaxMods->begin(); it != ndMaxMods->end(); it++) {
        if (dispProgress) {
            if (srchLvl == 1)
                cout << "\n" << counter << " of " << ndMaxMods->size();
        }

        if (!(*it)->GetMarked()) {
            NCluster * newConcept = new NCluster(2, false);
            IOSet *atts = Union(currAtts, *it);
            IOSet *objs = Intersect(currConcept->GetSet(OBJECTS), primes->at((*it)->Id()));
            newConcept->InitalizeSet(ATTRIBUTES, atts);
            newConcept->InitalizeSet(OBJECTS, objs);
            newConcept->ComputeHeight();
            totalNumConcepts++;
            bool objsSize;
            bool attsSize;
            bool contSim = true;
            if (objs->Size() >= minCard[objSetId]) objsSize = true;
            else objsSize = false;

            if (atts->Size() >= minCard[attSetId]) attsSize = true;
            else attsSize = false;
            if (objsSize) {
                //check other contexts
                NCluster *cluster = new NCluster(N);
                cluster->GetSet(attSetId)->DeepCopy(newConcept->GetSet(ATTRIBUTES));
                cluster->GetSet(attSetId)->SetId(attSetId);
                cluster->GetSet(objSetId)->DeepCopy(newConcept->GetSet(OBJECTS));
                cluster->GetSet(objSetId)->SetId(objSetId);

                for (int i = 0; i < neighborCtx.size(); i++) {

                    int setToQuery;
                    if (contextVector[neighborCtx[i]]->GetRowsId() == contextVector[ctxNum]->GetRowsId() ||
                            contextVector[neighborCtx[i]]->GetColsId() == contextVector[ctxNum]->GetRowsId()) {
                        setToQuery = contextVector[ctxNum]->GetRowsId();
                    } else {
                        setToQuery = contextVector[ctxNum]->GetColsId();
                    }

                    int otherId;
                    if (setToQuery == contextVector[ neighborCtx[i]]->GetColsId())
                        otherId = contextVector[ neighborCtx[i]]->GetRowsId();
                    else
                        otherId = contextVector[ neighborCtx[i]]->GetColsId();
                    IOSet *set_i;
                    if (setToQuery == attSetId)
                        set_i = CheckMinPrime(atts, neighborCtx[i], setToQuery, otherId, minCard[otherId]);
                    else
                        set_i = CheckMinPrime(objs, neighborCtx[i], setToQuery, otherId, minCard[otherId]);


                    if (set_i == NULL) {
                        contSim=false;
                        delete cluster;
                        break;
                    }else{
                        cluster->GetSet(otherId)->DeepCopy(set_i);
                        cluster->GetSet(otherId)->SetId(otherId);
                        delete set_i;
                    }
                }

                if (contSim) {
                    if (attsSize) {
                        numConcepts++;
                        if(outputConceptsNames)
                            cluster->Output( outConceptsNamesStrm,nameMapVector);
                        //add to all objs clustered
                        for(int i=0; i < N; i++)
                            allObjsClustered->GetSet(i)->DeepCopy(Union(cluster->GetSet(i),allObjsClustered->GetSet(i)));

                    }
                    IOSet *newMarked = new IOSet(marked);
                    EnumCands(newConcept, newMarked, attSetId, objSetId, ctxNum, neighborCtx);
                    srchLvl--;
                }

            }
            delete newConcept;
            //update marked
            IOSet *tmp = marked;
            marked = Union(marked, (*it));
            delete tmp;
            IOSet *lclDomInfo = domInfo->at((*it)->Id());
            int numDom = lclDomInfo->Size();

            for (int i = 0; i < numDom; i++) {
                IOSet *domMaxmod = maxmodsV->at(lclDomInfo->At(i));
                IOSet *tmp1 = marked;
                marked = Union(domMaxmod, marked);
                delete tmp1;
            }
        } else {
        }
        (*it) = NULL;
        counter++;
    }
    DstryVector(maxmodsV);
    DstryVector(primes);
    DstryVector(domInfo);
    delete ndMaxMods;
    delete marked;

}

list<IOSet*>* MaxMod_Partition(Context *ctx, IOSet *attributes, IOSet *objects) {

    list<IOSet*> *partition = new list<IOSet*>;
    IOSet *initial = ctx->GetAllColLabels();
    IOSet *tmp = initial;
    initial = Difference(initial, attributes); //reprsent sub-relation
    delete tmp;
    partition->push_back(initial);
    for (int i = 0; i < objects->Size(); i++) {
        int currSz = partition->size();
        list<IOSet*>::iterator it = partition->begin();
        for (int j = 0; j < currSz; j++) {
            if ((*it)->Size() > 1) {
                IOSet *prime = Difference(ctx->GetRow(objects->At(i)), attributes);
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
    return partition;
}

list<IOSet*>* NonDominating_MaxMods(Context *ctx, IOSet *attributes, IOSet *objects,
        vector<IOSet*>* maxmods,
        vector<IOSet*>* primes, vector<IOSet*>* domInfo) {

    //calculate all the primes first
    for (int i = 0; i < maxmods->size(); i++) {
        maxmods->at(i)->SetId(i);
        primes->at(i) = Intersect(ctx->GetCol((maxmods->at(i)->At(0))), objects);
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

void Cover(NCluster *currConcept, list<IOSet*>*nd, vector<IOSet*> *primes) {
    //cout<<"\nneighbors of: \n";
    //currConcept->Output();
    for (list<IOSet*>::iterator it = nd->begin(); it != nd->end(); it++) {
        IOSet * atts = Union(currConcept->GetSet(ATTRIBUTES), (*it));
        IOSet *objs = Intersect(currConcept->GetSet(OBJECTS), primes->at((*it)->Id()));
        numEdges++;
        // cout<<"\n";         atts->Output();
        // cout<<"\n";         objs->Output();
        delete atts;
        delete objs;
    }

}





