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

#include "../Headers/NCluster.h"
#include "../Headers/Ops.h"

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

    int GetId();
    void SetId(int);
    
    void SetDomainId(int setNum, int id);
    int GetDomainId(int setNum);
    string GetName();
    void SetName(string &);
    void PrintAsMatrix();
    void PrintAsMatrix(ofstream&);

    Context * GetSubContext(IOSet *a, IOSet *b,int aId, int bId); //return a sub context that contains the rows in a and columns in b

    void PrintAsFIMI();
    void PrintAsFIMI(ofstream &);
    int GetNumSets(int domainId);


    int GetNumOnes();
    double GetDensity();

    friend class RelationNode;
    friend class RelationEdge;
    friend class RelationGraph;
private:
    NCluster *domain1; //represent first domain
    NCluster *domain2; //represent second domain
    int id;  //id of the context
    string name; //name of the context
};


#endif	/* _CONTEXT_H */

