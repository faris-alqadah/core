
#include "../../headers/alpha_concepts/dispersion.h"


double Range(RSet *a, vector<double> &params){
    if(a->Size() < 1) return -1;
    return a->GetMaxElement().second - a->GetMinElement().second; 
}