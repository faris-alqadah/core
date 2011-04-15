//! Author: Faris Alqadah
/*! Basic operations for RSets which are essentially real-valued vectors.
 These include some basic matrix operations and statistical ops

 \sa RSet
 */




#ifndef _OPSR_H
#define _OPSR_H


#include"NRCluster.h"
#include <algorithm>
using namespace std;

//! Assumes a is the sparse representation of a sparse matrix, and transposes it
NRCluster *TransposeSparse(NRCluster *a);

//! Returns a the set of idxs that are common to both a and b
IOSet *Common_Idxs(RSet *a, RSet *b);



//! Union two RSets by indicies, assumes that the RSets are sorted by index
RSet *Union(RSet *a, RSet *b);



#endif


