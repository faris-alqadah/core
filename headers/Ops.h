/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2008
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Operations for search
 *
 *
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/
 

#ifndef _OPS_H
#define _OPS_H


#include"IOSet.h"
//#include"Globals.h"
#include <algorithm>
using namespace std;


//stats of operations

extern int numIntersection;
extern int numSubset;
extern int numUnion;
extern int numDifference;




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


////////////////////////////////////////////////////////////////////////////////
//Intersects two IOSets
//Precondition: both IOSets are sorted
//Postcondition: returns the intersection of the IOSets
////////////////////////////////////////////////////////////////////////////////
IOSet * Intersect( IOSet*, IOSet*);

////////////////////////////////////////////////////////////////////////////////
//Set difference of two IOSets
//Precondition: both IOSets are sorted
//Postcondition: returns the set difference of the IOSets
////////////////////////////////////////////////////////////////////////////////
IOSet * Difference( IOSet*,  IOSet*);

////////////////////////////////////////////////////////////////////////////////
//Set symmetric difference of two IOSets
//Precondition: both IOSets are sorted
//Postcondition: returns the set symmetric difference of the IOSets
////////////////////////////////////////////////////////////////////////////////
IOSet *SymmDifference( IOSet*, IOSet*);

////////////////////////////////////////////////////////////////////////////////
//Set union of two IOSets
//Precondition: both IOSets are sorted
//Postcondition: returns the set union of the IOSets
////////////////////////////////////////////////////////////////////////////////
IOSet* Union( IOSet*,  IOSet*);

////////////////////////////////////////////////////////////////////////////////
//Returns true if the first IOSet contains the second IOSet
//Precondition: both IOSets are sorted
//Postcondition: returns true if IOSet 1 is a superset of IOSet 2
////////////////////////////////////////////////////////////////////////////////
bool Contains( IOSet*,  IOSet* );

////////////////////////////////////////////////////////////////////////////////
//Returns true if the first IOSet is a proper subset of the second
//Precondition: both IOSets are sorted
//Postcondition: returns true if IOSet 1 is a proper subset of IOSet 2
////////////////////////////////////////////////////////////////////////////////
bool ProperSubSet(IOSet *, IOSet*);

////////////////////////////////////////////////////////////////////////////////
//Returns the Jaccard Coefficient of the two IOSets
//Precondition: both IOSets are sorted
//Postcondition: Jaccard coefficient of the two IOSets
////////////////////////////////////////////////////////////////////////////////
double PercentOverlap( IOSet*, IOSet* );


////////////////////////////////////////////////////////////////////////////////
//Returns the Jaccard Coefficient of the two IOSets
//Precondition: both IOSets are sorted
//Postcondition: Jaccard coefficient of the two IOSets
////////////////////////////////////////////////////////////////////////////////
double PercentOverlap_Sorensen( IOSet*, IOSet* );



#endif	/* _OPS_H */


