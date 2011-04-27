//! Author: Faris Alqadah
/*!
 Basic prefix tree enumeration or alpha-semii-concepts as described in "Query-Based Bi-clustering"
 
 */

#ifndef BASIC_PREFIX_H
#define	BASIC_PREFIX_H


#include "../LatticeAlgos.h"
#include "../NCluster.h"
class BasicPrefixAlphaAlgos : public LatticeAlgos{
public:
BasicPrefixAlphaAlgos():LatticeAlgos() {};


//! Inteface for the basic prefix tree enumeration of alpha semi-concepts
/*!
    \param K pointer to the real-valued context
    \param query the query set
    \param qId id of the domain from which the query set is derived

 So far only works with alpha sigma consistency and range, but should increase later.
 When changes are made, inidicate here what variables must be set in LatticeAlgos.

*/
void BasicPrefix(RContext *K,IOSet *query, int qId);

private :
//! Compute alpha-semi concepts using basic prefix tree enumeration
 /*!
    \param prefix the index of objects in the prefix set
    \param supSet index of objects in the supporting set of the prefix
    \param pMinMax NCLuster in which each IOSet only has 2 elements indicating the index of the minium and maxmimum element for each object row in the supporting set
    \param tail an ncluster of iosets that represent the ids or indicies of objects in the tail
    \param tailSupSet an ncluster of iosets of the objects in the support set of each element of tail
    \param tMinMax vector of NClusters in which each IOSet only has 2 elements that indicate the index of the minimum and maximum element for each object row in the tail supporting set

 So far only works with alpha sigma consistency and range, but should increase later.
 When changes are made, inidicate here what variables must be set in LatticeAlgos.

*/
 void Enumerate_BasicPrefix(IOSet *prefix, IOSet *supSet, NCluster * pMinMax, NCluster * tail, NCluster *tailSupSet, vector< NCluster* > &tMinMax );


 //! Compute the "Range Intersection" of two subspaces
 /*!
  The range intersection of two subspaces performs a set intersection of the indicies of the subspace while simultaneoulsy re-computing
  the range for each
    \param prefix the index of objects in the prefix set
    \param supSet index of objects in the supporting set of the prefix
    \param pMinMax NCLuster in which each IOSet only has 2 elements indicating the index of the minium and maxmimum element for each object row in the supporting set
    \param tail an ncluster of iosets that represent the ids or indicies of objects in the tail
    \param tailSupSet an ncluster of iosets of the objects in the support set of each element of tail
    \param tMinMax vector of NClusters in which each IOSet only has 2 elements that indicate the index of the minimum and maximum element for each object row in the tail supporting set

 So far only works with alpha sigma consistency and range, but should increase later.
 When changes are made, inidicate here what variables must be set in LatticeAlgos.

*/
 void Merge_Prefix_Intersect(IOSet *supSet1, IOSet *supSet2, vector<NCluster*> &minMax1, vector<NCluster*> &minMax2,
                      IOSet *supSetRslt, vector<NCluster*> &minMaxRslt);

};
#endif	/* BASIC_PREFIX_H */

