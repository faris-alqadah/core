
#include "../../headers/alpha_concepts/dispersion.h"


double Range(RSet *a, vector<double> &params){
    if(a->Size() < 1) return -1;
    return a->GetMaxElement().second - a->GetMinElement().second; 
}
double Imperfect_Query_Quality(NCluster *a, vector<double> &params){
    return a->GetSet(0)->Size();//( params[0] / (double)a->GetSet(0)->Size())*a->GetQuality();
}


double Distortion(IOSet *s1, IOSet *s2, IOSet *t1, IOSet *t2, RContext *k,int s, int t){
    double dist=0;
  IOSet *diffT = Intersect(t1,t2);
  double n = diffT->Size();
  for(int i=0; i < diffT->Size(); i++){
        RSet *rr = k->GetSet(t,diffT->At(i));
        IOSet *lclInter = Intersect(rr->GetIdxs(),s2);
        IOSet *lclDiff = Difference(s2,lclInter);
        //cout<<"\nrr: "; rr->Output();
       // cout<<"\nlclInter sz: "<<lclInter->Size();
       // cout<<"\nlclDiff sz: "<<lclDiff->Size();
       // cout<<"\ns1 sz: "<<s1->Size();
        if(lclInter->Size() > 0){
             RSet *fullSub =  rr->GetSubspace(lclInter);
             RSet *subSub = rr->GetSubspace(s1);
           //  cout<<"\n";
            // fullSub->Output();
            // cout<<"\n";
            //make q subSub->Output();
             double rangeFull = fullSub->GetMaxElement().second - fullSub->GetMinElement().second;
            // cout<<"\nfull range "<<fullSub->GetMaxElement().second<<"\t"<<fullSub->GetMinElement().second;
            // cout<<"\nsub range "<<subSub->GetMaxElement().second<<"\t"<<subSub->GetMinElement().second;
             double rangeSub =  subSub->GetMaxElement().second - subSub->GetMinElement().second;
             dist += (1+lclDiff->Size())* (1-(rangeSub/rangeFull));
             delete fullSub;
             delete subSub;
        }else{
                dist += (double)(1.0+lclDiff->Size());
        }
        //cout<<"\n"<<dist;
        delete lclInter;
        delete lclDiff;
  }
  delete diffT;
  return dist/n;
}


double Std_Within(NCluster *a, RContext *k, int s, int t){
    double sw = 0;
    double total=0;
    for(int i=0; i < a->GetSetById(s)->Size(); i++){
        RSet *cc =  k->GetSet(s,a->GetSetById(s)->At(i));
        RSet *cc1;
        if(a->GetSetById(t)->Size() > cc->Size()){
            cc1 = cc;
        }else{
         cc1 =  cc->GetSubspace(a->GetSetById(t));
        }
        double avg = cc1->Mean();
        for(int j=0; j < cc1->Size(); j++){
            sw += pow(cc1->At(j).second-avg,2);
            total++;
        }
    }
    return sqrt(sw/total);

}

double Std_Across(NCluster *a, RContext *k, int s, int t){
    double sw = 0;
    double c = a->GetSetById(s)->Size();
    for(int i=0; i < a->GetSetById(s)->Size(); i++){
        RSet *cc =  k->GetSet(s,a->GetSetById(s)->At(i));
        RSet *cc1;
        if(a->GetSetById(t)->Size() > cc->Size()){
            cc1 = cc;
        }else{
         cc1 =  cc->GetSubspace(a->GetSetById(t));
        }
        double avg = cc1->Mean();
        sw += pow(avg,2);
    }
    return sqrt(sw/(c));
}