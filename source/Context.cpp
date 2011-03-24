#include "../headers/Context.h"


Context::Context(int n1 ,int n2){
    domain1 = new NCluster(n1);
    domain2 = new NCluster(n2);
    domain1->SetId(0);
    domain2->SetId(1);
    id =0;
    name="~";
}
Context::Context( Context &a){
    domain1->DeepCopy(*a.domain1);
    domain2->DeepCopy(*a.domain2);
    id = a.id;
    name = a.name;
}
Context::Context(NCluster *d1, NCluster *d2){
    assert(d1 != NULL && d2 != NULL);
    domain1 = new NCluster(*d1);
    domain2 = new NCluster(*d2);
    id = 0;
    name = "~";
}
Context::~Context(){
    if (domain1 != NULL) delete domain1;
    if (domain2 != NULL) delete domain2;
    domain1 = NULL;
    domain2 = NULL;
    if(nameMap1 != NULL) delete nameMap1;
    if(nameMap2 != NULL) delete nameMap2;
    nameMap1 = NULL;
    nameMap2 = NULL;
}
IOSet * Context::GetSet(int domain,int setNum){
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
IOSet * Context::GetLabels(int domain){
    assert(domain == domain1->GetId() || domain == domain2->GetId());
    IOSet *ret = new IOSet;
    if(domain == domain1->GetId() ){
        for(int i=0; i < domain1->GetN(); i++) ret->Add(domain1->GetSet(i)->Id());
    }else{
        for(int i=0; i < domain2->GetN(); i++) ret->Add(domain2->GetSet(i)->Id());
    }
    return ret;
}

int Context::GetId(){return id;}

void Context::SetId(int iid){id = iid;}

string Context::GetName(){return name;}
void Context::SetName(string &n){ name = n;}

void Context::SetDomainId(int domain, int id){
    assert(domain == 0 || domain == 1);
    if(domain == 0) domain1->SetId(id);
    else domain2->SetId(id);
}
int Context::GetDomainId(int domain){
    assert(domain == 0 || domain == 1);
    if(domain == 0) return domain1->GetId();
    else return domain2->GetId();
}



void Context::PrintAsMatrix(){
    for(int i=0; i < domain1->GetN(); i++){
        IOSet *currRow = domain1->GetSet(i);
        for(int j=0,onesPtr=0; j < currRow->Size() && onesPtr < currRow->Size(); j++){
            if (j > 0)
                cout<<"\t";
            if( j == currRow->At(onesPtr)){
                cout<<"1";
                onesPtr++;
            }
            else cout<<"0"; // j can only be <= currRow[onesPtr]
            
        }
        cout<<"\n";
    }
}

void Context::PrintAsMatrix(ofstream &out){
     for(int i=0; i < domain1->GetN(); i++){
        IOSet *currRow = domain1->GetSet(i);
        for(int j=0,onesPtr=0; j < currRow->Size() && onesPtr < currRow->Size(); j++){
            if (j > 0)
                out<<"\t";
            if( j == currRow->At(onesPtr)){
                out<<"1";
                onesPtr++;
            }
            else out<<"0"; // j can only be <= currRow[onesPtr]

        }
        out<<"\n";
    }
}

Context * Context::GetSubContext(IOSet *a, IOSet *b){
    assert(a->Size() > 0 && b->Size() > 0);
    NCluster* d1 = new NCluster(a->Size());
    NCluster *d2 = new NCluster(b->Size());

    for(int i=0; i < a->Size(); i++)  d1->AssignSet(i,Intersect(domain1->GetSet(a->At(i)),b));
    for(int i=0; i < b->Size(); i++)  d2->AssignSet(i,Intersect(domain2->GetSet(b->At(i)),a));
    Context *ret= new Context(d1,d2);
    delete d1;
    delete d2;
    return ret;
}

void Context::PrintAsFIMI(){
    for(int i=0; i < domain1->GetN(); i++){
        domain1->GetSet(i)->Output();
        cout<<"\n";
    }

}
void Context::PrintAsFIMI(ofstream &out){
       for(int i=0; i < domain1->GetN(); i++){
        domain1->GetSet(i)->Output(out);
        out<<"\n";
    }
  }

int Context::GetNumSets(int domainId){
     assert(domainId == domain1->GetId() || domainId == domain2->GetId());
     if (domainId == domain1->GetId()) return domain1->GetN();
     else return domain2->GetN();

}

int Context::GetNumOnes(){
    int cnt=0;
    for(int i=0; i < domain1->GetN(); i++) cnt += domain1->GetSet(i)->Size();
    return cnt;
}

double Context::GetDensity(){
    return (double)GetNumOnes() / ( (double)domain1->GetN()*(double)domain2->GetN() );
}

pair<int,int> Context::GetDomainIds(){
    pair<int,int> ret;
    ret.first=domain1->GetId();
    ret.second=domain2->GetId();
    return ret;
}
void Context::SetNameMap(int dId, NameMap* nm){
    assert(nm != NULL);
    if(dId == domain1->GetId()){
        nameMap1 = nm;
    }else if (dId == domain2->GetId()){
        nameMap2 = nm;
    }
}
NameMap* Context::GetNameMap(int dId){
    cout<<"\nchck2 "<<nameMap1->GetNumEntries();
    cout<<"\nchck2 "<<nameMap2->GetNumEntries();
    cout<<"\n";
    if(dId == domain1->GetId()){
        return nameMap1;
    }else if (dId == domain2->GetId()){
        return nameMap2;
    }
}