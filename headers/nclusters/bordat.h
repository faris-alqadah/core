/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2009
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  Implementation of a "Local Approach to Concept Generation" by Bordat et al.
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/

#include "../../headers/LatticeOps.h"
#include "../../headers/LatticeAlgosExternals.h"
#include "../algos_helpers/basic.h"
#include "../algos_helpers/topk.h"
#include "../QualityMeasures.h"


#ifndef BORDAT_H
#define	BORDAT_H



/********************************************************************/
// Enum_NConcepts_Bordat implements the DFS algorithm specified in "A local approach to concept generation"
//                      by Bordat et. al and generalizes it to a star-shaped relation network
//  Pre-Condition: g is a star shaped hin, s and t are valid domain identifiers of a learner context
//  Post-Condition: the n-concepts of g are enumerated and eithier stored in CONCEPTS or output to the file OUTFILE
//                  the numConcepts variable is set to the number of concepts enumerated
//                  All lattice algorithm parameters have been set
//  returns: none
//  output: if ENUM_FILE mode is set n-concepts are outputed to OUTFILE.clusters.names and OUTFILE.clusters.ids
/********************************************************************/
void Enum_NConcepts_Bordat(NCluster *a, RelationGraph *g, IOSet *marked, int s, int t);

/********************************************************************/
//  Maxmod_Partition(ctx, c, s,t) returns an ORDERED vector of the maxmods of the 2-cluster c, over the sub relation
//                  (ctx[s,t] - c.s, t) (see paper for exact definition of maxmod)
//          
//  Pre-Condition: ctx and c are not null, s and t are valid domain identifiers of the domains in ctx
//  Post-Condition: none
//  returns: the maxmods of the 2-cluster c(s,t)
//  output: none
/********************************************************************/
vector<IOSet*>* MaxMod_Partition(Context *ctx, NCluster *c, int s, int t);

/********************************************************************/
//  NonDominating_Maxmods(ctx, c, s,t,maxmods,primes,domInfo) returns a list
//    of the non-dominating maxmods from maxmods. In additions stores the prime operation
//     of each maxmod into the vector primes. Finally, domInfo stores the index of
//     maxmods dominated by each non-dominated maxmod
//
//  Pre-Condition: ctx and c are not null, s and t are valid domain identifiers of the domains in ctx
//  Post-Condition: none
//  returns: non-dominating maxmods of maxmods
//  output: none
/********************************************************************/
list<IOSet*>* NonDominating_MaxMods(Context *ctx, NCluster * c, int s, int t,
        vector<IOSet*>* maxmods,
        vector<IOSet*>* primes, vector<IOSet*>* domInfo);

/********************************************************************/
// RemoveMarked(list<IOSet*> * ndMaxMods, IOSet *marked);
//   "Removes" the maxmods in ndMaxMods that contain elements from marked
//    by settinh the IOSet Marked flag
//
//  Pre-Condition: ndMaxMods is a list of non-dominating maxmods
//  Post-Condition: sets Marked() flag to each element of ndMaxMods if it contains an element from marked
//  returns: none
//  output: none
/********************************************************************/
void RemoveMarked(list<IOSet*> * ndMaxMods, IOSet *marked);





/********************************************************************/
//  MakeMatch(NCluster *lrnrConcept, RelationGraph *g, int s, int t);
//
//    attemtpts to form semi-concepts in all contexts of g except the (s,t) context
//              with the object-set lrnrConcept(s)
//
//  Pre-Condition: (s,t) must be a valid edge in g, g must be star shaped, and s must be the central domain
//  Post-Condition: none
//  returns: Ncluster with (s,t) as the original concept and all other domain ids as the formed semi-concepts if possible,
//              if the formed semi-concepts to do not meet the minimum pruning requirments NULL is returned
//  output: none
/********************************************************************/
NCluster * MakeMatch(NCluster *lrnrConcept, RelationGraph *g, int s, int t);



/********************************************************************/
//  UpperNeighbors(NCluster c, RelationGraph g, int s, int t)
//
//  Returns the set of upper neighbors of the 2-cluster c(s,t), using the following algorithms:
//
//      -Bordat : cover procedure described in "Local Approach to Concept Generation" by Bordat et al.
//
//  Pre-Condition: (s,t) is a valid edge in g, c contains IOSets with ids s and t, algo is a valid algorithm type algo is a valid algorithm type
//  Post-Condition: none
//  returns: pointer to vector of upper neighbors for the 2-cluster c(s,t)
//  output: none
/********************************************************************/
vector<NCluster*> * UpperNeighbors_Bordat(NCluster *c, RelationGraph *g, int s, int t);

/********************************************************************/
//  LowerNeighbors(NCluster c, RelationGraph g, int s, int t, int algo)
//
//  Returns the set of lower neighbors of the 2-cluster c(s,t), using the using one of the following algorithms:
//
//      -Bordat : cover procedure described in "Local Approach to Concept Generation" by Bordat et al.
//
//
//  Pre-Condition: (s,t) is a valid edge in g, c containts iosets with ids s and t, algo is a valid algorithm type
//  Post-Condition: none
//  returns: pointer to vector of lowers neighbors for the 2-cluster c(s,t)
//  output: none
/********************************************************************/
vector<NCluster*> * LowerNeighbors_Bordat(NCluster *c, RelationGraph *g, int s, int t);


#endif	/* BORDAT_H */

