/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2008
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  Represent a context from Formal Concept Analysis. The two sets in the relationship
 *  are denoted by domain ids. By default set 1 domain id = 0 and set 2 domain id = 1
 *
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/
#ifndef _CONTEXT_H
#define	_CONTEXT_H

#include "NCluster.h"
#include "Ops.h"
#include "NameMap.h"

class Context{
    
public:
/********************************************************************/
// Context(int n1, int n2): Constructor for context with num1 objects in domain 1 and num2 objects in domain 2
//  Pre-Condition: none
//  Post-Condition: intialized context with domain1 id = 0 and domain2 id = 0, each with n1 and n2 objects, self.name="~"
//  returns: initialized context
//  output: none
/********************************************************************/
    Context(int num1, int num2);
/********************************************************************/
// Context(Context a): Copy Constructor
//  Pre-Condition: none
//  Post-Condition: intialized context that is a deep copy of a, self.domain1 = a.domain1, self.domain2=a.domain2, self.id = a.id, self.name = a.name
//  returns: initialized context
//  output: none
/********************************************************************/
    Context( Context &a);
/********************************************************************/
// Context(NCluster *d1, NCluster *d2): Constructor that initalizes domain1 and domain2 to the input arguments
//  Pre-Condition: d1 and d2 are not null
//  Post-Condition: intialized context with domain1 id = d1->id and domain2 id = d2->id, where d1 and d2 are deep copied to form the context
//  returns: initialized context
//  output: none
/********************************************************************/
    Context(NCluster *d1, NCluster *d2);
/********************************************************************/
// ~Context(NCluster *d1, NCluster *d2): destructor
//  Pre-Condition: none
//  Post-Condition: self is destroyed
//  returns: none
//  output: none
/********************************************************************/
    ~Context();
/********************************************************************/
// GetSet(int domain, int setNum):
//  Pre-Condition: domain is a valid domain id and setNum >= 0 < domainId.GetN()
//  Post-Condition: none
//  returns: IOSet of objects that correspond to the setNum object-set in domain
//  output: none
/********************************************************************/
    IOSet * GetSet(int domain,int setNum);    
/********************************************************************/
// GetLabels(int domain):
//  Pre-Condition: domain is a valid domain id
//  Post-Condition: none
//  returns: IOSet of ids in the domain
//  output: none
/********************************************************************/
    IOSet *GetLabels(int domain);
/********************************************************************/
// GetId():
//  Pre-Condition: none
//  Post-Condition: none
//  returns: returns id of the context
//  output: none
/********************************************************************/
    int GetId();
/********************************************************************/
// GetDomainIds():
//  Pre-Condition: none
//  Post-Condition: none
//  returns: domain1.id and domain2.id as a pair
//  output: none
/********************************************************************/
     pair<int,int> GetDomainIds();
/********************************************************************/
// SetNameMap(int dId, NameMap *nm):
//  Pre-Condition: dId is a valid domain id for the context, nm is not null
//  Post-Condition: namemap of the correspodoning id is set
//  returns: none
//  output: none
/********************************************************************/
  void SetNameMap(int dId, NameMap *nm);
  /********************************************************************/
// GetNameMap(int dId):
//  Pre-Condition: dId is a valid domain id
//  Post-Condition: none
//  returns: name map corresponding to dId if dId is valie, NULL otherwise
//  output: none
/********************************************************************/
  NameMap * GetNameMap(int dId);

/********************************************************************/
// SetId(int iid):
//  Pre-Condition: none
//  Post-Condition: self.id = iid
//  returns: none
//  output: none
/********************************************************************/
 void SetId(int);
/********************************************************************/
// GetName():
//  Pre-Condition: none
//  Post-Condition: none
//  returns: returns name of the context
//  output: none
/********************************************************************/
    string GetName();
/********************************************************************/
// SetName(string s):
//  Pre-Condition: none
//  Post-Condition: self.name = s
//  returns: none
//  output: none
/********************************************************************/
    void SetName(string &);
/********************************************************************/
// SetDomainId(int setNum,int id):
//  Pre-Condition: setNum is in the set {0,1}
//  Post-Condition: self.domain[setNum].id = id
//  returns: none
//  output: none
/********************************************************************/
    void SetDomainId(int setNum, int id);
 /********************************************************************/
// GetDomainId(int setNum):
//  Pre-Condition: domain is in the set {0,1}
//  Post-Condition: none
//  returns: the id of self.domain[domain]
//  output: none
/********************************************************************/
    int GetDomainId(int setNum);
 /********************************************************************/
// PrintAsMatrix():
//  Pre-Condition: none
//  Post-Condition: none
//  returns: none
//  output: Prints the context as a binary matrix to stdout, where domain1 forms the rows
//            and domain2 forms the columns
/********************************************************************/
    void PrintAsMatrix();
 /********************************************************************/
// PrintAsMatrix(ofstream out):
//  Pre-Condition: out is open and points to a file
//  Post-Condition: none
//  returns: none
//  output: Prints the context as a binary matrix to out where domain1 forms the rows
//          and domain2 forms the columns
/********************************************************************/
    void PrintAsMatrix(ofstream&);
 /********************************************************************/
// GetSubContext(IOSet *a, IOSet *b):
//  Pre-Condition:  a and b are sorted and have size > 0. The values in a are within the bounds 0 and domain1.size()-1 and same holds for b
//  Post-Condition: none
//  returns: a context containng only those sets specified by a and b
//  output: none
/********************************************************************/
    Context * GetSubContext(IOSet *a, IOSet *b); 
 /********************************************************************/
// PrintAsFIMI():
//  Pre-Condition: none
//  Post-Condition: none
//  returns: none
//  output: Prints the context as in "FIMI" format, or sparse matrix format
//          where only the indices of the 1s are displayed...prints to stdout
/********************************************************************/
    void PrintAsFIMI();
/********************************************************************/
// PrintAsFIMI():
//  Pre-Condition: none
//  Post-Condition: none
//  returns: none
//  output: Prints the context as in "FIMI" format, or sparse matrix format
//          where only the indices of the 1s are displayed...prints to out
/********************************************************************/
    void PrintAsFIMI(ofstream &);
/********************************************************************/
// GetNumSets:
//  Pre-Condition: domainId is a valid domainId for this context
//  Post-Condition: none
//  returns: number of objects in the domain with domainId
//  output: none
/********************************************************************/
    int GetNumSets(int domainId);
/********************************************************************/
// GetNumOnes():
//  Pre-Condition: none
//  Post-Condition: none
//  returns: the number of 1s in the context, equivalentley the number of pairs in
//            domain1 and domain2 that are in relation
//  output: none
/********************************************************************/
 int GetNumOnes();
/********************************************************************/
// GetDensity():
//  Pre-Condition: none
//  Post-Condition: none
//  returns: self.GetNumOnes/ (domain1.size*domain2.size)
//  output: none
/********************************************************************/
    double GetDensity();

private:
    NCluster *domain1; //represent first domain
    NCluster *domain2; //represent second domain
    int id;  //id of the context
    string name; //name of the context
    NameMap *nameMap1; //name map associated with domain 1
    NameMap *nameMap2; //name map associated with domain 2
};


#endif	/* _CONTEXT_H */

