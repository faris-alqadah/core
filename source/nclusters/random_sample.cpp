#include "../../headers/nclusters/random_sample.h"


void NClusterRandomSample::NClusterFreq(RelationGraph *g){

}

NCluster* NClusterRandomSample::SubspaceFreq(Context *c, int s,int t){
    //weigh objects in domain t by
    vector<double> weights(c->GetNumSets(t));
    for (int i=0; i < c->GetNumSets(t); i++){
        double pwr=(double)c->GetSet(t,i)->Size()/10.0;
        weights[i] = pow(2,pwr); // weigh by size of power set
       //  cout<<"\npowerset weights 2 ^"<<pwr<<" is "<<weights[i];
    }
    //randomly draw object from t
    int randT =  WeightedUniformDraw(weights);
    //cout<<"\ntransaction selected: "<<randT;
    //uniformly select from power set of psi^t(randT)
    IOSet *ret = UniformSubsetDraw(c->GetSet(t,randT));
    NCluster *retN = new NCluster();
    ret->SetId(s);
    retN->AddSet(ret);

    return retN;

}
