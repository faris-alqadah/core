/********************************************************************/
//      EXTERNALS
//use these variables to keep track of some stats in the different algorithms
//recall that Ops.h also keeps count of basic set operations
/********************************************************************/

#ifndef LATTICEALGOSEXTERNALS_H
#define	LATTICEALGOSEXTERNALS_H


#include "RelationGraph.h"


extern int srchLvl; //the current search level of a lattice enumeration algorithm
extern int numConcepts; // the number concepts enumerated
extern bool dispProgress; //display progress of execution

extern int enumerationMode; // the mode of an enumeration algorithm
                            //for now 2 modes exist,
                            //ENUM_MEM: enumerate all concepts and store in memory (in a vector)
                            //ENUM_FILE: output concepts to a file as soon as a concept is discovered
extern const int ENUM_MEM; //ENUM_MEM mode for enumeration algorithms
extern const int ENUM_FILE; //ENUM_FILE mode for enumeration algorithms
extern vector<NCluster*> CONCEPTS; //if ENUM_MEM mode is set then concepts stored in this vector
extern ofstream OUTFILE; //if ENUM_FILE is selected then this stream is used to output the concepts

extern int pruneMode;  //the mode of pruning for enumeration algorithms
                       //for now 1 mode exists:
                       //  PRUNE_SIZE: inidcates to prune the concepts by size
extern const int PRUNE_SIZE; //PRUNE_SIZE mode for enumeration algorithms
extern vector<int> PRUNE_SIZE_VECTOR; //this vector will hold the size bouns for pruning by size

#endif	

