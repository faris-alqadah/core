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




//! The QBBC algorithm for query based clustering
/*!
 Functions assumes that the s,t, and K parameters of the class have been set.

    \param query the ids/indices of the query objects
    \param hits an initially empty vector of nclusters to hold the hit(s) (ie the result)

 So far only works with alpha sigma consistency and range, but should increase later.
 When changes are made, inidicate here what class variables must be set.

*/


void Qbbc(IOSet *query, vector<NCluster*> &hits);

private :

    //! Inteface for CHARM-like prefix tree search for the best alpha-concept that matches the query
/*!
 Functions assumes that the s,t, and K parameters of the class have been set.

    \param query the ids/indices of the query objects


 So far only works with alpha sigma consistency and range, but should increase later.
 When changes are made, inidicate here what class variables must be set.

*/
void Qbbc_Prefix_Search(IOSet *query);

//! Compute alpha-semi concepts using basic prefix tree enumeration
 /*!
   
    \param tail an ncluster of iosets that represent the ids or indicies of objects in the tail
    \param tailSupSet an ncluster of iosets of the objects in the support set of each element of tail
    \param tMinMax vector of NClusters in which each IOSet only has 2 elements that indicate the index of the minimum and maximum element for each object row in the tail supporting set

 So far only works with alpha sigma consistency and range, but should increase later.
 When changes are made, inidicate here what variables must be set in LatticeAlgos.

*/
 void Enumerate_Charm(list<IOSet*> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax);

//! The alpha charm algorithm for star-shaped information networks...
void Enumerate_Star_Charm(list< list<IOSet*>* > &tails, list< list<IOSet*> *> &tailSupSet, list < list<NCluster*> *> &tailMinMax, IOSet *tailRun);


//! Prunes the second tails based on the first
void Prune_Tails(list<IOSet*> &tail1, list<IOSet*> &tailSupSet1, list<NCluster*> &tailMinMax1,
                 list<IOSet*> &tail2, list<IOSet*> &tailSupSet2, list<NCluster*> &tailMinMax2);






//! Compute the "Charm" upper neighbors of alpha-concepts by applying single step of breadth-first CHARM

 /*!

 \param tail an ncluster of iosets that represent the ids or indicies of objects in the tail
    \param tailSupSet an ncluster of iosets of the objects in the support set of each element of tail
    \param tMinMax vector of NClusters in which each IOSet only has 2 elements that indicate the index of the minimum and maximum element for each object row in the tail supporting set

 So far only works with alpha sigma consistency and range, but should increase later.
 When changes are made, inidicate here what variables must be set in LatticeAlgos.

*/
 vector<NCluster*> * Charm_UpperNeighbors(list<IOSet*> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax);




 //! Compute the "Range Intersection" of two supporting sets
/*!
 Sets the quality of supSetRslt to the average range all of supporting rows/columns
 This function assumets
  K,s,t parameters are already predefined by algorithm interface.
 
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


 //! Find the "Range Closure" of a prefix and supporting set, along with the Charm upper neighbors
 /*!
  The closure is defined as the set of objects K[t] - prefix, that is
  also consistent in the space K[c+Prefix,supSet]. So far implementation is relativley naive.
  The charm upper neighbors are defined as those concepts under the same prefix branch.

  This function assumets
  K,s,t parameters are already predefined by algorithm interface.

  \param prefix the current prefix set of the bicluster K[prefix,supset]. This set will be update with the closure.
  \param supSet the current supporting set
  \param minMax indicies of min and max values of each supporting set object in supSet
 */
vector<NCluster*> * Range_Closure_Charm_Neighbors(IOSet * prefix,IOSet *supSet, NCluster *minMax);

 //! Given inital query objects, creates the prefix list,supporting sets and min-max indiices of those support sets for the prefix objects
 /*!
    This function assumets
  K,s,t parameters are already predefined by algorithm interface.
  \param query the  objects for which supporting sets and min-max indicies should be generated
  \param prefix an initially empty list that will hold prefix objects after function exectuiton
  \param supSets an initially empty list that will hold supporting sets after function exectuiton
  \param minMax an intially empty list that will hold the min-max indicies after function execution
  */
 void Make_Init_SupSets_MinMaxIdxs(IOSet *, list<IOSet*> &, list<IOSet*> &, list<NCluster*> &);


 //!Given query objects and their supporting sets calculates the min-max idxs
 /*!
   This function assumets
  K,s,t parameters are already predefined by algorithm interface.
  \param query the query objects
  \param supSets the supporting set objects
  */
 NCluster * Get_Min_Max_Idxs(IOSet *query, IOSet *supSets);

 void Construct_First_Level(int k,
                         list<IOSet *> &tail, list<IOSet*> &tailSupSet, list<NCluster*> &tailMinMax,
                         list<IOSet *> &newTail, list<IOSet *> &newTailSupSet, list<NCluster *> &newTailMinMax );
};
#endif	/* BASIC_PREFIX_H */

