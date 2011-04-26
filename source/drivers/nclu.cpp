/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2009
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  Simple driver program to test the core libraries 
 *
 *
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/

#include <stdlib.h>

#include "../../headers/Timing.h"
#include "../../headers/PreProcess.h"
#include "../../headers/nclusters/berry.h"


string inputFile="~";
int numArgs=3;
int lrnr=1;
double beta=0.1;

BerryLatticeAlgos la;

void DisplayUsage(){
    cout<<"\nUSAGE: test -i input_file -l lrnr -m n[min1 min2 ... min n] "
        <<"\nREQUIRED: "
        <<"\n-i <inputFile>"
        <<"\n-m n (number of domains) min1 min2 ... min_n the minimum cardinalites of each domain for n-cluster enumeration"
        <<"\nOPTIONAL: "
       <<"\n-l lrnr (specify which context in the input file is the learner context)"
        <<"\n\t First context appearing in input file is specifed as 1 etc."
        <<"\n\t If this option is not used then context 1 is default"
        <<"\n-o <filename> output n-clusters "
        <<"\n\t these will be output to filename.concepts and filenames.concepts.names"
        <<"\n-k <num>(>0) only enumerate the top k clusters (default is to use area to rank) "
        <<"\n-beta <num>(0-1) specify beta value to use beta-ranking of clusters "
        <<"\n-ovlp <num>(0-1) specify percent overlap to use when ranking top k (default is 0.25)"
        <<"\n-prog flag to display progress during enumeration"
        <<"\n\n";
    exit(1);
}

void CheckArguments(){
    if (inputFile == "~"){
        cerr<<"\nINPUT FILE NOT ENTERED!";
        DisplayUsage();
    }
    cout<<"\nLearner context: "<<lrnr;
    if(la.dispProgress)
        cout<<"\nDisplay progress option enabled";
    //check enumeration modes
    if(la.enumerationMode == la.ENUM_FILE && la.OUTFILE != "~")
        cout<<"\nOutput clusters option enabled: "<<la.OUTFILE;
    else if( la.enumerationMode == la.ENUM_FILE && la.OUTFILE == "~"){
        cout<<"\nOutput option enabled but file not specified!";
        DisplayUsage();
    }
    else if(la.enumerationMode == la.ENUM_TOPK_FILE && la.OUTFILE == "~"){
         cout<<"\nOutput option enabled but file not specified!";
        DisplayUsage();
    }
    else if(la.enumerationMode == la.ENUM_TOPK_FILE && la.OUTFILE != "~"){
          cout<<"\nOutput clusters option enabled: "<<la.OUTFILE;
    }
    if(la.enumerationMode == la.ENUM_TOPK_FILE || la.enumerationMode == la.ENUM_TOPK_MEM){
        if(la.ovlpThresh < 0 || la.ovlpThresh > 1){
            cout<<"\nOverlap threshold not in correct range!";
             DisplayUsage();
        }
        if(beta < 0 || beta > 1){
            cout<<"\nBeta value not in correct range!";
            DisplayUsage();
        }
        if(la.topKK < 0){
            cout<<"\nk not in correct range!";
            DisplayUsage();
        }
        //if reached here then all parameters are correct
        cout<<"\nTOP K mode enabled with k= "<<la.topKK<<" and max overlap= "<<la.ovlpThresh<<" and clusters ranked by ";
        if(la.qualityMode == la.BETA){
            cout<<"BETA-balance function";
            //set the correct parameters for params
            la.params.resize(2);
            la.params[0] = lrnr;
            la.params[1] = beta;
        }
        else if(la.qualityMode == la.AREA){
            cout<<"Area function";
            la.params.resize(1);
            la.params[0] = lrnr;
        }
    }

    if(la.pruneMode == la.PRUNE_SIZE){
        cout<<"\nPrune size enabled...limits are: ";
        if (la.PRUNE_SIZE_VECTOR.size() < 2){
            cout<<"\nMin cardinalities not defined!";
            DisplayUsage();
        }
        for(int i=0; i < la.PRUNE_SIZE_VECTOR.size(); i++)
            cout<<"\nDOMAIN "<<i+1<<" min: "<<la.PRUNE_SIZE_VECTOR[i];
    }
  
    cout<<"\n"<<endl;
}


void ProcessCmndLine(int argc, char ** argv){
    la.enumerationMode = la.ENUM_MEM;
    la.pruneMode = la.PRUNE_SIZE;
    la.dispProgress=false;
    if (argc < numArgs) DisplayUsage();
     else{
        for(int i=1; i < argc; i++){
           string temp = argv[i];
           if(temp == "-i")//input file
                inputFile = argv[++i];
           else if(temp == "-m"){
              int n = atoi(argv[++i]);
              la.PRUNE_SIZE_VECTOR.resize(n);
              for(int j=0; j < n; j++)
                  la.PRUNE_SIZE_VECTOR[j] = atoi(argv[++i]);

           }
           else if(temp == "-prog"){
                la.dispProgress= true;
           }
           else if(temp == "-o"){
               la.OUTFILE= argv[++i];
               if(la.enumerationMode == la.ENUM_TOPK_FILE)
                    la.enumerationMode = la.ENUM_TOPK_FILE;
               else if(la.enumerationMode == la.ENUM_TOPK_MEM)
                  la.enumerationMode = la.ENUM_TOPK_FILE;
               else
                   la.enumerationMode = la.ENUM_FILE;
           }
           else if(temp == "-l"){
               lrnr = atoi(argv[++i]);
           }
           else if(temp == "-k"){
               if(la.enumerationMode == la.ENUM_FILE)
                    la.enumerationMode = la.ENUM_TOPK_FILE;
               else
                   la.enumerationMode = la.ENUM_TOPK_MEM;
               la.topKK = atoi(argv[++i]);
           }
           else if(temp == "-ovlp"){
               la.ovlpThresh = atof(argv[++i]);
           }
           else if(temp == "-beta"){
               la.qualityMode= la.BETA;
               beta = atof(argv[++i]);

           }
           
        }
    }
    CheckArguments();
}
void OutputClustersFile(){
    int lim = la.CONCEPTS.size() > la.topKK ? la.topKK:la.CONCEPTS.size();
    for(int i=0; i < lim; i++){
        OutputCluster(la.CONCEPTS[i], la.OUT1);
        OutputCluster(la.CONCEPTS[i],la.OUT2,la.NAME_MAPS);
    }
    la.OUT1.close();
    la.OUT2.close();
}

void OutputStats(){
    ofstream outStat("stats");
    outStat<<"\nAlgorithm execution time: "<<ComputeTime();
    outStat<<"\n# set intersections "<<numIntersection
           <<"\n# subset checks "<<numSubset
           <<"\n# set unions "<<numUnion
           <<"\n# set differences "<<numDifference;
    outStat.close();

}

int main(int argc, char** argv) {
    ProcessCmndLine(argc,argv);
    RelationGraph *hin = MakeRelationGraph(inputFile);
    hin->Print();
    cout<<"\nRunning n-cluster enumeration algorithm...";
    StartTiming();
    la.Star_N_Concepts(hin,lrnr);
    EndTiming();
    OutputStats();
    //check if enum mode is top k and output to file
    //if true then output the clusters now
    //since this was not possible during execution
    //of the algorithm
    if(la.enumerationMode == la.ENUM_TOPK_FILE){
        OutputClustersFile();
    }
    cout<<"\n";
    return (EXIT_SUCCESS);
}

