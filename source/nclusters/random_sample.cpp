#include "../../headers/nclusters/random_sample.h"




vector<long double> * NClusterRandomSample::GetFreqWeights(Context *c, int s, int t){
    vector<long double> *weights = new vector<long double>(c->GetNumSets(t));
    long double sum = 0;
    for (int i=0; i < c->GetNumSets(t); i++){
        long double pwr=(long double)c->GetSet(t,i)->Size();///10.0;
       // cout<<"\nweights i "<<i<<" as "<<c->GetSet(t,i)->Size();
        (*weights)[i] = pow(2,pwr); // weigh by size of power set
        sum += (*weights)[i];
    }
    for(int i=0; i < weights->size(); i++) (*weights)[i] /= sum;
    return weights;
}

vector<long double> * NClusterRandomSample::GetFreqWeightsStar(RelationGraph *g, int s){
    IOSet *sObjs = g->GetLabels(s);
    vector<long double> *weights = new vector<long double>(sObjs->Size());
    vector<Context*> *ctxs = g->GetContexts(s);
    for(int i=0; i < sObjs->Size(); i++){
        long double avgTlength=0;
        for(int j=0; j < ctxs->size(); j++){
            pair<int,int> dIds = (*ctxs)[j]->GetDomainIds();
			int t= dIds.first == s ? dIds.second: dIds.first;
			long double currTLength = (*ctxs)[j]->GetSet(s,i)->Size();
			if (currTLength == 0){
				avgTlength = 0;
				break;
			}else{
				avgTlength += avgTlength;
			}
        }
		avgTlength /= (long double) ctxs->Size();
		(*weights)[i] = pow(2,avgTlength);
		
    }
    delete sObjs;
	for(int i=0; i < weights->size(); i++) (*weights)[i] /= sum;
    return weights;
}
vector<long double> * NClusterRandomSample::GetAreaWeights(Context *c, int s, int t){
    vector< long double> *weights = new vector<long double>(c->GetNumSets(t));
    long double sum=0;
    for (int i=0; i < c->GetNumSets(t); i++){
        double pwr=(long double)c->GetSet(t,i)->Size();///10.0;
        (*weights)[i] = pwr*pow(2,pwr-1); // weigh by size of power set
        sum += (*weights)[i];
    }
    for(int i=0; i < weights->size(); i++) (*weights)[i] /= sum;

    return weights;
}

IOSet* NClusterRandomSample::SubspaceFreq(Context *c, int s,int t, vector<long double> &weights){
    //randomly draw object from t
    cout<<"\nweights size: "<<weights.size();
    int randT;
    do{
        randT =  WeightedUniformDraw(weights);
    }while(c->GetSet(t,randT)->Size() == 0);
   cout<<"\ntransaction selected: "<<randT;
   cout<<"\ntransact had weight "<<weights[randT];
    //uniformly select from power set of psi^t(randT)
    cout<<"\t size: "<<c->GetSet(t,randT)->Size();
    cout.flush();
    IOSet *ret = UniformSubsetDraw(c->GetSet(t,randT));
    ret->SetId(s);
    return ret;

}
IOSet* NClusterRandomSample::SubspaceArea(Context *c, int s, int t, vector<long double> &weights){
    int randT =  WeightedUniformDraw(weights);
    //cout<<"\ntransaction selected: "<<randT;
    //uniformly select from power set of psi^t(randT)
    IOSet *ret =  BinomialSubsetDraw(c->GetSet(t,randT));
    ret->SetId(s);
    return ret;
}

NCluster* NClusterRandomSample::SubspaceStarShapedFreq(RelationGraph *g, int s ){
     //first construct weights
    vector<Context*> * sContexts = g->GetContexts(s);
    map< int,vector<long double>* > weightsMap;
    //construct weights
    for(int i=0; i < sContexts->size(); i++){
        Context *currContext = (*sContexts)[i];
        pair<int,int> ctxIds = currContext->GetDomainIds();
        int t = ctxIds.first == s ? ctxIds.second : ctxIds.first;
        vector<long double> *currWeights = GetFreqWeights(currContext, s,t);
        weightsMap.insert( pair<int,vector<long double>* > (t,currWeights));

    }
    IOSet *itrsct = new IOSet;
    //randomly draw subspaces ~ frequency until intersection is greater than 1 or max is reached
    int max=1000;
    int ctr=0;
    do{
        for(int i=0; i < sContexts->size(); i++){
              Context *currContext = (*sContexts)[i];
              pair<int,int> ctxIds = currContext->GetDomainIds();
              int t = ctxIds.first == s ? ctxIds.second : ctxIds.first;
              vector<long double> *currWeights = weightsMap[t];
              cout<<"\nGeting subspace from context "<<currContext->GetId()<<"\t s: "<<s<<"\tt: "<<t;
              IOSet *curr = SubspaceFreq(currContext,s,t, (*currWeights));
              IOSet *tmp = itrsct;
              cout<<"\ngot the curr: "; curr->Output();cout<<"\nadn the itrsct: "; itrsct->Output();
              if (i == 0)
                  itrsct = curr;
              else
                  itrsct = Intersect(curr,itrsct);

              delete tmp;
              tmp=NULL;
         }
         ctr++;
    }while(itrsct->Size() <= 0 && ctr < max);
    //now use prime operator to complete
    if (ctr < max){
        itrsct->SetId(s);
        NCluster *ret= new NCluster();
        ret->AddSet(itrsct);
        for(int i=0; i < sContexts->size(); i++){
                  Context *currContext = (*sContexts)[i];
                  pair<int,int> ctxIds = currContext->GetDomainIds();
                  int t = ctxIds.first == s ? ctxIds.second : ctxIds.first;
                  IOSet *primeT = Prime(ret,g,s,t,1);
                  primeT->SetId(t);
                  ret->AddSet(primeT);
        }
        return ret;
    }else{
        return NULL;
    }

}

NCluster* NClusterRandomSample::SubspaceStarShapedFreqSample(RelationGraph *g, int s, NCluster *sample, IOSet *completedDomains ){
    vector<Context*> *ctxs = g->GetContexts(s);
    //get sub contexts
    vector<Context*> * sContexts = new vector<Context*>();
    for(int i=0; i < ctxs->size(); i++){
        pair<int,int> ctxIds = (*ctxs)[i]->GetDomainIds();
        int t = ctxIds.first == s ? ctxIds.second : ctxIds.first;
        cout<<"\ngetting sub context for domains: "<<s<<"\t"<<t;
        cout<<"\nsample set sizes are: "<<sample->GetSetById(s)->Size()<<"\t"<<sample->GetSetById(t)->Size();
        sContexts->push_back(  (*ctxs)[i]->GetSubContext(sample->GetSetById(s),sample->GetSetById(t)) );
       // cout<<"\noutput sub contexts...\n";
        //(*sContexts)[i]->PrintAsFIMI();
    }

    //first construct weights
    map< int,vector<long double>* > weightsMap;
    //construct weights
    for(int i=0; i < sContexts->size(); i++){
        Context *currContext = (*sContexts)[i];
        pair<int,int> ctxIds = currContext->GetDomainIds();
        int t = ctxIds.first == s ? ctxIds.second : ctxIds.first;
        if(!completedDomains->Contains(t)) { //only perform operation in context not already sampled
            vector<long double> *currWeights = GetFreqWeights(currContext, s,t);
            weightsMap.insert( pair<int,vector<long double>* > (t,currWeights));
            }
    }
    IOSet *itrsct = new IOSet;
    //randomly draw subspaces ~ frequency until intersection is greater than 1 or max is reached
    int max=1000;
    int ctr=0;
    do{
        for(int i=0; i < sContexts->size(); i++){
              Context *currContext = (*sContexts)[i];
              pair<int,int> ctxIds = currContext->GetDomainIds();
              int t = ctxIds.first == s ? ctxIds.second : ctxIds.first;
              if(!completedDomains->Contains(t)) { //only perform operation in context not already sampled
                  vector<long double> *currWeights = weightsMap[t];
                  cout<<"\ncur context of "<<ctxIds.first<<" and "<<ctxIds.second;
                  cout.flush();
                  IOSet *curr = SubspaceFreq(currContext,s,t, (*currWeights));
                  NCluster *test = new NCluster;
                  curr->SetId(s);
                  test->AddSet(curr);
                  IOSet *thePrime = Prime(test,g,s,t,1);
                  cout<<"\ngot the curr: "; curr->Output();
                  cout<<"\ngot the prime: "; thePrime->Output();
                  cout<<"\nadn the itrsct: "; itrsct->Output();
                  IOSet *tmp = itrsct;
                  if (i == 0)
                      itrsct = curr;
                  else
                      itrsct = Intersect(curr,itrsct);

                  delete tmp;
                  tmp=NULL;
              }
         }
         ctr++;
    }while(itrsct->Size() <= 0 && ctr < max);
    //now use prime operator to complete
    if (ctr < max){
        NCluster *ret = new NCluster;
        //now update the source by taking intersection with original
        //IOSet *itrsct2 = Intersect(itrsct,sample->GetSetById(s));
        //itrsct2->SetId(s);
        itrsct->SetId(s);
        ret->AddSet(itrsct);
        delete itrsct;
        if(ret->GetSetById(s)->Size() > 0){
            for(int i=0; i < sContexts->size(); i++){
                      Context *currContext = (*sContexts)[i];
                      pair<int,int> ctxIds = currContext->GetDomainIds();
                      int t = ctxIds.first == s ? ctxIds.second : ctxIds.first;
                      if(!completedDomains->Contains(t)){
                        IOSet *primeT = Prime(ret,g,s,t,1);
                        if (primeT == NULL){
                             delete ret;
                            DstryVector(sContexts);
                            return NULL;
                        }
                        primeT->SetId(t);
                        ret->AddSet(primeT);
                      }
            }
            //destroy the subcontexts
            DstryVector( sContexts);
            //add all completed domains
            IOSet *neighbors = g->GetNeighbors(s);
            IOSet *tmp = completedDomains;
            completedDomains = Union(tmp,completedDomains);
            completedDomains->Add(s);
            completedDomains->Sort();
            delete neighbors; delete tmp;
            return ret;
        }
        delete ret;
        DstryVector(sContexts);
        return NULL;
    }else{
         DstryVector( sContexts);
        return NULL;
    }

}

NCluster* NClusterRandomSample::SubspaceStarShapedArea(RelationGraph *g, int s ){
     //first construct weights
    vector<Context*> * sContexts = g->GetContexts(s);
    map< int,vector<long double>* > weightsMap;
    //construct weights
    for(int i=0; i < sContexts->size(); i++){
        Context *currContext = (*sContexts)[i];
        pair<int,int> ctxIds = currContext->GetDomainIds();
        int t = ctxIds.first == s ? ctxIds.second : ctxIds.first;
        vector<long double> *currWeights = GetAreaWeights(currContext, s,t);
        weightsMap.insert( pair<int,vector<long double>* > (t,currWeights));

    }
    IOSet *itrsct = new IOSet;
    //randomly draw subspaces ~ frequency until intersection is greater than 1 or max is reached
    int max=1000;
    int ctr=0;
    do{
        for(int i=0; i < sContexts->size(); i++){
              Context *currContext = (*sContexts)[i];
              pair<int,int> ctxIds = currContext->GetDomainIds();
              int t = ctxIds.first == s ? ctxIds.second : ctxIds.first;
              vector<long double> *currWeights = weightsMap[t];
              IOSet *curr = SubspaceArea(currContext,s,t, (*currWeights));
              IOSet *tmp = itrsct;
              if (i == 0)
                  itrsct = curr;
              else
                  itrsct = Intersect(curr,itrsct);

              delete tmp;
              tmp=NULL;
         }
         ctr++;
    }while(itrsct->Size() <= 0 && ctr < max);
    //now use prime operator to complete
    if (ctr < max){
        itrsct->SetId(s);
        NCluster *ret= new NCluster();
        ret->AddSet(itrsct);
        for(int i=0; i < sContexts->size(); i++){
                  Context *currContext = (*sContexts)[i];
                  pair<int,int> ctxIds = currContext->GetDomainIds();
                  int t = ctxIds.first == s ? ctxIds.second : ctxIds.first;
                  IOSet *primeT = Prime(ret,g,s,t,1);
                  primeT->SetId(t);
                  ret->AddSet(primeT);
        }
        return ret;
    }else{
        return NULL;
    }

}

NCluster * NClusterRandomSample::SubspaceFreqNetwork(RelationGraph *g, int s){
    //first step is to generate n-cluster in initial star shaped hin
    //as defined by s
    IOSet *completedDomains = new IOSet; // keep tracking of the domains that have already been completed
    //get the common objects in s to all neighbors
    IOSet *commonS = g->GetCommonObjectsArtNode(s);
    commonS->SetId(s);
    IOSet *neighborIds = g->GetNeighbors(s);
    cout<<"\nsize of commonS: "<<commonS->Size();
    //construct the sample subspace
    NCluster *sampleSubspace = new NCluster;
    sampleSubspace->AddSet(commonS);
    for(int i=0; i < neighborIds->Size(); i++){
        IOSet *t = g->GetDomainObjs(neighborIds->At(i));
        t->SetId(neighborIds->At(i));
        sampleSubspace->AddSet(t);
    }
     NCluster *init1 = SubspaceStarShapedFreqSample(g,s,sampleSubspace,completedDomains);
     cout<<"\ngot init1...\n";
     if (init1 != NULL)
         init1->Output();
     else
         cout<<"\ninit is null";
//
//    //copy into init
//    for(int i=0; i < init1->GetN(); i++){
//        int currId = init1->GetSet(i)->Id();
//        init->GetSetById( currId)->DeepCopy(init1->GetSet(i));
//    }
//    delete init1;
//    //do a BFS using SubspaceStarShapedFreqSample
//    //insert all neighboring articulation nodes of s
//    IOSet *artDomains = g->GetArtDomains();
//    queue<int> q;
//     cout<<"\nthe init1\n"; init->Output();
//    cout.flush();
//    IOSet *sNeighbors = g->GetNeighbors(s);
//    for(int i=0; i < sNeighbors->Size(); i++){
//        if(artDomains->Contains(sNeighbors->At(i)))
//            q.push(sNeighbors->At(i));
//    }
//    cout<<"\nq size: "<<q.size();
//    delete sNeighbors;
//    while (!q.empty()){
//        int s1 = q.front();
//        q.pop();
//        init = SubspaceStarShapedFreqSample(g,s1,init);
//        cout<<"\n init after first step with s1 "<<s1<<"\n";
//        init->Output();
//        if(init == NULL) return NULL;
//        IOSet *sNeighbors = g->GetNeighbors(s);
//        for(int i=0; i < sNeighbors->Size(); i++){
//            int id = sNeighbors->At(i);
//            if(artDomains->Contains(sNeighbors->At(i)) && init->GetSetById(id)->Size() == 0)
//                q.push(sNeighbors->At(i));
//        }
//        delete sNeighbors;
//     }
    return init1;
}