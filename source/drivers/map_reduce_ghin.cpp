/*!
 Author: Faris Alqadah

 Driver program for map_reduce_GHIN
 */



#include <stdlib.h>
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
    srand(time(NULL));
     cout<<"\nRandomly sampling (freq) from context 1, domain 1....\n";
    //test random generation of n-clusters
     double avgFreq=0;
    for(int i=0; i < 1000; i++){
        NCluster *tmp = sampler.SubspaceFreq(framework.hin->GetContext(1), 2, 1);
       // cout<<"\nGot set: \n";
       // tmp->Output();
        //cout<<"\t";
        //tmp->GetSet(0)->Output(framework.NAME_MAPS[0]);
      //  cout<<"\nwith supporting set freq: ";
        IOSet *sup = Prime(tmp,framework.hin,2,1,1);
       // sup->Output(framework.NAME_MAPS[0]);
       // cout<<" "<<sup->Size()<<"\t"<<(double) sup->Size() / (double) framework.hin->GetContext(1)->GetLabels(1)->Size();
        avgFreq += sup->Size();
        delete tmp;
        delete sup;
    }
     avgFreq = avgFreq/(double)1000.0;
     cout<<"\nAvg freq w/sampling: "<<avgFreq;
     avgFreq=0;
     //randomly draw items and check out avg freq
     for(int i=0; i < 1000; i++){
         int r= rand() % framework.hin->GetContext(1)->GetNumSets(2);
        NCluster *tmp = new NCluster();
        tmp->AddSet(new IOSet());
        tmp->GetSet(0)->Add(r);
        tmp->GetSet(0)->SetId(2);
      // cout<<"\nGot set: \n";
        //tmp->Output();
        //cout<<"\t";
        //tmp->GetSet(0)->Output(framework.NAME_MAPS[0]);
      //  cout<<"\nwith supporting set freq: ";
        IOSet *sup = Prime(tmp,framework.hin,2,1,1);
       // sup->Output(framework.NAME_MAPS[0]);
        //cout<<" "<<sup->Size()<<"\t"<<(double) sup->Size() / (double) framework.hin->GetContext(1)->GetLabels(1)->Size();
        avgFreq += sup->Size();
        delete tmp;
        delete sup;
    }
     avgFreq = avgFreq/(double)1000.0;
     cout<<"\nAvg freq random item: "<<avgFreq;
    cout<<"\n";
    return (EXIT_SUCCESS);
}
