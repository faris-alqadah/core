#include "../headers/IOSet.h"

IOSet::IOSet(){
    size=0;
    id = -1;
    marked=false;
}
IOSet::IOSet(int sz){
    size = sz;
    id=-1;
    marked=false;
    d.resize(sz);
}
IOSet::IOSet( IOSet * a){
    assert(a != NULL);
    DeepCopy(a);
}

IOSet::~IOSet(){}

    int IOSet::Size(){return size;}
   int IOSet::Id(){return id;}
   void IOSet::SetId(int newId){id = newId;}
   void IOSet::Output() {
       for(int i=0; i < size; i++)
           cout<<d[i]<<" ";

   }
   void IOSet::Output(ofstream& out){
       assert(out.is_open());
       for(int i=0; i < size; i++){
            out<<d[i]+1;
            if(i != size-1) out<<" ";
       }
   }
   void IOSet::Output(ofstream& out, NameMap* nm){
        for(int i=0; i < size; i++){
            out<<nm->GetName(d[i]);
            out<<"\n";
       }
   }
   void IOSet::SetSize(int sz){
       assert(sz >= 0);
       size = sz;
   }

   void IOSet::Resize(int sz){
       assert(sz >= 0);
       d.resize(sz);
       size =sz;
   }
   void IOSet::Add(int a){
       assert( a >= 0);
       d.push_back(a);
       size++;
   }
   bool IOSet::Equal(IOSet &b){
       if( size != b.size) return false;
       else{
           for(int i=0; i < size; i++)
               if (d[i] != b.d[i]) return false;
           return true;
       }
   }
   bool IOSet::Contains(int a){
       for(int i=0; i < size; i++){
           if( d[i] == a) return true;
           else if( d[i] > a) return false;
       }
       return false;
   }
   void IOSet::DeepCopy(IOSet *a){
       assert(a !=NULL);
         d.resize(a->Size());
        for(int i=0; i < a->Size(); i++)
            d[i] = a->At(i);
        size = a->Size();
        id = a->Id();
        marked = a->marked;
   }
   void IOSet::Remove(int idx){
       assert(idx < size);
        d.erase(d.begin()+idx);
        size--;

   }

   void IOSet::FindRemove(int a){
       vector<unsigned int>::iterator it = find(d.begin(),d.end(),a);
       if (it != d.end()) d.erase(it);
       size--;
   }
   void IOSet::Sort(){
       sort(d.begin(),d.begin()+size);
   }
   int IOSet::At(int idx){
       assert(idx >= 0 && idx <= size);
       return d[idx];
   }
   void IOSet::Clear(){
       d.clear();
       size=0;
   }
   void IOSet::SetMarked(bool a){ marked = a;}
   bool IOSet::GetMarked(){return marked;}
   vector<unsigned int>::iterator IOSet::GetBegin(){ return d.begin();}

   vector<unsigned int>::iterator IOSet::GetEnd(){return d.end();}

   int IOSet::GetMaxElement(){
       if (size > 0)
        return *max_element(d.begin(),d.begin()+size);
       else return -1;
   }

bool Compare_Sup(IOSet *a, IOSet *b){
    assert(a != NULL && b != NULL);
    return a->Size() > b->Size();
}

bool Compare_Id(IOSet *a, IOSet *b){
    assert(a != NULL && b != NULL);
    return a->Id() > b->Id();
}


