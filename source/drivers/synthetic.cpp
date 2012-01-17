/*!
 Author: Faris Alqadah

 Driver program for synthetic data generator
 */



#include <stdlib.h>
#include<cfloat>
#include "../../headers/core/PreProcess.h"
#include "../../headers/synthetic/synthetic_hin.h"

int numArgs=3;
int cardA=-1;
int cardB=-1;
string outFile="~";

using namespace std;

void DisplayUsage() {
    cout << "\nUSAGE: ./bin "
            << "\nREQUIRED: "
            << "\n-a (number of elements in domain a)"
            <<"\n-b (number of element in domain b)"
            <<"\n-out (path to output file)\n";
    exit(1);
}

void CheckArguments() {
    if(cardA == -1){
        cerr<<"Number of elements in domain A not set";
        DisplayUsage();
    }
    if(cardB == -1){
         cerr<<"Number of elements in domain B not set";
        DisplayUsage();
    }
     if(outFile == "~"){
         cerr<<"Out file not set";
        DisplayUsage();
    }
}

void ProcessCmndLine(int argc, char ** argv) {
    if (argc < numArgs + 1) DisplayUsage();
    else {
        for(int i=1; i < argc; i++){
           string temp = argv[i];
           if(temp == "-out")//input file
                outFile = argv[++i];
           else if(temp == "-a"){
              cardA = atoi(argv[++i]);
           }
           else if(temp == "-b"){
               cardB = atoi(argv[++i]);
           }
        }
    }

    CheckArguments();
}

int main(int argc, char** argv) {
    ProcessCmndLine(argc, argv);
    cout<<"\nGenerating context....\n";
    srand(time(NULL));
    MakeSynContext_ScaleA_UniformB(cardA,cardB,outFile);
    cout << "\n";
    return (EXIT_SUCCESS);
}
