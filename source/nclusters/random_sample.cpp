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
    for(int i=0; i < weights->size(); i++) {

        //cout<<"\nweights i "<<i<<" as "<<(*weights)[i]<<" /  "<<sum<<" got: ";
         (*weights)[i] /= sum;
         //cout<<(*weights)[i];
    }
    
    return weights;
}

vector<long double> * NClusterRandomSample::GetAreaWeights(Context *c, int s, int t){
    vector< long double> *weights = new vector<long double>(c->GetNumSets(t));
    for (int i=0; i < c->GetNumSets(t); i++){
        double pwr=(long double)c->GetSet(t,i)->Size();///10.0;
        (*weights)[i] = pwr*pow(2,pwr-1); // weigh by size of power set
    }

    return weights;
}

NCluster* NClusterRandomSample::SubspaceFreq(Context *c, int s,int t, vector<long double> &weights){
    //randomly draw object from t
   // cout<<"\nweights size: "<<weights.size();
    int randT =  WeightedUniformDraw(weights);
   //cout<<"\ntransaction selected: "<<randT;
    //uniformly select from power set of psi^t(randT)
   // cout<<"\t size: "<<c->GetSet(t,randT)->Size();
    IOSet *ret = UniformSubsetDraw(c->GetSet(t,randT));
    NCluster *retN = new NCluster();
    ret->SetId(s);
    //cout<<"\nsubset selected: "; ret->Output();
    retN->AddSet(ret);
    return retN;

}
NCluster* NClusterRandomSample::SubspaceArea(Context *c, int s, int t, vector<long double> &weights){
    int randT =  WeightedUniformDraw(weights);
    //cout<<"\ntransaction selected: "<<randT;
    //uniformly select from power set of psi^t(randT)
    IOSet *ret =  BinomialSubsetDraw(c->GetSet(t,randT));
    NCluster *retN = new NCluster();
    ret->SetId(s);
    retN->AddSet(ret);
    return retN;
}

NCluster* NClusterRandomSample::SubspaceStarShaped(RelationGraph *g, int s ){
     //first
    IOSet *itrsct = new IOSet;
    vector<Context*> * = g->GetContexts(s);
    do{
        
    }while(itrsct->Size() <= 0);
}