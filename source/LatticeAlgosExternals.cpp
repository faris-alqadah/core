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
ofstream OUTFILE;
vector<int> PRUNE_SIZE_VECTOR;
