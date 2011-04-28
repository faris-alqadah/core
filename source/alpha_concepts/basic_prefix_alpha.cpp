#include "../../headers/alpha_concepts/basic_prefix_alpha.h"


void BasicPrefix::RunBasicPrefix(IOSet *query){
    pair<int,int> dIds = K->GetDomainIds();
    assert(s == dIds.first || s == dIds.second);
    //the level of the prefix tree will be generated here
    // this entails generating the tails and support rows for
    //each query column
    
    //1. generate all the prefixes as indivual columns
    vector<IOSet*> allPfx(query->Size());
    vector<NCluster*> minMaxIdxs(query->Size()); //store the idxs of the maximum and minimum elements
    NCluster *supportSets = new NCluster(query->Size());
    for(int i=0; i < query->Size(); i++){
        //make prefix node
        IOSet *prefix = new IOSet;
        RSet *currCol = K->GetSet(s,query->At(i));
        prefix->Add(query->At(i));
        prefix->SetId(currCol->Id());
        prefix->SetQuality(currCol->Size());
        allPfx[i]=prefix;
        //get support sets
        supportSets->AssignSet(i,currCol->GetIdxs());
        supportSets->GetSet(i)->SetId(currCol->Id());
        //make min max idxs
        minMaxIdxs[i] = new NCluster;
        for(int j=0; j < supportSets->GetSet(i)->Size();j++){
            IOSet *mm = new IOSet;
            mm->Add(query->At(i)); //just adding index of query object at this point since only single object
            mm->Add(query->At(i)); //this is true for both max and min
            mm->SetId(supportSets->GetSet(i)->At(j));
            minMaxIdxs[i]->AddSet(mm);
        }
    }
    //sort by size of supporting set
    sort(allPfx.begin(),allPfx.end(),Compare_Quality_IOSet);
    //now iterate again this time generating supporting sets and passing onto main algorithm
    for(int i=0; i < allPfx.size();i++){

        //generate tails
        NCluster *tail = new NCluster(query->Size()-(i+1));
        NCluster *tailSupSet = new NCluster(query->Size()-(i+1));
        vector< NCluster* > tailMinMaxIdxs(query->Size()-(i+1));
        for(int j=i+1; j < query->Size(); j++){
            tail->AssignSet(j-(i+1),allPfx[j]);
            tailSupSet->AssignSet(j-(i+1),supportSets->GetSet(j));
            tailMinMaxIdxs[j-(i+1)] = minMaxIdxs[j];
        }
        cout<<"\nOn prefix: \t"; allPfx[i]->Output();
        cout<<"\ntail: \n"; tail->Output();
        cout<<"\ntail sup set: \n"; tailSupSet->Output();
        cout<<"\ntail min max idxs: ("<<tailMinMaxIdxs.size()<<")\n";
        for(int j=0; j < tailMinMaxIdxs.size(); j++){
            tailMinMaxIdxs[j]->Output();
        }       
    }
     cout<<"\nTesting range intersect...\n";
        IOSet *rslt = new IOSet;
        NCluster *minMaxRslt = new NCluster;
        cout<<"\nrange intersecting 1 and 2....\n";
        Range_Intersect(supportSets->GetSet(0),supportSets->GetSet(1),minMaxIdxs[0],minMaxIdxs[1],rslt,minMaxRslt);
        cout<<"\nResult is: \n";
        rslt->Output(); cout<<"\n";
        minMaxRslt->Output();
}

 void BasicPrefix::Range_Intersect(IOSet *supSet1, IOSet *supSet2, NCluster* minMax1, NCluster* minMax2,
                      IOSet *supSetRslt, NCluster* minMaxRslt){
     assert( supSetRslt != NULL && minMaxRslt != NULL);
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
             IOSet *minMax = new IOSet;
             minMax->Add(minn);
             minMax->Add(maxx);
             minMax->SetId(rowId);
             minMaxRslt->AddSet(minMax);
         }
     }
     delete commonIdxs;
}
 void BasicPrefix::Charm_Optimize(IOSet *currPrefix, IOSet *currSupSet, NCluster *currMinMax, int k,
                     NCluster *tail, NCluster *tailSupSet, vector<NCluster*> &tailMinMax,
                     NCluster *newTail, IOSet *newSupSet, NCluster * newMinMax ){

     for(int i=k; i < tail->GetN(); i++){
          IOSet *supSetRslt = new IOSet;
          NCluster* minMaxRslt = new NCluster;
          //first perform intersection then all cases follow
          Range_Intersect(currSupSet,tailSupSet->GetSet(i),currMinMax,tailMinMax[i],
                           supSetRslt,minMaxRslt);
          //now implement each case....
          if (supSetRslt->Size() > 0){
              if (supSetRslt->Size() == currSupSet->Size() && supSetRslt->Size() == tailSupSet->GetSet(i)->Size()){
                  //update the curr prefix 
                  IOSet *tmp = currPrefix;
                  currPrefix = Union(currPrefix,tail->GetSet(i));
                  delete tmp;
                  //update the current tail and supporting sets
                  tail->RemoveSet(i);
                  tailSupSet->RemoveSet(i);
                  NCluster *tt = tailMinMax[i];
                  tailMinMax.erase(tailMinMax.begin()+i);
                  delete tt;

              }else if(supSetRslt->Size() == tailSupSet->GetSet(i)->Size()){
                //update the curr prefix
                  IOSet *tmp = currPrefix;
                  currPrefix = Union(currPrefix,tail->GetSet(i));
                  delete tmp;
              }else if(supSetRslt->Size() == currSupSet->Size()){
                  //add to new tail
                  newTail->AddSet(new IOSet(tail->GetSet(i)));
                  newSupSet
                 //update the current tail and supporting sets
                  tail->RemoveSet(i);
                  tailSupSet->RemoveSet(i);
                  NCluster *tt = tailMinMax[i];
                  tailMinMax.erase(tailMinMax.begin()+i);
                  delete tt;
                  
              }else{
                  //add to new tail
              }

          }else{
              delete supSetRslt;
              delete minMaxRslt;
          }
         }
 }