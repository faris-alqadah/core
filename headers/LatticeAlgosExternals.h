/********************************************************************/
//      EXTERNALS
//use these variables to keep track of some stats in the different algorithms
//recall that Ops.h also keeps count of basic set operations
/********************************************************************/

#ifndef LATTICEALGOSEXTERNALS_H
#define	LATTICEALGOSEXTERNALS_H


#include "RelationGraph.h"


    
extern int srchLvl; 
extern int numConcepts;
 extern bool dispProgress;

///////////////////Data structs and files////////////////////////////////////////////
extern vector<NCluster*> CONCEPTS;
extern vector<NameMap*> NAME_MAPS;
extern string OUTFILE; //if ENUM_FILE is selected then this file is used to output the concepts

extern ofstream OUT1;  //ofstream used to output to OUTFILE.concepts
extern ofstream OUT2;  //ofsteram used to output to OUTFILE.concept.names


///////////////////ENUMERATION MODES////////////////////////////////////////////
extern const int ENUM_MEM;
extern const int ENUM_FILE;
extern const int ENUM_TOPK_FILE;
extern const int ENUM_TOPK_MEM;

extern int enumerationMode; //seting default

///////////////////QUALITY MODES////////////////////////////////////////////
extern const int AREA;
extern const int BETA;

extern int qualityMode;

extern double(*qualityFunction)(NCluster*, vector<double> &);
extern vector<double> params; //store the parameters for a quality function here

///////////////////Overlap MODES////////////////////////////////////////////
extern const int AVG_JACCARD;
 extern int ovlpMode;

extern double(*ovlpFunction)(NCluster*,NCluster*);
extern double ovlpThresh;


///////////////////TOP K MODES////////////////////////////////////////////
extern int topKK;

///////////////////PRUNE MODES////////////////////////////////////////////
extern const int PRUNE_SIZE;
extern int pruneMode; 


extern vector<int> PRUNE_SIZE_VECTOR;


///////////////////Algorithm Seletions////////////////////////////////////////////
extern  const int BORDAT ;

extern int neighborAlgo;
extern int conceptEnumerationAlgo;





#endif	

