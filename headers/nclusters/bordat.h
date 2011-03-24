/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2009
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  Implementation of an "Incremental Approach to Concept Generation" by Bordat et al.
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/

#include "../../headers/LatticeOps.h"
#include "../../headers/LatticeAlgosExternals.h"

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
//  Maxmod_Partition(ctx, c, s,t) returns a vector filled wtih the maxmods of the 2-cluster
//          specified in c by domains s and t. The maxmods will consist of elemets from the set specified
//           by domain s
//  Pre-Condition: ctx and c are not null, s and t are valid domain identifiers of the domains in ctx
//  Post-Condition: none
//  returns: the maxmods of the 2-cluster c(s,t)
//  output: none
/********************************************************************/
vector<IOSet*>* MaxMod_Partition(Context *ctx, NCluster *c, int s, int t);


list<IOSet*>* NonDominating_MaxMods(Context *ctx, NCluster * c, int s, int t,
        vector<IOSet*>* maxmods,
        vector<IOSet*>* primes, vector<IOSet*>* domInfo);

void RemoveMarked(list<IOSet*> * ndMaxMods, IOSet *marked);

void DispProgress(int ctr,int total);

void StoreCluster(NCluster *c);

NCluster * MakeMatch(NCluster *lrnrConcept, RelationGraph *g, int s, int t);

void OutputCluster(NCluster *c, RelationGraph *g);

#endif	/* BORDAT_H */

