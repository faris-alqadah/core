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




void Update_AllTails_Iterators_Star_Charm(list< list<IOSet*>* > &tails, list< list<IOSet*> *> &tailSupSet, list < list<NCluster*> *> &tailMinMax){
    //cout<<"\nUpdating tails...\n";
    list< list<IOSet*>* >::iterator outerTailIt = tails.begin();
    list< list<IOSet*>* >::iterator outerSupIt = tailSupSet.begin();
    list< list<NCluster*>* >::iterator outerMinMaxIt = tailMinMax.begin();
    for(int j=0; j < tails.size(); j++){
        // cout<<"\nj: "<<j<<"\t"<<(*outerTailIt)->size();
         if( (*outerTailIt)->size() > 0 ){
             if( (*outerTailIt)->front() != NULL)     delete (*outerTailIt)->front();
             if((*outerSupIt)->front() != NULL )      delete (*outerSupIt)->front();
             if((*outerMinMaxIt)->front() != NULL)    delete (*outerMinMaxIt)->front();
             RemoveFromList(*(*outerTailIt), (*outerTailIt)->begin());
             RemoveFromList(*(*outerSupIt), (*outerSupIt)->begin());
             RemoveFromList(*(*outerMinMaxIt), (*outerMinMaxIt)->begin());
         }
         outerTailIt++;
         outerSupIt++;
         outerMinMaxIt++;
    }
}

void Output_Tail(list<IOSet*> &tail){
    list<IOSet*>::iterator it = tail.begin();
    cout<<"\n~~~";
    while(it != tail.end()){
        cout<<"| ";
        (*it)->Output();
        it++;
    }
    cout<<"\n~~~"<<tail.size();
}


void Delete_New_Tails_Star_Charm(list< list<IOSet*>* > &newTails, list< list<IOSet*> *> &newSupSets, list < list<NCluster*> *> &newMinMaxs){
    list< list<IOSet*>* >::iterator outerTailIt = newTails.begin();
    list< list<IOSet*>* >::iterator outerSupIt = newSupSets.begin();
    list< list<NCluster*>* >::iterator outerMinMaxIt = newMinMaxs.begin();
    while(outerTailIt != newTails.end()){
        list<IOSet*>::iterator tailIt = (*outerTailIt)->begin();
        list<IOSet*>::iterator supIt = (*outerSupIt)->begin();
        list<NCluster*>::iterator minMaxIt = (*outerMinMaxIt)->begin();
        while(tailIt != (*outerTailIt)->end()){
            if (*tailIt != NULL) delete (*tailIt);
            if(*supIt != NULL) delete (*supIt);
            if(*minMaxIt !=NULL) delete (*minMaxIt);
            tailIt = RemoveFromList(*(*outerTailIt), tailIt);
            supIt = RemoveFromList(*(*outerSupIt), supIt);
            minMaxIt = RemoveFromList(*(*outerMinMaxIt), minMaxIt);
        }
        delete (*outerTailIt);
        delete (*outerSupIt);
        delete (*outerMinMaxIt);
        outerTailIt = RemoveFromList( newTails,outerTailIt);
        outerSupIt = RemoveFromList(newSupSets,outerSupIt);
        outerMinMaxIt = RemoveFromList(newMinMaxs,outerMinMaxIt);

    }
}

bool Check_Size_Bounds( list< list<IOSet*>* > &supSets, vector<int> &minSup){
     list< list<IOSet*>* >::iterator supIt = supSets.begin();
     int idd=2;
     while(supIt != supSets.end()){
         if( (*(*supIt)->begin() )->Size() < minSup[idd-1] )
             return false;
         supIt++;
         idd++;
     }
     return true;
}