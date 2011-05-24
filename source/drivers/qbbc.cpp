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
string outFile="~";
int numArgs=5;
BasicPrefix la;
IOSet *query;
int queryDomain=-1;
int otherDomain=-1;
void(*paramFunction)(RContext *,IOSet *, int, int,int, vector<double> &);
double queryQuality;

void DisplayUsage(){
    cout<<"\nUSAGE: test -i input_file "
        <<"\nREQUIRED: "
        <<"\n-i <inputFile>"
        <<"\n-q <domain_id> <queryFile>"
        <<"\nOPTIONAL: "
        <<"\n-alpha <alpha value> (default is 1.0)"
        <<"\n-c <consistency function> 1- alpha_sigma 2- max_space_uniform (default is 1)"
        <<"\n-o <output file path>"
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
    if(outFile != "~"){
        cout<<"\nOutput file: "<<outFile;
    }
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
           if(temp == "-o"){
               outFile=argv[++i];
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
double GetQueryQuality(IOSet *query){
    //make the ncluster
    NCluster *queryCluster = new NCluster;
    queryCluster->AddSet(query);
    queryCluster->AddSet(la.K->GetLabels(la.t));
    queryCluster->GetSet(0)->SetId(la.s);
    queryCluster->GetSet(1)->SetId(la.t);
    queryQuality = Mean_Square_Error(queryCluster ,la.K, la.s, la.t);
}
void OutputFile(vector<NCluster*> &hits){
    //output each hit to a file
    for(int i=0; i < hits.size();i++){
         std::stringstream ss;
         ss << i;
         string fileName1 = outFile+"."+ss.str()+".idxs";
         string fileName2 = outFile+"."+ss.str()+".names";
         ofstream outF1(fileName1.c_str());
         ofstream outF2(fileName2.c_str());
         //hits[i]->SetQuality(Std_Across(hits[i] ,la.K, la.s, la.t)/Std_Within(hits[i] ,la.K, la.s, la.t));
         hits[i]->SetQuality(Mean_Square_Error(hits[i] ,la.K, la.s, la.t));
         hits[i]->Output(outF1);
         outF1<<"\n"<<queryQuality<<"\n"<<hits[i]->GetQuality();
         hits[i]->GetSetById(la.s)->Output(outF2,la.K->GetNameMap(la.s));
         outF1.close();
         outF2.close();
    }
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
    vector<NCluster*> hits;
    GetQueryQuality(query);
    la.Qbbc(query,hits);

    if(outFile != "~")
        OutputFile(hits);
    cout<<"\n";
    return (EXIT_SUCCESS);
}


