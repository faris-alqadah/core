#include "../../headers/synthetic/synthetic_hin.h"

void MakeSynContext(int cardA,int cardB, string &fileName){
    //generate gamma paramater
    double gamma = ((double) rand() / (double) RAND_MAX)+2.0;
    gamma=2.0;
    cout<<"\ngamma: "<<gamma;
    //assign scale-free network distributions to the node degrees of A and B
    vector<double> aDistb;
    double normalizing = ScaleFreeDist(cardA,gamma,aDistb);
    //compute b distribution using normalizing factor and gamma
    vector<double> bDistb;
    ScaleFreeDist(cardB,normalizing,gamma,bDistb);
    //assign degrees to to b using the distribution
    vector<int> bDegrees;
    AssignDegrees(cardB,bDistb,bDegrees);
    //compute probablity of seeing b_i in a row using the bDegrees count
    vector<double> bProbs(cardB);
    double sumBDegrees=0;
    for(int i=0; i < bDegrees.size(); i++) sumBDegrees += bDegrees[i];
    for(int i=0; i < bDegrees.size(); i++) bProbs[i ]= (double) bDegrees[i]/ (double) sumBDegrees;
    //now generate the context by first sampling degree of a_i, then randomly selecting the nodes from B
    //according to bProbs
    for(int i=0; i < cardA; i++){
        int deg = WeightedUniformDraw(aDistb)+1;
        IOSet *iA = new IOSet;
        for(int j=0; j < deg; j++) iA->Add( WeightedUniformDraw(bProbs));
        iA->Sort();
        cout<<"\n"<<i+1<<"\t"; iA->Output();
    }


}

  

void ScaleFreeDist(int maxK,double c,double gamma, vector<double> & distb){
    for(int i=0; i < maxK; i++){
        distb.push_back( pow(i+1,-1*gamma));
    }
    for(int i=0; i < maxK; i++) distb[i] /= c;
}
double ScaleFreeDist(int maxK,double gamma, vector<double> & distb){
    double c=0;
    for(int i=0; i < maxK; i++){
        distb.push_back( pow(i+1,-1*gamma));
        c += distb[i];
    }
    for(int i=0; i < maxK; i++) distb[i] /= c;
    return c;
}

void AssignDegrees(int numNodes, vector<double> &distb, vector<int> &degrees){
    for(int i=0; i < numNodes; i++) degrees.push_back(WeightedUniformDraw(distb)+1);
}
