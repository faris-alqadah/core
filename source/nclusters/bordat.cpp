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
    for (list<IOSet*>::iterator it = partition->begin(); it != partition->end(); it++){
        cout<<"\nmaxmod list: \n";
        (*it)->Output();
        partitionV->push_back(*it);
    }


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

    cout<<"\nmaxmods: ";
    for(int i=0; i < maxmods->size(); i++) {
        cout<<"\n"; maxmods->at(i)->Output();
    }
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
               cout<<"\nNd max mod: ";
               (*it)->Output();
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
                cout<<"\nlrnr concept: \n";
                lrnrConcept->Output();
                if(lrnrConcept->GetSetById(t)->Size() >= PRUNE_SIZE_VECTOR[t-1]){
                    bool sSat = lrnrConcept->GetSetById(s)->Size() >= PRUNE_SIZE_VECTOR[s-1];
                    NCluster *nCluster = MakeMatch(lrnrConcept,g,s,t);
                    bool simPrune = false;
                    if (nCluster == NULL) simPrune = true;
                    else if( sSat && enumerationMode == ENUM_MEM) StoreCluster(nCluster);
                    else if( sSat && enumerationMode == ENUM_FILE) OutputCluster(nCluster,g);

                    delete lrnrConcept;
                    if(!simPrune)
                         Enum_NConcepts_Bordat(nCluster,g,new IOSet(marked),s,t);
                    srchLvl--;
                } 
                else{
                    delete lrnrConcept;
                } // end if t sat
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
    NCluster *ret = new NCluster(2);
    ret->GetSet(0)->DeepCopy(lrnrConcept->GetSetById(s));
    ret->GetSet(1)->DeepCopy(lrnrConcept->GetSetById(t));
    IOSet * domainIds = g->GetAllDomainIds();
    for(int i=0; i < domainIds->Size(); i++){
        if (domainIds->At(i) != s && domainIds->At(i) != t){
            IOSet *tt = Prime(ret,g,s,domainIds->At(i));
            if ( tt != NULL){
                tt->SetId(domainIds->At(i));
                ret->AddSet(tt);

            }else if (tt->Size() < PRUNE_SIZE_VECTOR[domainIds->At(i)] ){
                delete tt;
                delete ret;
                return NULL;
            }else{
                delete ret;
                return NULL;
            }
        }

    }
    return ret;
}

void OutputCluster(NCluster *c, RelationGraph *g){
    c->Output(OUT2,NAME_MAPS);
    c->Output(OUT1);
}

void StoreCluster(NCluster *c){
    CONCEPTS.push_back(c);
    cout<<"\nstore cluster \n";
    c->Output();
}

void DispProgress(int counter, int total){
    cout<<"\nGot "<<CONCEPTS.size()<<" clusters";
    if (srchLvl == 1)
                cout << "\n" << counter << " of " << total;
}


