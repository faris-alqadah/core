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


#include"../Headers/RelationGraph.h"




//==================================RelationGraph impl==========================

    RelationGraph::RelationGraph(){}

    int RelationGraph::GetNumNodes(){ return nodes.size();}
    

    bool RelationGraph::IsEdge(int setId1,int setId2){
        for(int i=0; i < nodes.size(); i++){
            if(nodes[i]->GetId() == setId1) {
                vector<RelationNode*> *neighbors = nodes[i]->GetNeighbors();
                for(int i=0; i < neighbors->size(); i++){
                    if( (*neighbors)[i]->GetId() == setId2)
                        return true;
                }

            }else if(nodes[i]->GetId() == setId2){
                vector<RelationNode*> *neighbors = nodes[i]->GetNeighbors();
                for(int i=0; i < neighbors->size(); i++){
                    if( (*neighbors)[i]->GetId() == setId1)
                        return true;
                }
            }
        }
        return false;
    }

    void RelationGraph::SetColor(int nodeNum, int color){ nodes[nodeNum]->SetColor(color);}
    void RelationGraph::AddNode(RelationNode *a){ nodes.push_back(a);}

    bool RelationGraph::ContainsNode(int id){
        for(int i=0; i < nodes.size(); i++){
            if(nodes[i]->GetId() == id)  return true;
        }
        return false;
    }
    RelationNode * RelationGraph::GetNode(int id){
        for(int i=0; i < nodes.size(); i++)
            if(nodes[i]->GetId() == id) return nodes[i];

        return NULL;
    }
    void RelationGraph::Print(){
        cout<<"\n\n@@@@@@@@@@@@@@@RELATION GRAPH@@@@@@@@@@@@@@@@\n";
        for(int i=0; i < nodes.size(); i++){
            cout<<"\nSet: "<<nodes[i]->GetName()<<" ( "<<nodes[i]->GetId()<<" )";
            cout<<"\nConnected to: ";
            vector<RelationNode*>* neighbors = nodes[i]->GetNeighbors();
            for(int j=0; j < neighbors->size(); j++){
                cout<<"\n\t---"<<(*neighbors)[j]->GetName()<<" ( "<<(*neighbors)[j]->GetId()<<" ) ";
            }
        }
    }
    void RelationGraph::NeighborContexts(int rowId, int colId, int contextId, set<int> & neighbors ){
        if( IsEdge(rowId,colId)){
              vector<int>* nn = GetNode(rowId)->GetContexts();
              vector<int>* nn1 = GetNode(colId)->GetContexts();
              for(int i=0; i < nn->size(); i++){
                  if ( (*nn)[i] != contextId) neighbors.insert( (*nn)[i]);
              }
              for(int i=0; i < nn1->size(); i++){
                  if ( (*nn1)[i] != contextId) neighbors.insert( (*nn1)[i]);
              }
        }
    }
