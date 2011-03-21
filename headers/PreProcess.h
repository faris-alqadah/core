/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2008
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  Pre-processing for the hin-core. Mainly reads in an input file, and context files
 * to create a relation graph that is maintained in memory
 *
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/

#ifndef _PREPROCESS_H
#define	_PREPROCESS_H

#include "RelationGraph.h"

#include <utility>

using namespace std;

/********************************************************************/
// MakeRelationGraph(string inputFile):
//
// Reads an input file that specifies a full relation graph and creates
// a RelationGraph object.
// Format of input file:
//num_domains
// num_contexts
// domain1_name
//path_to_domain1_name_map
//.
//.
//.
//domainn
//path_to_domainn_name_map
//domainx--domainy
//path_to_domainx_domainy_fimi_file
//.
//.
//.
//###########################################
//Domain names are specifed first.
//On the next line the path to name map file of the domain
//After all domains specified contexts should be specified
//Contexts are specified as the name of domain1 followed by name of domain2. If these
//names do not match the names speficied in the first section error is thrown
//Next line is the path to the fimi file for the specified domain. The fimi file should
//be formatted correctly
//
//
//  Pre-Condition: all file paths are valid and fimi files are well formed, num_domains -1 = num_contexts
//  Post-Condition: none
//  returns: relation graph
//  output: none
/********************************************************************/
RelationGraph * MakeRelationGraph(string &inputFile);
/********************************************************************/
// MakeContext(string inputFile,int dId1,int dId2, string name, NameMap n1, NameMap n2)
//  Pre-Condition: inputFile is valid path, n1 and n2 are non null
//  Post-Condition: none
//  returns: a context with domain1 = d1 and domain2 = d2, and relations read in from inputFile and its transpose computed
//  output: none
/********************************************************************/
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

