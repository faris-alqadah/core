/*!
 Author: Faris Alqadah

 Driver program for GHIN (Game theortic framework for Heterogenous Information Network Clustering)
 For complete details see "A Game Theortic Framework for Heterogenous Information Network Clustering" by Alqadah et al. in KDD 2011
 */




#include <stdlib.h>
#include "../../headers/core/PreProcess.h"
#include "../../headers/core/LatticeOps.h"
#include "../../headers/core/Timing.h"

#include "../headers/ghin/Framework.h"
#include "../headers/ghin/Rewards.h"

int numArgs=2;

float enumConceptsTime;
float simMatTime;

//! framework object to implement algorithm
Ghin framework;



using namespace std;


void DisplayUsage(){
    cout<<"\nUSAGE: ./bin "
        <<"\nREQUIRED: "
        <<"\n-i <inputFile>"
        <<"\n-w <weight of zeros>"
        <<"\n-reward 1- simple weighted 2- expected satisfaction"
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
    if(framework.enumerationMode == framework.ENUM_FILE && framework.OUTFILE != "~")
        cout<<"\nOutput clusters option enabled: "<<framework.OUTFILE;
    else if( framework.enumerationMode == framework.ENUM_FILE && framework.OUTFILE == "~"){
        cout<<"\nOutput option enabled but file not specified!";
        DisplayUsage();
    }
    if(framework.dispProgress)
        cout<<"\nDisplay progress option enabled";
    
    if (framework.rewardMode == framework.SIMPLE_WEIGHTED){
         cout<<"\nReward function is: SIMPLE WEIGHTED with w= "<<framework.w;

     }else if(framework.rewardMode == framework.EXPECTED_SAT){
             cout<<"\nReward function is: SIMPLE WEIGHTED with w= "<<framework.w;
     }
     if(framework.tiredMode == true)
         cout<<"\nTiring mode enabled";
    


}
void OutputStats(){

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
               framework.enumerationMode == framework.ENUM_FILE;
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


int main(int argc, char** argv) {
    ProcessCmndLine(argc,argv);
    framework.hin = MakeRelationGraph(inputFile);
    framework.hin->Print();
    if(framework.tiredMode)
        framework.InitTiring();




    StartTiming();

    GHIN(RewardFunc);
    EndTiming();
    sort(finalClusters.begin(),finalClusters.end(), CompQualitySet);
    if(setClassify){
        ofstream out(setClassifyFile.c_str());
        OutputSetForClassify(setNumClassify,out);
    }
    sort(finalClusters.begin(),finalClusters.end(), CompQualitySet);
    if(outputClusters){
        ofstream out(clustersOutputFile.c_str());
        OutputClustersNamesTreeClu(out);
    }

    cout<<"\n";
    return (EXIT_SUCCESS);
}
