
#include "../../headers/algos_helpers/basic.h"

void StoreCluster(vector<NCluster*> &v, NCluster *c){
    v.push_back(c);
}

void SwapDelete(vector<NCluster*> &v, NCluster *c, int i){
    assert( i >= 0 && i < v.size());
    NCluster *tmp = v[i];
    v[i] = c;
    if(tmp) {
        delete tmp;
        tmp = NULL;
    }

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