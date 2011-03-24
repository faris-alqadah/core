#include "../headers/LatticeAlgos.h"

void Star_N_Concepts(RelationGraph *g,int lrnrContext){
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
    }else if(enumerationMode == ENUM_FILE){
        //setup name maps
        NAME_MAPS = *g->GetNameMaps();
        cout<<"\nNum entries "<<NAME_MAPS[0]->GetNumEntries();
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
         cout<<"\nGot top...";
         strt1 = GetTop(ctx);
         strt1->GetSet(0)->SetId(artDomain);
         strt1->GetSet(1)->SetId(otherDomain);

     }else{
         cout<<"\nGot bottom...";
         otherDomain = lrnrDomainIds.first;
         strt1 = GetBottom(ctx);
         strt1->GetSet(0)->SetId(otherDomain);
         strt1->GetSet(1)->SetId(artDomain);

     }
     strt->AssignSetById(artDomain,strt1->GetSetById(artDomain));
     strt->AssignSetById(otherDomain,strt1->GetSetById(otherDomain));

     cout<<"\nInitial concept: \n";
     strt->Output();
     Enum_NConcepts_Bordat(strt,g,new IOSet(),artDomain,otherDomain);
}

