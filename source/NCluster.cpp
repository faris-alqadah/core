#include "../headers/NCluster.h"
    NCluster::NCluster(){
        n=0;
        sets.resize(0);
        quality=0;
        id=0;
        marked=false;
    }
NCluster::NCluster(unsigned int nn) {
    n = nn;
    sets.resize(n);
    for(int i=0; i < n;i++) sets[i] = new IOSet;
    quality=0;
    id=0;
    marked=false;
}
NCluster::NCluster(unsigned int nn, vector<IOSet*> &v) {
    assert(v.size() == nn);
    n= nn;
    sets.resize(n);
    for(int i=0; i < n;i++) sets[i] = v[i];
    quality=0;
    id = 0;
    marked=false;

}
NCluster::NCluster(unsigned int nn, bool dontAllocate){
    n = nn;
    sets.resize(n);
    quality=0;
    id =0;
    marked=false;

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
}

void NCluster::DeepCopy(NCluster& a){
   n = a.GetN();
   quality = a.GetQuality();
   id = a.GetId();

   sets.resize(n);
   for(int i=0; i < n; i++){
       sets[i] = new IOSet( (a.GetSet(i) ));
   }
   marked = a.marked;
}
void NCluster::Output(){
    for(int i=0; i < n; i++){
        cout<<"["<<i+1<<"]\t";
        if(sets[i])       sets[i]->Output();
        cout<<"\n";
    }
}
void NCluster::Output(ofstream &out){
    assert(out.is_open());
    for(int i=0; i < n; i++){
        out<<"["<<i+1<<"]\t";
        sets[i]->Output(out);
    }
}
void NCluster::Output(ofstream& out, vector<NameMap*>& nm){
    assert(out.is_open() && nm.size() == n);
    for(int i=0; i < n; i++){
        int id = sets[i]->Id();
        out<<"\n"<<id<<")\n";
        
        sets[i]->Output(out,nm[id]);
    }
}

int NCluster::GetN() {return n;}





IOSet * NCluster::GetSet(int idx){ 
    assert(idx >= 0 && idx < n);
    return sets[idx];
}
IOSet * NCluster::GetSetById(int id){
    for(int i=0; i < n; i++)
        if (sets[i]->Id() == id)
            return sets[i];
      return NULL;
 }
void NCluster::AddSet(IOSet* a){
      assert(a != NULL);
      sets.push_back(a);
      n++;
  }

void NCluster::AssignSet(int idx, IOSet *a){
    assert(idx >= 0 && idx < n);
    IOSet *tmp = sets[idx];
    sets[idx] = a;
    delete tmp;
    tmp = NULL;
}
void NCluster::AssignSetById(int id, IOSet* a){
    assert(ContainsIOSetId(id));
    for(int i=0; i < n; i++){
        if(sets[i]->Id() == id){
            IOSet *tmp = sets[i];
            sets[i] = a;
            delete tmp;
            tmp=NULL;
            return;
        }
    }
  }
double NCluster::GetQuality(){return quality;}
void NCluster::SetQuality(double q){quality = q;}

int NCluster::GetId(){return id;}
void NCluster::SetId(int a){ id = a;}
bool NCluster::GetMarked(){return marked;}
void NCluster::SetMarked(bool m){marked =m;}
bool NCluster::ContainsIOSetId(int id){
    assert(id >= 0);
    for(int i=0; i < n; i++)
        if (sets[i]->Id() == id) return true;
    return false;
}
 int NCluster::GetMaxElement(){
     int max=-1;
     for(int i=0; i < n; i++){
         int m = sets[i]->GetMaxElement();
         if ( m> max)
             max = m;
     }
     return max;
 }
bool Compare_Quality(NCluster *a, NCluster *b){
    assert (a != NULL && b != NULL);
    return a->GetQuality() > b->GetQuality();
}





