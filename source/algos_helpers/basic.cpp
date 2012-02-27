




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

void AddBiCluster_Edges(NCluster *c,int id1, int id2, map<int, pair<int,int> > &theMap){
    IOSet *a = c->GetSetById(id1);
    IOSet *b = c->GetSetById(id2);
    if(a != NULL && b != NULL){
        for(int i=0; i < a->Size(); i++){
            int k1 = a->At(i);
            for(int j=0; j < b->Size(); j++){
                //make the cantor number
                pair<int,int> thePair;
                thePair.first=k1;
                thePair.second=b->At(j);
                int k2=thePair.second;
                int cantor = 0.5*(k1+k2)*(k1+k2+1)+k2;
                if( theMap.count(cantor) < 1){
                    pair<int, pair<int,int> > insrtPair;
                    insrtPair.first=cantor;
                    insrtPair.second = thePair;
                    theMap.insert(insrtPair);
                }
            }
        }
    }
}