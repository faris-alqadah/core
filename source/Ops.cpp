
#include "../Headers/Ops.h"


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
     IOSet *ret = new IOSet(a->Size());
     vector<int>::iterator it =set_difference (a->GetBegin(), a->GetBegin()+a->Size(),
                                                b->GetBegin(), b->GetBegin()+b->Size(),
                                                ret->GetBegin());

     ret->SetSize(int(it - ret->GetBegin()));
     numDifference++;
     return ret;
}



IOSet * Intersect( IOSet *a,  IOSet *b ){
    if (a->Size() == 0 || b->Size() == 0)
        return new IOSet;
    IOSet *ret = new IOSet;
     
     int minSize =  (a->Size() > b->Size()) ? b->Size() : a->Size();
     ret->Resize(minSize);
     vector<int>::iterator it =set_intersection (a->GetBegin(), a->GetBegin()+a->Size(),
                                                b->GetBegin(), b->GetBegin()+b->Size(),
                                                ret->GetBegin());
     int newSz = int(it - ret->GetBegin());
     ret->SetSize(newSz);
     numIntersection++;
     return ret;
}

IOSet * Union(IOSet *a,  IOSet *b ){
    //int maxSize =  (a->Size() > b->Size()) ? a->Size() : b->Size();

     IOSet *ret = new IOSet(a->Size()+b->Size());
     vector<int>::iterator it =set_union (a->GetBegin(), a->GetBegin()+a->Size(),
                                                b->GetBegin(), b->GetBegin()+b->Size(),
                                                ret->GetBegin());
     ret->SetSize(int(it - ret->GetBegin()));
     numUnion++;
     return ret;
}

bool Contains( IOSet *a,  IOSet *b){
    numSubset++;
   return includes(a->GetBegin(),a->GetBegin()+a->Size(),b->GetBegin(),b->GetBegin()+b->Size());
}
bool ProperSubSet(IOSet *a, IOSet *b){
    if(a->Size() < b->Size()){
        return Contains(b,a);
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

