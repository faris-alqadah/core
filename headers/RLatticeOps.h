//! Author: Faris Alqadah
//! Basic Lattice operations for real-valued concept lattices, see "Query-based Biclustering with Real-Valued Concept Lattices"
//! and "Efficient Enumeration of Real-Valued Concept lattices" by Alqadah et al.


#include "RContext.h"
#include "../headers/alpha_concepts/dispersion.h"
#include "../headers/alpha_concepts/consistency.h"





/*!
 This operation is first described in "Query-based Biclustering with Real-Valued Concept Lattice"
 Essentially given a query set of row (or columns) it identifies the columns that are 
 consistent in the given subspace. The implementation here will utilize a naive method to compute the prime
 nameley just following the definition.
    \param a the nrcluster that contains set s as a(s), this set is considered the query set
    \param K the full rcontext from which the subspaces are drawn from
    \param s indicates the source domain id in a to serve as the input for the prime operator
    \param t indicates the target domain
    \param d pointer to a dispersion function
    \param f pointer to a consistency function
    \param paramsD parameter vector for the dispersion function
    \param paramsF parameter vector for the consistency function

 */
IOSet *Prime_Alpha_Naive(NRCluster *a, RContext *K, int s, int t, double(*d)(NRCluster*,vector<double> &),double(*f)(NRCluster*,vector<double> &),
                    vector<double> &paramsD, vector<double> &paramsF);










