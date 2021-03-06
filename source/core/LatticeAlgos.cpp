#include "../../headers/core/LatticeAlgos.h"



void  LatticeHash::MakeHashKey(NCluster*a, int t, string &theKey){
    theKey="__";
    IOSet *tDomain = a->GetSetById(t);
    for(int i=0; i < tDomain->Size(); i++){
      stringstream ss;
      ss << tDomain->At(i);
      if(i > 0)
	theKey += '_';
      theKey += ss.str();
    }
}
  void LatticeHash::AddNode(NCluster *a){
     string hshKey;
     MakeHashKey(a,1,hshKey);
     map<string,LatticeHashNode*>::iterator it;
     it = latticeMap.find(hshKey);
     if( it == latticeMap.end()){
      LatticeHashNode *node = new LatticeHashNode();
      node->concept=new NCluster(*a);
      latticeMap[hshKey] = node;
     }
  }

 void  LatticeHash::AddNode(NCluster *a, vector<NCluster*> *upperNeighbors, vector<NCluster*> *lowerNeighbors){
   //first get the hash key
   string hshKey;
   MakeHashKey(a,1,hshKey);
   LatticeHashNode *node = new LatticeHashNode();
   node->concept=a;
   if(upperNeighbors != NULL){
    for(int i=0; i < upperNeighbors->size(); i++){
	string hshKey1;
	MakeHashKey(upperNeighbors->at(i),1,hshKey1);
	node->uppers.push_back(hshKey1);
    }
   }
   if(lowerNeighbors != NULL){
    for(int i=0; i < lowerNeighbors->size(); i++){
	string hshKey1;
	MakeHashKey(lowerNeighbors->at(i),1,hshKey1);
	node->lowers.push_back(hshKey1);
    }
   }
   latticeMap[hshKey] = node;
 }

    vector<NCluster*> * LatticeHash::GetUpperNeighbors(NCluster *a,int t){
       //get the key
    string hshKey;
    MakeHashKey(a,t,hshKey);
    map<string,LatticeHashNode*>::iterator it;
    it = latticeMap.find(hshKey);
    vector<NCluster *> * uppers = new vector<NCluster*>();
    if(it != latticeMap.end()){
	LatticeHashNode *theNode = latticeMap[hshKey];
	for(int i=0; i < theNode->uppers.size(); i++){
	    string currUpperId = theNode->uppers.at(i);
	    it = latticeMap.find(currUpperId);
	    if(it != latticeMap.end()){
	       LatticeHashNode *theNode = latticeMap[currUpperId];
	       NCluster *currUpper = new NCluster();
	       currUpper->DeepCopy(*latticeMap[currUpperId]->concept);
	       uppers->push_back(currUpper);
	    }

	  }
	}
    return uppers;  
 }
    

  void LatticeHash::UpdateUppers(NCluster *a, NCluster *theUpper){
    string hshKey;
    MakeHashKey(a,1,hshKey);
    string hshKey2;
    MakeHashKey(theUpper,1,hshKey2);
     map<string,LatticeHashNode*>::iterator it, it2;
    it = latticeMap.find(hshKey);
    it2 = latticeMap.find(hshKey2);
    if(it != latticeMap.end() && it2 != latticeMap.end()){
	LatticeHashNode *theNode = latticeMap[hshKey];
	theNode->uppers.push_back(hshKey2);
    }
  }
    
  void LatticeHash::UpdateLowers(NCluster *a, NCluster *theLower){
    string hshKey;
    MakeHashKey(a,1,hshKey);
    string hshKey2;
    MakeHashKey(theLower,1,hshKey2);
     map<string,LatticeHashNode*>::iterator it, it2;
    it = latticeMap.find(hshKey);
    it2 = latticeMap.find(hshKey2);
    if(it != latticeMap.end() && it2 != latticeMap.end()){
	LatticeHashNode *theNode = latticeMap[hshKey];
	theNode->lowers.push_back(hshKey2);
    }
  }
    
    vector<NCluster*> * LatticeHash::GetLowerNeighbors(NCluster *a,int t){
       //get the key
    string hshKey;
    MakeHashKey(a,t,hshKey);
    map<string,LatticeHashNode*>::iterator it;
    it = latticeMap.find(hshKey);
    vector<NCluster *> * lowers = new vector<NCluster*>();
    if(it != latticeMap.end()){
	LatticeHashNode *theNode = latticeMap[hshKey];
	for(int i=0; i < theNode->lowers.size(); i++){
	    string currLowerId = theNode->lowers.at(i);
	    it = latticeMap.find(currLowerId);
	    if(it != latticeMap.end()){
	       LatticeHashNode *theNode = latticeMap[currLowerId];
	       NCluster *currLower = new NCluster();
	       currLower->DeepCopy(*latticeMap[currLowerId]->concept);
	       lowers->push_back(currLower );
	    }
	  }
	}
    return lowers;  
 }
    
  void LatticeHash::OutputNode(NCluster *a,int t, ofstream& out,vector<NameMap*>&nm){
	  //make label and identifiers for a
	  string nclusterJson="{";
	  a->AsJson(nclusterJson,nm);
	  nclusterJson += "\"upper_neighbors\" : [";
	  //create identifier arrays for neighbors
	  vector<NCluster*> * upperNeighbors =  GetUpperNeighbors(a,t);
	  for (int i = 0; i < upperNeighbors->size(); i++) {
		  string label;
		  upperNeighbors->at(i)->GenerateLabel(label);
		  nclusterJson += "\"";
		  nclusterJson += label;
		  nclusterJson += "\",";
	  }

	  DstryVector(upperNeighbors);

	  nclusterJson += "],";
	  nclusterJson += "\"lower_neighbors\" : [";
	  vector<NCluster*>* lowerNeighbors =  GetLowerNeighbors(a,t);
	  for (int i = 0; i < lowerNeighbors->size(); i++) {
	  		  string label;
	  		  lowerNeighbors->at(i)->GenerateLabel(label);
	  		  nclusterJson += "\"";
	  		  nclusterJson += label;
	  		  nclusterJson += "\",";
	  	  }
	  nclusterJson += "]}";
	  out << nclusterJson <<",\n";
	  DstryVector(lowerNeighbors);
   }






