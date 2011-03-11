/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2008
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  A vector data structure that maps an integer (the index) to a string. Typically
 * used to store row or column labels
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/
#ifndef _NAMEMAP_H
#define	_NAMEMAP_H

#include <iomanip>
#include<string>
#include<vector>
#include<sstream>
#include<iostream>
#include <assert.h>
using namespace std;

class NameMap {
public:
    /********************************************************************/
    // NameMap(): default constructor
    //  Pre-Condition: none
    //  Post-Condition:   new NameMap created
    //  returns: new NameMap object
    //  output:none
    /********************************************************************/
    NameMap();
    /********************************************************************/
    // NameMap(string filename, unsigned int size): parameter constructor
    //  Pre-Condition: filename is an actual file and can be read, size is non-negative and > 0
    //  Post-Condition:   new NameMap with self.fileName = filename, self.numEntries = size
    //  returns: new NameMap object
    //  output:none
    /********************************************************************/
    NameMap(string, unsigned int);
    /********************************************************************/
    // GetFileName():
    //  Pre-Condition: none
    //  Post-Condition: none
    //  returns: self.fileName
    //  output:none
    /********************************************************************/
    string GetFileName();
    /********************************************************************/
    // GetName(unsigned int x):
    //  Pre-Condition: x is non-negative AND < self.numEntries
    //  Post-Condition: none
    //  returns: self.mapping[x]
    //  output:none
    /********************************************************************/
    string GetName(unsigned int);
    /********************************************************************/
    // GetNumEntries():
    //  Pre-Condition: none
    //  Post-Condition: none
    //  returns: self.numEntries
    //  output:none
    /********************************************************************/
    int GetNumEntries();
private:
    vector<string> mapping; //indcies of the vector map to strings
    int numEntries; //number of entries
    string fileName; //name of file containing the mapping


};

#endif	/* _NAMEMAP_H */

