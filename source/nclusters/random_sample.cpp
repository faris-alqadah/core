#include "../../headers/nclusters/random_sample.h"


void NClusterRandomSample::NClusterFreq(RelationGraph *g){

}

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
vector<long double> * NClusterRandomSample::GetFreqWeightsSub(Context *c, int s, int t,NCluster *sub){
    vector<long double> *weights = new vector<long double>(c->GetNumSets(t));
    long double sum = 0;
    for (int i=0; i < c->GetNumSets(t); i++){
        IOSet *tmp = Intersect(c->GetSet(t,i),sub->GetSetById(s));
        long double pwr=(long double)tmp->Size();
        (*weights)[i] = pow(2,pwr); // weigh by size of power set
        sum += (*weights)[i];
        delete tmp;
    }
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
    int randT =  WeightedUniformDraw(weights);
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
              IOSet *curr = SubspaceFreq(currContext,s,t, (*currWeights));
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

NCluster* NClusterRandomSample::SubspaceStarShapedFreqSample(RelationGraph *g, int s, NCluster *sample ){
    vector<Context*> * sContexts = g->GetContexts(s);
     //if only two objects already in the s subspace then just use a prime to get subspaces in neighboring domains
    if(sample->GetSetById(s)->Size() <= 2){
        for(int i=0; i < sContexts->size(); i++){
                  Context *currContext = (*sContexts)[i];
                  pair<int,int> ctxIds = currContext->GetDomainIds();
                  int t = ctxIds.first == s ? ctxIds.second : ctxIds.first;
                  if(sample->GetSetById(t)->Size() == 0){   //only perform operation in context not already sampled
                    IOSet *primeT = Prime(sample,g,s,t,1);
                    if (primeT == NULL){
                        delete sample;
                        delete primeT;
                        return NULL;
                    }
                    primeT->SetId(t);
                   // sample->AssignSetById(s,primeT);
                  }
        }
        return sample;
    }
    //first construct weights
    map< int,vector<long double>* > weightsMap;
    //construct weights
    for(int i=0; i < sContexts->size(); i++){
        Context *currContext = (*sContexts)[i];
        pair<int,int> ctxIds = currContext->GetDomainIds();
        int t = ctxIds.first == s ? ctxIds.second : ctxIds.first;
        if(sample->GetSetById(t)->Size() == 0) { //only perform operation in context not already sampled
            vector<long double> *currWeights = GetFreqWeightsSub(currContext, s,t,sample);
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
              if(sample->GetSetById(t)->Size() == 0) { //only perform operation in context not already sampled
                  vector<long double> *currWeights = weightsMap[t];
                  cout<<"\ncur context of "<<ctxIds.first<<" and "<<ctxIds.second;
                  cout.flush();
                  IOSet *curr = SubspaceFreq(currContext,s,t, (*currWeights));
                  cout<<"\ngot the curr: "; curr->Output();cout<<"\nadn the itrsct: "; itrsct->Output();
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
        //now update the source by taking intersection with original
        IOSet *itrsct2 = Intersect(itrsct,sample->GetSetById(s));
        itrsct2->SetId(s);
        sample-> AssignSetById(s,itrsct2);
        delete itrsct;
        if(sample->GetSetById(s)->Size() > 0){
            for(int i=0; i < sContexts->size(); i++){
                      Context *currContext = (*sContexts)[i];
                      pair<int,int> ctxIds = currContext->GetDomainIds();
                      int t = ctxIds.first == s ? ctxIds.second : ctxIds.first;
                      if(sample->GetSetById(t)->Size() == 0){
                        IOSet *primeT = Prime(sample,g,s,t,1);
                        primeT->SetId(t);
                        sample->AssignSetById(t,primeT);
                      }
            }
            return sample;
        }
        delete sample;
        return NULL;
    }else{
        delete sample;
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
    NCluster *init = new NCluster;
    IOSet *domainIds = g->GetAllDomainIds();
    for(int i=0; i < domainIds->Size(); i++){
        init->AddSet(new IOSet);
        init->GetSet(i)->SetId(domainIds->At(i));
    }
    NCluster *init1 = SubspaceStarShapedFreq(g,s);
    
    //copy into init
    for(int i=0; i < init1->GetN(); i++){
        int currId = init1->GetSet(i)->Id();
        init->GetSetById( currId)->DeepCopy(init1->GetSet(i));
    }
    delete init1;
    //do a BFS using SubspaceStarShapedFreqSample
    //insert all neighboring articulation nodes of s
    IOSet *artDomains = g->GetArtDomains();
    queue<int> q;
     cout<<"\nthe init1\n"; init->Output();
    cout.flush();
    IOSet *sNeighbors = g->GetNeighbors(s);
    for(int i=0; i < sNeighbors->Size(); i++){
        if(artDomains->Contains(sNeighbors->At(i)))
            q.push(sNeighbors->At(i));
    }
    cout<<"\nq size: "<<q.size();
    delete sNeighbors;
    while (!q.empty()){
        int s1 = q.front();
        q.pop();
        init = SubspaceStarShapedFreqSample(g,s1,init);
        cout<<"\n init after first step with s1 "<<s1<<"\n";
        init->Output();
        if(init == NULL) return NULL;
        IOSet *sNeighbors = g->GetNeighbors(s);
        for(int i=0; i < sNeighbors->Size(); i++){
            int id = sNeighbors->At(i);
            if(artDomains->Contains(sNeighbors->At(i)) && init->GetSetById(id)->Size() == 0)
                q.push(sNeighbors->At(i));
        }
        delete sNeighbors;
     }
    return init;
}