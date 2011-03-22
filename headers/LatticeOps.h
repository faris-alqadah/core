/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2009
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  Lattice operations derived from formal concept anaylsis. These are extended
 * to heterogenous information networks (HINs)
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/

#include "RelationGraph.h"





/********************************************************************/
// GetTop(Context c): Get the top level concept from c
//               this concept corresponds to the concept that contains all the objects from domain2, and
//                any element in domain1 that contains all objects from doamin2 or the empty set
//  Pre-Condition: none
//  Post-Condition: none
//  returns: a 2-cluster that correpsonds to the top cluster of c
//  output: none
/********************************************************************/
NCluster *GetTop(Context *c);

/********************************************************************/
// GetTop(Context c): Get the bottom level concept from c
//               this concept corresponds to the concept that contains all the objects from domain1, and
//                any element in domain2 that contains all objects from doamin1 or the empty set
//  Pre-Condition: none
//  Post-Condition: none
//  returns: a 2-cluster that correpsonds to the bottom cluster of c
//  output: none
/********************************************************************/
NCluster *GetBottom(Context *c);

/********************************************************************/
// Prime(NCluster a, RelationGraph *g, int s, int t): Computes the prime operator (defined by FCA)
//     s: indicates the source domain id in a to serve as the input for the prime operator
//     t: indicates the target domain
//    The operation differs slighty from the strict FCA definition in that if a[s] is empty
//    then the result is exactly a and not the top or bottom concept of the context
//  Pre-Condition: s and t are valid domain ids that form an edge in g, and are valid set ids in a
//  Post-Condition: none
//  returns: an ncluster in whcih all sets are equivalent to the sets of a, except set with domain id t
//  output: none
/********************************************************************/
NCluster *Prime(NCluster *a, RelationGraph *g, int s, int t);
/************************ENUMERATION ALGORITHMS********************************************/
//  All enumeration algorithms contain many different options.
//  These options should be set by the user by setting the different externally
//  available variables as described below.
//
//   enumerationMode must be set or it defaults to ENUM_MEM (see below)
//   pruneMode is always set to PRUNE_SIZE so PRUNE_SIZE_VECTOR must be initalized
//              if PRUNE_SIZE_VECTOR is not intialized it defaults to prune sizes of 1
//                  for each domain
//
//
/********************************************************************/


/********************************************************************/
//  Star_N_Concepts (RelationGraph g, int lrnrContext)
//
//  Enumerates the n-clusters from a star-shaped hin (heterogenous information network)
//   The exact algorithm is implemented by the Enum_NConcepts
//   Current implementations are:
//          * Extension of Bordat et al. algorithm given in "Incremental Approach to Concept Generation"
//              The extension utilizes ideas from "An Effective Algorithm for mining 3-clusters" by Alqadah et al.
//              No beta-pruning is used, but simulteanous minimum bound pruning is applied
//
//
//  Pre-Condition: g is a star shaped hin, lrnrContext is a valid context in g, and all Enum Algorithm preconditions
//                  as stated above
//  Post-Condition: the n-concepts of g are enumerated and eithier stored in CONCEPTS or output to the file OUTFILE
//                  the numConcepts variable is set to the number of concepts enumerated
//  returns: none
//  output: if ENUM_FILE mode is set n-concepts are outputed to OUTFILE.clusters.names and OUTFILE.clusters.ids
/********************************************************************/
void Star_N_Concepts(RelationGraph *g,int lrnrContext);

void Enum_NConcepts(NCluster *a, int s, int t);

/********************************************************************/
//      EXTERNALS
//use these variables to keep track of some stats in the different algorithms
//recall that Ops.h also keeps count of basic set operations
/********************************************************************/

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

