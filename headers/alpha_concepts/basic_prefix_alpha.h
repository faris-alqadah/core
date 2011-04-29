//! Author: Faris Alqadah
/*!
 Basic prefix tree enumeration or alpha-semii-concepts as described in "Query-Based Bi-clustering"
 
 */

#ifndef BASIC_PREFIX_H
#define	BASIC_PREFIX_H


#include "alpha_concepts_algos.h"

class BasicPrefix : public AlphaConceptsAlgos{
public:
BasicPrefix():AlphaConceptsAlgos() {};



//! Inteface for CHARM-like prefix tree search for the best alpha-concept that matches the query
/*!
 Functions assumes that the s,t, and K parameters of the class have been set.

    \param query the ids/indices of the query objects
  

 So far only works with alpha sigma consistency and range, but should increase later.
 When changes are made, inidicate here what class variables must be set.

*/
void Qbbc_Prefix_Search(IOSet *query);

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
 void Enumerate_Charm(list<IOSet*> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax);


 


 //! Compute the "Range Intersection" of two supporting sets
/*!
 Sets the quality of supSetRslt to the average range all of supporting rows/columns
 
    \param supSet1: indicies or ids of first supporting set
    \param supSet2: indicies or ids of second supporting set
    \param minMax1: indices of min and max values of each supporting set object with respect to the subspace1
    \param minMax2: indicies of min and max values of each supporting set object with respect to the subspace2
    \param supSetRslt: place holder for result, should be initialized but empty
    \param: minMaxRslt: place holder for result, should be initialized but empty
*/
 void Range_Intersect(IOSet *supSet1, IOSet *supSet2, NCluster* minMax1, NCluster* minMax2,
                      IOSet *supSetRslt, NCluster* minMaxRslt
                     );
 //! Applies "CHARM"-like optimization to the current prefix, to perform closure and generate next search subspace
/*!
    \param k this indicates where the tail should begin (k >= 0 and LESS than |tail| (see paper)
    \param tail ncluster of tails for currPrefix
    \param  tailSupSet ncluster of supporting sets, one for each node in tail
    \param tailMinMax vector of nucluster min max indicies for all supporting sets of all tail nodes
    \param newTail the newly generated tail, this should be initialized but empty
    \param newTailSupSet newly generated supporting set, this should be initialized but empty
    \param newTailMinMax newly generated minmax vectors, this should be intiialized but empty
*/
 void Charm_Optimize(int k,
                    list<IOSet *> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax,
                    list<IOSet *> &newTail, list<IOSet *> &newTailSupSet, list<NCluster *> &newTailMinMax );

 void Construct_First_Level(int k,
                     list<IOSet *> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax,
                    list<IOSet *> &newTail, list<IOSet *> &newTailSupSet, list<NCluster *> &newTailMinMax );
};
#endif	/* BASIC_PREFIX_H */

