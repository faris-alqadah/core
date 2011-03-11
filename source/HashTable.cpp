#include "../Headers/HashTable.h"


int HashKey(IOSet *a){
    int ret=0;
    for(int i=0; i < a->Size(); i++) ret += a->At(i);
    return ret;
}

 HashNode::HashNode(){}//discoveredU =false;}
void HashNode::DeepCopy(HashNode *b){
	sup = b->sup;
	hashValue = b->hashValue;
	a->DeepCopy(*(b->a));
}

HashNode::HashNode(NCluster * _a, int _hashKey, int _sup){
	a = _a;
	hashValue = _hashKey;
	sup = _sup;
       //discoveredU =false;
}
HashNode::~HashNode(){
	a= NULL;
}
void HashNode::Output(){
	a->GetSet(0)->Output(); cout<<"\thashKey: "<<hashValue<<"\tsup: "<<sup;
}
Hashtable::Hashtable(){}
Hashtable::~Hashtable(){}
int Hashtable::Size(){
	return hashTbl.size();
}

void Hashtable::Add(NCluster *a){
	HashNode *t = new HashNode(a,HashKey(a->GetSet(1)),a->GetSet(1)->Size());
	Add(t);
}

void Hashtable::Add(HashNode *a){
	int ins = hashTbl.hash_funct()(a->hashValue);
	hashTbl.insert(HTPair(ins, a));
	
}
void Hashtable::Remove(NCluster* a){
	HashNode *t = new HashNode(a,HashKey(a->GetSet(1)),a->GetSet(1)->Size());
	Remove(t);
}
void Hashtable::Remove(HashNode* a){
	a->a->GetSet(0)->Sort();
	int key = hashTbl.hash_funct() (a->hashValue);
        HTableFind range = hashTbl.equal_range(key);
	HTable::iterator it = range.first;
	while(it != range.second){
		if ( (*it).second->sup == a->sup && (*it).second->a->GetSet(0)->Equal(*(a->a->GetSet(0))) ){
			hashTbl.erase(it);
			delete a;
			break;
		}			
		it++;
	}	
}


void Hashtable::Output(){
	HTable::iterator it = hashTbl.begin();
	cout<<"\n~~~~~~~~~~~~~~~~~~~~Hash Table~~~~~~~~~~~~~~~~~~~~~~\n";
	while (it != hashTbl.end()){
		cout<<"\n"; (*it).second->Output();
		it++;
	}
	cout<<"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

//assumes t IS in the hash table

 NCluster * Hashtable::Get(NCluster *t){
	HashNode *a = new HashNode(t,HashKey(t->GetSet(1)),t->GetSet(1)->Size());
	//a->a->Objects->Sort();
	int ins = hashTbl.hash_funct()(a->hashValue);
    	HTableFind range = hashTbl.equal_range(ins);
    	HTable:: iterator it = range.first;
	 while( it != range.second){
		if ( (*it).second->sup == a->sup){
                  // cout<<"\nsame hash and support\nchecking items";
                  //  cout<<"\nin table: "; (*it).second->a->ItemVector[0]->Output();
                   // cout<<"\ncompare against: "; a->a->ItemVector[0]->Output();
                   	if( (*it).second->a->GetSet(1)->Equal(*(a->a->GetSet(1)))){	//found a subsumption
				return (*it).second->a;
			}
		}
		it++; 
	}
	return NULL;
}

bool Hashtable::InTable(HashNode *a){
    a->a->GetSet(0)->Sort();
    int ins = hashTbl.hash_funct()(a->hashValue);
    HTableFind range = hashTbl.equal_range(ins);
    HTable:: iterator it = range.first;
	 while( it != range.second){
		if ( (*it).second->sup == a->sup){
			if( (*it).second->a->GetSet(0)->Equal(*(a->a->GetSet(0)))){	//found a subsumption
				return true;
			}
		}
		it++; 
	}
	return false;
}

bool Hashtable::InTable(NCluster *a){
	HashNode *t = new HashNode(a,HashKey(a->GetSet(1)),a->GetSet(1)->Size());
	bool ret = InTable(t);
	delete t;
	return ret;
}



