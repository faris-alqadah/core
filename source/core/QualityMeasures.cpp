
#include "../../headers/core/QualityMeasures.h"



double Area(NCluster *a, vector<double> &params){
    assert(params.size() >= 1 );
    int art = params[0];
    assert( a->ContainsIOSetId(art));
    double sum=0;
    for(int i=0; i < a->GetN(); i++)
        if(a->GetSet(i)->Id() != art) sum += a->GetSet(i)->Size();
    return sum*a->GetSetById(art)->Size();
}


double Beta(NCluster *a, vector<double> &params){
    assert(params.size() >= 2 && (params[1] >= 0 && params[1] <= 1));
     int art = params[0];
    assert( a->ContainsIOSetId(art));
    double sum=0;
     for(int i=0; i < a->GetN(); i++)
        if(a->GetSet(i)->Id() != art) sum += a->GetSet(i)->Size();
    return sum*pow(1/params[1],a->GetSetById(art)->Size());
}
