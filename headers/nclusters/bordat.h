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


#ifndef BORDAT_H
#define	BORDAT_H



void Enum_NConcepts_Bordat(NCluster *a, RelationGraph *g, int s, int t);

list<IOSet*>* MaxMod_Partition(Context *ctx, NCluster *c, int s, int t);

list<IOSet*>* NonDominating_MaxMods(Context *ctx, NCluster * c, int s, int t,
        vector<IOSet*>* maxmods,
        vector<IOSet*>* primes, vector<IOSet*>* domInfo);

void RemoveMarked(list<IOSet*> * ndMaxMods, IOSet *marked);

#endif	/* BORDAT_H */

