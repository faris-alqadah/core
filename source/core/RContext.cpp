#include "../../headers/core/RContext.h"


RContext::RContext(int n1 ,int n2){
    domain1 = new NRCluster(n1);
    domain2 = new NRCluster(n2);
    domain1->SetId(0);
    domain2->SetId(1);
    stdDev1 = new RSet();
    stdDev2 = new RSet();
    range1 = new RSet();
    range2 = new RSet();
    id =0;
    name="~";
}
RContext::RContext( RContext &a){
    domain1->DeepCopy(*a.domain1);
    domain2->DeepCopy(*a.domain2);
    stdDev1->DeepCopy(a.stdDev1);
    stdDev2->DeepCopy(a.stdDev2);
    range1->DeepCopy(a.range1);
    range2->DeepCopy(a.range2);
    id = a.id;
    name = a.name;
}
RContext::RContext(NRCluster *d1, NRCluster *d2){
    assert(d1 != NULL && d2 != NULL);
    domain1 = new NRCluster(*d1);
    domain2 = new NRCluster(*d2);
    stdDev1 = new RSet();
    stdDev2 = new RSet();
    range1 = new RSet();
    range2 = new RSet();
    id = 0;
    name = "~";
}
RContext::~RContext(){
    if (domain1 != NULL) delete domain1;
    if (domain2 != NULL) delete domain2;
    if(stdDev1 != NULL) delete stdDev1;
    if(stdDev2 != NULL) delete stdDev2;
    if(range1 != NULL) delete range1;
    if(range2 != NULL) delete range2;
    domain1 = NULL;
    domain2 = NULL;
    stdDev1=NULL;
    stdDev2=NULL;
    if(nameMap1 != NULL) delete nameMap1;
    if(nameMap2 != NULL) delete nameMap2;
    nameMap1 = NULL;
    nameMap2 = NULL;
}
RSet * RContext::GetSet(int domain,int setNum){
    assert(setNum >= 0);
    assert(domain == domain1->GetId() || domain == domain2->GetId());
    if (domain == domain1->GetId()){
        assert(setNum < domain1->GetN());
        return domain1->GetSet(setNum);
    }else{
        assert(setNum < domain2->GetN());
        return domain2->GetSet(setNum);
    }
}
IOSet * RContext::GetLabels(int domain){
    assert(domain == domain1->GetId() || domain == domain2->GetId());
    IOSet *ret = new IOSet;
    if(domain == domain1->GetId() ){
        for(int i=0; i < domain1->GetN(); i++) ret->Add(domain1->GetSet(i)->Id());
    }else{
        for(int i=0; i < domain2->GetN(); i++) ret->Add(domain2->GetSet(i)->Id());
    }
    return ret;
}

int RContext::GetId(){return id;}

void RContext::SetId(int iid){id = iid;}

string RContext::GetName(){return name;}
void RContext::SetName(string &n){ name = n;}

void RContext::SetDomainId(int domain, int id){
    assert(domain == 0 || domain == 1);
    if(domain == 0) domain1->SetId(id);
    else domain2->SetId(id);
}
int RContext::GetDomainId(int domain){
    assert(domain == 0 || domain == 1);
    if(domain == 0) return domain1->GetId();
    else return domain2->GetId();
}



void RContext::PrintAsMatrix(){
    for(int i=0; i < domain1->GetN(); i++){
        RSet *currRow = domain1->GetSet(i);
        for(int j=0,onesPtr=0; j < currRow->Size() /**&& onesPtr < currRow->Size()*/; j++){
            if (j > 0)
                cout<<"\t";
            if( j == currRow->At(onesPtr).first){
                cout<<currRow->At(onesPtr).second;
                onesPtr++;
            }
            else cout<<"0"; // j can only be <= currRow[onesPtr]

        }
        cout<<"\n";
    }
}

void RContext::PrintAsMatrix(ofstream &out){
     for(int i=0; i < domain1->GetN(); i++){
        RSet *currRow = domain1->GetSet(i);
        for(int j=0,onesPtr=0; j < currRow->Size() /*&& onesPtr < currRow->GetMaxIdx()*/; j++){
            if (j > 0)
                out<<"\t";
            if( j == currRow->At(onesPtr).first){
                out<<currRow->At(onesPtr).second;
                onesPtr++;
            }
            else out<<"0"; // j can only be <= currRow[onesPtr]

        }
        out<<"\n";
    }
}

RContext * RContext::GetSubRContext(IOSet *a, IOSet *b){
    assert(a->Size() > 0 && b->Size() > 0);
    NRCluster* d1 = new NRCluster(a->Size());
    NRCluster *d2 = new NRCluster(b->Size());

    for(int i=0; i < a->Size(); i++){
        RSet *currRow = domain1->GetSet(a->At(i));
        IOSet *currIdxs = currRow->GetIdxs();
        IOSet *newIdxs = Intersect(currIdxs,b);
        RSet *newRow = new RSet();
        newRow->SetId(a->At(i));
        for(int j=0; j < newIdxs->Size(); j++){
            pair<int,double> val = currRow->At(newIdxs->At(j));
            newRow->Add(val);
        }
        d1->AssignSet(i,newRow);
        delete currIdxs;
        delete newIdxs;
    }
    for(int i=0; i < b->Size(); i++){
        RSet *currCol = domain2->GetSet(b->At(i));
        IOSet *currIdxs = currCol->GetIdxs();
        IOSet *newIdxs = Intersect(currIdxs,a);
        RSet *newCol = new RSet();
        newCol->SetId(b->At(i));
        for(int j=0; j < newIdxs->Size(); j++){
            pair<int,double> val = currCol->At(newIdxs->At(j));
           // val.first=j; //dont retain old index
            newCol->Add(val);
        }
        d2->AssignSet(i,newCol);
        delete currIdxs;
        delete newIdxs;
    }
    RContext *ret= new RContext(d1,d2);
    delete d1;
    delete d2;
    return ret;
}

void RContext::PrintAsSparse(){
    for(int i=0; i < domain1->GetN(); i++){
        domain1->GetSet(i)->Output();
        cout<<"\n";
    }

}
void RContext::PrintAsSparse(ofstream &out){
       for(int i=0; i < domain1->GetN(); i++){
        domain1->GetSet(i)->Output(out);
        out<<"\n";
    }
  }

int RContext::GetNumSets(int domainId){
     assert(domainId == domain1->GetId() || domainId == domain2->GetId());
     if (domainId == domain1->GetId()) return domain1->GetN();
     else return domain2->GetN();

}

int RContext::GetNumCells(){
    int cnt=0;
    for(int i=0; i < domain1->GetN(); i++) cnt += domain1->GetSet(i)->Size();
    return cnt;
}

double RContext::GetDensity(){
    return (double)GetNumCells() / ( (double)domain1->GetN()*(double)domain2->GetN() );
}

pair<int,int> RContext::GetDomainIds(){
    pair<int,int> ret;
    ret.first=domain1->GetId();
    ret.second=domain2->GetId();
    return ret;
}
void RContext::SetNameMap(int dId, NameMap* nm){
    assert(nm != NULL);
    if(dId == domain1->GetId()){
        nameMap1 = nm;
    }else if (dId == domain2->GetId()){
        nameMap2 = nm;
    }
}
NameMap* RContext::GetNameMap(int dId){
    if(dId == domain1->GetId()){
        return nameMap1;
    }else if (dId == domain2->GetId()){
        return nameMap2;
    }
}
void RContext::PrintBasicStats(){
    cout<<"\n########################\n"
        <<"\nRContext name: "<<name<<"\t"<<id
        <<"\nFirst domain id : "<<domain1->GetId()<<" with "<<domain1->GetN()<<" elements"
        <<"\nSecond domain id : "<<domain2->GetId()<<" with "<<domain2->GetN()<<" elements"
        <<"\nDensity: "<<GetDensity()
        <<"\n########################\n";
}
 void RContext::PrintStdDevs(){
     cout<<"\n###STD DEVS###########\n";
     cout<<"\nDomain: "<<domain1->GetId()<<"\n";
     stdDev1->Output();
     cout<<"\nDomain: "<<domain2->GetId()<<"\n";
     stdDev2->Output();
 }
void RContext::ComputeStdDevs(){
    stdDev1->Clear();
    stdDev2->Clear();
    for(int i=0; i < domain1->GetN(); i++){
        pair<int,double> val;
        val.first = domain1->GetSet(i)->Id();
        val.second = domain1->GetSet(i)->Std_Dev();
        stdDev1->Add(val);
    }
    for(int i=0; i < domain2->GetN(); i++){
        pair<int,double> val;
        val.first = domain2->GetSet(i)->Id();
        val.second = domain2->GetSet(i)->Std_Dev();
        stdDev2->Add(val);
    }
}

void RContext::ComputeRanges(){
    range1->Clear();
    range2->Clear();
    for(int i=0; i < domain1->GetN(); i++){
        pair<int,double> val;
        val.first = domain1->GetSet(i)->Id();
        val.second = domain1->GetSet(i)->Range();
        range1->Add(val);
    }
    for(int i=0; i < domain2->GetN(); i++){
        pair<int,double> val;
        val.first = domain2->GetSet(i)->Id();
        val.second = domain2->GetSet(i)->Range();
        range2->Add(val);
    }
}

double RContext::GetRange(int domain, int setNum){
     assert(setNum >= 0);
    assert(domain == domain1->GetId() || domain == domain2->GetId());
    if (domain == domain1->GetId()){
        return range1->At(setNum).second;
    }else{
         return range2->At(setNum).second;
    }

}

double RContext::GetStdDev(int domain, int setNum){
    assert(setNum >= 0);
    assert(domain == domain1->GetId() || domain == domain2->GetId());
    if (domain == domain1->GetId()){
        return stdDev1->At(setNum).second;
    }else{
         return stdDev2->At(setNum).second;
    }

}

