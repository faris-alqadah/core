#include "../headers/LatticeAlgos.h"

void Star_N_Concepts(RelationGraph *g,int lrnrContext, int algo){
    //check all pre conditions
    //if they are met then call Enum_NConcepts
    string file1 = OUTFILE+".concepts";
    string file2 = OUTFILE+".concepts.names";
    OUT1.open(file1.c_str());
    OUT2.open(file2.c_str());
    IOSet *artDomains = g->GetArtDomains();

    if( artDomains->Size() != 1 && g->GetNumNodes() > 2) {
        string errMsg = "Star_N_Concepts must be called with a star shaped hin\n";
        cerr<<errMsg; exit(-1);
    }
    if( g->GetContext(lrnrContext) == NULL){
        string errMsg = "Star_N_Concepts called with invalid learner context id for the given hin\n";
        cerr<<errMsg; exit(-1);
    }
    if(enumerationMode == ENUM_FILE && !OUT1.is_open()){
         string errMsg = "Star_N_Concepts called with ENUM_FILE mode, however, OUTFILE is not valid file or has not been set\n";
        cerr<<errMsg; exit(-1);
    }else if(enumerationMode == ENUM_FILE || enumerationMode == ENUM_TOPK_FILE){
        //setup name maps
        NAME_MAPS = *g->GetNameMaps();
 
    }
    if(enumerationMode == ENUM_TOPK_FILE &&!OUT1.is_open()){
         string errMsg = "Star_N_Concepts called with ENUM_FILE mode, however, OUTFILE is not valid file or has not been set\n";
        cerr<<errMsg; exit(-1);
    }
    if( PRUNE_SIZE_VECTOR.size() < g->GetNumNodes()){
        string errMsg = "Star_N_Concepts called with size pruning, however, PRUNE_SIZE_VECTOR does not contain threshold values for all domains\n";
        cerr<<errMsg; exit(-1);
    }

    //check values of prune_size_vector are all >= 1,
    for(int i=0; i < g->GetNumNodes(); i++)
        if(PRUNE_SIZE_VECTOR[i] < 1){
            PRUNE_SIZE_VECTOR[i] = 1;
            cout<<"\nReset prune size vector at "<<i<<" to 1\n";
        }
    //reset variables
     srchLvl=0;
     numConcepts=0;


     //set the quality function pointer and the ovlp function pointer
     if(qualityMode == AREA){
        qualityFunction=&Area;
     }else if(qualityMode == BETA){
         qualityFunction=&Beta;
     }
     //set the overlap function pointer
     if(ovlpMode == AVG_JACCARD){
         ovlpFunction=&AverageOverlap;
     }

    //done check and setting genering variables call enumeration algorithm
    //for now only Bordat is implemented
    //compute the top level concept of lrnr context and call function
     IOSet *domainIds = g->GetAllDomainIds();
     if(g->GetNumNodes() == 2)
         artDomains->Add(domainIds->At(0));
     
     NCluster *strt = new NCluster(g->GetNumNodes());
     for(int i=0; i < strt->GetN(); i++) strt->GetSet(i)->SetId(domainIds->At(i));

     int artDomain = artDomains->At(0);
     Context *ctx = g->GetContext(lrnrContext);
     pair<int,int> lrnrDomainIds = ctx->GetDomainIds();
     int otherDomain;
     NCluster *strt1;
     if(lrnrDomainIds.first == artDomain){
         otherDomain = lrnrDomainIds.second;
         strt1 = GetTop(ctx);
         strt1->GetSet(0)->SetId(artDomain);
         strt1->GetSet(1)->SetId(otherDomain);

     }else{
         otherDomain = lrnrDomainIds.first;
         strt1 = GetBottom(ctx);
         strt1->GetSet(0)->SetId(otherDomain);
         strt1->GetSet(1)->SetId(artDomain);

     }
     cout<<"\nThe articuluation domain id is: "<<artDomain<<"\n";
     cout<<"\nThe learner contex is: "<<ctx->GetName();
     strt->AssignSetById(artDomain,strt1->GetSetById(artDomain));
     strt->AssignSetById(otherDomain,strt1->GetSetById(otherDomain));
     if(algo == BORDAT)
        Enum_NConcepts_Bordat(strt,g,new IOSet(),artDomain,otherDomain);
     else{
         cerr <<"\nValid algorithm for n-cluster enumeration not specified!\n";
         exit(-1);
     }
     //clena up now
     if(enumerationMode == ENUM_FILE){
         OUT1.close();
         OUT2.close();
     }
}

vector<NCluster *> * UpperNeighbors(NCluster *c, RelationGraph *g, int s, int t,int algo){
    assert( g->IsEdge(s,t) && c->ContainsIOSetId(s) && c->ContainsIOSetId(t));
    //check if the concept is a top or bottom concept, if so then return empty vector
    if( c->GetSetById(s)->Size() == 0 || c->GetSetById(t)->Size() == 0)
        return new vector<NCluster*> ;
    else if(algo == BORDAT)
        return UpperNeighbors_Bordat(c,g,s,t);
    else{
         cerr<<"\nInvalid algorithm of upper neighbor computation specified!\n";
         exit(-1);
    }
}


vector<NCluster *> * LowerNeighbors(NCluster *c, RelationGraph *g, int s, int t,int algo){
    assert( g->IsEdge(s,t) && c->ContainsIOSetId(s) && c->ContainsIOSetId(t));
    if( c->GetSetById(s)->Size() == 0 || c->GetSetById(t)->Size() == 0)
        return new vector<NCluster*> ;
    else if(algo == BORDAT)
        return LowerNeighbors_Bordat(c,g,s,t);
    else{
        cerr<<"\nInvalid algorithm of lower neighbor computation specified!\n";
        exit(-1);
    }
    
    
}

