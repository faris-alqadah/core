/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2008
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  IOSet data structure. This data structure mimics the C++ std vector but adds
 *  some additional properties such as an id.
 *
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/
#include<string>
#include<iostream>
#include <map>
#include <set>
#include <algorithm>
#include<fstream>
#include<vector>
#include<list>
#include<deque>
#include <queue>
#include <math.h>
#include <iomanip>
#include<sstream>
#include <utility>
#include <numeric>
#include <assert.h>

#include "../Headers/NameMap.h"
using namespace std;


#ifndef _IOSET_H
#define	_IOSET_H

class IOSet {
public:

    /********************************************************************/
    // IOSet(): default constructor
    //  Pre-Condition: none
    //  Post-Condition:   size =0,id=-1,marked=false
    //  returns: new IOSet object
    //  output:none
    /********************************************************************/
    IOSet();
    /********************************************************************/
    // IOSet( IOSet *): copy constructor
    //  Pre-Condition: IOSet* is not null
    //  Post-Condition:   copy of *a
    //  returns: new IOSet object as a copy of IOSet*
    //  output:none
    /********************************************************************/
    IOSet(IOSet *a);
    /********************************************************************/
    // ~IOSet( IOSet *): destructor, nothing needs to be done so auto clean up
    //  Pre-Condition: none
    //  Post-Condition:   self is destroyed
    //  returns: none
    //  output:none
    /********************************************************************/
    ~IOSet();
    /********************************************************************/
    // Size():
    //  Pre-Condition: none
    //  Post-Condition:   none
    //  returns: size of IOSet
    //  output:none
    /********************************************************************/
    int Size();
    /********************************************************************/
    // Id():
    //  Pre-Condition: none
    //  Post-Condition:   none
    //  returns: id of IOset
    //  output:none
    /********************************************************************/
    int Id();
    /********************************************************************/
    // SetId(int id):
    //  Pre-Condition:
    //  Post-Condition: self.id = id
    //  returns: none
    //  output:none
    /********************************************************************/
    void SetId(int id);
    /********************************************************************/
    // Output():
    //  Pre-Condition: none
    //  Post-Condition:   none
    //  returns: none
    //  output: outputs the elements of self
    /********************************************************************/
    void Output();
    /********************************************************************/
    // Output(ofstream& f):
    //  Pre-Condition: f exists and can be opened
    //  Post-Condition:   none
    //  returns: none
    //  output: outputs elements of self to filestream f
    /********************************************************************/
    void Output(ofstream& f);
    /********************************************************************/
    // Output(ofstream& f, NameMap *n):
    //  Pre-Condition: f exists and can be opened, n is non null
    //  Post-Condition:   none
    //  returns: none
    //  output: outputs elements of self as mapped by n as strings
    /********************************************************************/
    void Output(ofstream &f, NameMap *n);
    /********************************************************************/
    // Add(int x):
    //  Pre-Condition: x is non negative
    //  Post-Condition:   x is appened to the end self, size++
    //  returns: none
    //  output: none
    /********************************************************************/
    void Add(int x);
    /********************************************************************/
    // SetSize(int x): This mainly used by operations such as set intersect and
    //                  should be avoided other than use by ops operations
    //  Pre-Condition: x is non-negative
    //  Post-Condition:   self.size = x
    //  returns: none
    //  output: none
    /********************************************************************/
    void SetSize(int x);

    /********************************************************************/
    // Resize(int x): This resizes the physical size of the underlying datastructure vector,
    //  Pre-Condition: x is non-negative
    //  Post-Condition:   d.resize(x)
    //  returns: none
    //  output: none
    /********************************************************************/
    void Resize(int x);
    /********************************************************************/
    // Equal(IOSet& a):
    //  Pre-Condition: none
    //  Post-Condition: none
    //  returns: true of a.d == self.d otherwise false
    //  output: none
    /********************************************************************/
    bool Equal(IOSet&);
    /********************************************************************/
    // Contains(int x):
    //  Pre-Condition: none
    //  Post-Condition: none
    //  returns: true if x is an element of self
    //  output: none
    /********************************************************************/
    bool Contains(int);
    /********************************************************************/
    // DeepCopy(IOSet *a):
    //  Pre-Condition: a is not null
    //  Post-Condition: self is an exact deep copy of a
    //  returns: none
    //  output: none
    /********************************************************************/
    void DeepCopy(IOSet*);
    /********************************************************************/
    // Remove(int x):
    //  Pre-Condition: x < self.size
    //  Post-Condition: removes the xth element from self, self.size --
    //  returns: none
    //  output: none
    /********************************************************************/
    void Remove(int);
    /********************************************************************/
    // FindRemove(int x):
    //  Pre-Condition: none
    //  Post-Condition: removes the element with value of x, if it exists then remove & self.size --, otherwise none
    //  returns: none
    //  output: none
    /********************************************************************/
    void FindRemove(int);
    /********************************************************************/
    // Sort():
    //  Pre-Condition: none
    //  Post-Condition: elements of self are sorted in ascending order
    //  returns: none
    //  output: none
    /********************************************************************/
    void Sort();
    /********************************************************************/
    // At(int x):
    //  Pre-Condition: x >= 0 && x < self.size
    //  Post-Condition: none
    //  returns: the xth element in self
    //  output: none
    /********************************************************************/
    int At(int);
    /********************************************************************/
    // Clear():
    //  Pre-Condition: none
    //  Post-Condition: all elements of self are removed, self.size = 0
    //  returns: none
    //  output: none
    /********************************************************************/
    void Clear();
    /********************************************************************/
    // GetBegin():
    //  Pre-Condition: none
    //  Post-Condition: none
    //  returns: a std:vector:iterator that points to the begging of self.d
    //  output: none
    /********************************************************************/
    vector<int>::iterator GetBegin();
    /********************************************************************/
    // GetBegin():
    //  Pre-Condition: none
    //  Post-Condition: none
    //  returns: a std:vector:iterator that points to the end of self.d
    //  output: none
    /********************************************************************/
    vector<int>::iterator GetEnd();
    /********************************************************************/
    // SetMarked(bool a):
    //  Pre-Condition: none
    //  Post-Condition: self.marked = a
    //  returns: none
    //  output: none
    /********************************************************************/
    void SetMarked(bool);
    /********************************************************************/
    // GetMarked():
    //  Pre-Condition: none
    //  Post-Condition: none
    //  returns: self.marked
    //  output: none
    /********************************************************************/
    bool GetMarked();


private:
    int size; // maintain the size of the ioset
    int id; //id of the ioset
    vector<unsigned int> d; //vector to hold the data
    bool marked; // has this ioset been marked???

};
/********************************************************************/
// Compare_Sup(IOSet *a, IOSet * b): Primarly inteded to be used with std:sort, to sort a collection of IOSets by their size
//  Pre-Condition: a and b are non-null
//  Post-Condition: none
//  returns: true if a.size > b.size false otherwise
//  output: none
/********************************************************************/
bool Compare_Sup(IOSet *a, IOSet *b);

/********************************************************************/
// Compare_Id(IOSet *a, IOSet * b): Primarly inteded to be used with std:sort, to sort a collection of IOSets by their ids
//  Pre-Condition: a and b are non-null
//  Post-Condition: none
//  returns: true if a.id > b.id false otherwise
//  output: none
/********************************************************************/
bool Compare_Id(IOSet *a, IOSet *b);








#endif	/* _IOSET_H */

