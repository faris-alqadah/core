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
#ifndef _RELATIONGRAPH_H
#define	_RELATIONGRAPH_H

#include"../Headers/RelationNode.h"




class RelationGraph{
public:
    RelationGraph();
    int GetNumNodes();
    bool IsEdge(int,int);
    void SetColor(int nodeNum,int color);
    void AddNode(RelationNode*);
    bool ContainsNode(int id);
    RelationNode* GetNode(int id);
    void Print();
    void NeighborContexts(int rowId, int colId, int contextId, set<int> & neighbors );
private:
    vector<RelationNode*> nodes;


};


#endif	/* _RELATIONGRAPH_H */

