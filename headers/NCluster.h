/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2009
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  An n-cluster is essentially a vector of IOSets with some additonal properties
 *  such as
 *  
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/

#ifndef _NCLUSTER_H
#define	_NCLUSTER_H

#include "../Headers/IOSet.h"

using namespace std;

class NCluster {
public:
/********************************************************************/
// NCluster(int n): Constructor for an n-cluster, allocates memory for n IOSets in the cluster
//  Pre-Condition: none
//  Post-Condition: a fully initialized n-cluster, self.quality=0, self.n=n, self.id = 0, marked=false
//  returns: initialized n-cluster
//  output: none
/********************************************************************/
    NCluster(unsigned int);
/********************************************************************/
// NCluster(int n, bool): Constructor for an n-cluster, without allocating memory to IOSets
//  Pre-Condition: none
//  Post-Condition: a fully initialized n-cluster, self.n=n, self.id = 0, marked=false
//  returns: initialized n-cluster
//  output: none
/********************************************************************/
    NCluster(unsigned int, bool);
/********************************************************************/
// NCluster(int n, vector<IOSet*> A ): Constructor for an n-cluster, assigns n and all data from vector to the ncluster
//                              NOTE: this only re-assigns pointers AND DOES NOT perform a deep copy
//  Pre-Condition: A.size() == n
//  Post-Condition: a fully initialized n-cluster, self.n=n, self.id = 0, all IOSets point to IOSets of A, marked=false
//  returns: initialized n-cluster
//  output: none
/********************************************************************/
    NCluster(unsigned int, vector<IOSet*> &);
/********************************************************************/
// NCluster(NCluster &a ): Copy constructor of NCluster
//  Pre-Condition: none
//  Post-Condition: a fully initialized n-cluster, self.n=a.n, self.id = a.id, all IOSets initialized to a copy of the argument, marked=a.marked
//  returns: initialized n-cluster
//  output: none
/********************************************************************/
    NCluster(NCluster &);
/********************************************************************/
// ~NCluster(): Destructor for n-cluster, deallocates all memory
//  Pre-Condition: none
//  Post-Condition: self is destroyed
//  returns: initialized n-cluster
//  output: none
/********************************************************************/
    ~NCluster();
/********************************************************************/
// DeepCopy(NCluster &a): Make a deep copy of a
//  Pre-Condition: none
//  Post-Condition: self is a copy of a, self.quality = a.quality, a.d[0] = self.d[1],...,a.d[n-1]=self.d[n-1]
//  returns: none
//  output: none
/********************************************************************/
    void DeepCopy(NCluster&);
/********************************************************************/
// Output: Output the elements of self
//  Pre-Condition: none
//  Post-Condition: none
//  returns: none
//  output: Prints each IOSet on a seperate line preceeded by the identity of each IOSET
/********************************************************************/
    void Output();
/********************************************************************/
// Output(ofstream &): Output the elements of self to a file
//  Pre-Condition: ofstream is open
//  Post-Condition: none
//  returns: none
//  output: Prints each IOSet on a seperate line preceeded by the identity of each IOSET to the specified stream
/********************************************************************/
    void Output(ofstream&);
/********************************************************************/
// Output(ofstream a, vector<namemap> nm): Output the elements of self mapped by name maps in nm
//  Pre-Condition: nm.size() == n and ofsteram is open
//  Post-Condition: none
//  returns: none
//  output: Prints each IOSet on a seperate line preceeded by the identity of each IOSET
/********************************************************************/
    void Output(ofstream&, vector<NameMap *>&nm); 
/********************************************************************/
// GetN()
//  Pre-Condition: none
//  Post-Condition: none
//  returns: self.n
//  output: none
/********************************************************************/
    int GetN(); 
/********************************************************************/
// GetSet(int i)
//  Pre-Condition: i >= 0 AND i < n
//  Post-Condition: none
//  returns: pointer to the ith set in self
//  output: none
/********************************************************************/
    IOSet * GetSet(int);
 /********************************************************************/
// AssignSet(int i, IOSet *a): Assign the ith set to a
//  Pre-Condition: i >= 0 AND i < n
//  Post-Condition: self.d[i] is deleted and self.d[i] = a
//  returns: none
//  output: none
/********************************************************************/
    void AssignSet(int, IOSet*);
  /********************************************************************/
// GetQuality()
//  Pre-Condition: none
//  Post-Condition: none
//  returns: self.quality
//  output: none
/********************************************************************/
    double GetQuality();
  /********************************************************************/
// SetQuality(double q)
//  Pre-Condition: none
//  Post-Condition: self.quality = q
//  returns: none
//  output: none
/********************************************************************/
   void SetQuality(double);
  /********************************************************************/
// GetId()
//  Pre-Condition: none
//  Post-Condition: none
//  returns: self.id
//  output: none
/********************************************************************/
int GetId();
   /********************************************************************/
// SetId(int id)
//  Pre-Condition: none
//  Post-Condition: self.id = id
//  returns: none
//  output: none
/********************************************************************/
 void SetId(int);

  /********************************************************************/
// GetMarked()
//  Pre-Condition: none
//  Post-Condition: none
//  returns: self.marked
//  output: none
/********************************************************************/
 bool GetMarked();
 /********************************************************************/
// SetMarked(bool m)
//  Pre-Condition: none
//  Post-Condition: self.marked = m
//  returns: none
//  output: none
/********************************************************************/
 void SetMarked( bool m);





protected:
    unsigned int n; //degree of cluster
    vector<IOSet*> sets; //the sets
    double quality;  //quality of the ncluster
    int id; // the id of the n-cluster
    bool marked; //is the ncluster marked or flagged

};

/********************************************************************/
// Compare_Quality(NCluster *a, NCluster * b): Primarly inteded to be used with std:sort, to sort a collection of NClusters by their size
//  Pre-Condition: a and b are non-null
//  Post-Condition: none
//  returns: true if a.quality > b.quality false otherwise
//  output: none
/********************************************************************/
bool Compare_Quality(NCluster *a, NCluster *b);



#endif

