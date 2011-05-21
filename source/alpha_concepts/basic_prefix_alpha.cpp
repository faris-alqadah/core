#include "../../headers/alpha_concepts/basic_prefix_alpha.h"

void BasicPrefix::Qbbc(IOSet *query,vector<NCluster*> &hits){
    //1. Get all hits for prefix search only in query space
    IOSet *origQuery = new IOSet(query);
    while(query->Size() > 1){
        Qbbc_Prefix_Search(query);
        //did we get a hit?
        if (CONCEPTS.size() > 0){
            bool gotAll = CONCEPTS[0]->GetSetById(s)->Size() == query->Size(); //were all query objects retrieved
            //find closure
            NCluster *minMax = Get_Min_Max_Idxs(CONCEPTS[0]->GetSetById(s),CONCEPTS[0]->GetSetById(t));
            NCluster *bicluster = CONCEPTS[0];
            IOSet *closure = Range_Closure(bicluster->GetSetById(s),bicluster->GetSetById(t),minMax);
            if(closure->Size() > bicluster->GetSetById(s)->Size()){
                IOSet *closureUnion = Union(bicluster->GetSetById(s),closure);
                closureUnion->SetId(s);
                bicluster->AssignSetById(s,closureUnion);
            }
            delete closure;
            delete minMax;
            //if closure is not a superset of query attempt to include more objects via top neighbors
           // if(Contains(origQuery,bicluster->GetSetById(s))){
                //best upper neighbor
                cout<<"\nInitial hit: \n";
                bicluster->Output();
                cout<<"\nshould get neighbors here...got these";
                 NCluster *minMaxN = Get_Min_Max_Idxs(bicluster->GetSetById(s),bicluster->GetSetById(t));
                 vector<NCluster*> *neighbors = Charm_UpperNeighbors(bicluster->GetSetById(s),bicluster->GetSetById(t), minMaxN);
                 //select best neighbor as least distortion
                 if (neighbors->size() > 0){
                     for(int i =0; i < neighbors->size(); i++){
                         NCluster *currNeighbor = neighbors->at(i);
                         currNeighbor->GetSet(0)->SetId(s);
                         currNeighbor->GetSet(1)->SetId(t);
                         double dist = Distortion(bicluster->GetSetById(s),currNeighbor->GetSetById(s),
                                                  bicluster->GetSetById(t),currNeighbor->GetSetById(t),K,s,t);
                         currNeighbor->SetQuality(-1*dist);
                     }
                     sort(neighbors->begin(),neighbors->end(),Compare_Quality_NCluster);
                     int kk = neighbors->size() < 20 ? neighbors->size() : 20;
                     IOSet *uu = new IOSet(bicluster->GetSetById(s));
                     for(int i=0; i < kk; i++){
                          IOSet *tmp = uu;
                          uu = Union(neighbors->at(i)->GetSetById(s),uu);
                          delete tmp;
                     }
                     uu->SetId(s);
                     //delete all neighbors
                     for(int i =0; i < neighbors->size(); i++)
                         delete neighbors->at(i);
                     delete neighbors;
                     bicluster->AssignSetById(s,uu);
                     
                 }
            //}
            IOSet *tmp = query;
            query= Difference(query,bicluster->GetSetById(s));
            delete tmp;
            hits.push_back(new NCluster(*bicluster));
            delete CONCEPTS[0]; //reset for next round
            CONCEPTS.clear(); //reset for next round

        }else{
            break;
        }
    }
    cout<<"\nGot total of "<<hits.size()<<" hits";
    for(int i=0; i < hits.size(); i++){
         cout<<"\nHit "<<i+1<<"||\n"; hits[i]->GetSetById(s)->Output(K->GetNameMap(s));
     }   
}
void BasicPrefix::Qbbc_Prefix_Search(IOSet *query){
    pair<int,int> dIds = K->GetDomainIds();
    assert(s == dIds.first || s == dIds.second);
    //the level of the prefix tree will be generated here
    // this entails generating the tails and support rows for
    //each query column
    
    //1. generate all the prefixes as indivual columns
    list<IOSet *> allPfx;
    list<NCluster*> minMaxIdxs; //store the idxs of the maximum and minimum elements
    list<IOSet*> supportSets;
    Make_Init_SupSets_MinMaxIdxs(query,allPfx,supportSets,minMaxIdxs);
    //sort by size of supporting set
    //allPfx->SortSets();
    //setup algorithm parameters
    enumerationMode = ENUM_TOPK_MEM;//only looking for the top hit
    topKK = 1; //only one top hit
    ovlpMode = AVG_JACCARD;  //overlap function will not really be used...but set it anyway
    ovlpFunction = &GreaterEqualSize; //overlap function will not really be used...but set it anyway
    params.push_back(query->Size()); //set the params for use with the quality function...the quality function is fraction of query*avg_range
    qualityFunction = &Imperfect_Query_Quality;
    PRUNE_SIZE_VECTOR.resize(2);   // make the prune size vector (at least 2 elements from each domain must be enumerated)
    PRUNE_SIZE_VECTOR[0] =1; PRUNE_SIZE_VECTOR[1]=1;
    //now iterate again this time generating supporting sets and passing onto main algorithm
    int i=0;
    for(list<IOSet*>::iterator it = allPfx.begin(); it != allPfx.end(); i++){
        list<IOSet*> newTail;
        list<IOSet*>newTailSupSets;
        list<NCluster*> newTailMinMax;
        Construct_First_Level(i,allPfx,supportSets,minMaxIdxs,newTail,newTailSupSets,newTailMinMax);
        Enumerate_Charm(newTail,newTailSupSets,newTailMinMax);
        it++;
    }

    if(CONCEPTS.size() > 0){
        sort(CONCEPTS.begin(),CONCEPTS.end(),Compare_Quality_NCluster);
        for(int i=0; i < CONCEPTS.size(); i++){
            CONCEPTS[i]->GetSet(0)->SetId(s);
            CONCEPTS[i]->GetSet(1)->SetId(t);
        }
    }
}

void BasicPrefix::Make_Init_SupSets_MinMaxIdxs(IOSet *query, list<IOSet*> &prefix,list<IOSet*> &supSets, list<NCluster*> &minMax){
     for(int i=0; i < query->Size(); i++){
        //make prefix node
        IOSet *prefixx = new IOSet;
        RSet *currCol = K->GetSet(s,query->At(i));
        prefixx->Add(query->At(i));
        prefixx->SetId(currCol->Id());
        prefixx->SetQuality(currCol->Size());
        prefix.push_back(prefixx);
        supSets.push_back(currCol->GetIdxs());
        supSets.back()->SetId(currCol->Id());
        minMax.push_back(new NCluster);
        for(int j=0; j < supSets.back()->Size();j++){
            IOSet *mm = new IOSet;
            RSet *theRow = K->GetSet(t,supSets.back()->At(j));
            int idxPtr=theRow->GetIndexPtr(query->At(i));
            mm->Add(idxPtr); //just adding index of query object at this point since only single object
            mm->Add(idxPtr); //this is true for both max and min
            mm->SetId(supSets.back()->At(j));
            minMax.back()->AddSet(mm);
        }
    }
}
NCluster * BasicPrefix::Get_Min_Max_Idxs(IOSet *query, IOSet *supSets){
     NCluster *ret = new NCluster();
     for(int i=0; i < supSets->Size(); i++){
         ret->AddSet(new IOSet);
         pair<int,int> minMax = K->GetSet(t,supSets->At(i))->GetMinMaxSubspaceIdxs(query);
         ret->GetSet(i)->Add(minMax.first);
         ret->GetSet(i)->Add(minMax.second);
         ret->GetSet(i)->SetId(supSets->At(i));
     }
     return ret;
 }

 IOSet* BasicPrefix::Range_Closure(IOSet *prefix,IOSet *supSet, NCluster *minMax){
     //get all other objects and their supporting sets + min-max idxs
     IOSet *otherObjs = Difference(K->GetLabels(s),prefix);
     IOSet *closure = new IOSet;
     list<IOSet *> allPfx;
     list<NCluster*> minMaxIdxs; //store the idxs of the maximum and minimum elements
     list<IOSet*> supportSets;
     Make_Init_SupSets_MinMaxIdxs(otherObjs,allPfx,supportSets,minMaxIdxs);
     //for each other object range intersect supporting sets, if result is of equal size to supSet, then include item in closure
     list<IOSet*>::iterator supSetIt = supportSets.begin();
     list<NCluster*>::iterator minMaxIt = minMaxIdxs.begin();
     for(int i=0; i < otherObjs->Size(); i++){
         if((*supSetIt)->Size() >= supSet->Size()){
             IOSet *supSetRslt = new IOSet;
             NCluster *minMaxRslt = new NCluster;
             Range_Intersect(supSet,(*supSetIt),minMax,(*minMaxIt),supSetRslt,minMaxRslt);
             if( supSetRslt->Size() == supSet->Size()){
                 closure->Add( otherObjs->At(i));
             }
             //delete and increment iterator
             delete supSetRslt;
             delete minMaxRslt;
         }
         delete (*supSetIt);
         delete (*minMaxIt);
         supSetIt =  RemoveFromList(supportSets,supSetIt); 
         minMaxIt =  RemoveFromList(minMaxIdxs,minMaxIt);
     }
     delete otherObjs;
     DstryList(allPfx);
     return closure;
 }

 vector<NCluster*> * BasicPrefix::Charm_UpperNeighbors(IOSet *prefix, IOSet *supSet, NCluster *minMax){
     IOSet *otherObjs = Difference(K->GetLabels(s),prefix);
     vector<NCluster*> *neighbors = new vector<NCluster*>;
     list<IOSet *> allPfx;
     list<NCluster*> minMaxIdxs; //store the idxs of the maximum and minimum elements
     list<IOSet*> supportSets;
     Make_Init_SupSets_MinMaxIdxs(otherObjs,allPfx,supportSets,minMaxIdxs);

     list<IOSet*>::iterator allPfxIt = allPfx.begin();
     list<IOSet*>::iterator supSetIt = supportSets.begin();
     list<NCluster*>::iterator minMaxIt = minMaxIdxs.begin();
     for(int i=0; i < otherObjs->Size(); i++){
             IOSet *pfx = new IOSet(prefix);
            
             IOSet *supSetRslt = new IOSet;
             NCluster *minMaxRslt = new NCluster;
             Range_Intersect(supSet,(*supSetIt),minMax,(*minMaxIt),supSetRslt,minMaxRslt);
             if( supSetRslt->Size() > PRUNE_SIZE_VECTOR[1]){
                 pfx->Add( otherObjs->At(i));
                 IOSet *otherObjs1 = new IOSet;
                 for(int j=i+1; j < otherObjs->Size(); j++) otherObjs1->Add(otherObjs->At(j));
                 list<IOSet *> allPfx1;
                 list<NCluster*> minMaxIdxs1; //store the idxs of the maximum and minimum elements
                 list<IOSet*> supportSets1;
                 Make_Init_SupSets_MinMaxIdxs(otherObjs1,allPfx1,supportSets1,minMaxIdxs1);
                 pfx->Sort();
                 IOSet *closure =Charm_Closure(pfx,supSetRslt,minMaxRslt,allPfx1,supportSets1,minMaxIdxs1);
                 NCluster *bicluster = new NCluster;
                 bicluster->AddSet(closure);
                 bicluster->AddSet(new IOSet(supSetRslt));
                 neighbors->push_back(bicluster);
                 delete otherObjs1;
             }
             //delete and increment iterator
             delete supSetRslt;
             delete minMaxRslt;
             delete pfx;
             delete (*supSetIt);
             delete (*minMaxIt);
             delete (*allPfxIt);
             supSetIt =  RemoveFromList(supportSets,supSetIt);
             minMaxIt =  RemoveFromList(minMaxIdxs,minMaxIt);
             allPfxIt =  RemoveFromList(allPfx,allPfxIt);
     }
     delete otherObjs;
     return neighbors;
      
        
 }

IOSet* BasicPrefix::Charm_Closure(IOSet *currPrefix, IOSet *currSupSet, NCluster *currMinMax,
                        list<IOSet*> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax){
                IOSet * ret = new IOSet(currPrefix);
                list<IOSet*>::iterator tailItC = tail.begin();
                list<IOSet*>::iterator tailSupItC =tailSupSet.begin() ;
                list<NCluster*>::iterator minMaxItC = tailMinMax.begin();
                while(tailItC != tail.end()){
                    IOSet *supSetRslt = new IOSet;
                    NCluster* minMaxRslt = new NCluster;
                    //first perform intersection then all cases follow
                    Range_Intersect(currSupSet,(*tailSupItC),currMinMax,(*minMaxItC),supSetRslt,minMaxRslt);
                    //now implement each case....
                    if (supSetRslt->Size() > PRUNE_SIZE_VECTOR[1]){
                      if (supSetRslt->Size() == currSupSet->Size() && supSetRslt->Size() == (*tailSupItC)->Size()){
                          //update the curr prefix
                          IOSet *tmp = ret;
                          ret = Union(ret,(*tailItC));
                          delete tmp;
                          //update the current tail and supporting sets
                          delete (*tailItC); delete (*tailSupItC); delete (*minMaxItC);
                          tailItC = RemoveFromList( tail, tailItC);
                          tailSupItC = RemoveFromList(tailSupSet,tailSupItC);
                          minMaxItC = RemoveFromList(tailMinMax,minMaxItC);

                      }else if(supSetRslt->Size() == currSupSet->Size() ){
                        //update the curr prefix
                          IOSet *tmp = ret;
                          ret = Union(ret,(*tailItC));
                          delete tmp;
                          delete (*tailItC); delete (*tailSupItC); delete (*minMaxItC);
                          //increment iterators
                          tailItC++; tailSupItC++; minMaxItC++;
                         // cout<<"\ncase2";
                      }else if(supSetRslt->Size() == (*tailSupItC)->Size() ){
                         //update the current tail and supporting sets
                          delete (*tailItC); delete (*tailSupItC); delete (*minMaxItC);
                          tailItC = RemoveFromList( tail, tailItC);
                          tailSupItC = RemoveFromList(tailSupSet,tailSupItC);
                          minMaxItC = RemoveFromList(tailMinMax,minMaxItC);
                         

                      }else{
                           delete (*tailItC); delete (*tailSupItC); delete (*minMaxItC);
                          tailItC++; tailSupItC++; minMaxItC++;
                      }
                      delete supSetRslt;
                      delete minMaxRslt;
                   }else{
                      delete supSetRslt;
                      delete minMaxRslt;
                      //increment iterators
                      delete (*tailItC); delete (*tailSupItC); delete (*minMaxItC);
                      tailItC++; tailSupItC++; minMaxItC++;
                   }
                }
  }

 void BasicPrefix::Range_Intersect(IOSet *supSet1, IOSet *supSet2, NCluster* minMax1, NCluster* minMax2,
                      IOSet *supSetRslt, NCluster* minMaxRslt){
     assert( supSetRslt != NULL && minMaxRslt != NULL);
     supSetRslt->SetQuality(0.0);
     //first intersect the indices
     IOSet *commonIdxs = Intersect(supSet1,supSet2);
     //update min max results for each index compute range and add sets that meet range requirement
     for(int i=0; i < commonIdxs->Size();i++){
         //get max and mins for curr row
         int rowId = commonIdxs->At(i);
         RSet *row = K->GetSet(t,rowId);
         int oldMin1=minMax1->GetSetById(rowId)->At(0);
         int oldMin2=minMax2->GetSetById(rowId)->At(0);
         int oldMax1=minMax1->GetSetById(rowId)->At(1);
         int oldMax2=minMax2->GetSetById(rowId)->At(1);
         double minVal = min(K->GetSet(t,rowId)->At(oldMin1).second,K->GetSet(t,rowId)->At(oldMin2).second );
         double maxVal = max(K->GetSet(t,rowId)->At(oldMax1).second,K->GetSet(t,rowId)->At(oldMax2).second );
         int minn =  (minVal == K->GetSet(t,rowId)->At(oldMin1).second) ? oldMin1 : oldMin2;
         int maxx = (maxVal == K->GetSet(t,rowId)->At(oldMax1).second) ? oldMax1 : oldMax2;
         //get range using new idxs
         //double oldRange = K->GetSet(t,rowId)->At(oldMax1).second-K->GetSet(t,rowId)->At(oldMin1).second;
         double range = K->GetSet(t,rowId)->At(maxx).second-K->GetSet(t,rowId)->At(minn).second;
        // cout<<"\nold range: "<<oldRange<<"\tnew range"<<range;
         //construct parameters for consistency function
         vector<double> lclParamsF; //construct parameter vector for the consistency function, make alpha the first element though
         lclParamsF.push_back(alpha);  //assign the variance for this particlar row / column
         paramFunction(K,commonIdxs,s,t,rowId,lclParamsF);
         //now do consistency check
         if( range < consistencyFunction(row,lclParamsF)){
             supSetRslt->Add(rowId);
             supSetRslt->SetQuality( supSetRslt->GetQuality()+range);
             IOSet *minMax = new IOSet;
             minMax->Add(minn);
             minMax->Add(maxx);
             minMax->SetId(rowId);
             minMaxRslt->AddSet(minMax);
         }
     }
     if(supSetRslt->Size() > 0) supSetRslt->SetQuality( supSetRslt->GetQuality() / (double) supSetRslt->Size());
     else supSetRslt->SetQuality(-1);
     delete commonIdxs;
}

  void BasicPrefix::Enumerate_Charm(list<IOSet*> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax){
      list<IOSet*>::iterator tailIt = tail.begin();
      list<IOSet*>::iterator tailSupIt = tailSupSet.begin();
      list<NCluster*>::iterator minMaxIt = tailMinMax.begin();
      while(tailIt != tail.end()){
                IOSet *currPrefix = (*tailIt);
                IOSet *currSupSet = (*tailSupIt);
                NCluster *currMinMax = (*minMaxIt);
                list<IOSet*> newTail;
                list<IOSet*> newTailSupSets;
                list<NCluster*> newTailMinMax;
                //-------------BEGIN CHARM OPTIMIZATION-------------------------
                list<IOSet*>::iterator tailItC = tailIt;
                list<IOSet*>::iterator tailSupItC =tailSupIt ;
                list<NCluster*>::iterator minMaxItC = minMaxIt;
                tailItC++; tailSupItC++; minMaxItC++;
                while(tailItC != tail.end()){
                    IOSet *supSetRslt = new IOSet;
                    NCluster* minMaxRslt = new NCluster;
                    //first perform intersection then all cases follow
                    Range_Intersect(currSupSet,(*tailSupItC),currMinMax,(*minMaxItC),supSetRslt,minMaxRslt);
                    //now implement each case....
                    if (supSetRslt->Size() > PRUNE_SIZE_VECTOR[1]){
                      if (supSetRslt->Size() == currSupSet->Size() && supSetRslt->Size() == (*tailSupItC)->Size()){
                          //update the curr prefix
                          IOSet *tmp = currPrefix;
                          currPrefix = Union(currPrefix,(*tailItC));
                          delete tmp;
                          //update the current tail and supporting sets
                          delete (*tailItC); delete (*tailSupItC); delete (*minMaxItC);
                          tailItC = RemoveFromList( tail, tailItC);
                          tailSupItC = RemoveFromList(tailSupSet,tailSupItC);
                          minMaxItC = RemoveFromList(tailMinMax,minMaxItC);
                          delete supSetRslt;
                          delete minMaxRslt;
                        //  cout<<"\ncase1";

                      }else if(supSetRslt->Size() == currSupSet->Size() ){
                        //update the curr prefix
                          IOSet *tmp = currPrefix;
                          currPrefix = Union(currPrefix,(*tailItC));
                          delete tmp;
                          delete supSetRslt;
                          delete minMaxRslt;
                          //increment iterators
                          tailItC++; tailSupItC++; minMaxItC++;
                         // cout<<"\ncase2";
                      }else if(supSetRslt->Size() == (*tailSupItC)->Size() ){
                          //add to new tail
                          newTail.push_back(Union(currPrefix,(*tailItC)));
                          newTailSupSets.push_back(supSetRslt);
                          newTailMinMax.push_back(minMaxRslt);
                         //update the current tail and supporting sets
                          delete (*tailItC); delete (*tailSupItC); delete (*minMaxItC);
                          tailItC = RemoveFromList( tail, tailItC);
                          tailSupItC = RemoveFromList(tailSupSet,tailSupItC);
                          minMaxItC = RemoveFromList(tailMinMax,minMaxItC);
                         // cout<<"\ncase3";

                      }else{
                          //add to new tail
                           newTail.push_back(Union(currPrefix,(*tailItC)));
                           newTailSupSets.push_back(supSetRslt);
                           newTailMinMax.push_back(minMaxRslt);
                         //   cout<<"\ncase4";
                          //increment iterators
                          tailItC++; tailSupItC++; minMaxItC++;
                      }
                   }else{
                      delete supSetRslt;
                      delete minMaxRslt;
                      //increment iterators
                      tailItC++; tailSupItC++; minMaxItC++;
                   }
                }
                //-------------END CHARM OPTIMIZATION---------------------------
                //the current tail(i) and its support set should constitute a new bi-cluster
                NCluster *bicluster = new NCluster;
                bicluster->AddSet(new IOSet(currPrefix));
                bicluster->AddSet( new IOSet(currSupSet));
                //remove the current prefix,support set, and indicies and advance pointers to next element in tail
                delete currPrefix;
                delete currSupSet;
                delete currMinMax;
                tailIt = RemoveFromList( tail, tailIt);
                tailSupIt = RemoveFromList(tailSupSet,tailSupIt);
                minMaxIt = RemoveFromList(tailMinMax,minMaxIt);
                //check for different output modes
                if(enumerationMode == ENUM_TOPK_MEM && bicluster->GetSet(1)->Size() > PRUNE_SIZE_VECTOR[1]){
                        SetQuality(bicluster,params,qualityFunction);
                        RetainTopK_Overlap(CONCEPTS,bicluster,ovlpFunction,ovlpThresh,topKK);
                }
                delete bicluster;
                //now recurse
                Enumerate_Charm(newTail,newTailSupSets,newTailMinMax);
      }
  }

   

   void BasicPrefix::Construct_First_Level(int k,
                     list<IOSet *> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax,
                    list<IOSet *> &newTail, list<IOSet *> &newTailSupSet, list<NCluster *> &newTailMinMax ){
     //iterate to kth element
     list<IOSet*>::iterator tailIt = tail.begin();
     list<IOSet*>::iterator tailSupIt = tailSupSet.begin();
     list<NCluster*>::iterator minMaxIt = tailMinMax.begin();
     int i=0;
     while(i < k){
         tailIt++;
         tailSupIt++;
         minMaxIt++;
         i++;
     }
     IOSet *currPrefix = (*tailIt);
     IOSet *currSupSet =  (*tailSupIt);
     NCluster *currMinMax = (*minMaxIt);
     i++;
     tailIt++;
     tailSupIt++;
     minMaxIt++;
     while(i < tail.size()){
          IOSet *supSetRslt = new IOSet;
          NCluster* minMaxRslt = new NCluster;
          Range_Intersect(currSupSet,(*tailSupIt),currMinMax,(*minMaxIt),
                           supSetRslt,minMaxRslt);
          if(supSetRslt->Size() > 0){
               newTail.push_back(Union(currPrefix,(*tailIt)));
               newTailSupSet.push_back(supSetRslt);
               newTailMinMax.push_back(minMaxRslt);
          }else{
              delete supSetRslt;
              delete minMaxRslt;
          }
          i++;
          tailIt++;
          tailSupIt++;
          minMaxIt++;
     }

 }