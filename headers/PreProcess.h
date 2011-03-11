/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2008
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  Global variables
 *
 *
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/

#ifndef _PREPROCESS_H
#define	_PREPROCESS_H

#include "../Headers/NCluster.h"
#include "../Headers/Globals.h"
#include <utility>


////////////////////////////////////////////////////////////////////////////////
//Reads in the input file including context names, files, and relation
//Assumes that standard global variables are defined
////////////////////////////////////////////////////////////////////////////////
void ReadInputFile();

////////////////////////////////////////////////////////////////////////////////
//Constructs a context from the file located at ctxFiles[ctxId]
//Assumes that standard global variables are defined
// Input: rowsId: id of the set that forms the rows
//        colsId: id of the set that forms the cols
//        ctxId: id of the context to be generated
////////////////////////////////////////////////////////////////////////////////
void MakeContext(int rowsId, int colsId, int ctxId);





////////////////////////////////////////////////////////////////////////////////
//Extracts the name of a set and the number of entries in that set
//Assumes that standard global variables are defined
// Input: 1) string containing the name and number
//        2) set id
//        3) context id
////////////////////////////////////////////////////////////////////////////////
void GetSetNameNumEntries(const string&,int i);

////////////////////////////////////////////////////////////////////////////////
//Extracts an edge of the relation graph from the string, also calls functions to
//produce a context
//Assumes that standard global variables are defined
// Input: 1) string containing the edge\
//        2) id of the context
////////////////////////////////////////////////////////////////////////////////
void GetRelationGraph(const string&, int ctxId);


////////////////////////////////////////////////////////////////////////////////
//Outputs each set and number of elements it contains
////////////////////////////////////////////////////////////////////////////////
void OutputSetStats();


////////////////////////////////////////////////////////////////////////////////
//Outputs each context and its stats
////////////////////////////////////////////////////////////////////////////////
void OutputContextStats();

////////////////////////////////////////////////////////////////////////////////
//Tokeinzes a string into a vector of strings
// Input: 1) string to be tokenized
//        2) vector to contain tokens
//        3) the delimiter
////////////////////////////////////////////////////////////////////////////////
void Tokenize(const string&, vector<string>& , const string&);







#endif	/* _PREPROCESS_H */

