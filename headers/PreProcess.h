//! Author: Faris Alqadah
/*!
 Pre-processing for the hin-core. Mainly reads in an input file, and context files
  to create a relation graph that is maintained in memory

 \sa RelationGraph
 */
#ifndef _PREPROCESS_H
#define	_PREPROCESS_H

#include "RelationGraph.h"

#include <utility>

using namespace std;


//! Reads an input file that specifies a full relation graph and creates the relation graph store in memory as RelationGraph object
/*!
 See the README file for the format of the input file.
 \param inputFile the path to the input file
 \sa RelationGraph
 */
RelationGraph * MakeRelationGraph(string &inputFile);

//! Returns a context object initialized to all the input variables
/*!
    

 */

Context * MakeContext(string &inputFile,int dId1, int dId2, string &name, int ctxId, NameMap *nm1, NameMap *nm2);


/********************************************************************/
// MakeNClusterFromFimi(string inputFile)
//  Pre-Condition: inputFile is valid path
//  Post-Condition: none
//  returns: creates an n-cluster by reading in a fimi file
//  output: none
/********************************************************************/
NCluster *MakeNClusterFromFimi(string &inputFile);



/********************************************************************/
// Tokenize(string& str, vector<string>& tokens, const string& delimiters)
//  Pre-Condition: none
//  Post-Condition: tokens contains tokens from str as delimited by delimiters
//  returns: none
//  output: none
/********************************************************************/
void Tokenize(const string&, vector<string>& , const string&);

/********************************************************************/
// Error(string message)
//  Pre-Condition: none 
// Post-Condition:error message is displayed and program is terminated
//  returns: none
//  output: none
/********************************************************************/
void Error(string &message);





#endif	/* _PREPROCESS_H */

