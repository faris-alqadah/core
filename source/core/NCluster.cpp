#include "../../headers/core/NCluster.h"
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
        cout<<"["<<sets[i]->Id()<<"]\t";
        if(sets[i])       sets[i]->Output();
        cout<<"\n";
    }
}
void NCluster::Output(ofstream &out){
    assert(out.is_open());
    for(int i=0; i < n; i++){
        out<<"["<<sets[i]->Id()<<"]\t";
        sets[i]->Output(out);
        out<<"\n";
    }
}

void NCluster::Output(ofstream& out, vector<NameMap*>& nm){
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
            out<<"["<<sets[i]->Id()<<"]\t";
            sets[i]->Output(out);
            out<<"\n";
        }
    }
}

void NCluster::GenerateLabel(string &labelStr){
	if (sets[0] != NULL)
		sets[0]->GenerateLabel(labelStr);
}

void NCluster::AsJson(string &json,vector<NameMap*>&nm){
	//get the generate id
	json += "\"cluster_id\": \"";
	string labelString;
	GenerateLabel(labelString);
	json += labelString;
	json += "\",";
	//get the element names

	for(int i=0; i < n; i++){
	        bool found=false;
	        for(int j=0; j < nm.size(); j++){
	        	//cerr<<"\nHERE JSON FIRST! "<<j<<"\t"<<nm[j]->GetId()<<"\t"<<sets[i]->Id();
	            if (nm[j]->GetId() == sets[i]->Id()){
	            	string idStr = to_string(nm[j]->GetId());
	            	json += "\"";
	            	json += idStr;
	            	json +="\": [";
	            	for(int k=0; k < sets[i]->Size(); k++){
	            		json += "\"";
	            		json += nm[j]->GetName(sets[i]->At(k));
	            		json += "\",";
	            	}
	            	json += "],";
	                found = true;
	                break;
	            }
	        }
//	        if(!found){
//	        	json += to_string(i);
//				json += "\": [";
//				cerr<<"\nHERE JSON FIRST! "<<<<"\t"<<nm[i]->GetId()<<"\t"<<sets[i]->Id();
//				for(int k=0; k < sets[i]->Size(); k++){
//					json += "\"";
//					json += nm[i]->GetName(sets[i]->At(k));
//					json += "\",";
//				}
//				json += "],";
//	        }
	    }
}


void NCluster::Output(vector<NameMap *>&nm){
    for(int i=0; i < n; i++){
        bool found=false;
        for(int j=0; j < nm.size(); j++){
            if (nm[j]->GetId() == sets[i]->Id()){
                cout<<"["<<sets[i]->Id()<<"]\t";
                sets[i]->Output(nm[j]);
                found = true;
                cout<<"\n";
                break;
            }
        }
        if(!found){
            cout<<"["<<sets[i]->Id()<<"]\t";
            sets[i]->Output();
            cout<<"\n";
        }
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
    void NCluster::RemoveSet(int i){
        assert( i >= 0 && i < n);
        IOSet *tmp = sets[i];
        delete tmp;
        sets.erase(sets.begin()+i);
        n--;
    }

void NCluster::AssignSet(int idx, IOSet* a){
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
            sets[i] =a;
            delete tmp;
            tmp=NULL;
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
 void NCluster::SortSets(){
     sort(sets.begin(),sets.end(),Compare_Quality_IOSet);
 }
bool Compare_Quality_NCluster(NCluster *a, NCluster *b){
    assert (a != NULL && b != NULL);
    return a->GetQuality() > b->GetQuality();
}





