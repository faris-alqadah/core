#include "../../headers/alpha_concepts/basic_prefix_alpha.h"

void BasicPrefix::Qbbc(IOSet *query, vector<NCluster*> &hits) {
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

        if ((*supSetIt)->Size() >= supSet->Size()) {
            IOSet *supSetRslt = new IOSet;
            NCluster *minMaxRslt = new NCluster;
            Range_Intersect(supSet, (*supSetIt), minMax, (*minMaxIt), supSetRslt, minMaxRslt);
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
    list<IOSet*>::iterator tailIt = tail.begin();
    list<IOSet*>::iterator tailSupIt = tailSupSet.begin();
    list<NCluster*>::iterator minMaxIt = tailMinMax.begin();
    tailIt++;
    tailSupIt++;
    minMaxIt++; //advance one to only compute other neighbors since prefix is first memeber of tail
    while (tailIt != tail.end()) {
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
            Range_Intersect(currSupSet, (*tailSupItC), currMinMax, (*minMaxItC), supSetRslt, minMaxRslt);
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
                //cout<<"\ncase1";

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
}

void BasicPrefix::Range_Intersect(IOSet *supSet1, IOSet *supSet2, NCluster* minMax1, NCluster* minMax2,
        IOSet *supSetRslt, NCluster* minMaxRslt) {
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
        lclParamsF.push_back(alpha); //assign the variance for this particlar row / column
        paramFunction(K, commonIdxs, s, t, rowId, lclParamsF);
        //now do consistency check
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


void BasicPrefix::Enumerate_Star_Charm(list< list<IOSet*>* > &tails, list< list<IOSet*> *> &tailSupSet, list < list<NCluster*> *> &tailMinMax){

    //create vector of iterators for all tails, support sets and minMaxs
    vector< list<IOSet*>::iterator > tailIts;
    vector< list<IOSet*>::iterator > supIts;
    vector< list<NCluster*>::iterator > minMaxIts;
    Create_AllTails_Iterators_Star_Charm(tails, tailSupSet, tailMinMax, tailIts, supIts, minMaxIts);

    while (tailIts[0] != tails.front()->end()) {


        //outer iterator of tails
        list< list<IOSet*>* >::iterator outerTailIt = tails.begin();
        list< list<IOSet*>* >::iterator outerSupIt = tailSupSet.begin();
        list< list<NCluster*>* >::iterator outerMinMaxIt = tailMinMax.begin();

        list<list<IOSet*> *> newTails;
        list<list<IOSet*> *> newSupSets;
        list<list<NCluster*> *> newMinMaxs;

        for (int i = 0; i < tailIts.size(); i++) {
            //run charm on this edge?
                list<IOSet *>* newTail = new list<IOSet*>;
                list<IOSet *>* newSupSet = new list<IOSet*>;
                list<NCluster *>* newMinMax = new list<NCluster*>;
                Charm_Step(**outerTailIt, **outerSupIt, **outerMinMaxIt, *newTail, *newSupSet, *newMinMax);
                newTails.push_back(newTail);
                newSupSets.push_back(newSupSet);
                newMinMaxs.push_back(newMinMax);
                outerTailIt++;
                outerSupIt++;
                outerMinMaxIt++;
        }
        //determine if cluster is found
        if( Determine_Clusters(tails,newTails,newSupSets,newMinMaxs)){
            //create cluster and recurse
            cout<<"\nFound cluster!\n";
              //prune new search space
            list<IOSet*> * lrnrNewTails = newTails.front();
            list<IOSet*> * lrnrNewSupSets = newSupSets.front();
            list<NCluster*> * lrnrNewMinMaxs = newMinMaxs.front();
            list< list<IOSet*>* >::iterator newTailIt = newTails.begin();
            list< list<IOSet*>* >::iterator newSupIt = newSupSets.begin();
            list< list<NCluster*>* >::iterator newMinMaxIt = newMinMaxs.begin();
            newTailIt++; newSupIt++; newMinMaxIt++;
            while(newTailIt != newTails.end()){
                Prune_Tails(*lrnrNewTails,*lrnrNewSupSets,*lrnrNewMinMaxs,*(*newTailIt),*(*newSupIt),*(*newMinMaxIt));
                newTailIt++; newSupIt++; newMinMaxIt++;
            }
            Enumerate_Star_Charm(newTails,newSupSets,newMinMaxs);
        }else{
            //delete all new tails
            Delete_New_Tails_Star_Charm(newTails,newSupSets,newMinMaxs);
        }
         //update all the tails
         Update_AllTails_Iterators_Star_Charm(tails, tailSupSet, tailMinMax,tailIts,supIts,minMaxIts);
    }
}

bool BasicPrefix::Determine_Clusters(list< list<IOSet*>* > &tails, list< list<IOSet*>* > &newTtails,
                        list< list<IOSet*> *> &newSupSets, list < list<NCluster*> *> &newMinMaxs){

    list<list<IOSet*> *>::iterator tailIt = tails.begin();
    IOSet *lrnrSet = (*(*tailIt)->begin());
    tailIt++;
    while (tailIt != tails.end()){
        IOSet *currSet = (*(*tailIt)->begin());
        IOSet *intersect = Intersect(currSet,lrnrSet);
        if( currSet->Size() == lrnrSet->Size() && intersect->Size() == currSet->Size()){
                ;//do nothing since equal

        }
        else if(intersect->Size() == lrnrSet->Size() && lrnrSet->Size() < currSet->Size()){
            ; //do nothing since this also constitutes a cluster
        }
        else if(intersect->Size() == currSet->Size() && lrnrSet->Size() > currSet->Size()){
                //learner is super set of currset, so more testing needed
                //to determine if a cluster can be formed with curr set
            delete intersect;
            return false;
        }
        else{
            delete intersect;
            return false;
        }
        delete intersect;
        tailIt++;
    }
    return true;
}


void BasicPrefix::Enumerate_Charm(list<IOSet*> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax) {
    if (tail.size() == 0) {
        return;
    }
    list<IOSet*>::iterator tailIt = tail.begin();
    list<IOSet*>::iterator tailSupIt = tailSupSet.begin();
    list<NCluster*>::iterator minMaxIt = tailMinMax.begin();
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
        // cout<<"\nin outer loop before inner...\n";
        // cout<<"\n";
        while (tailItC != tail.end()) {
            IOSet *supSetRslt = new IOSet;
            NCluster* minMaxRslt = new NCluster;
            //first perform intersection then all cases follow
            //cout<<"\nin charm doing intersect...";
            //cout<<"\n";
            Range_Intersect(currSupSet, (*tailSupItC), currMinMax, (*minMaxItC), supSetRslt, minMaxRslt);
            // cout<<"\nfinished intersect....";
            //cout<<"\n";
            //now implement each case....
            if (supSetRslt->Size() > PRUNE_SIZE_VECTOR[1]) {
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
                    delete supSetRslt;
                    delete minMaxRslt;
                    //cout<<"\ncase1";

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
                    delete (*tailItC);
                    delete (*tailSupItC);
                    delete (*minMaxItC);
                    tailItC = RemoveFromList(tail, tailItC);
                    tailSupItC = RemoveFromList(tailSupSet, tailSupItC);
                    minMaxItC = RemoveFromList(tailMinMax, minMaxItC);
                    // cout<<"\ncase3";

                } else {
                    //add to new tail
                    newTail.push_back(Union(currPrefix, (*tailItC)));
                    newTailSupSets.push_back(supSetRslt);
                    newTailMinMax.push_back(minMaxRslt);
                    // cout<<"\ncase4";
                    //increment iterators
                    tailItC++;
                    tailSupItC++;
                    minMaxItC++;
                }
            } else {
                delete supSetRslt;
                delete minMaxRslt;
                //  cout<<"\nnone...";
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

void BasicPrefix::Charm_Step(list<IOSet*> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax,
                 list<IOSet*> &newTail, list<IOSet*> &newTailSupSets, list<NCluster*> &newTailMinMax ) {
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
    while (tailItC != tail.end()){
            IOSet *supSetRslt = new IOSet;
            NCluster* minMaxRslt = new NCluster;
            //first perform intersection then all cases follow
            Range_Intersect(currSupSet, (*tailSupItC), currMinMax, (*minMaxItC), supSetRslt, minMaxRslt);
            //now implement each case....
            if (supSetRslt->Size() > PRUNE_SIZE_VECTOR[1]){
                if (supSetRslt->Size() == currSupSet->Size() && supSetRslt->Size() == (*tailSupItC)->Size()) {
                    //update the curr prefix
                    IOSet *tmp = currPrefix;
                    currPrefix = Union(currPrefix, (*tailItC));
                    currPrefix->SetId(tmp->Id());
                    delete tmp;
                    //update the current tail and supporting sets
                    delete (*tailItC);
                    delete (*tailSupItC);
                    delete (*minMaxItC);
                    tailItC = RemoveFromList(tail, tailItC);
                    tailSupItC = RemoveFromList(tailSupSet, tailSupItC);
                    minMaxItC = RemoveFromList(tailMinMax, minMaxItC);
                    delete supSetRslt;
                    delete minMaxRslt;
                    //cout<<"\ncase1";

                } else if (supSetRslt->Size() == currSupSet->Size()) {
                    //update the curr prefix
                    IOSet *tmp = currPrefix;
                    currPrefix = Union(currPrefix, (*tailItC));
                    currPrefix->SetId(tmp->Id());
                    delete tmp;
                    delete supSetRslt;
                    delete minMaxRslt;
                    //increment iterators
                    tailItC++;
                    tailSupItC++;
                    minMaxItC++;
                } else if (supSetRslt->Size() == (*tailSupItC)->Size()) {
                    //add to new tail
                    IOSet *newNode = Union(currPrefix, (*tailItC));
                    newNode->SetId((*tailItC)->Id());
                    newTail.push_back(newNode);
                    newTailSupSets.push_back(supSetRslt);
                    newTailMinMax.push_back(minMaxRslt);
                    //update the current tail and supporting sets
                    delete (*tailItC);
                    delete (*tailSupItC);
                    delete (*minMaxItC);
                    tailItC = RemoveFromList(tail, tailItC);
                    tailSupItC = RemoveFromList(tailSupSet, tailSupItC);
                    minMaxItC = RemoveFromList(tailMinMax, minMaxItC);
                    // cout<<"\ncase3";

                } else {
                    //add to new tail
                    IOSet *newNode = Union(currPrefix, (*tailItC));
                    newNode->SetId((*tailItC)->Id());
                    newTail.push_back(newNode);
                    newTailSupSets.push_back(supSetRslt);
                    newTailMinMax.push_back(minMaxRslt);
                    // cout<<"\ncase4";
                    //increment iterators
                    tailItC++;
                    tailSupItC++;
                    minMaxItC++;
                }
            } else {
                delete supSetRslt;
                delete minMaxRslt;
                //  cout<<"\nnone...";
                //increment iterators
                tailItC++;
                tailSupItC++;
                minMaxItC++;
            }
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
                supSetRslt, minMaxRslt);
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