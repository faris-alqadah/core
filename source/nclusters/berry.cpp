#include "../../headers/nclusters/berry.h"


void BerryLatticeAlgos::Star_N_Concepts(RelationGraph* g, int lrnrContext){
    //check all pre conditions
    //if they are met then call Enum_NConcepts
    string file1 = OUTFILE+".concepts";
    string file2 = OUTFILE+".concepts.names";
    OUT1.open(file1.c_str());
    OUT2.open(file2.c_str());
    IOSet *artDomains = g->GetArtDomains();

    if( artDomains->Size() != 1 && g->GetNumNodes() > 2) {
        string errMsg = "Star_N_Concepts must be called with a star shaped hin\n";
        cerr<<errMsg; exit(-1);
    }
    if( g->GetContext(lrnrContext) == NULL){
        string errMsg = "Star_N_Concepts called with invalid learner context id for the given hin\n";
        cerr<<errMsg; exit(-1);
    }
    if(enumerationMode == ENUM_FILE && !OUT1.is_open()){
         string errMsg = "Star_N_Concepts called with ENUM_FILE mode, however, OUTFILE is not valid file or has not been set\n";
        cerr<<errMsg; exit(-1);
    }else if(enumerationMode == ENUM_FILE || enumerationMode == ENUM_TOPK_FILE){
        //setup name maps
        NAME_MAPS = *g->GetNameMaps();

    }
    if(enumerationMode == ENUM_TOPK_FILE &&!OUT1.is_open()){
         string errMsg = "Star_N_Concepts called with ENUM_FILE mode, however, OUTFILE is not valid file or has not been set\n";
        cerr<<errMsg; exit(-1);
    }
    if( PRUNE_SIZE_VECTOR.size() < g->GetNumNodes()){
        string errMsg = "Star_N_Concepts called with size pruning, however, PRUNE_SIZE_VECTOR does not contain threshold values for all domains\n";
        cerr<<errMsg; exit(-1);
    }

    //check values of prune_size_vector are all >= 1,
    for(int i=0; i < g->GetNumNodes(); i++)
        if(PRUNE_SIZE_VECTOR[i] < 1){
            PRUNE_SIZE_VECTOR[i] = 1;
            cout<<"\nReset prune size vector at "<<i<<" to 1\n";
        }
    //reset variables
     srchLvl=0;
     numConcepts=0;


     //set the quality function pointer and the ovlp function pointer
     if(qualityMode == AREA){
        qualityFunction=&Area;
     }else if(qualityMode == BETA){
         qualityFunction=&Beta;
     }
     //set the overlap function pointer
     if(ovlpMode == AVG_JACCARD){
         ovlpFunction=&AverageOverlap;
     }

    //done check and setting genering variables call enumeration algorithm
    //for now only Bordat is implemented
    //compute the top level concept of lrnr context and call function
     IOSet *domainIds = g->GetAllDomainIds();
     if(g->GetNumNodes() == 2)
         artDomains->Add(domainIds->At(0));

     NCluster *strt = new NCluster(g->GetNumNodes());
     for(int i=0; i < strt->GetN(); i++) strt->GetSet(i)->SetId(domainIds->At(i));

     int artDomain = artDomains->At(0);
     Context *ctx = g->GetContext(lrnrContext);
     pair<int,int> lrnrDomainIds = ctx->GetDomainIds();
     int otherDomain;
     NCluster *strt1;
     if(lrnrDomainIds.first == artDomain){
         otherDomain = lrnrDomainIds.second;
         strt1 = GetTop(ctx);
         strt1->GetSet(0)->SetId(artDomain);
         strt1->GetSet(1)->SetId(otherDomain);

     }else{
         otherDomain = lrnrDomainIds.first;
         strt1 = GetBottom(ctx);
         strt1->GetSet(0)->SetId(otherDomain);
         strt1->GetSet(1)->SetId(artDomain);

     }
     cout<<"\nThe articuluation domain id is: "<<artDomain<<"\n";
     cout<<"\nThe learner contex is: "<<ctx->GetName();
     strt->AssignSetById(artDomain,strt1->GetSetById(artDomain));
     strt->AssignSetById(otherDomain,strt1->GetSetById(otherDomain));
        Enum_NConcepts_Berry(strt,g,new IOSet(),artDomain,otherDomain);
     
     //clena up now
     if(enumerationMode == ENUM_FILE){
         OUT1.close();
         OUT2.close();
     }
}
vector<IOSet*>* BerryLatticeAlgos::MaxMod_Partition(Context *ctx, NCluster *c, int s, int t) {
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
        partitionV->push_back(*it);
    }


    partition = NULL;
    return partitionV;
}

list<IOSet*>* BerryLatticeAlgos::NonDominating_MaxMods(Context *ctx, NCluster * c, int s, int t,
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
                    if (ProperSubSet(primes->at(i), primes->at(j))) {
                        domInfo->at(i)->Add(j); //add to domination info
                        maxmods->at(j)->SetMarked(true);
                    }
                }
            }
        }//added
    }
    return nd;
}

void BerryLatticeAlgos::RemoveMarked(list<IOSet*> * ndMaxMods, IOSet *marked) {
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

void BerryLatticeAlgos::Enum_NConcepts_Berry(NCluster *a, RelationGraph *g, IOSet *marked, int s, int t){
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
        if(dispProgress && srchLvl == 1) DispProgress(ctr,ndMaxMods->size());
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
                if(lrnrConcept->GetSetById(t)->Size() >= PRUNE_SIZE_VECTOR[t-1]){
                    bool sSat = lrnrConcept->GetSetById(s)->Size() >= PRUNE_SIZE_VECTOR[s-1];
                    NCluster *nCluster = MakeMatch(lrnrConcept,g,s,t);
                    bool simPrune = false;
                    //different output options
                    if (nCluster == NULL) simPrune = true;
                    else if( sSat && enumerationMode == ENUM_MEM)
                        StoreCluster(CONCEPTS,nCluster);
                    else if( sSat && enumerationMode == ENUM_FILE) {
                        OutputCluster(nCluster,OUT1);
                        OutputCluster(nCluster,OUT2,NAME_MAPS);
                    }
                    else if( (sSat && enumerationMode == ENUM_TOPK_MEM) || (sSat && enumerationMode == ENUM_TOPK_FILE)){
                        SetQuality(nCluster,params,qualityFunction);
                        RetainTopK_Overlap(CONCEPTS,nCluster,ovlpFunction,ovlpThresh,topKK);
                    }
                    delete lrnrConcept;
                    if(!simPrune){
                         Enum_NConcepts_Berry(nCluster,g,new IOSet(marked),s,t);
                        srchLvl--;
                    }
                    if(nCluster) delete nCluster; //free up the memory as stored nclusters are copies and non-stored clusters are not needed
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

NCluster * BerryLatticeAlgos::MakeMatch(NCluster *lrnrConcept, RelationGraph *g, int s, int t){
    NCluster *ret = new NCluster(*lrnrConcept);
    IOSet * domainIds = g->GetAllDomainIds();
    for(int i=0; i < domainIds->Size(); i++){
        if (domainIds->At(i) != s && domainIds->At(i) != t){
            IOSet *tt = Prime(lrnrConcept,g,s,domainIds->At(i),PRUNE_SIZE_VECTOR[domainIds->At(i)-1]);
            if ( tt != NULL ){
                tt->SetId(domainIds->At(i));
                ret->AddSet(tt);

            }else{
                delete ret;
                return NULL;
            }
        }

    }
    return ret;
}



vector<NCluster*> * BerryLatticeAlgos::UpperNeighbors(NCluster *c, RelationGraph *g, int s, int t){
    Context *ctx = g->GetContext(s,t);
    //1.compute maxmods
    vector<IOSet*>* maxmods = MaxMod_Partition(ctx,c,s,t);
     //2. Find the set of non-dominating maxmods
   vector<IOSet*>* primes = new vector<IOSet*>(maxmods->size());
   vector<IOSet*>* domInfo = new vector<IOSet*>(maxmods->size());
   list<IOSet*> * ndMaxMods = NonDominating_MaxMods(ctx,c,s,t,maxmods,primes,domInfo);
   vector<NCluster*> *ret = new vector<NCluster*>;
   for (list<IOSet*>::iterator it = ndMaxMods->begin(); it != ndMaxMods->end(); it++) {
            if (!(*it)->GetMarked()) {
                NCluster * neighbor = new NCluster;
                neighbor->AddSet(Union(c->GetSetById(s),*it));
                neighbor->GetSet(0)->SetId(s);
                neighbor->AddSet(Intersect(c->GetSetById(t),primes->at((*it)->Id())));
                neighbor->GetSet(1)->SetId(t);
                ret->push_back(neighbor);
            }
            (*it) = NULL;
   }
   DstryVector(maxmods);
   DstryVector(primes);
   DstryVector(domInfo);
   delete ndMaxMods;
   return ret;
}


vector<NCluster*> * BerryLatticeAlgos::LowerNeighbors(NCluster *c, RelationGraph *g, int s, int t){
    return UpperNeighbors(c,g,t,s);
    
}


