//! Author: Faris Alqadah
/*!
 This is a derived class of LatticeAlgos and implements Lattice based algorithms
 based on the paper "A local approach to Concept Generation" by Berry et al.
 Essentialy, n-clusters as described in the disseration "Mining multi-domain information networks"
 are mined by generalizing the algorithm presented by Berry et al. to a star shaped network.
 In addition, functions for computing the upper and lower neighbors of a given concept in a single
 context are also provided.

 */
#ifndef BERRY_H
#define	BERRY_H

#include "../core/LatticeAlgos.h"
class BerryLatticeAlgos : public LatticeAlgos{
public:
BerryLatticeAlgos():LatticeAlgos() {};
    //! Inteface for Berry-based algorithms that compute the n-concepts or n-clusters from a star shaped HIN
/*!
    \param g pointer to relation graph for which computation will take place. This must be a star-shaped relation graph
    \param lrnrContext the id of the context that will server as the learner (See "An effective algorithm for 3-clustering" by Alqadah et al.)

 The following external variables from LatticeAlgosExternals should be set:

 @see enumerationMode
 @see qualityMode
 @see ovlpMode
 @see pruneMode
 @see ovlpThresh
 @see topKK

*/
void Star_N_Concepts(RelationGraph *g,int lrnrContext);

//! Computes the upper neighbors of a concept using the "Cover" algorithm in Berry et al.
/*!
    \param c the n-cluster containg the concept as c(s,t)
    \param g the relation graph for which (s,t) should be a context
    \param s the "source id" of the domain for which the concept a(s,t) is the current concept, the upper neighbors will be super-sets of domain s
    \param t the "target id" of the domain for which the concept a(s,t) is the current concept, the upper neighbors will be sub-sets of domain t
 */

vector<NCluster*> * UpperNeighbors(NCluster *c, RelationGraph *g, int s, int t);

//! Computes the lower neighbors of a concept using the "Cover" algorithm in Berry et al.
/*!
    \param c the n-cluster containg the concept as c(s,t)
    \param g the relation graph for which (s,t) should be a context
    \param s the "source id" of the domain for which the concept a(s,t) is the current concept, the lower neighbors will be sub-sets of domain s
    \param t the "target id" of the domain for which the concept a(s,t) is the current concept, the lower neighbors will be super-sets of domain t
 */
vector<NCluster*> * LowerNeighbors(NCluster *c, RelationGraph *g, int s, int t);

private :
//! Computes n-clusters utilizing the algorithm described in "A local approach to Concept generation" berry et al. as a basis
/*!
 Performs the berry depth first search version
    \param a the current ncluster which contains the current concept for this search level (see berry et al.)
    \param marked corresponds to the marked set (see berry et al.)
    \param g the relation graph, should be star shaped
    \param s the "source id" of the domain for which the concept a(s,t) is the current concept
    \param t the "target id" of the domain for which the concept a(s,t) is the current concept
 */
void Enum_NConcepts_Berry(NCluster *a, RelationGraph *g, IOSet *marked, int s, int t);

//! Computes the maxmods of the sub-context ctx(A-c(s),c(t)) where domain s takes the place of attributes (see berry et al.)
/*!
 This is a generalization of the Maxmod-Partition algorithm described in berry et al.
    \param ctx the full context which corresponds to g(s,t)
    \param c contains the concept c(s,t)
    \param s the "source id" of the domain for which the concept c(s,t) is the current concept
    \param t the "target id" of the domain for which the concept c(s,t) is the current concept
 */
vector<IOSet*>* MaxMod_Partition(Context *ctx, NCluster *c, int s, int t);

//! Returns the set of non-dominating maxmods given a set of maxmods
/*!
 In addtion the primes of each maxmod is computed, while we keep track of what maxmods
 dominate each non-dominating maxmod

    \param ctx The full context for which the non-dominating maxmods will be computed
    \param c the n-cluster that contains the concept with which the maxmods were generated
    \param maxmods list of maxmods
    \param primes this vector should be empty, and the routine will store the primes of the maxmods in here
    \param domInfo this vector should be empty, the route will store the index to which maxmods dominate every maxmod here

 */
list<IOSet*>* NonDominating_MaxMods(Context *ctx, NCluster * c, int s, int t,
        vector<IOSet*>* maxmods,
        vector<IOSet*>* primes, vector<IOSet*>* domInfo);

//! Sets the flag to true for each maxmod which is non-domninating
void RemoveMarked(list<IOSet*> * ndMaxMods, IOSet *marked);




//! Attempts to find mathcing semi-concepts to lrnrConcept(s,t) in the rest of g
/*!
 Assumes g is a star-shaped HIN. Then tries to find matching semi-concepts to lrnrConcept(s,t) in g
    \param lrnrConcept the learner concept that we are trying match is lrnrConcept(s,t)
    \param g the star-shaped relation graph
    \param s the domain id of the articulation node and concept lrnrConcept(s,t)
    \param t the domain id of the other domain that makes up the learner concept

 */
NCluster * MakeMatch(NCluster *lrnrConcept, RelationGraph *g, int s, int t);





};


#endif	/* BERRY_H */

