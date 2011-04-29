//! Author: Faris Alqadah
//! General Helper functions specifically for the alpha concepts algorithms

#ifndef HELPERS_H
#define	HELPERS_H


#include "../RContext.h"
#include "../../headers/alpha_concepts/dispersion.h"

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




#endif	/* HELPERS_H */

