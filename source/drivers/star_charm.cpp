/*!
 Driver program for qbbc clustering (query based bi-clustering)


 */

#include <stdlib.h>

#include "../../headers/core/Timing.h"
#include "../../headers/core/PreProcess.h"
#include "../../headers/alpha_concepts/basic_prefix_alpha.h"
#include "../../headers/core/RLatticeOps.h"
#include "../../headers/alpha_concepts/helpers.h"


string inputFile="~";
string outFile="~";
int numArgs=2;
BasicPrefix la;
IOSet *query;
int queryDomain=-1;
int otherDomain=-1;
void(*paramFunction)(RContext *,IOSet *, int, int,int, vector<double> &);
double queryQuality;

void DisplayUsage(){
    cout<<"\nUSAGE: test -i input_file "
        <<"\nREQUIRED: "
        <<"\n-i <inputFile>"
        <<"\nOPTIONAL: "
        <<"\n-alpha <alpha value> (default is 1.0)"
        <<"\n-c <consistency function> 1- alpha_sigma 2- max_space_uniform (default is 1)"
        <<"\n-o <output file path>"
        <<"\n\n";
    exit(1);
}

void CheckArguments(){
    if (inputFile == "~"){
        cerr<<"\nINPUT FILE NOT ENTERED!";
        DisplayUsage();
    }
    if(la.consistencyMode < 1 || la.consistencyMode > 2){
        cerr<<"\nINVALID CONSISTENCY MODE ENTERED";
        DisplayUsage();
    }else{
        if (la.consistencyMode == la.ALPHA_SIGMA){
            la.consistencyFunction=&AlphaSigma;
            la.paramFunction =&Construct_AlphaSigma_Params;
        }
        else if(la.consistencyMode == la.MAX_SPACE_UNIFORM){
            la.consistencyFunction=&AlphaSigma;
            la.paramFunction = &Construct_MaxSpaceUniform_Params;
        }
    }
    la.dispersionFunction=&Range;
    cout<<"\ninput file: "<<inputFile<<"\nalpha: "<<la.alpha<<"\nconsistency mode: "<<la.consistencyMode;
    if(outFile != "~"){
        cout<<"\nOutput file: "<<outFile;
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
               la.alpha = atof(argv[++i]);
           if(temp == "-c"){
               la.consistencyMode = atoi(argv[++i]);
           }
           if(temp == "-o"){
               outFile=argv[++i];
           }
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
    la.NETWORK = MakeRelationGraphR(inputFile);
    //compute standard deviations of each context
    la.NETWORK->ComputeStdDevs();
    la.NETWORK->Print();
    la.s = 1; //central domain assumed to be first domain listed
    la.StarCharm();
    cout<<"\n";
    return (EXIT_SUCCESS);
}



