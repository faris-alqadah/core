
#include "../../headers/core/Ops.h"


int numIntersection=0;
int numSubset=0;
int numUnion=0;
int numDifference=0;

IOSet* SymmDifference( IOSet *a,  IOSet *b){
    IOSet *a1 = Intersect(a,b);
    IOSet *a2 = Union(a,b);
    IOSet *ret = Difference(a2,a1);
    delete a1; delete a2;
    return ret;
}
IOSet * Difference( IOSet *a, IOSet *b){
    assert(a != NULL && b != NULL);
     IOSet *ret = new IOSet(a->Size());
     vector<unsigned int>::iterator it =set_difference (a->GetBegin(), a->GetBegin()+a->Size(),
                                                b->GetBegin(), b->GetBegin()+b->Size(),
                                                ret->GetBegin());
     
     ret->SetSize(int(it - ret->GetBegin()));
     numDifference++;
     return ret;
}



IOSet * Intersect( IOSet *a,  IOSet *b ){
    assert(a != NULL && b != NULL);
    if (a->Size() == 0 || b->Size() == 0)
        return new IOSet;
    IOSet *ret = new IOSet;
     
     int minSize =  (a->Size() > b->Size()) ? b->Size() : a->Size();
     ret->Resize(minSize);
     vector<unsigned int>::iterator it =set_intersection (a->GetBegin(), a->GetBegin()+a->Size(),
                                                b->GetBegin(), b->GetBegin()+b->Size(),
                                                ret->GetBegin());
     int newSz = int(it - ret->GetBegin());
     ret->SetSize(newSz);
     numIntersection++;
     return ret;
}

IOSet * Union(IOSet *a,  IOSet *b ){
    //int maxSize =  (a->Size() > b->Size()) ? a->Size() : b->Size();
    assert(a != NULL && b != NULL);
     IOSet *ret = new IOSet(a->Size()+b->Size());
     vector<unsigned int>::iterator it =set_union (a->GetBegin(), a->GetBegin()+a->Size(),
                                                b->GetBegin(), b->GetBegin()+b->Size(),
                                                ret->GetBegin());
     ret->SetSize(int(it - ret->GetBegin()));
     numUnion++;
     return ret;
}

bool Contains( IOSet *a,  IOSet *b){
    assert(a != NULL && b != NULL);
    numSubset++;
   return includes(a->GetBegin(),a->GetBegin()+a->Size(),b->GetBegin(),b->GetBegin()+b->Size());
}
bool ProperSubSet(IOSet *a, IOSet *b){
    if(a->Size() > b->Size()){
        return Contains(a,b);
    }else return false;
}
double PercentOverlap( IOSet *a,  IOSet *b){
    IOSet *inter = Intersect(a,b);
    IOSet *unin = Union(a,b);
    double top = (double)inter->Size();
    double bottom = (double)unin->Size();
    delete inter; delete unin;
    return top/bottom;
}
double PercentOverlap_Sorensen( IOSet *a, IOSet *b ){
    IOSet *inter = Intersect(a,b);
    double ret =  ( (double)2*inter->Size() )/(a->Size() + b->Size());
    delete inter;
    return ret;

}
double AverageOverlap(NCluster *a, NCluster *b){
    assert(a != NULL && b != NULL);
    assert(a->GetN() == b->GetN());
    double accum=0;
    for(int i=0; i < a->GetN(); i++) accum += PercentOverlap( a->GetSet(i), b->GetSet(i) );
    return accum/(double) a->GetN();
}
double GreaterEqualSize(NCluster *a, NCluster *b){
    if (b->GetSet(0)->Size() >= a->GetSet(0)->Size()) return 1;
    else return 0;
}

NCluster *TransposeFimi(NCluster *a){
    assert(a != NULL);
    NCluster *ret = new NCluster(a->GetMaxElement()+1);
    for(int i=0; i < a->GetN(); i++)
        for(int j=0; j < a->GetSet(i)->Size(); j++)
            ret->GetSet(a->GetSet(i)->At(j))->Add(i);
     //set ids
     for(int i=0; i < ret->GetN(); i++) ret->GetSet(i)->SetId(i);
    return ret;
    }


int WeightedUniformDraw(vector<double> &weights){
    double sum=0;
    for(int i=0; i < weights.size(); i++) sum += weights[i];
    for(int i=0; i < weights.size(); i++) weights[i]  /= sum;
    //srand(time(NULL));
    double rnd = (double) rand() / RAND_MAX;
 // cout<<"\nrnd sum: "<<sum;
    random_shuffle(weights.begin(),weights.end());
    for(int i=0; i < weights.size(); i++){
   //   cout<<"\nrnd "<<rnd<<"\tw "<<weights[i];
        if(rnd <= weights[i])
            return i;
        else
            rnd -= weights[i];
    }    
}

IOSet * UniformSubsetDraw(IOSet *t){
    //randomly select size of subset
    //first select the size of the subset as weighted size
   // cout<<"\nset size: "<<t->Size();
    int rndSize = rand() % t->Size();
    vector<int> idxs(t->Size());
    for(int i=0; i < t->Size(); i++){
        idxs[i] = i;
    }
   // cout<<"\nrand select size: "<<rndSize;
    random_shuffle(idxs.begin(),idxs.end());
    IOSet *ret = new IOSet;
    for(int i=0; i < rndSize+1; i++){
        ret->Add(t->At(idxs[i]));
    }
    ret->Sort();
    return ret;
    
}

unsigned int NChooseK(unsigned int n, unsigned int k){
    if (k > n) {
        return 0;
    }
    unsigned int r = 1;
    for (unsigned int d = 1; d <= k; ++d) {
        r *= n--;
        r /= d;
    }
    return r;

}