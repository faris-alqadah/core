




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
                int cantor = CantorPairing(k1,k2);//0.5*(k1+k2)*(k1+k2+1)+k2;
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

void OutputEdges( map<int, pair<int,int> > &theMap, ofstream &out,int id1, int id2, vector<NameMap*>& nm){
    //first find name correct name maps
    NameMap *nm1=NULL;
    NameMap *nm2 = NULL;
    for(int i=0; i < nm.size(); ++i){
        if(nm[i]->GetId() == id1 ){
            nm1 = nm[i];
        }else if(nm[i]->GetId() == id2){
            nm2 = nm[i];
        }
    }
    if(nm1 != NULL && nm2 != NULL){
        map<int,pair<int,int> >::iterator it;
        for ( it=theMap.begin() ; it != theMap.end(); it++ ){
           pair<int,int> currEdge=(*it).second;
            out <<nm1->GetName(currEdge.first) << " " <<nm2->GetName(currEdge.second)<< endl;
        }
    }else{
        out<<"\nName maps could not be found for specified domain ids "<<id1<<" and "<<id2;
    }
}

void OutputEdgesBinaryMatrix( map<int, pair<int,int> > &theMap, ofstream &out, int sz1, int sz2){
    for(int i=0; i < sz1; i++){
        for(int j=0; j < sz2; j++){
            int key = CantorPairing(i,j);
            if(j > 0)
                out<<" ";
            if(theMap.count(key)>0)
                out<<"1";
            else
                out<<"0";
        }
        if(i != sz1-1)
            out<<"\n";
    }
}