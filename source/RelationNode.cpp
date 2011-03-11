
#include"../Headers/RelationNode.h"

RelationNode::RelationNode(int sId, string &sName){
        setId = sId;
        setName = sName;
    }
    int RelationNode::GetColor(){
        return color;
    }
    void RelationNode::SetColor(int cc){color = cc;}
    int RelationNode::GetId(){return setId;}
    void RelationNode::SetId(int sid){setId = sid;}
    string RelationNode::GetName(){return setName;}
    void RelationNode::SetName(string &sName){ setName = sName;}

    vector<int>* RelationNode::GetContexts(){
        vector<int> * ret = new vector<int>;
        for(int i=0; i < contextEdges.size(); i++)
            ret->push_back(contextEdges[i]->GetId());
        return ret;
    }

    vector<RelationNode*>* RelationNode::GetNeighbors(){
        vector<RelationNode*> *ret = new vector<RelationNode*>;
        for(int i=0; i < edges.size(); i++ ){
            ret->push_back(edges[i]);
        }
        return ret;
    }
    int RelationNode::GetNumEdges(){ return edges.size();}
    void RelationNode::AddEdge(RelationNode* e, Context *c){
        edges.push_back(e);
        contextEdges.push_back(c);
    }


