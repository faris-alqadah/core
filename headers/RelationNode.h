

#ifndef _RELATIONNODE_H
#define	_RELATIONNODE_H

#include"../Headers/Context.h"

class RelationNode{
public:
    RelationNode(int sId, string &sName);
    int GetColor();
    void SetColor(int);
    int GetId();
    void SetId(int); //set the id of this relation node
    string GetName();
    void SetName(string &);
    vector<int>* GetContexts();  //returns a vector of context ids for the contexts this set is in
    int GetNumEdges();  //get the number of nodes
    void AddEdge(RelationNode *,Context *); //add an edge by specifying a node to connect to and the associted context
    vector<RelationNode*>* GetNeighbors();
private:
   vector<RelationNode*> edges;
   vector<Context*> contextEdges;
   int setId;
   int color;
   string setName;

};


#endif	/* _RELATIONNODE_H */

