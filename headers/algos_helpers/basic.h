//! Author: Faris Alqadah
//! General Helper functions for algorithms




#ifndef BASIC_H
#define	BASIC_H

#include "../RelationGraph.h"






//! adds a new copy of to v
void StoreCluster(vector<NCluster*> &v, NCluster *c);


//! swaps out the ith pointer in v for a new copy of c
void SwapDelete(vector<NCluster*> &v, NCluster *c, int i);




//! Sets the quality of c by calling the Quality(c,params) function
/*!
 Every Quality function should have a specification for what it expects in params vector.
  \sa QualityMeasures
    \param c the ncluster for which the quality should be computed
    \param params a vector of paramaters that will be used to compute the quality
    \param Quality a valid pointer to a function for whcih quality will be computed. The functions
 should be derived from QualityMeasures.h

 @see QualityMeasures
 */
void SetQuality(NCluster *c,vector<double> &params, double (*Quality)(NCluster*,vector<double> &) );

//! Displays progress to stdout as ctr / total
void DispProgress(int ctr,int total);

//! Writes c to the file pointed to by out and flushes the buffer for instant writing
void OutputCluster(NCluster *c, ofstream &out);
//! Writes c to the file pointed to by out using the namemap vectors and flushes the buffer for instant writing
void OutputCluster(NCluster *c, ofstream &out, vector<NameMap*> &);

#endif	/* BASIC_H */

