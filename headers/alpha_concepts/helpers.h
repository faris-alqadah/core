//! Author: Faris Alqadah
//! General Helper functions specifically for the alpha concepts algorithms

#ifndef HELPERS_H
#define	HELPERS_H


#include "../core/RContext.h"
#include "../../headers/alpha_concepts/dispersion.h"
#include "../core/Ops.h"

//! Constructs the paramater vector  for the alpha sigma consistency function
/*!
 The params vector should have the alpha value as the first element and the standard deviation in ascending order
 of each row/column corresponding to the query vairables as the rest of the elements
    \params K the context for which the row/column standard deviations should be computed
    \params query the set of query objects
    \params s the id of the domain corresponding to the query objects
    \params t the id of the other domain (non-query) in the context K
    \param tRow the id of the row (or column) for which support for the query objects is being computed
    \params the vector of parameters that will modified,should contain the alpha value already!

 \sa RLatticeOps
 \sa AlphaSigma

 */
void Construct_AlphaSigma_Params(RContext *K, IOSet* query, int s, int t, int tRow, vector<double> &params);

//! Constructs the paramater vector  for the max space uniform consistency function
/*!
 The params vector should have the alpha value as the first element and the cardinality of the query set as each other element
    \params K the context for which the row/column standard deviations should be computed
    \params query the set of query objects
    \params s the id of the domain corresponding to the query objects
    \params t the id of the other domain (non-query) in the context K
    \param tRow the id of the row (or column) for which support for the query objects is being computed
    \params the vector of parameters that will modified,should contain the alpha value already!

  \sa RLatticeOps
  \sa MaxSpaceUniform
 */
void Construct_MaxSpaceUniform_Params(RContext* K, IOSet *query, int s,int t, int tRow, vector<double> &params);

//! Ouptut a list of iosets that represent a tail
void Output_Tail(list<IOSet*> &tail);



//! update vectors of iterators for use in Star Charm
void Update_AllTails_Iterators_Star_Charm(list< list<IOSet*>* > &tails, list< list<IOSet*> *> &tailSupSet, list < list<NCluster*> *> &tailMinMax);


//! Delete lists of lists of tails, supporting sets, and min max indices
void Delete_New_Tails_Star_Charm(list< list<IOSet*>* > &newTails, list< list<IOSet*> *> &newSupSets, list < list<NCluster*> *> &newMinMaxs);

#endif	/* HELPERS_H */

