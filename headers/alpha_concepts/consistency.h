//! Author: Faris Alqadah
/*!
 Consistency functions to be used with alpha-concepts, these serve as
 'checking' functions to indicate if the level of dispersion is truly significant
 \sa LatticeAlgos
 */

#ifndef CONSISTENCY_H
#define	CONSISTENCY_H

#include "../RContext.h"
#include "../OpsR.h"
#include <cmath>

//! Computes the alpha-sigma consistency function (see "Query-based Bi-clustering with real-valued concept lattices")
/*!

    \params a the NRcluster to compute alpha-sigma for
    \params params params[0] should contain the alpha value, params[1] contains the standard deviation of the row / column of interest
 */
double AlphaSigma(RSet *a, vector<double> &params);

//! Computes the maximum spacing uniform estimator consistency function (see "Query-based Bi-clustering with real-valued concept lattices")
/*!

    \params a the NRcluster to compute the maximum spacing uniform estimator for
    \params params params[0] should contain the alpha value, params[1] the cardinality of M (see paper), params[2] range of the subspace (see paper)
 */
double MaxSpaceUniform(RSet *a, vector<double> &params);

#endif

