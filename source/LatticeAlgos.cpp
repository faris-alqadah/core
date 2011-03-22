#include "../headers/LatticeAlgos.h"

void Star_N_Concepts(RelationGraph *g,int lrnrContext){
    //check all pre conditions
    //if they are met then call Enum_NConcepts
    IOSet *artDomains = g->GetArtDomains();
    if( artDomains->Size() != 1) {
        string errMsg = "Star_N_Concepts must be called with a star shaped hin";
        cerr<<errMsg; exit(-1);
    }
    if( g->GetContext(lrnrContext) == NULL){
        string errMsg = "Star_N_Concepts called with invalid learner context id for the given hin";
        cerr<<errMsg; exit(-1);
    }
    if(enumerationMode == ENUM_FILE && !OUTFILE.is_open()){
         string errMsg = "Star_N_Concepts called with ENUM_FILE mode, however, OUTFILE is not valid file or has not been set";
        cerr<<errMsg; exit(-1);
    }
    if( PRUNE_SIZE_VECTOR.size() < g->GetNumNodes()){
        string errMsg = "Star_N_Concepts called with size pruning, however, PRUNE_SIZE_VECTOR does not contain threshold values for all domains";
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
    //done check and setting genering variables call enumeration algorithm
    //for now only Bordat is implemented
    //compute the top level concept of lrnr context and call function
     IOSet *domainIds = g->GetAllDomainIds();
     NCluster *strt = new NCluster(g->GetNumNodes());
     for(int i=0; i < strt->GetN(); i++) strt->GetSet(i)->SetId(domainIds->At(i));

     int artDomain = artDomains->At(0);
     Context *ctx = g->GetContext(lrnrContext);
     pair<int,int> lrnrDomainIds = ctx->GetDomainIds();
     int otherDomain;
     NCluster *strt1;
     if(lrnrDomainIds.first == artDomain){
         otherDomain = lrnrDomainIds.second;
         strt1 = GetBottom(ctx);
         strt1->GetSet(0)->SetId(artDomain);
         strt1->GetSet(1)->SetId(otherDomain);

     }else{
         otherDomain = lrnrDomainIds.first;
         strt1 = GetTop(ctx);
         strt1->GetSet(0)->SetId(otherDomain);
         strt1->GetSet(1)->SetId(artDomain);
     }
     strt->AssignSetById(artDomain,strt1->GetSetById(artDomain));
     strt->AssignSetById(otherDomain,strt1->GetSetById(otherDomain));
     Enum_NConcepts_Bordat(strt,g,artDomain,otherDomain);
}

