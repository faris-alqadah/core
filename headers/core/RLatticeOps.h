//! Author: Faris Alqadah
//! Basic Lattice operations for real-valued concept lattices, see "Query-based Biclustering with Real-Valued Concept Lattices"
//! and "Efficient Enumeration of Real-Valued Concept lattices" by Alqadah et al.


#include "RContext.h"
#include "../alpha_concepts/dispersion.h"
#include "../alpha_concepts/consistency.h"





/*!
 This operation is first described in "Query-based Biclustering with Real-Valued Concept Lattice"
 Essentially given a query set of row (or columns) it identifies the columns that are 
 consistent in the given subspace. The implementation here will utilize a naive method to compute the alpha set,
 nameley just following the definition.
    \param K the full rcontext from which the subspaces are drawn from
    \param query the indices of the query objects
    \param s indicates the source domain id, in other words the query objects are derived from this domain
    \param t indicates the target domain (in other words the other domain of K is this domain id)
    \param alpha the alpha value used for computing the operation
    \param d pointer to a dispersion function
    \param f pointer to a consistency function
    \param p pointer to helper function to create the paramater vector for the corresponding consistency function

 */
IOSet *Prime_Alpha_Naive(RContext *K, IOSet *query, int s, int t,double alpha,
                            double(*d)(RSet*,vector<double> &),
                            double(*f)(RSet*,vector<double> &),
                            void(*p)(RContext *,IOSet *, int, int,int, vector<double> &));










