#include "../../headers/alpha_concepts/helpers.h"




void Construct_AlphaSigma_Params(RContext *K,IOSet* query, int s,int t, int tRow, vector<double> &params){
    assert(params.size() == 1 );
    pair<int,int> dIds = K->GetDomainIds();
    assert( (s == dIds.first && t == dIds.second) || (s == dIds.second && t == dIds.first));
    params.push_back(K->GetStdDev(t,tRow)); //std dev of full row/column <b>M</b>
         
}


void Construct_MaxSpaceUniform_Params(RContext* K, IOSet* query, int s, int t, int tRow, vector<double> &params){
    assert(params.size()==1);
    pair<int,int> dIds = K->GetDomainIds();
    assert(s == dIds.first || s == dIds.second);
    params.push_back((double)K->GetNumSets(t)); //size of <b>M</b>
    vector<double> rangeParams;
    RSet *sset = K->GetSet(t,tRow);
    double rr = Range(sset,rangeParams);
    params.push_back(rr); //range of full row/column <b>M</b>
    params.push_back(query->GetQuality());
}

IOSet *Naive_Range_Intersect(IOSet *supSet1, IOSet *supSet2){
assert( supSetRslt != NULL && minMaxRslt != NULL);
     //first intersect the indices
     IOSet *commonIdxs = Intersect(supSet1,supSet2);
     //remove common idxs that do meet min max requirements
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
     return commonIdxs;
}