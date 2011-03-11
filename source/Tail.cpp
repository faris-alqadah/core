
#include "../Headers/Tail.h"

using namespace std;
Tail::Tail() {
}
Tail::~Tail() {
   for(std::list<IOSet*>::iterator it=tailList.begin(); it!=tailList.end(); it++){
		if( (*it)) {
			delete (*it);
		}
	}
}

void Tail::AddNode(IOSet* a){
    tailList.push_back(a);
}

list<IOSet*>::iterator Tail::RemoveNode2(list<IOSet*>::iterator it){
	IOSet *t = &(*(*it));
        it = tailList.erase(it);
	if(t != NULL) {
            delete t;
        }
        return it;
}

list<IOSet*>::iterator Tail::RemoveNode1(int id, int &ct){
    std::list<IOSet*>::iterator it=tailList.begin();
    while( it != tailList.end()){
        if( (*it)->Id() == id){
	    IOSet *t = &(*(*it));
            it = tailList.erase(it);
	    delete t;
            return it;
        }
	it++;
    }
}

list<IOSet*>::iterator Tail::RemoveNode(int id){
    std::list<IOSet*>::iterator it=tailList.begin();
    while( it != tailList.end()){
        if( (*it)->Id() == id){
            it = RemoveNode2(it);
            return it;
        }
	it++;
    }
}
void Tail::Clear(){
   std::list<IOSet*>::iterator it=tailList.begin();
   while( it != tailList.end()){
	  it = RemoveNode2(it);

   }

}
IOSet* Tail::GetIds(){
   IOSet *ret = new IOSet;
  for(std::list<IOSet*>::iterator it=tailList.begin(); it!=tailList.end(); it++){
      ret->Add((*it)->Id());
   }
  return ret;
}

void Tail::DeepCopy(Tail *tl){
    tailList.clear();
    std::list<IOSet*>::iterator it=tl->tailList.begin();
    while(it != tl->tailList.end()){
        IOSet *a = new IOSet;
        a->DeepCopy((*it));
        this->AddNode(a);
        it++;
    }
}

void Tail::Output(){
    for(std::list<IOSet*>::iterator it=tailList.begin(); it!=tailList.end(); it++){

        cout<<" "<<(*it)->Id(); cout<<": "; (*it)->Output();
    }
}
void Tail::Output1(){
    for(std::list<IOSet*>::iterator it=tailList.begin(); it!=tailList.end(); it++){

        cout<<"\n";(*it)->Output();
    }
}


void Tail::Sort(){
    tailList.sort(Compare_Sup);
}
void Tail::Sort_Id(){
    tailList.sort(Compare_Id);
}
int Tail::Size(){
    return tailList.size();
}
IOSet* Tail::Get(int index){
    int count=0;
    for(std::list<IOSet*>::iterator it=tailList.begin(); it!=tailList.end(); it++){
      if(count == index)
          return (*it);
      count++;
    }
}
IOSet* Tail::Get_Id(int id){
	for(std::list<IOSet*>::iterator it=tailList.begin(); it!=tailList.end(); it++){
      	 if((*it)->Id() == id)
          	return (*it);
    }

}

bool Tail::Contains(int id){
    for(std::list<IOSet*>::iterator it=tailList.begin(); it!=tailList.end(); it++){
      if((*it)->Id() == id)
          return true;
    }
    return false;
}

void Tail::RemoveNotInList(IOSet *list){
    std::list<IOSet*>::iterator it=tailList.begin();
    while( it!=tailList.end()){
        int currId= (*it)->Id();
        if (!list->Contains(currId)){
            it=RemoveNode2(it);
        }
        else{
            it++;
        }
    }
}

