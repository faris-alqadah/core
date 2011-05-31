#include "../../headers/alpha_concepts/helpers.h"




void Construct_AlphaSigma_Params(RContext *K,IOSet* query, int s,int t, int tRow, vector<double> &params){
    assert(params.size() == 1 );
    pair<int,int> dIds = K->GetDomainIds();
    assert( (s == dIds.first && t == dIds.second) || (s == dIds.second && t == dIds.first));
    params.push_back(K->GetStdDev(t,tRow)); //std dev of full row/column <b>M</b>
         
}


void Construct_MaxSpaceUniform_Params(RContext* K, IOSet* query, int s, int t, int tRow, vector<double> &params){
    assert(params.size()==1);
    pair<int,int> dIds = K->GetDomainIds();
    assert(s == dIds.first || s == dIds.second);
    params.push_back((double)K->GetNumSets(t)); //size of <b>M</b>
    vector<double> rangeParams;
    RSet *sset = K->GetSet(t,tRow);
    double rr = Range(sset,rangeParams);
    params.push_back(rr); //range of full row/column <b>M</b>
    params.push_back(query->GetQuality());
}

IOSet *Naive_Range_Intersect(IOSet *supSet1, IOSet *supSet2){

}

void Create_AllTails_Iterators_Star_Charm(list< list<IOSet*>* > &tails, list< list<IOSet*> *> &tailSupSet, list < list<NCluster*> *> &tailMinMax, IOSet *tailRun,
                                vector< list< list<IOSet*> *>::iterator > &tailIts,
                                vector< list< list<IOSet*> *>::iterator > &supIts,
                                vector< list< list<NCluster*> *>::iterator > &minMaxIts){

    list< list<IOSet*>* >::iterator outerTailIt = tails.begin();
    list< list<IOSet*>* >::iterator outerSupIt = tailSupSet.begin();
    list< list<NCluster*>* >::iterator outerMinMaxIt = tailMinMax.begin();
    for(int i=0; i < tails.size(); i++){
        if(tailRun->Contains(i)){
            tailIts.push_back( (*outerTailIt)->begin());
            supIts.push_back( (*outerSupIt)->begin());
            minMaxIts.push_back( (*outerMinMaxIt)->begin());
        }
        outerTailIt++;
        outerSupIt++;
        outerMinMaxIt++;
    }

}

void Update_AllTails_Iterators_Star_Charm(list< list<IOSet*>* > &tails, list< list<IOSet*> *> &tailSupSet, list < list<NCluster*> *> &tailMinMax, IOSet *tailRun,
                                vector< list< list<IOSet*> *>::iterator > &tailIts,
                                vector< list< list<IOSet*> *>::iterator > &supIts,
                                vector< list< list<NCluster*> *>::iterator > &minMaxIts){

    list< list<IOSet*>* >::iterator outerTailIt = tails.begin();
    list< list<IOSet*>* >::iterator outerSupIt = tailSupSet.begin();
    list< list<NCluster*>* >::iterator outerMinMaxIt = tailMinMax.begin();
    int i=0;
    for(int j=0; j < tails.size(); j++){
           if(tailRun->Contains(j)){
            delete (*tailIts[i]);
            delete (*supIts[i]);
            delete (*minMaxIts[i]);
            tailIts[i] = RemoveFromList((*outerTailIt), tailIts[i]);
            supIts[i] = RemoveFromList((*outerSupIt), supIts[i]);
            minMaxIts[i] = RemoveFromList((*outerMinMaxIt), minMaxIts[i]);
            i++;
           }
        outerTailIt++;
        outerSupIt++;
        outerMinMaxIt++;
    }

}



bool Done_Star_Charm(list< list<IOSet*>* > &tails,IOSet *tailRun,
                    vector< list< list<IOSet*> *>::iterator > &tailIts){

     list< list<IOSet*>* >::iterator outerTailIt = tails.begin();
    for(int i=0; i < tails.size(); i++){
        if(tailRun->Contains(i)){
            if(tailIts[i] == (*outerTailIt)->end())
                return true;
        }
        outerTailIt++;
    }
    return false;
}