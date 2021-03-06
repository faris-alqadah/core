#include "../../headers/core/NRCluster.h"
    NRCluster::NRCluster(){
        n=0;
        sets.resize(0);
        quality=0;
        id=0;
        marked=false;
    }
NRCluster::NRCluster(unsigned int nn) {
    n = nn;
    sets.resize(n);
    for(int i=0; i < n;i++) sets[i] = new RSet;
    quality=0;
    id=0;
    marked=false;
}
NRCluster::NRCluster(unsigned int nn, vector<RSet*> &v) {
    assert(v.size() == nn);
    n= nn;
    sets.resize(n);
    for(int i=0; i < n;i++) sets[i] = v[i];
    quality=0;
    id = 0;
    marked=false;

}
NRCluster::NRCluster(unsigned int nn, bool dontAllocate){
    n = nn;
    sets.resize(n);
    quality=0;
    id =0;
    marked=false;

}

NRCluster::NRCluster(NRCluster &a){
    this->DeepCopy(a);
}
NRCluster::~NRCluster(){
        for(int i=0; i < n; i++)
            if( sets[i]) {
                delete sets[i];
                sets[i] = NULL;
            }
}

void NRCluster::DeepCopy(NRCluster& a){
   n = a.GetN();
   quality = a.GetQuality();
   id = a.GetId();

   sets.resize(n);
   for(int i=0; i < n; i++){
       sets[i] = new RSet( (a.GetSet(i) ));
   }
   marked = a.marked;
}
void NRCluster::Output(){
    for(int i=0; i < n; i++){
        cout<<"["<<sets[i]->Id()<<"]\t";
        if(sets[i])       sets[i]->Output();
        cout<<"\n";
    }
}
void NRCluster::Output(ofstream &out){
    assert(out.is_open());
    for(int i=0; i < n; i++){
        out<<"["<<sets[i]->Id()<<"]\t";
        sets[i]->Output(out);
        out<<"\n";
    }
}
void NRCluster::Output(ofstream& out, vector<NameMap*>& nm){
    assert(out.is_open());
    for(int i=0; i < n; i++){
        bool found=false;
        for(int j=0; j < nm.size(); j++){
            if (nm[j]->GetId() == sets[i]->Id()){
                out<<"["<<sets[i]->Id()<<"]\t";
                sets[i]->Output(out,nm[j]);
                found = true;
                out<<"\n";
                break;
            }
        }
        if(!found){
            sets[i]->Output(out);
            out<<"\n";
        }
    }
}

int NRCluster::GetN() {return n;}





RSet * NRCluster::GetSet(int idx){
    assert(idx >= 0 && idx < n);
    return sets[idx];
}

RSet * NRCluster::GetSetById(int id){
    for(int i=0; i < n; i++)
        if (sets[i]->Id() == id)
            return sets[i];
      return NULL;
 }
void NRCluster::AddSet(RSet* a){
      assert(a != NULL);
      sets.push_back(a);
      n++;
  }

void NRCluster::AssignSet(int idx, RSet *a){
    assert(idx >= 0 && idx < n);
    RSet *tmp = sets[idx];
    sets[idx] = a;
    delete tmp;
    tmp = NULL;
}
void NRCluster::AssignSetById(int id, RSet* a){
    assert(ContainsRSetId(id));
    for(int i=0; i < n; i++){
        if(sets[i]->Id() == id){
            RSet *tmp = sets[i];
            sets[i] = a;
            delete tmp;
            tmp=NULL;
            return;
        }
    }
  }
double NRCluster::GetQuality(){return quality;}
void NRCluster::SetQuality(double q){quality = q;}

int NRCluster::GetId(){return id;}
void NRCluster::SetId(int a){ id = a;}
bool NRCluster::GetMarked(){return marked;}
void NRCluster::SetMarked(bool m){marked =m;}
bool NRCluster::ContainsRSetId(int id){
    assert(id >= 0);
    for(int i=0; i < n; i++)
        if (sets[i]->Id() == id) return true;
    return false;
}
pair<int,double> NRCluster::GetMaxElement(){
     pair<int,double> max;
     max.first = -1;
     max.second=-1;
     for(int i=0; i < n; i++){
         pair<int,double> m = sets[i]->GetMaxElement();
         if ( m.second > max.second)
             max = m;
     }
     return max;
 }
int NRCluster::GetMaxIdx(){
    int max=-1;
    for(int i=0; i < n; i++){
        int curr = sets[i]->GetMaxIdx();
        if (curr > max) max = curr;
    }
    return max;
}

 void NRCluster::SortSets(){
     sort(sets.begin(),sets.end(),RSet_Compare_Quality);
 }


bool Compare_Quality(NRCluster *a, NRCluster *b){
    assert (a != NULL && b != NULL);
    return a->GetQuality() > b->GetQuality();
}






