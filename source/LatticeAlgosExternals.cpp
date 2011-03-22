#include "../headers/LatticeAlgosExternals.h"

int srchLvl =0;
int numConcepts=0;
bool dispProgress=false;

const int ENUM_MEM=1;
const int ENUM_FILE=2;
int enumerationMode=ENUM_MEM;

const int PRUNE_SIZE=1;
int pruneMode=PRUNE_SIZE;

vector<NCluster*> CONCEPTS;
string OUTFILE; //if ENUM_FILE is selected then this file is used to output the concepts

ofstream OUT1;  //ofstream used to output to OUTFILE.concepts
ofstream OUT2;  //ofsteram used to output to OUTFILE.concept.names

vector<int> PRUNE_SIZE_VECTOR;
