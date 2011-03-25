/********************************************************************/
//      basic helper file
//
//   Functionality is provided for basic operations with respect to
//      any algorithm, such as outputing to files etc
/********************************************************************/

#ifndef BASIC_H
#define	BASIC_H

#include "../RelationGraph.h"





/********************************************************************/
// StoreCluster(vector<NCluster*> &v, NCluster *c)
//
//   adds a new copy of c to v
//
//  Pre-Condition: c not null

//  Post-Condition:  v.push_back(c)
//  returns:none
//  output: none
/********************************************************************/
void StoreCluster(vector<NCluster*> &v, NCluster *c);

/********************************************************************/
// SwapDelete(vector<NCluster*> &v, NCluster *c, int i)
//
//   swaps out the ith pointer in v for a new copy of c
//
//  Pre-Condition: i < v.size(), i >= 0

//  Post-Condition:  v[i] = c, and previous v[i] is deallocated
//  returns:none
//  output: none
/********************************************************************/
void SwapDelete(vector<NCluster*> &v, NCluster *c, int i);



/********************************************************************/
// SetQuality(NCluster *c, vecotr<double> &params, double (*Quality)(NCluster*),vector<double>&) )
//
//  
//
//  Pre-Condition: c not null, Quality is a valid function pointer that takes the params vectors
//                      as its parametes
//  Post-Condition:  c->Quality = Quality(c,params)
//  returns:none
//  output: none
/********************************************************************/
void SetQuality(NCluster *c,vector<double> &params, double (*Quality)(NCluster*,vector<double> &) );

/********************************************************************/
// DispProgress(int ctr,int total);
//
//  Display the progress of the algorithm
//
//  Pre-Condition: none
//  Post-Condition: none
//  returns: none
//  output: ctr / total
/********************************************************************/
void DispProgress(int ctr,int total);

/********************************************************************/
//  OutputCluster(NCluster *c,ofstream &out)
//
//   Outputs c to the files defined by streams out
//
//  Pre-Condition: c is not null and out is valid ofstreams and open
//  Post-Condition:  out is flushed
//  returns: none
//  output: outputs the contents of c
/********************************************************************/
void OutputCluster(NCluster *c, ofstream &out);
/********************************************************************/
//  OutputCluster(NCluster *c,ofstream &out, vector<NameMap*> &)
//
//   Outputs c to the files defined by streams out using the namempas
//
//  Pre-Condition: c is not null and out is valid ofstreams and open
//  Post-Condition:  out is flushed
//  returns: none
//  output: outputs the contents of c
/********************************************************************/
void OutputCluster(NCluster *c, ofstream &out, vector<NameMap*> &);

#endif	/* BASIC_H */

