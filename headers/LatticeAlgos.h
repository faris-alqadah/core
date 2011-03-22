/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2009
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *   Concept lattice enumeration and hin-clustering algorithms derived from formal concept anaylsis.
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/

#ifndef LATTICEALGOS_H
#define	LATTICEALGOS_H

#include "LatticeOps.h"
#include "LatticeAlgosExternals.h"
#include "nclusters/bordat.h"

#include<fstream>
using namespace std;
/************************ENUMERATION ALGORITHMS********************************************/
//  All enumeration algorithms contain many different options.
//  These options should be set by the user by setting the different externally
//  available variables as described in LatticeAlgosExternals.h .
//
//   enumerationMode must be set or it defaults to ENUM_MEM (LatticeAlgosExternals)
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


#endif	/* LATTICEALGOS_H */

