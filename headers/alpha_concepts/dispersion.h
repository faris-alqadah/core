//! Author: Faris Alqadah
/*!
 Dispersion functions to be used with alpha-concepts, to quantify the
 level of dispersion in a subspace

 \sa LatticeAlgos
 */

#ifndef DISPERSION_H
#define	DISPERSION_H

#include "../RContext.h"
#include "../OpsR.h"
#include <cmath>

//! Returns the range of the NRCluster
/*!
 Range is computed as the maximum value - minimum value. The values
 are all assumed to be stored in the rset at a[0]. This is the naive computation.
 Runs in O(n) time.
    \params a the NRcluster to compute range for
    \params params no parameters needed so pass empty vector
 */
double Range(RSet *a, vector<double> &params);


#endif


