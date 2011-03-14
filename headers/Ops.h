/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2008
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Basic operations for sets. Also included are some set similarity measures
 * such as jaccard and Sorensen coefficent.
 *
 *External variables included to keep track of the number of times these
 * basic operations are invoked
 *
 * Any template functions for set / vector operations should also be placed here
 *______________________________________________________________________________
 *_____________________________________________________________________________*/
 

#ifndef _OPS_H
#define _OPS_H


#include"IOSet.h"
#include <algorithm>
using namespace std;


//external variables that can
//be used to keep track of the number of basic
//set operations
extern int numIntersection; //number of intersections performed
extern int numSubset; //number of subset checks performed
extern int numUnion; //number of set unions performed
extern int numDifference; //number of set differences performed



/********************************************************************/
// DstryVector (vector<t*> *v>: deallocated and destroy all elements of the vector pointed to by v
//  Pre-Condition: none
//  Post-Condition:  v is destroyed and so are all of its elements
//  returns: none
//  output: none
/********************************************************************/
template <class t>
void DstryVector( vector<t*> *v){
    for(int i=0; i < v->size(); i++){
        if (v->at(i) != NULL){
            delete v->at(i);
            v->at(i) = NULL;
        }
    }
    delete v;
}

/********************************************************************/
// Intersect(IOSet *a, IOSet *b): Set intersection
//  Pre-Condition: a and b non-null AND a and b are sorted
//  Post-Condition:  numIntersection++
//  returns: set intersection of a and b
//  output: none
/********************************************************************/
IOSet * Intersect( IOSet*, IOSet*);

/********************************************************************/
// Difference(IOSet *a, IOSet *b): Set difference
//  Pre-Condition: a and b non-null AND a and b are sorted
//  Post-Condition:  numDifference++
//  returns: set difference of a and b
//  output: none
/********************************************************************/
IOSet * Difference( IOSet*,  IOSet*);

/********************************************************************/
// SymmDifference(IOSet *a, IOSet *b): Symmetric Set difference
//  Pre-Condition: a and b non-null AND a and b are sorted
//  Post-Condition:  numDifference incremented by number of operations
//  returns: set symmetric set difference of a and b
//  output: none
/********************************************************************/
IOSet *SymmDifference( IOSet*, IOSet*);

/********************************************************************/
// Union(IOSet *a, IOSet *b): Set union
//  Pre-Condition: a and b non-null AND a and b are sorted
//  Post-Condition:  numUnion++
//  returns: set union of a and b
//  output: none
/********************************************************************/
IOSet* Union( IOSet*,  IOSet*);

/********************************************************************/
// Contains(IOSet *a, IOSet *b): Contains
//  Pre-Condition: a and b non-null AND a and b are sorted
//  Post-Condition:  numSubset++
//  returns: true if b is subset of a false otherwise
//  output: none
/********************************************************************/
bool Contains( IOSet*,  IOSet* );

/********************************************************************/
// ProperContains(IOSet *a, IOSet *b): Contains
//  Pre-Condition: a and b non-null AND a and b are sorted
//  Post-Condition:  numSubset++ (possibly)
//  returns: true if b is proper subset of a false otherwise
//  output: none
/********************************************************************/
bool ProperSubSet(IOSet *, IOSet*);

/********************************************************************/
// PercentOverlap(IOSet *a, IOSet *b): Compute Jaccard Coefficent
//  Pre-Condition: a and b non-null AND a and b are sorted
//  Post-Condition:  intersection and union counters incremented
//  returns: the Jaccard coefficient between a and b
//  output: none
/********************************************************************/
double PercentOverlap( IOSet*, IOSet* );


/********************************************************************/
// PercentOverlap_Sorensen(IOSet *a, IOSet *b): Compute Sorensen Coefficent
//  Pre-Condition: a and b non-null AND a and b are sorted
//  Post-Condition:  intersection and union counters incremented
//  returns: the  Sorensent coefficient between a and b
//  output: none
/********************************************************************/
double PercentOverlap_Sorensen( IOSet*, IOSet* );



#endif	/* _OPS_H */


