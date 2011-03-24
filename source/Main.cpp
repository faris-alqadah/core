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
#include "../headers/PreProcess.h"
#include "../headers/LatticeAlgos.h"

string inputFile="~";
int numArgs=3;
int lrnr=1;
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
    if(enumerationMode == ENUM_FILE && OUTFILE != "~")
        cout<<"\nOutput clusters option enabled: "<<OUTFILE;
    else if( enumerationMode == ENUM_FILE && OUTFILE == "~"){
        cout<<"\nOutput option enabled but file not specified!";
        DisplayUsage();
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
               enumerationMode = ENUM_FILE;
           }
           else if(temp == "-l"){
               lrnr = atoi(argv[++i]);
           }
           
        }
    }
    CheckArguments();
}

int main(int argc, char** argv) {
    ProcessCmndLine(argc,argv);
    RelationGraph *hin = MakeRelationGraph(inputFile);
    hin->Print();
    cout<<"\nRunning n-cluster enumeration algorithm...";
    Star_N_Concepts(hin,lrnr,BORDAT);
    cout<<"\n";
    return (EXIT_SUCCESS);
}

