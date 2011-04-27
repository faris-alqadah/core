#include "../../headers/alpha_concepts/basic_prefix_alpha.h"


void BasicPrefixAlphaAlgos::BasicPrefix(RContext *K,IOSet *query, int qId){
    pair<int,int> dIds = K->GetDomainIds();
    assert(qId == dIds.first || qId == dIds.second);
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
        RSet *currCol = K->GetSet(qId,query->At(i));
        prefix->Add(query->At(i));
        prefix->SetId(currCol->Id());
        prefix->SetQuality(currCol->Size());
        allPfx[i]=prefix;
        //get support sets
        supportSets->AssignSet(i,currCol->GetIdxs());
        //make min max idxs
        minMaxIdxs[i] = new NCluster;
        for(int j=0; j < supportSets->GetSet(i)->Size();j++){
            IOSet *mm = new IOSet;
            mm->Add(query->At(i)); //just adding index of query object at this point since only single object
            mm->Add(query->At(i)); //this is true for both max and min
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
//         Enumerate_BasicPrefix(allPfx[i],supportSets->GetSet(i),minMaxIdxs[i],
//                               tail,tailSupSet,tailMinMaxIdxs);
    }

}

void BasicPrefixAlphaAlgos::Enumerate_BasicPrefix(IOSet *prefix, IOSet *supSet, NCluster *pMinMax, NCluster * tail,
                                                  NCluster *tailSupSet, vector< NCluster* > &tMinMax ){
          
    
}
