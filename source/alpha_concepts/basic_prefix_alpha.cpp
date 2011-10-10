#include "../../headers/alpha_concepts/basic_prefix_alpha.h"

void BasicPrefix::Qbbc(IOSet *query, vector<NCluster*> &hits) {
    //make name maps
     NAME_MAPS.resize(2);
     NAME_MAPS[0] = K->GetNameMap(1);
     NAME_MAPS[1] = K->GetNameMap(2);
    //1. Get all hits for prefix search only in query space

    IOSet *origQuery = new IOSet(query);
    while (query->Size() > 1) {
        Qbbc_Prefix_Search(query);
        //did we get a hit?
        if (CONCEPTS.size() > 0) {
            bool gotAll = CONCEPTS[0]->GetSetById(s)->Size() == query->Size(); //were all query objects retrieved
            //find closure
            // cout<<"\ngetting min max...";
            NCluster *minMax = Get_Min_Max_Idxs(CONCEPTS[0]->GetSetById(s), CONCEPTS[0]->GetSetById(t));
            NCluster *bicluster = new NCluster(*CONCEPTS[0]);
            cout << "\nbi cluster: \n";
            bicluster->Output();
            cout<<"\nGetting closure and neighbors...\n";
            cout.flush();
            vector<NCluster*> *neighbors = Range_Closure_Charm_Neighbors(bicluster->GetSetById(s), bicluster->GetSetById(t), minMax);
            // cout<<"\ndone\n";
            // delete minMax;
            cout << "\nInitial hit: \n";
            bicluster->Output();
            //cout.flush();
            cout << "\nshould get neighbors here...got these " << neighbors->size();
            //select best neighbor as least distortion
            if (neighbors->size() > 0) {
                for (int i = 0; i < neighbors->size(); i++) {
                    NCluster *currNeighbor = neighbors->at(i);
                    currNeighbor->GetSet(0)->SetId(s);
                    currNeighbor->GetSet(1)->SetId(t);
                    double dist = Distortion(bicluster->GetSetById(s), currNeighbor->GetSetById(s),
                            bicluster->GetSetById(t), currNeighbor->GetSetById(t), K, s, t);
                    currNeighbor->SetQuality(-1 * dist);
                }
                sort(neighbors->begin(), neighbors->end(), Compare_Quality_NCluster);
                int kk = neighbors->size() < 5 ? neighbors->size() : 5;
                IOSet *uu = new IOSet(bicluster->GetSetById(s));
                for (int i = 0; i < kk; i++) {
                    IOSet *tmp = uu;
                    uu = Union(neighbors->at(i)->GetSetById(s), uu);
                    delete tmp;
                }
                uu->SetId(s);
                //delete all neighbors
                for (int i = 0; i < neighbors->size(); i++)
                    delete neighbors->at(i);
                delete neighbors;
                bicluster->AssignSetById(s, uu);

            }
            IOSet *tmp = query;
            query = Difference(query, bicluster->GetSetById(s));
            delete tmp;
            hits.push_back(bicluster);
            delete CONCEPTS[0]; //reset for next round
            CONCEPTS.clear(); //reset for next round

        } else {
            break;
        }
    }
    cout << "\nGot total of " << hits.size() << " hits";
    for (int i = 0; i < hits.size(); i++) {
        cout << "\nHit " << i + 1 << "||\n";
        hits[i]->GetSetById(s)->Output(K->GetNameMap(s));
        cout<<"\n";
         hits[i]->GetSetById(t)->Output(K->GetNameMap(t));
         cout << "\nHit " << i + 1 << "||\n";
        hits[i]->GetSetById(s)->Output();
        cout<<"\n";
         hits[i]->GetSetById(t)->Output();

    }
}


void BasicPrefix::StarCharm(){
    IOSet *artDomains = NETWORK->GetArtDomains();
    if(!artDomains->Contains(s) && NETWORK->GetNumNodes() >2){
         string errMsg = "Specifed artiucaltion node domain is not an articulation node\n";
        cerr<<errMsg; exit(-1);
    }
    if( artDomains->Size() != 1 && NETWORK->GetNumNodes() > 2) {
        string errMsg = "StarCharm must be called with a star shaped hin\n";
        cerr<<errMsg; exit(-1);
    }
    if(NETWORK->GetNumNodes() < 2){
        string errMsg = "Network has fewer than two nodes!\n";
        cerr<<errMsg; exit(-1);
    }
    if( NETWORK->GetRContext(1) == NULL){
        string errMsg = "StarCharm called with invalid learner context id for the given hin\n";
        cerr<<errMsg; exit(-1);
    }
    string file1 = OUTFILE+".concepts";
    string file2 = OUTFILE+".concepts.names";
    OUT1.open(file1.c_str());
    OUT2.open(file2.c_str());

    if(enumerationMode == ENUM_FILE && !OUT1.is_open()){
         string errMsg = "Star_CHARM called with ENUM_FILE mode, however, OUTFILE is not valid file or has not been set\n";
        cerr<<errMsg; exit(-1);
    }else if(enumerationMode == ENUM_FILE || enumerationMode == ENUM_TOPK_FILE){
        //setup name maps
        NAME_MAPS = (*NETWORK->GetNameMaps());

    }
    if(enumerationMode == ENUM_TOPK_FILE &&!OUT1.is_open()){
         string errMsg = "Star_CHARM called with ENUM_FILE mode, however, OUTFILE is not valid file or has not been set\n";
        cerr<<errMsg; exit(-1);
    }
    if( PRUNE_SIZE_VECTOR.size() < NETWORK->GetNumNodes()){
        string errMsg = "Star_CHARM called with size pruning, however, PRUNE_SIZE_VECTOR does not contain threshold values for all domains\n";
        cerr<<errMsg; exit(-1);
    }

    //check values of prune_size_vector are all >= 1,
    for(int i=0; i < NETWORK->GetNumNodes(); i++)
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

    //reset variables
     srchLvl=0;
     numConcepts=0;
     //make the initial lists to call c
     list<list<IOSet*> *> tails;
     list<list<IOSet*> *> tailSupSet;
     list<list<NCluster*> *> tailMinMax;
     cout<<"\nMaking initial support sets....\n";
     Make_Init_SupSets_MinMaxIdxs_Star_Charm(tails,tailSupSet,tailMinMax);
     cout<<"\nCalling star charm...\n";
     Enumerate_Star_Charm(tails,tailSupSet,tailMinMax);
}

 void BasicPrefix::Make_Init_SupSets_MinMaxIdxs_Star_Charm(list< list<IOSet*>* > &tails, list< list<IOSet*> *> &tailSupSet, list < list<NCluster*> *> &tailMinMax){
     IOSet *fullSpace = NETWORK->GetRContext(1)->GetLabels(s);
     for(int i=0; i < NETWORK->GetNumNodes()-1; i++){
         tails.push_back(new list<IOSet*>);
         tailSupSet.push_back(new list<IOSet*>);
         tailMinMax.push_back(new list<NCluster*>);
         list<IOSet*> * currTail = tails.back();
         list<IOSet*> * currSupSet = tailSupSet.back();
         list<NCluster*> *currMinMax = tailMinMax.back();

         RContext *currContext = NETWORK->GetRContext(i+1);
         pair<int,int> domains = currContext->GetDomainIds();
         int otherDomain = domains.first == s ? domains.second : domains.first;
         for(int j=0; j < fullSpace->Size(); j++){
             IOSet *currPfx = new IOSet;
             RSet *currCol = currContext->GetSet(s,j);
             if(currCol->Size() >= PRUNE_SIZE_VECTOR[i+1]){  //prune small columns from the get go
                 currPfx->Add(fullSpace->At(j));
                 currPfx->SetId(fullSpace->At(j));
                 currPfx->SetQuality(0);
                 currTail->push_back(currPfx);
                 currSupSet->push_back(currCol->GetIdxs());
                 currSupSet->back()->SetId(currCol->Id());
                 currMinMax->push_back(new NCluster);
                 for(int k=0; k < currSupSet->back()->Size(); k++){
                     IOSet *mm = new IOSet;
                     RSet *theRow = currContext->GetSet(otherDomain,currSupSet->back()->At(k));
                     mm->Add(fullSpace->At(j)); //just adding index of object at this point twice since single object
                     mm->Add(fullSpace->At(j));
                     mm->SetId(currSupSet->back()->At(k));
                     currMinMax->back()->AddSet(mm);
                 }
             }
         }
     }
     //prune the search spaces
     list< list<IOSet*> *>::iterator tailIt = tails.begin();
     list< list<IOSet*> *>::iterator supIt = tailSupSet.begin();
     list< list<NCluster*> *>::iterator minMaxIt = tailMinMax.begin();
     while( tailIt != tails.end()){
         list< list<IOSet*> *>::iterator tailIt2 = tailIt;
         list< list<IOSet*> *>::iterator supIt2 = supIt;
         list< list<NCluster*> *>::iterator minMaxIt2 = minMaxIt;
         tailIt2++; supIt2++; minMaxIt2++;
         while(tailIt2 != tails.end()){
            Prune_Tails( (**tailIt) ,(**supIt),(**minMaxIt),(**tailIt2),(**supIt2),(**minMaxIt2));
            tailIt2++; supIt2++; minMaxIt2++;
         }
         tailIt++;supIt++;minMaxIt++;
     }
 }

void BasicPrefix::Qbbc_Prefix_Search(IOSet *query) {
    pair<int, int> dIds = K->GetDomainIds();
    assert(s == dIds.first || s == dIds.second);
    //the level of the prefix tree will be generated here
    // this entails generating the tails and support rows for
    //each query column

    //1. generate all the prefixes as indivual columns
    list<IOSet *> allPfx;
    list<NCluster*> minMaxIdxs; //store the idxs of the maximum and minimum elements
    list<IOSet*> supportSets;
    Make_Init_SupSets_MinMaxIdxs(query, allPfx, supportSets, minMaxIdxs);
    //sort by size of supporting set
    //allPfx->SortSets();
    //setup algorithm parameters
    enumerationMode = ENUM_TOPK_MEM; //only looking for the top hit
    topKK = 1; //only one top hit
    ovlpMode = AVG_JACCARD; //overlap function will not really be used...but set it anyway
    ovlpFunction = &GreaterEqualSize; //overlap function will not really be used...but set it anyway
    params.push_back(query->Size()); //set the params for use with the quality function...the quality function is fraction of query*avg_range
    qualityFunction = &Imperfect_Query_Quality;
    PRUNE_SIZE_VECTOR.resize(2); // make the prune size vector (at least 2 elements from each domain must be enumerated)
    PRUNE_SIZE_VECTOR[0] = 1;
    PRUNE_SIZE_VECTOR[1] = 1;
    //now iterate again this time generating supporting sets and passing onto main algorithm
    int i = 0;
    for (list<IOSet*>::iterator it = allPfx.begin(); it != allPfx.end(); i++) {
        list<IOSet*> newTail;
        list<IOSet*>newTailSupSets;
        list<NCluster*> newTailMinMax;
        Construct_First_Level(i, allPfx, supportSets, minMaxIdxs, newTail, newTailSupSets, newTailMinMax);
        Enumerate_Charm(newTail, newTailSupSets, newTailMinMax);
        it++;
        i++;
    }
    if (CONCEPTS.size() > 0) {
        sort(CONCEPTS.begin(), CONCEPTS.end(), Compare_Quality_NCluster);
        cout.flush();
        for (int i = 0; i < CONCEPTS.size(); i++) {
            CONCEPTS[i]->GetSet(0)->SetId(s);
            CONCEPTS[i]->GetSet(1)->SetId(t);
        }
        CONCEPTS[0]->Output();
    }
}

void BasicPrefix::Make_Init_SupSets_MinMaxIdxs(IOSet *query, list<IOSet*> &prefix, list<IOSet*> &supSets, list<NCluster*> &minMax) {
    for (int i = 0; i < query->Size(); i++) {
        //make prefix node
        IOSet *prefixx = new IOSet;
        RSet *currCol = K->GetSet(s, query->At(i));
        prefixx->Add(query->At(i));
        prefixx->SetId(currCol->Id());
        prefixx->SetQuality(currCol->Size());
        prefix.push_back(prefixx);
        supSets.push_back(currCol->GetIdxs());
        supSets.back()->SetId(currCol->Id());
        minMax.push_back(new NCluster);
        for (int j = 0; j < supSets.back()->Size(); j++) {
            IOSet *mm = new IOSet;
            RSet *theRow = K->GetSet(t, supSets.back()->At(j));
            mm->Add(query->At(i)); //just adding index of query object at this point since only single object
            mm->Add(query->At(i)); //this is true for both max and min
            mm->SetId(supSets.back()->At(j));
            minMax.back()->AddSet(mm);
        }
    }
}

NCluster * BasicPrefix::Get_Min_Max_Idxs(IOSet *query, IOSet *supSets) {
    NCluster *ret = new NCluster();
    for (int i = 0; i < supSets->Size(); i++) {
        ret->AddSet(new IOSet);
        pair<int, int> minMax = K->GetSet(t, supSets->At(i))->GetMinMaxSubspaceIdxs(query);
        ret->GetSet(i)->Add(minMax.first);
        ret->GetSet(i)->Add(minMax.second);
        ret->GetSet(i)->SetId(supSets->At(i));
    }
    return ret;
}

vector<NCluster*>* BasicPrefix::Range_Closure_Charm_Neighbors(IOSet *prefix, IOSet *supSet, NCluster *minMax) {
    //compute closure and all the upper neighbors
    //get all other objects and their supporting sets + min-max idxs
    IOSet *otherObjs = Difference(K->GetLabels(s), prefix);
    IOSet *closure = new IOSet;
    //the current lists
    list<IOSet *> allPfx;
    list<NCluster*> minMaxIdxs; //store the idxs of the maximum and minimum elements
    list<IOSet*> supportSets;
    Make_Init_SupSets_MinMaxIdxs(otherObjs, allPfx, supportSets, minMaxIdxs);
    //     //now make lists that will be used for upper neighbors
    list<IOSet *> tail;
    list<IOSet *> tailSupSet;
    list<NCluster *> tailMinMax;

    //for each other object range intersect supporting sets, if result is of equal size to supSet, then include item in closure
    list<IOSet*>::iterator supSetIt = supportSets.begin();
    list<NCluster*>::iterator minMaxIt = minMaxIdxs.begin();
    for (int i = 0; i < otherObjs->Size(); i++) {
        //cout<<"\non closure: "<<i<<"\t"<<otherObjs->Size();
       // cout.flush();
        if ((*supSetIt)->Size() >= supSet->Size()) {
            IOSet *supSetRslt = new IOSet;
            NCluster *minMaxRslt = new NCluster;
            Range_Intersect(supSet, (*supSetIt), minMax, (*minMaxIt), supSetRslt, minMaxRslt,K->GetId());
            if (supSetRslt->Size() > 0) {
                if (supSetRslt->Size() == supSet->Size()) {
                    prefix->Add(otherObjs->At(i));
                    delete supSetRslt; //delete since they will not be used in the tail to generate neighbors
                    delete minMaxRslt;
                } else {
                    IOSet *single = new IOSet;
                    single->Add(otherObjs->At(i));
                    tail.push_back(Union(prefix, single)); //make a union for the prefix tail
                    tailSupSet.push_back(supSetRslt); //store the supporting set
                    tailMinMax.push_back(minMaxRslt); //store the min max idxs
                    delete single;
                }

            } else {
                delete supSetRslt;
                delete minMaxRslt;
            }
        }
        delete (*supSetIt);
        delete (*minMaxIt);
        supSetIt = RemoveFromList(supportSets, supSetIt);
        minMaxIt = RemoveFromList(minMaxIdxs, minMaxIt);
    }
    delete otherObjs;
    DstryList(allPfx);
    //     //replace prefix by closure
    prefix->Sort(); //incase it was unsorted by additions
    //delete tmp;
    //now get the neighbors
    vector<NCluster*> * neighbors = Charm_UpperNeighbors(tail, tailSupSet, tailMinMax);
    return neighbors;
}

vector<NCluster*> * BasicPrefix::Charm_UpperNeighbors(list<IOSet*> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax) {
    vector<NCluster*> *neighbors = new vector<NCluster*>;
    if (tail.size() == 0) {
        return neighbors;
    }
   // cout<<"\nUppers to check "<<tail.size();
    list<IOSet*>::iterator tailIt = tail.begin();
    list<IOSet*>::iterator tailSupIt = tailSupSet.begin();
    list<NCluster*>::iterator minMaxIt = tailMinMax.begin();
    tailIt++;
    tailSupIt++;
    minMaxIt++; //advance one to only compute other neighbors since prefix is first memeber of tail
    int ctr=1;
    while (tailIt != tail.end()) {
        //cout<<"\nneighbor: "<<ctr<<"\tof\t"<<tail.size();
        //cout.flush();
        IOSet *currPrefix = (*tailIt);
        IOSet *currSupSet = (*tailSupIt);
        NCluster *currMinMax = (*minMaxIt);
        //-------------BEGIN CHARM OPTIMIZATION-------------------------
        list<IOSet*>::iterator tailItC = tailIt;
        list<IOSet*>::iterator tailSupItC = tailSupIt;
        list<NCluster*>::iterator minMaxItC = minMaxIt;
        tailItC++;
        tailSupItC++;
        minMaxItC++;
        bool makeNeighbor = true;
        while (tailItC != tail.end()) {
            IOSet *supSetRslt = new IOSet;
            NCluster* minMaxRslt = new NCluster;
            Range_Intersect(currSupSet, (*tailSupItC), currMinMax, (*minMaxItC), supSetRslt, minMaxRslt,K->GetId());
           // cout<<"\nsup set size: "<<supSetRslt->Size();
            if (supSetRslt->Size() == currSupSet->Size() && supSetRslt->Size() == (*tailSupItC)->Size()) {
                //update the curr prefix
                IOSet *tmp = currPrefix;
                currPrefix = Union(currPrefix, (*tailItC));
                delete tmp;
                //update the current tail and supporting sets
                delete (*tailItC);
                delete (*tailSupItC);
                delete (*minMaxItC);
                tailItC = RemoveFromList(tail, tailItC);
                tailSupItC = RemoveFromList(tailSupSet, tailSupItC);
                minMaxItC = RemoveFromList(tailMinMax, minMaxItC);

            } else if (supSetRslt->Size() == currSupSet->Size()) {
                //update the curr prefix
                IOSet *tmp = currPrefix;
                currPrefix = Union(currPrefix, (*tailItC));
                delete tmp;
                //get rid of subsets...these are not upper neighbors
                delete (*tailItC);
                delete (*tailSupItC);
                delete (*minMaxItC);
                tailItC = RemoveFromList(tail, tailItC);
                tailSupItC = RemoveFromList(tailSupSet, tailSupItC);
                minMaxItC = RemoveFromList(tailMinMax, minMaxItC);
            } else if (supSetRslt->Size() == (*tailSupItC)->Size()) {
                //get rid of self, cannot be an upper neighbor
                makeNeighbor = false;
                tailItC = tail.end(); //to break the loop

            } else {
                tailItC++;
                tailSupItC++;
                minMaxItC++;
            }
            //clear the results, they were just for checking
            delete supSetRslt;
            delete minMaxRslt;

        }
        //-------------END CHARM OPTIMIZATION---------------------------
        if (makeNeighbor) {
            if (currPrefix->Size() > 1 && currSupSet->Size() > 1) {
                NCluster *bicluster = new NCluster;
                bicluster->AddSet(new IOSet(currPrefix));
                bicluster->AddSet(new IOSet(currSupSet));
                bicluster->GetSet(0)->SetId(s);
                bicluster->GetSet(1)->SetId(t);
                // cout<<"\n neighbor: !\n"; bicluster->Output();
                neighbors->push_back(bicluster);
            }
        }
        delete currPrefix;
        delete currSupSet;
        delete currMinMax;
        tailIt = RemoveFromList(tail, tailIt);
        tailSupIt = RemoveFromList(tailSupSet, tailSupIt);
        minMaxIt = RemoveFromList(tailMinMax, minMaxIt);
        ctr++;

    }
    return neighbors;
}

void BasicPrefix::Prune_Tails(list<IOSet*> &tail1, list<IOSet*> &tailSupSet1, list<NCluster*> &tailMinMax1,
        list<IOSet*> &tail2, list<IOSet*> &tailSupSet2, list<NCluster*> &tailMinMax2) {
    list<IOSet*>::iterator tailIt1 = tail1.begin();
    list<IOSet*>::iterator tailSupIt1 = tailSupSet1.begin();
    list<NCluster*>::iterator minMaxIt1 = tailMinMax1.begin();
    list<IOSet*>::iterator tailIt2 = tail2.begin();
    list<IOSet*>::iterator tailSupIt2 = tailSupSet2.begin();
    list<NCluster*>::iterator minMaxIt2 = tailMinMax2.begin();

    //cout<<"\nIn prune tails...";
    //cout<<"\ntail1 "<<tail1.size()<<"\ttail2 "<<tail2.size();
    //take symmetric difference of ids and remove those nodes
    //get symmetric difference
    IOSet *ids1 = new IOSet;
    IOSet *ids2 = new IOSet;
    while (tailIt1 != tail1.end()) {
        ids1->Add((*tailIt1)->Id());
        tailIt1++;
    }
    while (tailIt2 != tail2.end()) {
        ids2->Add((*tailIt2)->Id());
        tailIt2++;
    }
    IOSet *symmDiff = SymmDifference(ids1, ids2);
    //now remove those nodes that have ids in symmDiff
    tailIt1 = tail1.begin();
    int i = 0;
    while (i < symmDiff->Size() && tailIt1 != tail1.end()) {
        if (symmDiff->At(i) == (*tailIt1)->Id()) { //remove this node
            delete (*tailIt1);
            delete (*tailSupIt1);
            delete (*minMaxIt1);
            tailIt1 = RemoveFromList(tail1, tailIt1);
            tailSupIt1 = RemoveFromList(tailSupSet1, tailSupIt1);
            minMaxIt1 = RemoveFromList(tailMinMax1, minMaxIt1);
            i++;
        } else if (symmDiff->At(i) < (*tailIt1)->Id()) {
            i++;
        } else {
            tailIt1++;
            tailSupIt1++;
            minMaxIt1++;
        }
    }
    tailIt2 = tail2.begin();
    i = 0;
    while (i < symmDiff->Size() && tailIt2 != tail2.end()) {
        if (symmDiff->At(i) == (*tailIt2)->Id()) { //remove this node
            delete (*tailIt2);
            delete (*tailSupIt2);
            delete (*minMaxIt2);
            tailIt2 = RemoveFromList(tail2, tailIt2);
            tailSupIt2 = RemoveFromList(tailSupSet2, tailSupIt2);
            minMaxIt2 = RemoveFromList(tailMinMax2, minMaxIt2);
            i++;
        } else if (symmDiff->At(i) < (*tailIt2)->Id()) {
            i++;
        } else {
            tailIt2++;
            tailSupIt2++;
            minMaxIt2++;
        }
    }
    //clean up
    delete ids1;
    delete ids2;
    delete symmDiff;
    //cout<<"\ndone prune tails...";
   // cout<<"\ntail1 "<<tail1.size()<<"\ttail2 "<<tail2.size();
}


bool BasicPrefix::Adjust_Tails(list<IOSet*> &tail1, list<IOSet*> &tailSupSet1, list<NCluster*> &tailMinMax1,
                                list<IOSet*> &tail2, list<IOSet*> &tailSupSet2, list<NCluster*> &tailMinMax2, int otherId) {

    IOSet *inter = Intersect(tail1.front(),tail2.front());
    bool ret=true;
//    cout<<"\nIn adjust tails....\n";
//    cout<<"\nHere are prefixes: \n"; tail1.front()->Output();
//    cout<<"\n"; tail2.front()->Output();
//    cout.flush();
    if(inter->Size() == tail1.front()->Size() && tail1.front()->Size() == tail2.front()->Size()){
        //the two prefixes are equal nothing to do!
       // cout<<"\nEqual! Cluster is possible"; cout.flush();
        Prune_Tails(tail1,tailSupSet1,tailMinMax1,tail2,tailSupSet2,tailMinMax2);
        ;
    }else if( inter->Size() == tail2.front()->Size() && tail1.front()->Size() > tail2.front()->Size()){
          //tail1.front() is a superset of of tail2.front()
          //try to locate matching cluster for tail2.front() in the tail
          // if cannot find return false
       // cout<<"\nLearner is superset of client ...check if possible to make cluster!"; cout.flush();
        if(Determine_Subset_Cluster_Star_Charm(tail1.front(),tail2,tailSupSet2,tailMinMax2,otherId)){
              //possible to form cluster
          //  cout<<"\nCould form cluster!"; cout.flush();
            ret=true;
        }else{
           // cout<<"\nCould NOT form cluster!"; cout.flush();
            ret=false;
        }
        //adjust the tails
        Prune_Tails(tail1,tailSupSet1,tailMinMax1,tail2,tailSupSet2,tailMinMax2);

    }else if(inter->Size() == tail1.front()->Size() && tail1.front()->Size() < tail2.front()->Size() ){
        //tail1.front() is subset of tail2.front()
        //prune tail2.front() or tail1.front() but still return true
        Prune_Tails(tail1,tailSupSet1,tailMinMax1,tail2,tailSupSet2,tailMinMax2);
       // cout<<"\nclient is super set...cluster is possible"; cout.flush();
    }else{
        //no relation between two prefixes
        //cluster formation not possible
        //return false
        ret=false;
       // cout<<"\nNo relation, no cluster possible...."; cout.flush();
        Prune_Tails(tail1,tailSupSet1,tailMinMax1,tail2,tailSupSet2,tailMinMax2);
    }
    delete inter;
    return ret;
}

void BasicPrefix::Range_Intersect(IOSet *supSet1, IOSet *supSet2, NCluster* minMax1, NCluster* minMax2,
        IOSet *supSetRslt, NCluster* minMaxRslt,int ctxId) {
    assert(supSetRslt != NULL && minMaxRslt != NULL);
    supSetRslt->SetQuality(0.0);
    //first intersect the indices
    IOSet *commonIdxs = Intersect(supSet1, supSet2);
    //update min max results for each index compute range and add sets that meet range requirement
    for (int i = 0; i < commonIdxs->Size(); i++) {
        //get max and mins for curr row
        int rowId = commonIdxs->At(i);
        RSet *row = K->GetSet(t, rowId);
        int oldMin1 = minMax1->GetSetById(rowId)->At(0);
        int oldMin2 = minMax2->GetSetById(rowId)->At(0);
        int oldMax1 = minMax1->GetSetById(rowId)->At(1);
        int oldMax2 = minMax2->GetSetById(rowId)->At(1);
        double minVal = min(K->GetSet(t, rowId)->At(oldMin1).second, K->GetSet(t, rowId)->At(oldMin2).second);
        double maxVal = max(K->GetSet(t, rowId)->At(oldMax1).second, K->GetSet(t, rowId)->At(oldMax2).second);
        int minn = (minVal == K->GetSet(t, rowId)->At(oldMin1).second) ? oldMin1 : oldMin2;
        int maxx = (maxVal == K->GetSet(t, rowId)->At(oldMax1).second) ? oldMax1 : oldMax2;
        //get range using new idxs
        double range = K->GetSet(t, rowId)->At(maxx).second - K->GetSet(t, rowId)->At(minn).second;
        //construct parameters for consistency function
        vector<double> lclParamsF; //construct parameter vector for the consistency function, make alpha the first element though
        lclParamsF.push_back(alpha[ctxId]); //assign the variance for this particlar row / column
        paramFunction(K, commonIdxs, s, t, rowId, lclParamsF);
        //now do consistency check
        //cout<<"\nrange: "<<range<<"\t"<<consistencyFunction(row, lclParamsF);
        if (range < consistencyFunction(row, lclParamsF)) {
            supSetRslt->Add(rowId);
            supSetRslt->SetQuality(supSetRslt->GetQuality() + range);
            IOSet *minMax = new IOSet;
            minMax->Add(minn);
            minMax->Add(maxx);
            minMax->SetId(rowId);
            minMaxRslt->AddSet(minMax);
        }
    }
    if (supSetRslt->Size() > 0) supSetRslt->SetQuality(supSetRslt->GetQuality() / (double) supSetRslt->Size());
    else supSetRslt->SetQuality(-1);
    delete commonIdxs;
}

 bool BasicPrefix::Satisfy_Half_Condition(list<IOSet*> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax,RContext *k,int ss, int tt){
     list<IOSet*>::iterator tailIt = tail.begin();
     list<IOSet*>::iterator tailSupIt = tailSupSet.begin();
     list<NCluster*>::iterator minMaxIt = tailMinMax.begin();
     int ctr=1;
     while (tailIt != tail.end()) {
         //cout<<"\nctr\t"<<ctr<<"\t"<<tail.size();
         IOSet *commonIdxs = (*tailSupIt);
         if((*tailIt)->Size() >= 2){
             NCluster *currMinMax = (*minMaxIt);
              for (int i = 0; i < commonIdxs->Size(); i++) {
                int rowId = commonIdxs->At(i);
                RSet *row = k->GetSet(tt, rowId);
                double range = k->GetSet(tt, rowId)->At(currMinMax->GetSetById(rowId)->At(1)).second - k->GetSet(tt, rowId)->At(currMinMax->GetSetById(rowId)->At(0)).second;
                vector<double> lclParamsF; //construct parameter vector for the consistency function, make alpha the first element though
                lclParamsF.push_back(alpha[k->GetId()]); //assign the variance for this particlar row / column
                paramFunction(k, commonIdxs, ss, tt, rowId, lclParamsF);
                //now do consistency check
               // cout<<"\nrange: "<<range;
                if (range > consistencyFunction(row, lclParamsF)*0.5){
                   return false;
                }
              }
         }
        tailIt++;
        tailSupIt++;
        minMaxIt++;
        ctr++;
     }
     return true;
 }

void BasicPrefix::Range_Intersect_Star_Charm(IOSet *supSet1, IOSet *supSet2, NCluster* minMax1, NCluster* minMax2,
                      IOSet *supSetRslt, NCluster* minMaxRslt, int otherDomain,int ctxId){

//    cout<<"\nIntersecting: \n"; supSet1->Output(); cout<<"\n"; supSet2->Output(); cout.flush();
//    cout<<"\nother domain : "<<otherDomain;
//    cout<<"\nMin maxs: \n"; minMax1->Output(); cout<<"\n"; minMax2->Output(); cout.flush();
     assert(supSetRslt != NULL && minMaxRslt != NULL);
     RContext *currContext = NETWORK->GetRContext(s,otherDomain);
     //supSetRslt->SetQuality(0.0);
    //first intersect the indices
     IOSet *commonIdxs = Intersect(supSet1, supSet2);
    // cout<<"\n"<<commonIdxs->Size()<<"\t"<<PRUNE_SIZE_VECTOR[otherDomain-1];
     if (commonIdxs->Size() >= PRUNE_SIZE_VECTOR[otherDomain-1]){
    //update min max results for each index compute range and add sets that meet range requirement
         for (int i = 0; i < commonIdxs->Size(); i++) {
            //get max and mins for curr row
            int rowId = commonIdxs->At(i);
            RSet *row = currContext->GetSet(otherDomain, rowId);
            int oldMin1 = minMax1->GetSetById(rowId)->At(0);
            int oldMin2 = minMax2->GetSetById(rowId)->At(0);
            int oldMax1 = minMax1->GetSetById(rowId)->At(1);
            int oldMax2 = minMax2->GetSetById(rowId)->At(1);
            double minVal = min(currContext->GetSet(otherDomain, rowId)->At(oldMin1).second, currContext->GetSet(otherDomain, rowId)->At(oldMin2).second);
            double maxVal = max(currContext->GetSet(otherDomain, rowId)->At(oldMax1).second, currContext->GetSet(otherDomain, rowId)->At(oldMax2).second);
            int minn = (minVal == currContext->GetSet(otherDomain, rowId)->At(oldMin1).second) ? oldMin1 : oldMin2;
            int maxx = (maxVal == currContext->GetSet(otherDomain, rowId)->At(oldMax1).second) ? oldMax1 : oldMax2;
            //get range using new idxs
            double range = currContext->GetSet(otherDomain, rowId)->At(maxx).second - currContext->GetSet(otherDomain, rowId)->At(minn).second;
            //construct parameters for consistency function
            vector<double> lclParamsF; //construct parameter vector for the consistency function, macurrContexte alpha the first element though
            lclParamsF.push_back(alpha[ctxId]); //assign the variance for this particlar row / column
            paramFunction(currContext, commonIdxs, s, otherDomain, rowId, lclParamsF);
            //now do consistency checcurrContext
            if (range < consistencyFunction(row, lclParamsF)) {
                supSetRslt->Add(rowId);
                supSetRslt->SetQuality(supSetRslt->GetQuality() + range);
                IOSet *minMax = new IOSet;
                minMax->Add(minn);
                minMax->Add(maxx);
                minMax->SetId(rowId);
                minMaxRslt->AddSet(minMax);
            }
        }
     }
   //  cout<<"\nsupSetRslt size: "<<supSetRslt->Size();
    //if (supSetRslt->Size() > 0) supSetRslt->SetQuality(supSetRslt->GetQuality() / (double) supSetRslt->Size());
    //else supSetRslt->SetQuality(-1);
    delete commonIdxs;
   // cout<<"\nresult: \n"; supSetRslt->Output(); cout<<"\n"; minMaxRslt->Output();
 }


void BasicPrefix::Enumerate_Star_Charm(list< list<IOSet*>* > &tails, list< list<IOSet*> *> &tailSupSet, list < list<NCluster*> *> &tailMinMax){

    srchLvl++;
    int lclIters=0;
    while (tails.front()->size() > 0) {
        if(dispProgress && srchLvl == 1){
            cout<<"\nOn "<<lclIters+1<<" of "<<tails.front()->size()<<"\tgot "<<numConcepts<<" cluster so far...";
            cout.flush();
            
        }
        if(lclIters > 0){
//            cout<<"\ncompare tails....\n";
//            Output_Tail(**tails.begin());
//            cout<<"\n";
//             Output_Tail(**(++tails.begin()));
        }
        //outer iterator of tails
        list< list<IOSet*>* >::iterator outerTailIt = tails.begin();
        list< list<IOSet*>* >::iterator outerSupIt = tailSupSet.begin();
        list< list<NCluster*>* >::iterator outerMinMaxIt = tailMinMax.begin();
        
        list<list<IOSet*> *> newTails;
        list<list<IOSet*> *> newSupSets;
        list<list<NCluster*> *> newMinMaxs;
        int otherId=2;
        //cout<<"\ntails size: "<<tails.size();
        bool clusterFound=true;
        bool cont=true; //continue with search?
        // cout<<"\ncompare tails....\n";
       //  Output_Tail(**tails.begin());
       //  cout<<"\n~sup size: "<<tailSupSet.front()->size();
       //  cout<<"\n...next one....\n";
       //  cout<<"\n~sup size: "<<(*++tailSupSet.begin())->size();
       //   Output_Tail(**++tails.begin());
        for (int i = 0; i < tails.size(); i++){
            //run charm on this edge?
          // cout<<"\nCHARM STEP "<<otherId;
                clusterFound=true;
                list<IOSet *>* newTail = new list<IOSet*>;
                list<IOSet *>* newSupSet = new list<IOSet*>;
                list<NCluster *>* newMinMax = new list<NCluster*>;
              // cout<<"\ncurr tail before charm: ";
              // Output_Tail(**outerTailIt);
              // cout<<"\nsup set before: \n";
              // (*outerSupIt)->front()->Output();
             //  cout.flush();
                Star_Charm_Step(**outerTailIt, **outerSupIt, **outerMinMaxIt, *newTail, *newSupSet, *newMinMax,otherId);
                //first add new tails if any..and do pruning...
                if(newTail->size() > 0){
                     //prune new tails
                     newTails.push_back(newTail);
                     newSupSets.push_back(newSupSet);
                     newMinMaxs.push_back(newMinMax);
                     //prune new tails if necessary
                     if(i > 0 ){
                                Prune_Tails(*newTails.front(),*newSupSets.front(),*newMinMaxs.front(),
                                    *newTails.back(),*newSupSets.back(),*newMinMaxs.back() );
                     }
                }else{
                    delete newTail;
                    delete newSupSet;
                    delete newMinMax;
                    cont=false;
                }
                //now check if cluster formation is possible
                if(i > 0){
                        if(Adjust_Tails(*tails.front(),*tailSupSet.front(),*tailMinMax.front(), **outerTailIt,**outerSupIt,**outerMinMaxIt,otherId)){
                                //cluster formation possible
                            
                        }
                        else{ //not possible to make cluster
                            clusterFound=false;
                            cont=false;
                        }
                }
                if(!clusterFound){
                    break;
                }
               // cout<<"\ncurr tail after charm: ";
              //  Output_Tail(**outerTailIt);
               // cout<<"\nnew sup set after: ";
                //(*outerSupIt)->front()->Output();
                //cout.flush();
                outerTailIt++;
                outerSupIt++;
                outerMinMaxIt++;
                otherId++;
//                cout<<"\npotential cluster: ";
//                (*(*tails.begin())->begin())->Output();
//                cout<<"\nsupport set: ";
//                (*(*tailSupSet.begin())->begin())->Output();
        }
//         cout<<"\ncompare tails after charm steps....\n";
//         Output_Tail(**tails.begin());
//         cout<<"\n~sup size: "<<tailSupSet.front()->size();
//         cout<<"\n...next one....\n";
//         cout<<"\n~sup size: "<<(*++tailSupSet.begin())->size();
//         Output_Tail(**++tails.begin());
        //determine if cluster is found
        if( clusterFound){
            //create cluster and recurse
            //cout<<"\ncluster found...\nis tail large enough?"; cout.flush();
            if( (*(*tails.begin())->begin())->Size() >= PRUNE_SIZE_VECTOR[0] && Check_Size_Bounds(tailSupSet, PRUNE_SIZE_VECTOR) ){
                 // cout<<"\nNew cluster!\n";
                NCluster *ncluster = new NCluster;
                //(*(*tails.begin())->begin())->Output();
               // cout<<"\n";
               // cout.flush();
                ncluster->AddSet(new IOSet((*(*tails.begin())->begin())));
                ncluster->GetSet(0)->SetId(1);
                 list< list<IOSet*>* >::iterator supIt = tailSupSet.begin();
                 int idd=2;
                 while(supIt != tailSupSet.end()){
                      // (*(*supIt)->begin())->Output();
                       // cout<<"\n";
                       // cout.flush();
                         ncluster->AddSet(new IOSet((*(*supIt)->begin())));
                       
                         ncluster->GetSet(idd-1)->SetId(idd);
                     
                     supIt++;
                     idd++;
                 }
//                 cout<<"\nNew cluster: \n";
//                 ncluster->Output();
//                 cout.flush();
                 if( enumerationMode == ENUM_MEM){
                        StoreCluster(CONCEPTS,ncluster);
                        numConcepts=CONCEPTS.size();
                 }
                 else if(enumerationMode == ENUM_FILE) {
                        OutputCluster(ncluster,OUT1);
                        OutputCluster(ncluster,OUT2,NAME_MAPS);
                        numConcepts++;
                  }else if( ( enumerationMode == ENUM_TOPK_MEM) || (enumerationMode == ENUM_TOPK_FILE)){
                        SetQuality(ncluster,topKparams,qualityFunction);
                        RetainTopK_Overlap(CONCEPTS,ncluster,ovlpFunction,ovlpThresh,topKK);
                        numConcepts=CONCEPTS.size();
                  }
            }
           // cout<<"\nnew tails size before recursive: "<<newTails.size(); cout.flush();
            if(newTails.size() == NETWORK->GetNumNodes()-1 && cont){
               // cout<<"\nrecursing...";
               // cout.flush();
                Enumerate_Star_Charm(newTails,newSupSets,newMinMaxs);
                srchLvl--;
            }else{
               // cout<<"\ndeleting new tails...\n";
               // cout.flush();
                Delete_New_Tails_Star_Charm(newTails,newSupSets,newMinMaxs);
            }
        }else{ //no cluseter found
            Delete_New_Tails_Star_Charm(newTails,newSupSets,newMinMaxs);
        }
         //update all the tails
         //cout<<"\nUpdate tails";
        // cout.flush();
         Update_AllTails_Iterators_Star_Charm(tails, tailSupSet, tailMinMax);
         lclIters++;
    }
}






bool BasicPrefix::Determine_Subset_Cluster_Star_Charm(IOSet *lrnrSet, list<IOSet*> &clientTails, list<IOSet*> &clientSupSets,
                                                      list<NCluster*> &clientMinMaxs,int clientId){
    IOSet *clientSet = clientTails.front();
    IOSet *currSupSet = clientSupSets.front();
    NCluster *currMinMax = clientMinMaxs.front();
    IOSet *diff = Difference(lrnrSet,clientSet);
    list<IOSet*>::iterator it = clientTails.begin();
    list<IOSet*>::iterator supIt = clientSupSets.begin();
    list<NCluster*>::iterator minMaxIt = clientMinMaxs.begin();
    it++; supIt++; minMaxIt++;
    bool ret=true;
    while(it != clientTails.end()){
        if( diff->Contains( (*it)->Id()) ){
            IOSet *supSetRslt = new IOSet;
            NCluster* minMaxRslt = new NCluster;
            Range_Intersect_Star_Charm(currSupSet, (*supIt), currMinMax, (*minMaxIt), supSetRslt, minMaxRslt,clientId,NETWORK->GetRContext(1,clientId)->GetId());
            if(supSetRslt->Size() > 0){
                clientSet->Add( (*it)->Id());
                IOSet *tmp = currSupSet;
                NCluster *tmp1 = currMinMax;
                currSupSet = supSetRslt;
                currMinMax = minMaxRslt;
                delete tmp; delete tmp1;

            }else{
                delete supSetRslt;delete minMaxRslt;
                ret=false;
                break;
            }
        }
        it++;supIt++;minMaxIt++;
    }
    //get rid of old tails and replace with new ones
    clientTails.erase(clientTails.begin()); clientSet->Sort(); clientTails.push_front(clientSet);
    clientSupSets.erase(clientSupSets.begin()); clientSupSets.push_front(currSupSet);
    clientMinMaxs.erase(clientMinMaxs.begin()); clientMinMaxs.push_front(currMinMax);
    delete diff;
    return ret;
}

void BasicPrefix::Enumerate_Charm(list<IOSet*> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax) {
    if (tail.size() == 0) {
        return;
    }
    list<IOSet*>::iterator tailIt = tail.begin();
    list<IOSet*>::iterator tailSupIt = tailSupSet.begin();
    list<NCluster*>::iterator minMaxIt = tailMinMax.begin();
    bool flg = Satisfy_Half_Condition(tail,tailSupSet, tailMinMax,K,s,t);
    while (tailIt != tail.end()) {
        IOSet *currPrefix = (*tailIt);
        IOSet *currSupSet = (*tailSupIt);
        NCluster *currMinMax = (*minMaxIt);
        list<IOSet*> newTail;
        list<IOSet*> newTailSupSets;
        list<NCluster*> newTailMinMax;
        //-------------BEGIN CHARM OPTIMIZATION-------------------------
        list<IOSet*>::iterator tailItC = tailIt;
        list<IOSet*>::iterator tailSupItC = tailSupIt;
        list<NCluster*>::iterator minMaxItC = minMaxIt;
        tailItC++;
        tailSupItC++;
        minMaxItC++;

        while (tailItC != tail.end()) {
            IOSet *supSetRslt = new IOSet;
            NCluster* minMaxRslt = new NCluster;
            //first perform intersection then all cases follow
            Range_Intersect(currSupSet, (*tailSupItC), currMinMax, (*minMaxItC), supSetRslt, minMaxRslt,K->GetId());
            //now implement each case....
            if (supSetRslt->Size() > PRUNE_SIZE_VECTOR[1]) {
                if (supSetRslt->Size() == currSupSet->Size() && supSetRslt->Size() == (*tailSupItC)->Size()) {
                    //update the curr prefix
                    IOSet *tmp = currPrefix;
                    currPrefix = Union(currPrefix, (*tailItC));
                    delete tmp;
                    //update the current tail and supporting sets
                    if (flg){
                        delete (*tailItC);
                        delete (*tailSupItC);
                        delete (*minMaxItC);
                        tailItC = RemoveFromList(tail, tailItC);
                        tailSupItC = RemoveFromList(tailSupSet, tailSupItC);
                        minMaxItC = RemoveFromList(tailMinMax, minMaxItC);
                    }else{
                        tailItC++;
                        tailSupItC++;
                        minMaxItC++;
                    }
                    delete supSetRslt;
                    delete minMaxRslt;
                } else if (supSetRslt->Size() == currSupSet->Size()) {
                    //update the curr prefix
                    IOSet *tmp = currPrefix;
                    currPrefix = Union(currPrefix, (*tailItC));
                    delete tmp;
                    delete supSetRslt;
                    delete minMaxRslt;
                    //increment iterators
                    tailItC++;
                    tailSupItC++;
                    minMaxItC++;
                } else if (supSetRslt->Size() == (*tailSupItC)->Size()) {
                    //add to new tail
                    newTail.push_back(Union(currPrefix, (*tailItC)));
                    newTailSupSets.push_back(supSetRslt);
                    newTailMinMax.push_back(minMaxRslt);
                    //update the current tail and supporting sets
                    if(flg){
                        delete (*tailItC);
                        delete (*tailSupItC);
                        delete (*minMaxItC);
                        tailItC = RemoveFromList(tail, tailItC);
                        tailSupItC = RemoveFromList(tailSupSet, tailSupItC);
                        minMaxItC = RemoveFromList(tailMinMax, minMaxItC);
                    }else{
                         tailItC++;
                         tailSupItC++;
                         minMaxItC++;
                    }

                } else {
                    //add to new tail
                    newTail.push_back(Union(currPrefix, (*tailItC)));
                    newTailSupSets.push_back(supSetRslt);
                    newTailMinMax.push_back(minMaxRslt);
                    //increment iterators
                    tailItC++;
                    tailSupItC++;
                    minMaxItC++;
                }
            } else {
                delete supSetRslt;
                delete minMaxRslt;
                //increment iterators
                tailItC++;
                tailSupItC++;
                minMaxItC++;
            }
        }
        //-------------END CHARM OPTIMIZATION---------------------------
        //the current tail(i) and its support set should constitute a new bi-cluster
        NCluster *bicluster = new NCluster;
        if (currPrefix->Size() > 1 && currSupSet->Size() > 1) {

            bicluster->AddSet(new IOSet(currPrefix));
            bicluster->AddSet(new IOSet(currSupSet));
        } else {
            bicluster->AddSet(new IOSet());
            bicluster->AddSet(new IOSet());
        }

        //remove the current prefix,support set, and indicies and advance pointers to next element in tail
        delete currPrefix;
        delete currSupSet;
        delete currMinMax;
        tailIt = RemoveFromList(tail, tailIt);
        tailSupIt = RemoveFromList(tailSupSet, tailSupIt);
        minMaxIt = RemoveFromList(tailMinMax, minMaxIt);
        //check for different output modes
        if (enumerationMode == ENUM_TOPK_MEM && bicluster->GetSet(1)->Size() > PRUNE_SIZE_VECTOR[1]) {
            SetQuality(bicluster, params, qualityFunction);
            RetainTopK_Overlap(CONCEPTS, bicluster, ovlpFunction, ovlpThresh, topKK);
        }
        delete bicluster;
        //now recurse
        Enumerate_Charm(newTail, newTailSupSets, newTailMinMax);
    }
}

void BasicPrefix::Star_Charm_Step(list<IOSet*> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax,
                 list<IOSet*> &newTail, list<IOSet*> &newTailSupSets, list<NCluster*> &newTailMinMax,int otherDomain ) {
    if (tail.size() == 0) {
        return;
    }
    list<IOSet*>::iterator tailIt = tail.begin();
    list<IOSet*>::iterator tailSupIt = tailSupSet.begin();
    list<NCluster*>::iterator minMaxIt = tailMinMax.begin();
    IOSet *currPrefix = (*tailIt);
    IOSet *currSupSet = (*tailSupIt);
    NCluster *currMinMax = (*minMaxIt);
    //-------------BEGIN CHARM OPTIMIZATION-------------------------
    list<IOSet*>::iterator tailItC = tailIt;
    list<IOSet*>::iterator tailSupItC = tailSupIt;
    list<NCluster*>::iterator minMaxItC = minMaxIt;
    tailItC++;
    tailSupItC++;
    minMaxItC++;
   // cout<<"\nchecking half condition....\n";
   // cout.flush();
    bool flg = Satisfy_Half_Condition(tail,tailSupSet, tailMinMax,NETWORK->GetRContext(1,otherDomain),1,otherDomain);
    int ctr=1;
    while (tailItC != tail.end()){
            IOSet *supSetRslt = new IOSet;
            NCluster* minMaxRslt = new NCluster;
            //cout<<"\nctr\t"<<ctr<<"\t"<<tail.size();
            //first perform intersection then all cases follow
            //cout<<"\nrange intersecting....\n";
            //cout.flush();
            Range_Intersect_Star_Charm(currSupSet, (*tailSupItC), currMinMax, (*minMaxItC), supSetRslt, minMaxRslt,otherDomain,NETWORK->GetRContext(1,otherDomain)->GetId());
            //now implement each case....
            //cout<<"\ntesting "<<supSetRslt->Size()<<"\t"<<PRUNE_SIZE_VECTOR[otherDomain-1];
            //cout.flush();
            if (supSetRslt->Size() >=  PRUNE_SIZE_VECTOR[otherDomain-1]){
                if (supSetRslt->Size() == currSupSet->Size() && supSetRslt->Size() == (*tailSupItC)->Size()) {
                    //update the curr prefix
                    IOSet *tmp = (*tailIt);
                    (*tailIt) = Union((*tailIt), (*tailItC));
                    (*tailIt)->SetId(tmp->Id());
                    delete tmp;
                    //update the current tail and supporting sets
                    if (flg){
                        delete (*tailItC);
                        delete (*tailSupItC);
                        delete (*minMaxItC);
                        tailItC = RemoveFromList(tail, tailItC);
                        tailSupItC = RemoveFromList(tailSupSet, tailSupItC);
                        minMaxItC = RemoveFromList(tailMinMax, minMaxItC);
                    }else{
                         tailItC++;
                         tailSupItC++;
                          minMaxItC++;
                    }
                    delete supSetRslt;
                    delete minMaxRslt;
                   // cout<<"\ncase1 "; (*tailIt)->Output();

                } else if (supSetRslt->Size() == currSupSet->Size()) {
                    //update the curr prefix
                    IOSet *tmp = (*tailIt);
                    (*tailIt)= Union((*tailIt), (*tailItC));
                    (*tailIt)->SetId(tmp->Id());
                    delete tmp;
                    delete supSetRslt;
                    delete minMaxRslt;
                    //increment iterators
                    tailItC++;
                    tailSupItC++;
                    minMaxItC++;
                    //cout<<"\ncase2";

                } else if (supSetRslt->Size() == (*tailSupItC)->Size()) {
                    //add to new tail
                    IOSet *newNode = Union((*tailIt), (*tailItC));
                    newNode->SetId((*tailItC)->Id());
                    newTail.push_back(newNode);
                    newTailSupSets.push_back(supSetRslt);
                    newTailMinMax.push_back(minMaxRslt);
                    //update the current tail and supporting sets
                    if(flg){
                        delete (*tailItC);
                        delete (*tailSupItC);
                        delete (*minMaxItC);
                        tailItC = RemoveFromList(tail, tailItC);
                        tailSupItC = RemoveFromList(tailSupSet, tailSupItC);
                        minMaxItC = RemoveFromList(tailMinMax, minMaxItC);
                     }else{
                        tailItC++;
                        tailSupItC++;
                         minMaxItC++;
                     }
                   //  cout<<"\ncase3"; newNode->Output();

                } else {
                    //add to new tail
                    IOSet *newNode = Union((*tailIt), (*tailItC));
                    newNode->SetId((*tailItC)->Id());
                    newTail.push_back(newNode);
                    newTailSupSets.push_back(supSetRslt);
                    newTailMinMax.push_back(minMaxRslt);
                    // cout<<"\ncase4";
                    //increment iterators
                    tailItC++;
                    tailSupItC++;
                    minMaxItC++;
                  //  cout<<"\ncase4 "; newNode->Output();
                }
            } else {
                delete supSetRslt;
                delete minMaxRslt;
                // cout<<"\nno case";
                //increment iterators
                tailItC++;
                tailSupItC++;
                minMaxItC++;
            }
            ctr++;
        }
}

void BasicPrefix::Construct_First_Level(int k,
        list<IOSet *> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax,
        list<IOSet *> &newTail, list<IOSet *> &newTailSupSet, list<NCluster *> &newTailMinMax) {
    //iterate to kth element
    list<IOSet*>::iterator tailIt = tail.begin();
    list<IOSet*>::iterator tailSupIt = tailSupSet.begin();
    list<NCluster*>::iterator minMaxIt = tailMinMax.begin();
    int i = 0;
    while (i < k) {
        tailIt++;
        tailSupIt++;
        minMaxIt++;
        i++;
    }
    IOSet *currPrefix = (*tailIt);
    IOSet *currSupSet = (*tailSupIt);
    NCluster *currMinMax = (*minMaxIt);
    i++;
    tailIt++;
    tailSupIt++;
    minMaxIt++;
    while (i < tail.size()) {
        IOSet *supSetRslt = new IOSet;
        NCluster* minMaxRslt = new NCluster;
        Range_Intersect(currSupSet, (*tailSupIt), currMinMax, (*minMaxIt),
                supSetRslt, minMaxRslt,K->GetId());
        if (supSetRslt->Size() > 0) {
            newTail.push_back(Union(currPrefix, (*tailIt)));
            newTailSupSet.push_back(supSetRslt);
            newTailMinMax.push_back(minMaxRslt);
        } else {
            delete supSetRslt;
            delete minMaxRslt;
        }
        i++;
        tailIt++;
        tailSupIt++;
        minMaxIt++;
    }

}