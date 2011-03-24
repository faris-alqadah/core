/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2008
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 * Quality measures utilized for clustering, formal concept analysis and information
 * network analysis
 *______________________________________________________________________________
 *_____________________________________________________________________________*/


#include "RelationGraph.h"
#include "Ops.h"
#include <cmath>

/********************************************************************/
// Area(NCluster *a, vector<double> &params);
//  Computes are of the ncluster as articulation node*(sum_all other nodes)
//  Pre-Condition: articulation node is first parameter, and we assume a is star shaped
//  Post-Condition:  none
//  returns: area of a star shaped ncluster
//  output: none
/********************************************************************/
double Area(NCluster *a, vector<double> &params);

/********************************************************************/
// Beta(NCluster *a, vector<double> &params);
//  Computes the beta area of a, this is described in "An effective algorithm for mining 3-clusters" by Alqadah et al.
//  Pre-Condition: articulation node is first parameter, beta is second parameter, assume a is star shaped
//  Post-Condition:  none
//  returns: beta-area of a star shaped ncluster
//  output: none
/********************************************************************/
double Beta(NCluster *a, vector<double> &params);


