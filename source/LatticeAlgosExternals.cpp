#include "../headers/LatticeAlgosExternals.h"



 int srchLvl =0; //default
 int numConcepts=0; //default
  bool dispProgress=false; //default

///////////////////Data structs and files////////////////////////////////////////////
 vector<NCluster*> CONCEPTS;
 vector<NameMap*> NAME_MAPS;
 string OUTFILE; //if ENUM_FILE is selected then this file is used to output the concepts

 ofstream OUT1;  //ofstream used to output to OUTFILE.concepts
 ofstream OUT2;  //ofsteram used to output to OUTFILE.concept.names


///////////////////ENUMERATION MODES////////////////////////////////////////////
 const int ENUM_MEM=1;
 const int ENUM_FILE=2;
 const int ENUM_TOPK_FILE=3;
 const int ENUM_TOPK_MEM=4;

 int enumerationMode=1; //seting default

///////////////////QUALITY MODES////////////////////////////////////////////
 const int AREA=1;
 const int BETA=2;

 int qualityMode=1; //default

 double(*qualityFunction)(NCluster*, vector<double> &);
 vector<double> params; //store the parameters for a quality function here

///////////////////Overlap MODES////////////////////////////////////////////
 const int AVG_JACCARD=1;   //setting default
  int ovlpMode=1;

 double(*ovlpFunction)(NCluster*,NCluster*);
 double ovlpThresh=0.25;  //this is the overlap threshold...setting default


///////////////////TOP K MODES////////////////////////////////////////////
 int topKK = 100;      //this is "K" for top k enumerations...setting default

///////////////////PRUNE MODES////////////////////////////////////////////
 const int PRUNE_SIZE=1;
 int pruneMode=1; //defaults


 vector<int> PRUNE_SIZE_VECTOR;


///////////////////Algorithm Seletions////////////////////////////////////////////
  const int BORDAT =1;

 int neighborAlgo=1;            //defaults
 int conceptEnumerationAlgo=1;  //defaults




