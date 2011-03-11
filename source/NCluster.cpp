#include "../Headers/NCluster.h"

NCluster::NCluster(unsigned int nn) {
    n = nn;
    sets.resize(n);
    for(int i=0; i < n;i++) sets[i] = new IOSet;
    quality=0;
    heightVec.resize(n);
    fill(heightVec.begin(),heightVec.end(),false);
    heightVec[0]=true;
    height=0;
    width=0;
}
NCluster::NCluster(unsigned int nn, vector<IOSet*> &v) {
    n= nn;
    sets.resize(n);
    for(int i=0; i < n;i++) sets[i] = v[i];
    quality=0;
    heightVec.resize(n);
    fill(heightVec.begin(),heightVec.end(),false);
    heightVec[0]=true;
    ComputeHeight();
    ComputeWidth();
}
NCluster::NCluster(unsigned int nn, bool dontAllocate){
    n = nn;
    sets.resize(n);
    quality=0;
    heightVec.resize(n);
    fill(heightVec.begin(),heightVec.end(),false);
    heightVec[0]=true;
    height=0;
    width=0;
}
void NCluster::InitalizeSet(int idx,IOSet *a){
    sets[idx] = a;
}
NCluster::NCluster(NCluster &a){
    this->DeepCopy(a);
}
NCluster::~NCluster(){
        for(int i=0; i < n; i++)
            if( sets[i]) {
                delete sets[i];
                sets[i] = NULL;
            }
        for(int i=0; i < uppers.size(); i++) uppers[i] = NULL;
        for(int i=0; i < lowers.size(); i++) lowers[i] = NULL;
}

void NCluster::DeepCopy(NCluster& a){
   n = a.GetN();
   quality = a.GetQuality();
   heightVec.resize(n);
   sets.resize(n);
   for(int i=0; i < n; i++){
       sets[i] = new IOSet( (a.GetSet(i) ));
       heightVec[i] = a.IsHeight(i);
   }
   vector<NCluster*> *un = a.GetUppers();
   vector<NCluster*> *ln = a.GetLowers();
   SetUpperNeighbors( (*un));
   SetUpperNeighbors( (*ln) );
}
void NCluster::Output(){
    for(int i=0; i < n; i++){
        cout<<"["<<i+1<<"]\t";
        if(sets[i])       sets[i]->Output();
        cout<<"\n";
    }
}
void NCluster::Output(ofstream &out){
    for(int i=0; i < n; i++){
        out<<"["<<i+1<<"]\t";
        sets[i]->Output(out);
    }
}
void NCluster::Output(ofstream& out, vector<NameMap*>& nm){
    for(int i=0; i < n; i++){
        int id = sets[i]->Id();
        out<<"\n"<<id<<")\n";
        
        sets[i]->Output(out,nm[id]);
    }
}
double NCluster::GetHeight(){return height;}
double NCluster::GetWidth(){return width;}
void NCluster::SetHeight(double h){height = h;}
void NCluster::SetWidth(double w){width = w;}
int NCluster::GetN() {return n;}
vector<NCluster*>* NCluster::GetUppers(){ return &uppers;}
vector<NCluster*>* NCluster::GetLowers(){ return &lowers;}

void NCluster::SetUpperNeighbors(vector<NCluster*> &a){
    uppers.clear();
    uppers.resize(a.size());
    for(int i=0; i < a.size(); i++) uppers[i] = a[i];
}

void NCluster::SetLowerNeighbors(vector<NCluster*> &a){
    lowers.clear();
    lowers.resize(a.size());
    for(int i=0; i < a.size(); i++) lowers[i] = a[i];
}
IOSet * NCluster::GetSet(int idx){ return sets[idx];}

void NCluster::AssignSet(int idx, IOSet *a){
    IOSet *tmp = sets[idx];
    sets[idx] = a;
    delete tmp;
    tmp = NULL;
}
double NCluster::GetQuality(){return quality;}
double NCluster::SetQuality(double q){quality = q;}

bool NCluster::IsHeight(int idx){return heightVec[idx];}

void NCluster::ComputeHeight(){height= sets[0]->Size();} //fill for different applications
 void NCluster::ComputeWidth(){width= sets[1]->Size();}   //fill for different applicatoins





