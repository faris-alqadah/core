//! Author: Faris Alqadah
//! General Helper functions for algorithms




#ifndef BASIC_H
#define	BASIC_H

#include "../core/RelationGraph.h"







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


/*!
 *     Adds "edges" from this bi-cluster in the hash table of edges as
 *    c=(A,B) A=a1...an B=b1...bm all ai_bj are added as edges
 *    Edge ids are generated as cantor pair number of (A,B)
 *
 *
 */
void AddBiCluster_Edges(NCluster *c,int id1, int id2, map<int, pair<int,int> > &theMap);

/*!
 * Output the edges in the basic cytoscape pairs format
 *
 *  @param theMap maps edge ids to a pair. The pair contains two object ids representing the edge (e_1,e_2)
 *  @param out stream to the file to be output to
 *  @param id1 id of the first domain from which object ids are drawn from in edges(e_1,e_2)
 *  @param id2 id of the second domain from which object ids are drawn from in edges(e_1,e_2)
 *  @param nm vector of pointers to namemaps. It is assumed that name maps with domain ids id1 and id2
 *              exist in this vector, and they will be used for output
 */
void OutputEdges( map<int, pair<int,int> > &theMap, ofstream &out,int id1, int id2, vector<NameMap*>& nm);

/*!
 * Output enumerated edges as a full binary matrix, where non-edges output as 0
 *
 * 
 *  @param theMap maps edge ids to a pair. The pair contains two object ids representing the edge (e_1,e_2)
 *  @param out stream to the file to be output to
 *  @param sz1 total number of rows, or total number of objects in domain 1, from which e_1s are drawn from
 *  @param sz2 total number of coulmns, or total number of objects in domain 2, from which e_2s are drawn from
 */
void OutputEdgesBinaryMatrix( map<int, pair<int,int> > &theMap, ofstream &out, int sz1, int sz2);

#endif	/* BASIC_H */

