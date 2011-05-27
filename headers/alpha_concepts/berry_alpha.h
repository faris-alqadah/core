//! Author: Faris Alqadah
/*!
 This is a derived class of ALphaConceptAlgos and implements Lattice based algorithms
 based on the paper "A local approach to Concept Generation" by Berry et al. extended to variance based
 bi-clusters in real values.

 */

#ifndef BERRYALPHA_H
#define	BERRYALPHA_H

#include "alpha_concepts_algos.h"
#include "basic_prefix_alpha.h" // for range intersection


class BerryAlphaAlgos : public AlphaConceptsAlgos{
public:
BerryAlphaAlgos():AlphaConceptsAlgos() {};


//! Computes the variance-based maxmods of the sub-context rctx(A-c(s),c(t)) where domain s takes the place of attributes (see berry et al.)
/*!
 This is a generalization of the Maxmod-Partition algorithm described in berry et al.
    \param rctx the full context which corresponds to g(s,t)
    \param c contains the concept c(s,t)
    \param s the "source id" of the domain for which the concept c(s,t) is the current concept
    \param t the "target id" of the domain for which the concept c(s,t) is the current concept
 */
vector<IOSet*>* MaxMod_Partition(RContext *rctx, NCluster *c, int s, int t);

#endif	

