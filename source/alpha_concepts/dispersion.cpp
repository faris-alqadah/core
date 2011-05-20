
#include "../../headers/alpha_concepts/dispersion.h"


double Range(RSet *a, vector<double> &params){
    if(a->Size() < 1) return -1;
    return a->GetMaxElement().second - a->GetMinElement().second; 
}
double Imperfect_Query_Quality(NCluster *a, vector<double> &params){
    return a->GetSet(0)->Size();//( params[0] / (double)a->GetSet(0)->Size())*a->GetQuality();
}


double Distortion(NCluster *sub, IOSet *diff, RContext *k,int s, int t){
  double n = sub->GetSetById(s)->Size();
  double dist=0;
  IOSet *full = Union(sub->GetSetById(t),diff);
  for(int i=0; i < sub->GetSetById(s)->Size(); i++){
     RSet *rr = k->GetSet(s,sub->GetSetById(s)->At(i));
     IOSet *idxs = rr->GetIdxs();
     if( Contains(idxs,full)){
         RSet *fullSub =  rr->GetSubspace(full);
         RSet *subSub = rr->GetSubspace(sub->GetSetById(t));
         double rangeFull = fullSub->GetMaxElement().second - fullSub->GetMinElement().second;
         double rangeSub =  subSub->GetMaxElement().second - subSub->GetMinElement().second;
         dist += rangeFull/rangeSub;
         delete fullSub;
         delete subSub;
     }
     delete idxs;
 }
  delete full;
  return dist/n;
}