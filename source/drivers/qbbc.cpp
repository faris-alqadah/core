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
#include "../../headers/LatticeAlgos.h"
#include "../../headers/RLatticeOps.h"
#include "../../headers/alpha_concepts/helpers.h"


string inputFile="~";
double alpha=1.0;
int numArgs=1;
LatticeAlgos la;


void DisplayUsage(){
    cout<<"\nUSAGE: test -i input_file "
        <<"\nREQUIRED: "
        <<"\n-i <inputFile>"
        <<"\nOPTIONAL: "
        <<"\n-alpha <alpha value> (default is 1.0)"
        <<"\n\n";
    exit(1);
}

void CheckArguments(){
    if (inputFile == "~"){
        cerr<<"\nINPUT FILE NOT ENTERED!";
        DisplayUsage();
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
               alpha = atof(argv[++i]);
        }
    }
    CheckArguments();
}


void OutputStats(){
    ofstream outStat("stats");
    outStat.close();

}

int main(int argc, char** argv) {
    ProcessCmndLine(argc,argv);
    RContext *matrix = MakeSingleRContext(inputFile);
    matrix->PrintBasicStats();
    cout<<"\nPrinting in full matrix form....\n";
    matrix->PrintAsMatrix();
    cout<<"\nTesting std devs....\n";
    matrix->ComputeStdDevs();
    cout<<"\n";
    matrix->PrintStdDevs();
    cout<<"\nTesting sub context....\n";
    IOSet *subRows = new IOSet; subRows->Add(0); subRows->Add(4); subRows->Add(5);
    IOSet *subCols = new IOSet; subCols->Add(0); subCols->Add(3);

    RContext *subMatrix = matrix->GetSubRContext(subRows,subCols);
    subMatrix->PrintAsSparse();
     cout<<"\nTesting std devs....\n";
    subMatrix->ComputeStdDevs();
    cout<<"\n";
    subMatrix->PrintStdDevs();
    cout<<"\n";
    cout<<"\n\nNow test alpha operator for with range,3-sigma, and uniform space....\n";
    //setup paramater vectors here
    vector<double> threeSigmaParams(1);
    vector<double> uniformParams(1);
    vector<double> paramsRange;
    la.consistencyFunction=&AlphaSigma;
    la.dispersionFunction=&Range;
    threeSigmaParams[0]=uniformParams[0]=alpha;
    cout<<"\nQUERY SET (COLUMNS): \n"; subCols->Output();
    cout<<"\nnow applying alpha (3 sigma) with alpha=...."<<alpha<<"\n";
    Construct_AlphaSigma_Params(matrix,subCols,2, threeSigmaParams);
    Construct_MaxSpaceUniform_Params(matrix,subCols,2, uniformParams);
    IOSet *result = Prime_Alpha_Naive(matrix, subCols,2,1, la.dispersionFunction,la.consistencyFunction,paramsRange, uniformParams);
    cout<<"\nResult is: \n";
    result->Output();

    cout<<"\n";
    return (EXIT_SUCCESS);
}


