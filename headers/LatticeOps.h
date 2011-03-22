/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2009
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  Basic Lattice operations derived from formal concept anaylsis. These are extended
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










