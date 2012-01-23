//! Author: Faris Alqadah
/*!
 Pre-processing for the hin-core. Mainly reads in an input file, and context files
  to create a relation graph that is maintained in memory

 \sa RelationGraph
 */
#ifndef _PREPROCESS_H
#define	_PREPROCESS_H

#include "RelationGraph.h"
#include "RContext.h"
#include "RelationGraphR.h"
#include "OpsR.h"

#include <utility>

using namespace std;


//! Reads an input file that specifies a full relation graph and creates the relation graph store in memory as RelationGraph object
/*!
 See the README file for the format of the input file.
 \param inputFile the path to the input file
 \sa RelationGraph
 */
RelationGraph * MakeRelationGraph(string &inputFile);

//! Reads an input file that specifies a full relation graph and creates the relation graph store in memory as RelationGraphR object
/*!
 See the README file for the format of the input file.
 \param inputFile the path to the input file
 \sa RelationGraphR
 */
RelationGraphR * MakeRelationGraphR(string &inputFile);

//! Reads an input file that only specifies a single real-valued context and stores it in memory as RContext object
/*!
 See the README file for the format of the input file.
 \param inputFile the path to the input file
 \sa RContext, MakeRContext
 */
RContext * MakeSingleRContext( string & inputFile);

//! Returns a context object initialized to all the input variables
/*!
    \param inputFile the path to the input file
    \param dId1 id of domain1
    \param dId2 id of domain2
    \param name the name of the context
    \param ctxId id of the context
    \param nm1 pointer to the name map associated with domain1
    \param nm2 pointer to the name map associated with domain2
    \param sz1 number of elements in domain 1
    \param sz2 number of elements in domain 2

 */
Context * MakeContext(string &inputFile,int dId1, int dId2, string &name, int ctxId, NameMap *nm1, NameMap *nm2,int sz1,int sz2);


//! Returns a RContext object initialized to all the input variables
/*!
    \param inputFile the path to the input file
    \param dId1 id of domain1
    \param dId2 id of domain2
    \param name the name of the context
    \param ctxId id of the context
    \param nm1 pointer to the name map associated with domain1
    \param nm2 pointer to the name map associated with domain2

 */

RContext * MakeRContext(string &inputFile,int dId1, int dId2, string &name, int ctxId, NameMap *nm1, NameMap *nm2);

//! Returns an Ncluster representing the FIMI file in inputFile
NCluster *MakeNClusterFromFimi(string &inputFile);

//! Reutrns an NRCluster from a file in spare matrix format
NRCluster *MakeNRClusterFromSparseFile(string &inputFile);


//! Tokenizes the string into a vector of string based on token
/*!
        \param str the string to be tokenized
        \param tokens emptry string vector that will be filled with tokens after function is called
        \param delim the character or string that is the delimiter to create tokens

 */
void Tokenize(const string& str, vector<string> &tokens , const string &delim);

//! Output an error message and exit the program
void Error(string &message);





#endif	/* _PREPROCESS_H */

