
#include "../../headers/alpha_concepts/dispersion.h"


double Range(RSet *a, vector<double> &params){
    if(a->Size() < 1) return -1;
    return a->GetMaxElement().second - a->GetMinElement().second; 
}
double Imperfect_Query_Quality(NCluster *a, vector<double> &params){
    return a->GetSet(0)->Size();//( params[0] / (double)a->GetSet(0)->Size())*a->GetQuality();
}