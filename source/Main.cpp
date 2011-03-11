/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2009
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  Implementation of an nCluster data structure
 *
 *
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/

#include <stdlib.h>
#include "../Headers/PreProcess.h"
#include "../Headers/LatticeOps.h"
#include "../Headers/Timing.h"
#include "../Headers/SimMeasures.h"
#include "../Headers/TreeCluAlgs.h"
#include "../Headers/PostProcess.h"

int numArgs=1;

float enumConceptsTime;
float simMatTime;

void DisplayUsage(){
    cout<<"\nUSAGE: ./bin "
        <<"\nREQUIRED: "
        <<"\n-i <inputFile>";
    cout<<"\nOPTIONAL "
        <<"\n-min <numsets> <min1> <min2> ... <minNumSets> set minimum cardinality of each set in a cluster"
        <<"\n-outConcepts <file> name of file to output concepts to "
        <<"\n-outConceptName <file> name of file to ouptut the concepts with labels"
        <<"\n-prog display progress "
        <<"\n-stats <file> output algorithm stats to <file> (default file is stats)"
        <<"\n\n";
    exit(1);
}

void CheckArguments(){
    if (inputFile == "~"){
        cerr<<"\nINPUT FILE NOT ENTERED!";
        DisplayUsage();
    }
    if(minCardSet){
        cout<<"\nMin cardinalities set!";
    }else{
        cout<<"\nMin cardinaltiy not set....defaulting to 1 per set";      
    }
    if(computeCover)
        cout<<"\nCompute cover option enabled";
    if(dispProgress)
        cout<<"\nDisplay progress option enabled";
    if(outputConcepts)
        cout<<"\noutput concepts option enabled...file= "<<outConceptFile;
    if(outputConceptsNames)
        cout<<"\noutput concepts w/names enabled...file= "<<outConceptsNames;
}
void OutputStats(){
    ofstream outS;
    outS.open(statsFile.c_str(),ios::out | ios::app);
     // cout<<"\n"<</*"\nnum concepts: "<<*/numConcepts
      //    <<"\n"<<H.Size();
         /*<<"\nnum edges: "<<numEdges
          <<"\nIntersections: "<<numIntersection
          <<"\nDifference: "<<numDifference
          <<"\nSubset check: "<<numSubset
          <<"\nUnions: "<<numUnion
          <<"\nTime: "<<ComputeTime();*/
}

void ProcessCmndLine(int argc, char ** argv){
    if (argc < numArgs+1) DisplayUsage();
     else{
        for(int i=1; i < argc; i++){
           string temp = argv[i];
           if(temp == "-i")//input file
                inputFile = argv[++i];
           else if(temp == "-prog"){
               dispProgress= true;
           }else if(temp == "-min"){
               int numSets = atoi(argv[++i]);
               minCard.resize(numSets);
               for(int j=0; j < numSets; j++){
                   minCard[j] = atoi(argv[++i]);
               }
               minCardSet=true;
           }else if(temp == "-outConcepts"){
                 outputConcepts=true;
                 outConceptFile = (argv[++i]);                    
           }else if(temp=="-outConceptName"){
               outputConceptsNames=true;
               outConceptsNames=argv[++i];
           }else if(temp == "-stats"){
               statsFile = argv[++i];
               outputStats=true;
           }
        }
    }
    CheckArguments();
}


int main(int argc, char** argv) {
    ProcessCmndLine(argc,argv);
    ReadInputFile();
    if(!minCardSet){
         minCard.resize(N);
        fill(minCard.begin(),minCard.end(),1);
    }
    OutputSetStats();
    OutputContextStats();
    relationGraph.Print();
   //contextVector[0]->PrintAsFIMI();
    StartTiming();
    //TreeCluAlg();
	StarCluAlg();
    EndTiming();
    enumConceptsTime = ComputeTime();
    if(outputStats)
        OutputStats();
    cout<<"\n";
    return (EXIT_SUCCESS);
}

