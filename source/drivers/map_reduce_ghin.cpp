/*!
 Author: Faris Alqadah

 Driver program for map_reduce_GHIN
 */



#include <stdlib.h>
#include<cfloat>
#include "../../headers/core/PreProcess.h"
#include "../../headers/core/LatticeOps.h"
#include "../../headers/core/Timing.h"

#include "../../headers/ghin/Framework.h"
#include "../../headers/nclusters/random_sample.h"

int numArgs=6;

float enumConceptsTime;
float simMatTime;

//! framework object to implement algorithm
Ghin framework;
string inputFile="~";
NClusterRandomSample sampler;

using namespace std;


void DisplayUsage(){
    cout<<"\nUSAGE: ./bin "
        <<"\nREQUIRED: "
        <<"\n-i <inputFile>"
        <<"\n-w <weight of zeros>"
        <<"\n-reward 1- simple weighted 2- Hypogeometric expected satisfaction"
        <<"\nOPTIONAL (use in this order):  "
        <<"\n-tiring (use tiring party goers mode)"
        <<"\n-prog display progress"
        <<"\n-o <filename> output final clusters"
        <<"\n\tthese will be output to filename.clusters and filenames.clusters.names"
        <<"\n\n";
    exit(1);
}

void CheckArguments(){
    //first check for errors
    if (inputFile == "~"){
        cerr<<"\nINPUT FILE NOT ENTERED!";
        DisplayUsage();
    }
    if(framework.w == -1){
        cout<<"\nw value is not set!";
        DisplayUsage();
    }
    if( framework.rewardMode < 1 || framework.rewardMode >= 3){
        cout<<"\nInvalid reward mode!";
        DisplayUsage();
    }
    //now check operation modes
    if(framework.enumerationMode == framework.ENUM_FILE && framework.OUTFILE != "~"){
        cout<<"\nOutput clusters option enabled: "<<framework.OUTFILE;
        string file1 = framework.OUTFILE+".concepts";
        string file2 = framework.OUTFILE+".concepts.names";
        framework.OUT1.open(file1.c_str());
        framework.OUT2.open(file2.c_str());
    }
    else if( framework.enumerationMode == framework.ENUM_FILE && framework.OUTFILE == "~"){
        cout<<"\nOutput option enabled but file not specified!";
        DisplayUsage();
    }
    if(framework.dispProgress)
        cout<<"\nDisplay progress option enabled";

    if (framework.rewardMode == framework.SIMPLE_WEIGHTED){
         cout<<"\nReward function is: SIMPLE WEIGHTED with w= "<<framework.w;
         framework.RewardFunc = &Simple_Weighted_Score;

     }else if(framework.rewardMode == framework.EXPECTED_HYPGEO_SAT){
             cout<<"\nReward function is: HYPO-GEOMETRIC EXPECTED SATISFACTION with w= "<<framework.w;
              framework.RewardFunc = &Exp_Sat_HypGeo_Score;
     }
     if(framework.tiredMode == true)
         cout<<"\nTiring mode enabled";



}


void ProcessCmndLine(int argc, char ** argv){
    if (argc < numArgs+1) DisplayUsage();
     else{
        for(int i=1; i < argc; i++){
           string temp = argv[i];
           if(temp == "-i")//input file
                inputFile = argv[++i];
           else if(temp == "-w"){
              framework.w = atof(argv[++i]);
           }
           else if(temp == "-prog"){
               framework.dispProgress=true;
           }
           else if(temp == "-o"){
               framework.enumerationMode = framework.ENUM_FILE;
               framework.OUTFILE= argv[++i];
           }
           else if(temp == "-tiring"){
               framework.tiredMode=true;
           }
           else if(temp =="-reward"){
               framework.rewardMode=atoi(argv[++i]);
           }
        }
    }
    CheckArguments();
}
void OutputClustersFile(){
    int lim = framework.CONCEPTS.size() > framework.topKK ? framework.topKK:framework.CONCEPTS.size();
    for(int i=0; i < lim; i++){
        OutputCluster(framework.CONCEPTS[i], framework.OUT1);
        OutputCluster(framework.CONCEPTS[i],framework.OUT2,framework.NAME_MAPS);
    }
    framework.OUT1.close();
    framework.OUT2.close();
}
void OutputStats(){
    ofstream outStat("stats");
    outStat<<"\nAlgorithm execution time: "<<ComputeTime()
           <<"\nTotal iterations : "<<framework.totalIters
           <<"\nTotal candidates for nash: "<<framework.totalCands
           <<"\nAverage iterations to find nash or fail "<<framework.avgNashIters/framework.totalCands
           <<"\n# set intersections "<<numIntersection
           <<"\n# subset checks "<<numSubset
           <<"\n# set unions "<<numUnion
           <<"\n# set differences "<<numDifference;
    outStat.close();
}
int main(int argc, char** argv) {
    ProcessCmndLine(argc,argv);
    framework.hin = MakeRelationGraph(inputFile);
    framework.NAME_MAPS = *framework.hin->GetNameMaps();
    framework.hin->Print();
  //  cout<<"\nLongest set in context is : "<<framework.hin->GetContext(1)->GetLongestSet();
    srand(time(NULL));
   //  cout<<"\nRandomly sampling (freq) from context 1, domain 1....\n";
     cout<<"\nLong double max: "<<LDBL_MAX;
    //test random generation of n-clusters
    double avgArea=0;
    double avgS1=0;
    double avgS2=0;
    int s=2;
    int t=1;
    for(int i=0; i < 1000; i++){
        NCluster *tmp = sampler.SubspaceFreqNetwork(framework.hin,s);
        if(tmp !=  NULL){
            avgArea += tmp->GetSetById(s)->Size()*tmp->GetSetById(t)->Size();
            avgS1 += tmp->GetSetById(s)->Size();
            avgS2 += tmp->GetSetById(t)->Size();
            cout<<"\nlook: "; tmp->Output(framework.NAME_MAPS);
            delete tmp;
        }
    }
     avgArea = avgArea/(double)1000.0;
     avgS1 = avgS1/(double)1000.0;
     avgS2 = avgS2/(double)1000.0;
       cout<<"\nAverage area with freq sampling: "<<avgArea<<" ( "<<avgS1<<" by "<<avgS2<<" )";
    cout<<"\n";
    return (EXIT_SUCCESS);
}
