/*!
 Driver program for qbbc clustering (query based bi-clustering)


 */

#include <stdlib.h>

#include "../../headers/core/Timing.h"
#include "../../headers/core/PreProcess.h"
#include "../../headers/alpha_concepts/basic_prefix_alpha.h"
#include "../../headers/core/RLatticeOps.h"
#include "../../headers/alpha_concepts/helpers.h"


string inputFile="~";
string queryFile="~";
int numArgs=5;
BasicPrefix la;
IOSet *query;
int queryDomain=-1;
int otherDomain=-1;
void(*paramFunction)(RContext *,IOSet *, int, int,int, vector<double> &);

void DisplayUsage(){
    cout<<"\nUSAGE: test -i input_file "
        <<"\nREQUIRED: "
        <<"\n-i <inputFile>"
        <<"\n-q <domain_id> <queryFile>"
        <<"\nOPTIONAL: "
        <<"\n-alpha <alpha value> (default is 1.0)"
        <<"\n-c <consistency function> 1- alpha_sigma 2- max_space_uniform (default is 1)"
        <<"\n\n";
    exit(1);
}

void CheckArguments(){
    if (inputFile == "~"){
        cerr<<"\nINPUT FILE NOT ENTERED!";
        DisplayUsage();
    }
    if(queryFile == "~"){
        cerr<<"\nQUERY FILE NOT ENTERED!";
        DisplayUsage();
    }
    if(queryDomain == -1){
        cerr<<"\nQUERY DOMAIN NOT ENTERED!";
        DisplayUsage();
    }
    if(la.consistencyMode < 1 || la.consistencyMode > 2){
        cerr<<"\nINVALID CONSISTENCY MODE ENTERED";
        DisplayUsage();
    }else{
        if (la.consistencyMode == la.ALPHA_SIGMA){
            la.consistencyFunction=&AlphaSigma;
            la.paramFunction =&Construct_AlphaSigma_Params;
        }
        else if(la.consistencyMode == la.MAX_SPACE_UNIFORM){
            la.consistencyFunction=&AlphaSigma;
            la.paramFunction = &Construct_MaxSpaceUniform_Params;
        }
    }
    la.dispersionFunction=&Range;
    cout<<"\ninput file: "<<inputFile<<"\nquery file: "<<queryFile<<"\nalpha: "<<la.alpha<<"\nconsistency mode: "<<la.consistencyMode;
    cout<<"\n"<<endl;
}


void ProcessCmndLine(int argc, char ** argv){

    if (argc < numArgs+1) DisplayUsage();
     else{
        for(int i=1; i < argc; i++){
           string temp = argv[i];
           if(temp == "-i")//input file
                inputFile = argv[++i];
           if(temp == "-alpha")
               la.alpha = atof(argv[++i]);
           if(temp == "-q"){
               queryDomain=atoi(argv[++i]);
               queryFile=argv[++i];
           }
           if(temp == "-c"){
               la.consistencyMode = atoi(argv[++i]);
           }
        }
    }
    CheckArguments();
}
void ReadQueryFile(){
    pair<int,int> dIds = la.K->GetDomainIds();
    if(queryDomain == dIds.first){
        otherDomain = dIds.second;
    }else if (queryDomain == dIds.second){
        otherDomain = dIds.first;
    }else{
        cerr<<"\nInvalid query domain entered!!\n";
        exit(-1);
    }
    
    query = new IOSet;
    ifstream myfile(queryFile.c_str());
    NameMap *nm = la.K->GetNameMap(queryDomain);
    if (myfile.is_open()){
        string line;
        while (!myfile.eof()){
            getline(myfile, line);
            if(myfile.eof()) break;
            int curr = nm->NameToId(line);
            if( curr != -1)
                query->Add(curr);
        }
        query->Sort();
        if (query->Size() < 2){
            cerr<<"\nNot enough valid query terms!!\n";
            exit(-1);
        }
    }else{
        cerr<<"\nCould not open query file!\n";
        exit(-1);
    }
    
}

void OutputStats(){
    ofstream outStat("stats");
    outStat.close();

}

int main(int argc, char** argv) {
    ProcessCmndLine(argc,argv);
    la.K = MakeSingleRContext(inputFile);
    la.K->ComputeStdDevs();
    ReadQueryFile();
    la.K->PrintBasicStats();
    //this may change later
    la.s = queryDomain;
    la.t=otherDomain;
    cout<<"\nGot query with "<<query->Size()<<" objects...."
        <<"\nTesting for exact hit.....\n";
    //IOSet *initRslt = Prime_Alpha_Naive(la.K,query,la.s,la.t,la.alpha, la.dispersionFunction,la.consistencyFunction,la.paramFunction);
    //if(initRslt->Size() > 0 ){
   //     cout<<"\nGOT DIRECT HIT!!\n";
   //     initRslt->Output();
   // }else{
       // cout<<"\nNO direct hit..."
         //   <<"\nUsing prefix tree....\n";
    vector<NCluster*> hits;
        la.Qbbc(query,hits);
    //}
    cout<<"\n";
    return (EXIT_SUCCESS);
}


