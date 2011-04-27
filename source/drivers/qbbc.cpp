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
#include "../../headers/alpha_concepts/basic_prefix_alpha.h"
#include "../../headers/RLatticeOps.h"
#include "../../headers/alpha_concepts/helpers.h"


string inputFile="~";
string queryFile="~";
double alpha=1.0;
int numArgs=2;
BasicPrefixAlphaAlgos la;
IOSet *query;
void(*paramFunction)(RContext *,IOSet *, int, int,int, vector<double> &);

void DisplayUsage(){
    cout<<"\nUSAGE: test -i input_file "
        <<"\nREQUIRED: "
        <<"\n-i <inputFile>"
        <<"\n-q <queryFile>"
        <<"\nOPTIONAL: "
        <<"\n-alpha <alpha value> (default is 1.0)"
        <<"\n-c <consistency function> 1- alpha_sigma 2- max_space_uniform (default is 1)"
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
    if(la.consistencyMode < 1 || la.consistencyMode > 2){
        cerr<<"\nINVALID CONSISTENCY MODE ENTERED";
        DisplayUsage();
    }else{
        if (la.consistencyMode == la.ALPHA_SIGMA){
            la.consistencyFunction=&AlphaSigma;
            paramFunction =&Construct_AlphaSigma_Params;
        }
        else if(la.consistencyMode == la.MAX_SPACE_UNIFORM){
            la.consistencyFunction=&AlphaSigma;
            paramFunction = &Construct_MaxSpaceUniform_Params;
        }
    }
    la.dispersionFunction=&Range;
    cout<<"\ninput file: "<<inputFile<<"\nquery file: "<<queryFile<<"\nalpha: "<<alpha<<"\nconsistency mode: "<<la.consistencyMode;
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
           if(temp == "-q")
               queryFile=argv[++i];
           if(temp == "-c"){
               la.consistencyMode = atoi(argv[++i]);
           }
        }
    }
    CheckArguments();
}
void ReadQueryFile(){
    query = new IOSet;
    ifstream myfile(queryFile.c_str());
    if (myfile.is_open()){
        for (string line; getline(myfile, line);)
            query->Add(atoi(line.c_str()));
        query->Sort();
    }else{
        cerr<<"\nCould not open query file!\n";
        exit(-1);
    }
    
}

void OutputStats(){
    ofstream outStat("stats");
    outStat.close();

}

int main(int argc, char** argv) {
    ProcessCmndLine(argc,argv);
    ReadQueryFile();
    RContext *matrix = MakeSingleRContext(inputFile);
    cout<<"\nGot query with "<<query->Size()<<" objects...."
        <<"\nTesting for exact hit.....\n";
    IOSet *initRslt = Prime_Alpha_Naive(matrix,query,2,1,alpha, la.dispersionFunction,la.consistencyFunction,paramFunction);
    if(initRslt->Size() > 0 ){
        cout<<"\nGOT DIRECT HIT!!";
    }else{
        cout<<"\nNO direct hit..."
            <<"\nUsing prefix tree....\n";
        la.BasicPrefix(matrix,query,2);
    }
    cout<<"\n";
    return (EXIT_SUCCESS);
}


