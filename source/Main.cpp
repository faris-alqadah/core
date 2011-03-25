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
#include "../headers/Timing.h"
#include "../headers/PreProcess.h"
#include "../headers/LatticeAlgos.h"

string inputFile="~";
int numArgs=3;
int lrnr=1;
double beta=0.1;


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
    if(dispProgress)
        cout<<"\nDisplay progress option enabled";
    //check enumeration modes
    if(enumerationMode == ENUM_FILE && OUTFILE != "~")
        cout<<"\nOutput clusters option enabled: "<<OUTFILE;
    else if( enumerationMode == ENUM_FILE && OUTFILE == "~"){
        cout<<"\nOutput option enabled but file not specified!";
        DisplayUsage();
    }
    else if(enumerationMode == ENUM_TOPK_FILE && OUTFILE == "~"){
         cout<<"\nOutput option enabled but file not specified!";
        DisplayUsage();
    }
    else if(enumerationMode == ENUM_TOPK_FILE && OUTFILE != "~"){
          cout<<"\nOutput clusters option enabled: "<<OUTFILE;
    }
    if(enumerationMode == ENUM_TOPK_FILE || enumerationMode == ENUM_TOPK_MEM){
        if(ovlpThresh < 0 || ovlpThresh > 1){
            cout<<"\nOverlap threshold not in correct range!";
             DisplayUsage();
        }
        if(beta < 0 || beta > 1){
            cout<<"\nBeta value not in correct range!";
            DisplayUsage();
        }
        if(topKK < 0){
            cout<<"\nk not in correct range!";
            DisplayUsage();
        }
        //if reached here then all parameters are correct
        cout<<"\nTOP K mode enabled with k= "<<topKK<<" and max overlap= "<<ovlpThresh<<" and clusters ranked by ";
        if(qualityMode == BETA){
            cout<<"BETA-balance function";
            //set the correct parameters for params
            params.resize(2);
            params[0] = lrnr;
            params[1] = beta;
        }
        else if(qualityMode == AREA){
            cout<<"Area function";
            params.resize(1);
            params[0] = lrnr;
        }
    }

    if(pruneMode == PRUNE_SIZE){
        cout<<"\nPrune size enabled...limits are: ";
        if (PRUNE_SIZE_VECTOR.size() < 2){
            cout<<"\nMin cardinalities not defined!";
            DisplayUsage();
        }
        for(int i=0; i < PRUNE_SIZE_VECTOR.size(); i++)
            cout<<"\nDOMAIN "<<i+1<<" min: "<<PRUNE_SIZE_VECTOR[i];
    }
  
    cout<<"\n"<<endl;
}


void ProcessCmndLine(int argc, char ** argv){
    enumerationMode = ENUM_MEM;
    pruneMode = PRUNE_SIZE;
    dispProgress=false;
    if (argc < numArgs) DisplayUsage();
     else{
        for(int i=1; i < argc; i++){
           string temp = argv[i];
           if(temp == "-i")//input file
                inputFile = argv[++i];
           else if(temp == "-m"){
              int n = atoi(argv[++i]);
              PRUNE_SIZE_VECTOR.resize(n);
              for(int j=0; j < n; j++)
                  PRUNE_SIZE_VECTOR[j] = atoi(argv[++i]);

           }
           else if(temp == "-prog"){
                dispProgress= true;
           }
           else if(temp == "-o"){
               OUTFILE= argv[++i];
               if(enumerationMode == ENUM_TOPK_FILE)
                    enumerationMode = ENUM_TOPK_FILE;
               else if(enumerationMode == ENUM_TOPK_MEM)
                   enumerationMode = ENUM_TOPK_FILE;
               else
                   enumerationMode = ENUM_FILE;
           }
           else if(temp == "-l"){
               lrnr = atoi(argv[++i]);
           }
           else if(temp == "-k"){
               if(enumerationMode == ENUM_FILE)
                    enumerationMode = ENUM_TOPK_FILE;
               else
                   enumerationMode = ENUM_TOPK_MEM;
               topKK = atoi(argv[++i]);
           }
           else if(temp == "-ovlp"){
               ovlpThresh = atof(argv[++i]);
           }
           else if(temp == "-beta"){
               qualityMode= BETA;
               beta = atof(argv[++i]);

           }
           
        }
    }
    CheckArguments();
}
void OutputClustersFile(){
    int lim = CONCEPTS.size() > topKK ? topKK:CONCEPTS.size();
    for(int i=0; i < lim; i++){
        OutputCluster(CONCEPTS[i], OUT1);
        OutputCluster(CONCEPTS[i],OUT2,NAME_MAPS);
    }
    OUT1.close();
    OUT2.close();
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
    Star_N_Concepts(hin,lrnr,BORDAT);
    EndTiming();
    OutputStats();
    //check if enum mode is top k and output to file
    //if true then output the clusters now
    //since this was not possible during execution
    //of the algorithm
    if(enumerationMode == ENUM_TOPK_FILE){
        OutputClustersFile();
    }
    cout<<"\n";
    return (EXIT_SUCCESS);
}

