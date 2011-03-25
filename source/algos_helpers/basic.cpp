
#include "../../headers/algos_helpers/basic.h"

void StoreCluster(vector<NCluster*> &v, NCluster *c){
    v.push_back(new NCluster(*c));
}

void SwapDelete(vector<NCluster*> &v, NCluster *c, int i){
    assert( i >= 0 && i < v.size());
    delete v[i];
    v[i]= new NCluster(*c);

    
}

void SetQuality(NCluster *c, vector<double> & params, double (*Quality)(NCluster*,vector<double>&) ){
    c->SetQuality(Quality(c,params));
}

void DispProgress(int ctr,int total){
    cout<<"\n"<<ctr<<" of "<<total;
}


void OutputCluster(NCluster *c, ofstream &out){
    c->Output(out);
    out<<";;;\n";
    out.flush();
}
void OutputCluster(NCluster *c, ofstream &out, vector<NameMap*> &nms){
    c->Output(out,nms);
    out<<";;;\n";
    out.flush();
}