#include "../../headers/alpha_concepts/basic_prefix_alpha.h"


void BasicPrefix::Qbbc_Prefix_Search(IOSet *query){
    pair<int,int> dIds = K->GetDomainIds();
    assert(s == dIds.first || s == dIds.second);
    //the level of the prefix tree will be generated here
    // this entails generating the tails and support rows for
    //each query column
    
    //1. generate all the prefixes as indivual columns
    list<IOSet *> allPfx;
    list<NCluster*> minMaxIdxs; //store the idxs of the maximum and minimum elements
    list<IOSet *> supportSets;
    for(int i=0; i < query->Size(); i++){
        //make prefix node
        IOSet *prefix = new IOSet;
        RSet *currCol = K->GetSet(s,query->At(i));
        prefix->Add(query->At(i));
        prefix->SetId(currCol->Id());
        prefix->SetQuality(currCol->Size());
        allPfx.push_back(prefix);
        supportSets.push_back(currCol->GetIdxs());
        supportSets.back()->SetId(currCol->Id());
        minMaxIdxs.push_back(new NCluster);
        for(int j=0; j < supportSets.back()->Size();j++){
            IOSet *mm = new IOSet;
            mm->Add(query->At(i)); //just adding index of query object at this point since only single object
            mm->Add(query->At(i)); //this is true for both max and min
            mm->SetId(supportSets.back()->At(j));
            minMaxIdxs.back()->AddSet(mm);
        }
    }
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
    PRUNE_SIZE_VECTOR[0] =2; PRUNE_SIZE_VECTOR[1]=2;
    //now iterate again this time generating supporting sets and passing onto main algorithm
    int i=0;
    for(list<IOSet*>::iterator it = allPfx.begin(); it != allPfx.end(); i++){
        cout<<"\nOn "<<i+1<<" OF "<<allPfx.size();
        cout.flush();
        list<IOSet*> newTail;
        list<IOSet*>newTailSupSets;
        list<NCluster*> newTailMinMax;
        Construct_First_Level(i,allPfx,supportSets,minMaxIdxs,newTail,newTailSupSets,newTailMinMax);
        Enumerate_Charm(newTail,newTailSupSets,newTailMinMax);
        //clean up
       // DstryList(newTail);
       // DstryList(newTailSupSets);
       // DstryList(newTailMinMax);
        it++;
    }
    if (CONCEPTS.size() > 0){
        cout<<"\nGot top hit: \n";
        CONCEPTS[0]->Output();
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
         double range = K->GetSet(t,rowId)->At(maxx).second-K->GetSet(t,rowId)->At(minn).second;
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
                if(enumerationMode == ENUM_TOPK_MEM){
                        SetQuality(bicluster,params,qualityFunction);
                        RetainTopK_Overlap(CONCEPTS,bicluster,ovlpFunction,ovlpThresh,topKK);
                }
                delete bicluster;
                //now recurse
                Enumerate_Charm(newTail,newTailSupSets,newTailMinMax);
                //cout<<"\nnewTail size: "<<newTailMinMax.size();
                //clean up
               // DstryList(newTail);
               // DstryList(newTailSupSets);
               // DstryList(newTailMinMax);
          
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