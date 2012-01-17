//! Author: Faris Alqadah
/*!
 This is a derived class of LatticeAlgos and implements random sampling of concepts as based on the paper "Direct Local Pattern Sampling by Efficient Two-Step Random procedures"

 */

#ifndef _RANDOM_SAMPLE_H
#define	_RANDOM_SAMPLE_H

#include "../core/LatticeAlgos.h"
#include "../../headers/core/Ops.h"


class NClusterRandomSample : public LatticeAlgos{
public:
NClusterRandomSample():LatticeAlgos() {};
    //! Inteface for  random sampling algorithms for n-clustsers




//! Randomly samples an n-cluster from a HIN where each subspace in domain has high frequency support
/*!
    \param g the relation graph or HIN
 */
void NClusterFreq(RelationGraph *g);


//! Compute weights associated with each object in domain t to perform random frequecy based sampling of domain s
/*!
    \param c the context  which (s,t) should be the domains
    \params s the domain from which a subspace should be randomly sampled
    \params t id of other domain in context c
 *
 */
vector<long double> * GetFreqWeights(Context *c, int s, int t);

//! Compute weights associated with each object in domain t to perform random frequecy based sampling of domain s. However frequency
//! is computed over a subcontext in whcih objects (t,:) is intersected with sum(t,:).
/*!
    \param c the context  which (s,t) should be the domains
    \params s the domain from which a subspace should be randomly sampled
    \params t id of other domain in context c
 *
 */
vector<long double> * GetFreqWeightsSub(Context *c, int s, int t,NCluster *sub);


//! Compute weights associated with each object in domain t to perform random area based sampling of domain s
/*!
    \param c the context  which (s,t) should be the domains
    \params s the domain from which a subspace should be randomly sampled
    \params t id of other domain in context c
 *
 */
vector<long double> * GetAreaWeights(Context *c, int s, int t);

//! Randomly select a subspace from domain s in context c ~ frequency or support
/*!
    \param c the context  which (s,t) should be the domains
    \params s the domain from which a subspace should be randomly sampled
    \params t id of other domain in context c
    \params weights the weights corresponding to objects in t
 *
 */
IOSet* SubspaceFreq(Context *c, int s, int t, vector< long double> &weights);

//! Randomly a subspace from domain s in context c ~ area
/*!
    \param c the context  which (s,t) should be the domains
    \params s the domain from which a subspace should be randomly sampled
    \params t id of other domain in context c
    \params weights the weights corresponding to objects in t
 *
 */
IOSet* SubspaceArea(Context *c, int s, int t, vector<long double> &weights);


//! Randomly select a subspace from a star shaped HIN ~ frequncy or support
/*!
   \param g the hin
   \param s id of the central node in the star shaped HIN
 */

NCluster* SubspaceStarShapedFreq(RelationGraph *g, int s );


//! Randomly select a subspace from a star shaped HIN ~ frequncy or support
//! However computation is performed over a subcontext in whcih objects (t,:) is intersected (s,:).
/*!
   \param g the hin
   \param s id of the central node in the star shaped HIN
   \param sample the current sampled n-cluster that should contain objects in domain s
 */
NCluster * SubspaceStarShapedFreqSample(RelationGraph *g, int s, NCluster *sample );

//! Randomly select a subspace from a star shaped HIN ~ area
/*!
   \param g the hin
   \param s id of the central node in the star shaped HIN
 */

NCluster* SubspaceStarShapedArea(RelationGraph *g, int s );


//! Randomly select a subspace from a HIN
/*!
   \param g the hin
   \param s id of the central node in the star shaped HIN
 */
NCluster * SubspaceFreqNetwork(RelationGraph *g, int s);

};
#endif	/* _RANDOM_SAMPLE_H */

