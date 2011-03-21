/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2008
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  Relation graph represents a heterogenous information network (HIN)
 * Every node in the graph is a domain, while each edge is a context. Assumption
 * is that the relation graphs are in fact trees (they do not contian loops)!
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/
#ifndef _RELATIONGRAPH_H
#define	_RELATIONGRAPH_H

#include"Context.h"




class RelationGraph{
public:
 /********************************************************************/
// RelationGraph(): Default constructor for a relation graph
//  Pre-Condition: none
//  Post-Condition: intialized relation graph
//  returns: initialized relation grpah
//  output: none
/********************************************************************/
    RelationGraph();
 /********************************************************************/
// ~RelationGraph(): Destructor for relation graph
//  Pre-Condition: none
//  Post-Condition: relation graph is destroyed
//  returns: none
//  output: none
/********************************************************************/
    ~RelationGraph();
/********************************************************************/
// AddContext(Context *c):
//  Pre-Condition: c is not contained in the self already
//  Post-Condition: add c to self.contexts, adjust domain context map and domain relations
//  returns: none
//  output: none
/********************************************************************/
    void AddContext(Context *c);
/********************************************************************/
// GetNumNodes():
//  Pre-Condition: none
//  Post-Condition: none
//  returns: number of nodes (domains) in the relation graph
//  output: none
/********************************************************************/
int GetNumNodes();
/********************************************************************/
// IsEdge(int id1, int id2):
//  Pre-Condition: id1 and id2 are valid ids of  domains in the relation graph
//  Post-Condition: none
//  returns: true if a context exists with domains id1 and id2, false other wise
//  output: none
/********************************************************************/
    bool IsEdge(int id1,int id2);
/********************************************************************/
// GetContexts(int domain)
//  Pre-Condition: domain is a valid domain id in the relation graph
//  Post-Condition: none
//  returns: return vector of context points to which domain belongs, NULL if pre-condition not met
//  output: none
/********************************************************************/
    vector<Context*> * GetContexts(int domain);
 /********************************************************************/
// GetArtDomains()
//  Pre-Condition: none
//  Post-Condition: none
//  returns: returns IOSet of domain ids that are articulation points (assuming tree shaped relation graph)
//  output: none
/********************************************************************/
    IOSet *GetArtDomains();
 /********************************************************************/
// GetContext(int ctxId)
//  Pre-Condition: ctxId is a valid context id
//  Post-Condition: none
//  returns: pointer to the context specifed by ctxId, NULL if pre-condition not met
//  output: none
/********************************************************************/
    Context * GetContext(int ctxId);
 /********************************************************************/
// GetNeighbors(int domaint)
//  Pre-Condition: domain is a valid domain id for a doamin in the self
//  Post-Condition: none
//  returns: IOSet of domain ids that consitute neighbors of domain
//  output: none
/********************************************************************/
    IOSet * GetNeighbors(int domain);
 /********************************************************************/
// GetAllContextIds()
//  Pre-Condition: none
//  Post-Condition: none
//  returns: IOSet of all context ids
//  output: none
/********************************************************************/
    IOSet * GetAllContextIds();
 /********************************************************************/
// GetAllDomainIds()
//  Pre-Condition: none
//  Post-Condition: none
//  returns: IOSet of all domain ids
//  output: none
/********************************************************************/
    IOSet *GetAllDomainIds();
 /********************************************************************/
// IsDomainId(int dId)
//  Pre-Condition: none
//  Post-Condition: none
//  returns: true if dId is a valid domain id in this relation graph, false otherwise
//  output: none
/********************************************************************/
    bool IsDomainId(int dId);
 /********************************************************************/
// IsContextId(int cId)
//  Pre-Condition: none
//  Post-Condition: none
//  returns: true if cId is a valid context id in this relation graph, false otherwise
//  output: none
/********************************************************************/
    bool IsContextId(int cId);
 /********************************************************************/
// Print()
//  Pre-Condition: none
//  Post-Condition: none
//  returns: none
//  output: prints the relation graph
/********************************************************************/
    void Print();
 /********************************************************************/
// GetContextId(int s, int t)
//  Pre-Condition: (s,t) is an edge in the graph
//  Post-Condition: none
//  returns: return the context that corresponds to the (s,t) edge and NULL if pre-condition is not met
//  output: none
/********************************************************************/
    Context* GetContext(int s, int t);

private:
    NCluster domainContextMap; //maps domains to contexts
    vector<Context*> contexts; //contains all the contexts
    NCluster domainRelations; //adjacenly list of the actual graph

};


#endif	/* _RELATIONGRAPH_H */

