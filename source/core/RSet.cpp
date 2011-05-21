#include "../../headers/core/RSet.h"

RSet::RSet(){
    size=0;
    id = -1;
    marked=false;
    quality=-1;
}
RSet::RSet(int sz){
    size = sz;
    id=-1;
    marked=false;
    vals.resize(sz);
    quality=-1;
}
RSet::RSet( RSet * a){
    assert(a != NULL);
    DeepCopy(a);
}

RSet::~RSet(){}

    int RSet::Size(){return size;}
   int RSet::Id(){return id;}
   void RSet::SetId(int newId){id = newId;}
   void RSet::Output() {
       for(int i=0; i < size; i++){
           cout<<vals[i].first<<","<<vals[i].second;
           if (i != size-1) cout <<" ";
       }

   }
   void RSet::Output(ofstream& out){
       assert(out.is_open());
       for(int i=0; i < size; i++){
           out<<vals[i].first<<","<<vals[i].second;
           if (i != size-1) out <<" ";
       }
   }
   void RSet::Output(ofstream& out, NameMap* nm){
       for(int i=0; i < size; i++){
           out<<nm->GetName(vals[i].first)<<","<<vals[i].second;
           if (i != size-1) out <<"\t";
       } 
   }
   void RSet::SetSize(int sz){
       assert(sz >= 0);
       size = sz;
   }

   void RSet::Resize(int sz){
       assert(sz >= 0);
       vals.resize(sz);
       size =sz;
   }
   void RSet::Add(pair<int,double> a){
       assert(a.first >= 0);
       vals.push_back(a);
       size++;
   }
   bool RSet::Equal(RSet &b){
       if( size != b.size) return false;
       else{
           for(int i=0; i < size; i++)
               if ( (vals[i].first != b.vals[i].first) || (vals[i].second != b.vals[i].second)) return false;
           return true;
       }
   }
   bool RSet::Contains(pair<int,double> a){
       for(int i=0; i < size; i++)
           if( (vals[i].second == a.second) && (vals[i].first == a.first) )
               return true;
      
       return false;
   }
   void RSet::DeepCopy(RSet *a){
       assert(a !=NULL);
         vals.resize(a->Size());
        for(int i=0; i < a->Size(); i++){
            vals[i] = a->At(i);
        }
        size = a->Size();
        id = a->Id();
        marked = a->marked;
        quality = a->GetQuality();
   }
   void RSet::Remove(int idx){
       assert(idx < size);
        vals.erase(vals.begin()+idx);
        size--;
   }
   void RSet::FindRemove(pair<int,double> a){
       for(int i=0; i < size; i++){
           if(vals[i].second == a.second && vals[i].first == a.first){
               vals.erase(vals.begin()+i);
               size--;
               break;
           }
       }
   }

   bool comp_pair_vals_greater (pair<int,double> a,pair<int,double> b){
            return a.second > b.second;
   }
   bool comp_pair_vals_less (pair<int,double> a,pair<int,double> b){
            return a.second < b.second;
   }
   bool comp_pair_idxs_greater (pair<int,double> a,pair<int,double> b){
            return a.first > b.first;
   }
   bool comp_pair_idxs_less (pair<int,double> a,pair<int,double> b){
            return a.first < b.first;
   }
   int RSet::GetMaxIdx(){
       if (size > 0)
        return (*max_element(vals.begin(),vals.begin()+size,comp_pair_idxs_less)).first;
       else
           return -1;
   }

   void RSet::Sort(){
       sort(vals.begin(),vals.begin()+size,comp_pair_idxs_greater);
   }
   pair<int,double> RSet::At(int idx){
       assert(idx >= 0 && idx <= size);
       return vals[idx];
   }
    //! Returns the actual index of the object id objId in self
    int RSet::GetIndexPtr(int objId){
        for(int i=0; i < size; i++)
            if (vals[i].first == objId) return i;
        return -1;
    }
    IOSet *RSet::GetIdxs(){
        IOSet *ret = new IOSet;
        for(int i=0; i < size; i++) ret->Add(vals[i].first);
        return ret;
    }
    RSet* RSet::GetSubspace(IOSet *idxs){
        assert(size >= idxs->Size() && idxs->Size() >= 1);
        RSet *ret = new RSet;
        for(int aPtr=0,bPtr=0; aPtr < size && bPtr < idxs->Size();){
            if(vals[aPtr].first == idxs->At(bPtr)){
                ret->Add(vals[aPtr]);
                aPtr++;
                bPtr++;
            }else if(vals[aPtr].first > idxs->At(bPtr)) bPtr++;
            else aPtr++;
       }
       return ret;
    }
    pair<double,double> RSet::GetMinMaxSubspace(IOSet *idxs){
        assert(size >= idxs->Size() && idxs->Size() >= 1);
        pair<double,double> ret;
        ret.first=vals[0].second;
        ret.second=vals[0].second;
        for(int aPtr=0,bPtr=0; aPtr < size && bPtr < idxs->Size();){
            if(vals[aPtr].first == idxs->At(bPtr)){
                if(vals[aPtr].second < ret.first)
                    ret.first = vals[aPtr].second;
                else if (vals[aPtr].second > ret.second)
                    ret.second = vals[aPtr].second;
                aPtr++;
                bPtr++;
            }else if(vals[aPtr].first > idxs->At(bPtr)) bPtr++;
            else aPtr++;
       }
       return ret;
    }
    pair<int,int> RSet::GetMinMaxSubspaceIdxs(IOSet *idxs){
        assert(size >= idxs->Size() && idxs->Size() >= 1);
        pair<double,double> ret;
        ret.first=0;
        ret.second=0;
        double min=vals[0].second;
        double max=vals[0].second;
        for(int aPtr=0,bPtr=0; aPtr < size && bPtr < idxs->Size();){
            if(vals[aPtr].first == idxs->At(bPtr)){
                if(vals[aPtr].second < min){
                    ret.first = aPtr;
                    min = vals[aPtr].second;
                }
                else if (vals[aPtr].second >max){
                    ret.second = aPtr;
                    max=vals[aPtr].second;
                }
                aPtr++;
                bPtr++;
            }else if(vals[aPtr].first > idxs->At(bPtr)) bPtr++;
            else aPtr++;
       }
       return ret;
    }
   void RSet::Clear(){
       vals.clear();
       size=0;
   }
   void RSet::SetMarked(bool a){ marked = a;}
   bool RSet::GetMarked(){return marked;}
   void RSet::SetQuality(double q){quality = q;}
   double RSet::GetQuality(){return quality;}

   pair<int,double> RSet::GetMaxElement(){
       if (size > 0){
           pair<int,double> maxE = vals[0];
           double maxVal = maxE.second;
           for(int i=1; i < size; i++){
               if(vals[i].second > maxVal){
                   maxVal = vals[i].second;
                   maxE = vals[i];
               }
           }
           return maxE;
       }
       else {
           pair<int,double> ret;
           ret.first=-1;
           ret.second=-1;
           return ret;
       }
   }
   pair<int,double> RSet::GetMinElement(){
       if (size > 0){
        pair<int,double> minE = vals[0];
           double minVal = minE.second;
           for(int i=1; i < size; i++){
               if(vals[i].second < minVal){
                   minVal = vals[i].second;
                   minE = vals[i];
               }
           }
           return minE;
       }
       else {
           pair<int,double> ret;
           ret.first=-1;
           ret.second=-1;
           return ret;
       }
   }
   double RSet::FindReturn(int idx){
       for(int i=0; i < size; i++)
           if (vals[i].first == idx)
               return vals[i].second;
       return -99999990999;
   }
  
   double RSet::Mean(){
       double accum=0;
       for(int i=0; i < size; i++) accum += vals[i].second;
       return accum/(double)size;
   }
double RSet::Variance(){
    double qi=0,q1=0,ai=0,a1=0,sz=size;
    for(double i=0.0; i < size; i++){
        ai=a1+(1.0/(i+1.0))*(vals[i].second-a1);
        qi=q1+(vals[i].second-a1)*(vals[i].second-ai);
        q1=qi;
        a1=ai;
    }
    return (1.0/(size-1.0))*qi;
}

double RSet::Std_Dev(){
    return sqrt(Variance());

}
bool RSet_Compare_Sup(RSet *a, RSet *b){
    assert(a != NULL && b != NULL);
    return a->Size() > b->Size();
}

bool RSet_Compare_Id(RSet *a, RSet *b){
    assert(a != NULL && b != NULL);
    return a->Id() > b->Id();
}



