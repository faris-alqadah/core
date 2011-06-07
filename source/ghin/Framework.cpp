#include "../../headers/ghin/Framework.h"


NCluster* Ghin::SelectInit(int domain, NCluster *sampleSet){
    int obj = SelectRandomObjectFromSet(sampleSet->GetSet(domain));
    if(obj == -1) //no more objects
        return NULL;
    NCluster *init = new NCluster(hin->GetNumNodes());
    //set the ids as linear
    for(int i=0; i < hin->GetNumNodes(); i++) init->GetSet(i)->SetId(i+1);
    //randomly select object from sampleSet of domain
    init->GetSetById(domain)->Add(obj);
    //form concept with this object from a random neighbor in the HIN
    IOSet *neighbors = hin->GetNeighbors(domain);
    int neighborDomain = neighbors->At( rand() % neighbors->Size());
    Context *ctx = hin->GetContext(domain,neighborDomain);
    IOSet *a =  Prime(init,hin,domain,neighborDomain,1);
    if ( a != NULL){
        init->GetSetById(neighborDomain)->DeepCopy(a);
        IOSet *b = Prime(init,hin,neighborDomain,domain,1);
        if (b != NULL){
            init->GetSetById(domain)->DeepCopy(b);
            NCluster *final = new NCluster(*init);
            delete a;
            delete b;
            UpdateSampleSet(sampleSet,init);
            delete init;
            return final;
        }else{
            delete a;
            //update the sample set
             UpdateSampleSet(sampleSet,init);
            delete init;
            return NULL;
        }
    }else{
        //update the sample set
         UpdateSampleSet(sampleSet,init);
        delete init;
        return NULL;
    }
}

NCluster* Ghin::MakeDeal(NCluster *candidate,double(*RewardFunc)(NCluster*,int,int)){
    int N = hin->GetNumNodes();
    int max_iters = 50;
    int cnt=0;
    bool change=true;
    bool add=true;
    vector<int> order(N);
    for(int i=0; i < N; i++) order[i] =i+1;
    //clear
    random_shuffle(order.begin(),order.end());
    int no_change_cnt=0;
    while (no_change_cnt < 2 && cnt < max_iters ){
        vector<IOSet*> changedSets(N+1);
        change=false;
        //randomize the order of deal making
        for(int i=0; i < N; i++){
            changedSets[order[i]] = new IOSet(candidate->GetSet(order[i]));
            bool currChange = MaximizeDomain(candidate,changedSets[order[i]],order[i],RewardFunc,add);
            change = change || currChange;
        }
        //destroy the vector
        for(int i=0; i < N; i++) {
            delete changedSets[i];
            changedSets[i] = NULL;
        }
        if(change) no_change_cnt=0;
        else no_change_cnt++;
        if(add && !change) add = false;  //cannot add anymore
        else if (add && change) add=true; //can maybe add some more
        else if (!add & !change) add = true; //cannot remove anymore
        else if (!add && change) add=false; //can maybe still remove move
        cnt++;
    }
    NCluster *ret = new NCluster(N);
    ret->DeepCopy(*candidate);
    if(no_change_cnt > 1 && cnt < max_iters){
        ret->SetQuality(1.0);
    }else{
        ret->SetQuality(0.0);
    }
    return ret;
}

bool Ghin::MaximizeDomain(NCluster *a, IOSet *c, int domain ,double(*RewardFunc)(NCluster*,int,int),bool add ){
    bool ret=false;
    if(!add){
        IOSet *removed = RemoveSet_Reward(a,domain,RewardFunc);
        if( removed != NULL){
            IOSet *diff = Difference(c,removed);
            a->GetSetById(domain)->DeepCopy(diff);
            delete removed;
            delete diff;
            ret = true;
        }
    }else{
        IOSet *add = AddSet_Reward(a,domain,RewardFunc);
        if ( add != NULL){
            IOSet *unin = Union(c,add);
            a->GetSetById(domain)->DeepCopy(unin);
            delete add;
            delete unin;
            ret = true;
        }
    }
    return ret;
}

////////////////////////////////Reward Funtions/////////////////////////////////

double Ghin::Simple_Weighted_Score(NCluster *a, int obj, int domain){

    IOSet *neighbors = hin->GetNeighbors(domain);
    double score = 0.0;
    for(int i=0; i < neighbors->Size(); i++){
        double B= a->GetSetById( neighbors->At(i))->Size() ;
        if(B > 0){
            Context *currContext = hin->GetContext(domain,neighbors->At(i));
            IOSet *tmp = Intersect(currContext->GetSet(domain,obj),a->GetSetById( neighbors->At(i) ));
            IOSet *tmp1 = Difference(a->GetSetById( neighbors->At(i) ),currContext->GetSet(domain,obj));
            double zeros =  tmp1->Size();
            double ones = tmp->Size();
            score += (ones - w*zeros) / B;
            delete tmp;
            delete tmp1;
        }
    }
    delete neighbors;
    return score;
}


void Ghin::InitTiring(){
    tired.resize(hin->GetNumNodes());
    for(int i=0; i < hin->GetNumNodes(); i++){
               tired[i].resize(hin->NumObjsInDomain(i+1));
               fill(tired[i].begin(),tired[i].end(),1);
      }
}

void Ghin::UpdateTired(NCluster *c){
    for(int i=0; i < hin->GetNumNodes(); i++){
        for(int j=0; j < c->GetSet(i)->Size(); j++)
            tired[i][c->GetSet(i)->At(j)]++;
    }
}





double Ghin::Exp_Sat_Score(NCluster *a, int obj, int domain){

    IOSet *neighbors = hin->GetNeighbors(domain);
    double score = 0.0;
    for(int i=0; i < neighbors->Size(); i++){
        double B= a->GetSetById( neighbors->At(i))->Size() ;
        if(B > 0){
            Context *currContext = hin->GetContext(domain,neighbors->At(i));
            IOSet *tmp = Intersect(currContext->GetSet(domain,obj),a->GetSetById( neighbors->At(i) ));
            int n = a->GetSetById( neighbors->At(i) )->Size();
            int m = currContext->GetSet(domain,obj)->Size();
            int NN =currContext->GetNumSets(neighbors->At(i));
            double expected = Hypgeo_Mean(n,m,NN);
            double std = Hypgeo_Dev(n,m,NN);
            double ones = tmp->Size();
            double tfactor = 1.0/pow(tired[domain][obj],1.5);
            double zscore = Z_Score(ones,expected,std);
            double z = tfactor*zscore-w;
            score += z;
            delete tmp;
        }
    }
    delete neighbors;
    return score;
}



IOSet *Ghin::AddSet_Reward(NCluster *a, int domain, double(*RewardFunc)(NCluster*,int,int)){
    IOSet *all = hin->GetLabels(domain);
    IOSet *test1 = Difference(all,a->GetSetById(domain));
    delete all;
    IOSet *add = new IOSet;
    for(int i=0; i < test1->Size(); i++){
        if(RewardFunc(a,test1->At(i),domain) > 0)
            add->Add(test1->At(i));
    }
    delete test1;
    if (add->Size() > 0) return add;
    else{
        delete add;
        return NULL;
    }
}

IOSet *Ghin::RemoveSet_Reward(NCluster *a, int domain, double(*RewardFunc)(NCluster*,int,int)){
    IOSet *remove = new IOSet;
    for(int i=0; i < a->GetSet(domain)->Size(); i++){
        if(RewardFunc(a,a->GetSet(domain)->At(i),domain) < 0)
            remove->Add(a->GetSet(domain)->At(i));
    }
    if (remove->Size() > 0) return remove;
    else{
        delete remove;
        return NULL;
    }

}

void Ghin::Compute_Score(NCluster *a,double(*RewardFunc)(NCluster*,int,int) ){

    for(int i=0; i < hin->GetNumNodes(); i++){
        double quality_cnt=0;
        for(int j=0; j < a->GetSet(i)->Size(); j++){
            double s=RewardFunc(a,a->GetSet(i)->At(j),i+1);
           // a->GetSet(i)->SetScore(j,s);
            quality_cnt += s;
        }
        a->GetSet(i)->SetQuality(quality_cnt);
    }
}

void  Ghin::GHIN_Alg(){
    srand ( time(NULL) );
    //1. mark domains (optional)
    //2. Set selection set to everything
    NCluster *selection = MakeInitialSampleSet();
    NCluster *clustered = new NCluster(hin->GetNumNodes());
    //3. Iteratte until selection is not empty
    int numIters = 0;
    while( !SelectEmpty(selection)){
         //1. select random domain
        int strtDomain = SelectRandomDomain();
        //2. Create initial cluster
        NCluster *init = SelectInit(strtDomain,selection);
        if(init != NULL){
             NCluster *initCopy = new NCluster(*init);
            //3. Attempt to make deal
            NCluster *result = MakeDeal(init,RewardFunc);

            bool foundCluster=false;
            if (result->GetQuality() == 1 && !CheckRepeat(result)){
                Compute_Score(result,RewardFunc);
                if(dispProgress){
                    cout<<"\nfinal selection \n"; result->Output();

                }
                foundCluster=true;
                 CONCEPTS.push_back(result);
                if(tiredMode){
                    UpdateTired(result);
                }
                UpdateSampleSet(selection,init,clustered);
                UpdateSampleSet(selection,result,clustered);
            }
            //4. Update selection
            delete initCopy;
            if (!foundCluster) delete result;
            delete init;
        }
        if (dispProgress && (numIters % 100 == 0)) {
                 cout<<"\nProgress...";
                 for(int i=0; i < hin->GetNumNodes(); i++)
                        cout<<"\n"<<selection->GetSet(i)->Size()<<" of "<<hin->NumObjsInDomain(i+1);
                 cout<<"\nGot "<<CONCEPTS.size()<<" clusters...\nnum_iters: "<<numIters;

       }
       numIters++;
    }
}

///////////////////HELPER FUNCTIONS/////////////////////////////////////////////
int Ghin::SelectRandomDomain(){

   return (rand() % hin->GetNumNodes())+1;
}

bool Ghin::SelectEmpty(NCluster* select) {
    int emptyCnt=0;
    for (int i = 0; i < hin->GetNumNodes(); i++){
        if (select->GetSet(i)->Size()  < 1)
            emptyCnt++;
    }
   if (emptyCnt >= hin->GetNumNodes())
    return true;
  else
    return false;

}
int Ghin::SelectRandomObjectFromSet(IOSet *a){
     if (a->Size() == 1)
         return a->At(0);
     else if (a->Size() == 0)
         return -1;
    return a->At(rand() % a->Size());
}

bool Ghin::CheckRepeat(NCluster *a){
    if(CONCEPTS.size() == 0) return false;
    else{
        for(int i=0; i < CONCEPTS.size(); i++){
            for(int j=0; j < hin->GetNumNodes(); j++){
                if ( a->GetSet(j)->Equal(*CONCEPTS[i]->GetSet(j)) || a->GetSet(j)->Size() < 1)
                    return true;

            }
        }
        return false;

    }
}

NCluster* Ghin::MakeInitialSampleSet(){
    NCluster *init = new NCluster(hin->GetNumNodes());
    for(int i=0; i < hin->GetNumNodes(); i++)
        for(int j=0; j < hin->NumObjsInDomain(i+1); j++)
            init->GetSet(i)->Add(j);
     return init;
}



void Ghin::UpdateSampleSet(NCluster *selection, NCluster *currCluster, NCluster *clustered){
    for(int i=0; i < hin->GetNumNodes(); i++){
        IOSet *n = Difference(selection->GetSet(i),currCluster->GetSet(i));
        IOSet *u = Union(currCluster->GetSet(i),clustered->GetSet(i));
        clustered->GetSet(i)->DeepCopy(u);
        selection->GetSet(i)->DeepCopy(n);
        delete u;
        delete n;
    }

}