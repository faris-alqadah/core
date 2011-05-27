
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
        if(lclInter->Size() > 0){
             RSet *fullSub =  rr->GetSubspace(lclInter);
             RSet *subSub = rr->GetSubspace(s1);
             double rangeFull = fullSub->GetMaxElement().second - fullSub->GetMinElement().second;
             double rangeSub =  subSub->GetMaxElement().second - subSub->GetMinElement().second;
             dist += (1+lclDiff->Size())* (1-(rangeSub/rangeFull));
             delete fullSub;
             delete subSub;
        }else{
                dist += (double)(1.0+lclDiff->Size());
        }
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

double Mean_Square_Error(NCluster *a, RContext *k, int s, int t){
    IOSet *cols = a->GetSetById(s);
    IOSet *rows = a->GetSetById(t);
    double mse=0;
    double allCnt; // cnt of all values in the subspace
    double allSum=0; //the sum of all values
    double allMean=0;
    vector<double> rowMeans(rows->Size());
    vector<double> colMeans(cols->Size());
    //get row means and total mean
    for(int i=0; i < rows->Size();i++){
        RSet *lclRow = k->GetSet(t,rows->At(i));
        RSet *subSpace = lclRow->GetSubspace(cols);
        if(subSpace->Size() > 0){
            double sum = subSpace->Sum();
            allSum += sum;
            allCnt += subSpace->Size();
            rowMeans.push_back( sum/(double)cols->Size());
        }
        delete subSpace;
    }
    allMean = allSum/allCnt;
    //get col means
    for(int i=0; i < cols->Size(); i++){
        RSet *lclCol = k->GetSet(s,cols->At(i));
        RSet *subSpace = lclCol->GetSubspace(rows);
        if(subSpace->Size() > 0){
            colMeans.push_back(subSpace->Mean());
        }
        delete subSpace;
    }
    //now sum for all elements
    for(int i=0; i < rows->Size();i++){
        RSet *lclRow = k->GetSet(t,rows->At(i));
        RSet *subSpace = lclRow->GetSubspace(cols);
        if(subSpace->Size() > 0){
            for(int j=0; j < subSpace->Size(); j++){
                pair<int,double> val = subSpace->At(cols->At(j));
                if(val.first != -1)
                    mse += pow(val.second-rowMeans[j]-colMeans[j]+allMean,2);
            }
        }
        delete subSpace;
    }
    return mse/allCnt;
}