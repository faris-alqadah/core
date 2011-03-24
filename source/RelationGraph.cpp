#include"../headers/RelationGraph.h"

RelationGraph::RelationGraph() {
}

RelationGraph::~RelationGraph() {
    for (int i = 0; i < contexts.size(); i++) contexts[i] = NULL;
}

void RelationGraph::AddContext(Context *c) {
    //first check if context is context already exists (by id)
    for (int i = 0; i < contexts.size(); i++)
        if (contexts[i]->GetId() == c->GetId())
            return;
    //if reached here then context is not contained so
    //add it and make adjustements
    contexts.push_back(c);
    // modify domain context map by adding in new domains
    //and adjusting any old ones
    pair<int, int> newDomains = c->GetDomainIds();
    if (domainContextMap.ContainsIOSetId(newDomains.first)) {
        domainContextMap.GetSetById(newDomains.first)->Add(c->GetId());
        domainRelations.GetSetById(newDomains.first)->Add(newDomains.second);
    } else {
        IOSet *a = new IOSet();
        a->SetId(newDomains.first);
        a->Add(c->GetId());
        domainContextMap.AddSet(a);
        IOSet *b = new IOSet();
        b->SetId(newDomains.first);
        b->Add(newDomains.second);
        domainRelations.AddSet(b);
    }
    if (domainContextMap.ContainsIOSetId(newDomains.second)) {
        domainContextMap.GetSetById(newDomains.second)->Add(c->GetId());
        domainRelations.GetSetById(newDomains.second)->Add(newDomains.first);
    } else {
        IOSet *a = new IOSet();
        a->SetId(newDomains.second);
        a->Add(c->GetId());
        domainContextMap.AddSet(a);
        IOSet *b = new IOSet();
        b->SetId(newDomains.second);
        b->Add(newDomains.first);
        domainRelations.AddSet(b);
    }
    //sort all iosets
    domainContextMap.GetSetById(newDomains.first)->Sort();
    domainContextMap.GetSetById(newDomains.second)->Sort();
    domainRelations.GetSetById(newDomains.first)->Sort();
    domainRelations.GetSetById(newDomains.second)->Sort();
}

int RelationGraph::GetNumNodes() {
    return domainRelations.GetN();
}

bool RelationGraph::IsEdge(int id1, int id2) {
    if (domainRelations.ContainsIOSetId(id1) && domainRelations.ContainsIOSetId(id2)) {
        //since symmetric only need to check one adjacency list
        if (domainRelations.GetSetById(id1)->Contains(id2)) return true;
        else return false;
    } else return false;
}

vector<Context*> * RelationGraph::GetContexts(int domain) {
    if (domainRelations.ContainsIOSetId(domain)) {
        IOSet *t = domainContextMap.GetSetById(domain);
        vector<Context*> *ret = new vector<Context*>;
        for (int i = 0; i < t->Size(); i++)
            ret->push_back(GetContext(t->At(i)));
        return ret;
    } else return NULL;
}

Context * RelationGraph::GetContext(int ctxId) {
    for (int i = 0; i < contexts.size(); i++)
        if (contexts[i]->GetId() == ctxId)
            return contexts[i];
    return NULL;
}

IOSet *RelationGraph::GetArtDomains() {
    IOSet *ret = new IOSet;
    for (int i = 0; i < domainRelations.GetN(); i++)
        if (domainRelations.GetSet(i)->Size() > 1)
            ret->Add(domainRelations.GetSet(i)->Id());
    return ret;
}

IOSet * RelationGraph::GetNeighbors(int domain) {
    if (domainRelations.ContainsIOSetId(domain))
        return new IOSet(domainRelations.GetSetById(domain));
    else return new IOSet();
}

IOSet * RelationGraph::GetAllContextIds() {
    IOSet * ret = new IOSet;
    for (int i = 0; i < contexts.size(); i++)
        ret->Add(contexts[i]->GetId());
    return ret;
}

IOSet * RelationGraph::GetAllDomainIds() {
    IOSet * ret = new IOSet;
    for (int i = 0; i < domainRelations.GetN(); i++)
        ret->Add(domainRelations.GetSet(i)->Id());
    return ret;
}

bool RelationGraph::IsDomainId(int dId) {
    return domainRelations.ContainsIOSetId(dId);
}

bool RelationGraph::IsContextId(int cId) {
    for (int i = 0; i < contexts.size(); i++)
        if (contexts[i]->GetId() == cId)
            return true;
    return false;
}

void RelationGraph::Print(){
    //first print context view
        cout<<"\n\n@@@@@@@@@@@@@@@RELATION GRAPH@@@@@@@@@@@@@@@@\n";
        cout<<"\n\nCONTEXTS\n\n";
        for(int i=0; i < contexts.size(); i++){
            pair<int,int> dIds = contexts[i]->GetDomainIds();
            cout<<"\n\nContext "<<contexts[i]->GetName()
                 <<"\n\tId: "<<contexts[i]->GetId()
                <<"\n\tConnecting domains "<<dIds.first<<" AND "<<dIds.second
                <<"\n\t Num elements in domain "<<dIds.first<<": "<<contexts[i]->GetNumSets(dIds.first)
                <<"\n\tNum elements in domain "<<dIds.second<<": "<<contexts[i]->GetNumSets(dIds.second)
                <<"\n\tDensity: "<<contexts[i]->GetDensity();
        }
        //next print domain relation view
        cout<<"\n\nDOMAIN RELATIONS\n\n";
        for(int i=0; i < domainRelations.GetN(); i++){
            cout<<"\n"<<domainRelations.GetSet(i)->Id()<<"  ---- ";
            for(int j=0; j < domainRelations.GetSet(i)->Size(); j++){
                cout<<" "<<domainRelations.GetSet(i)->At(j);
            }
        }

}

    Context* RelationGraph::GetContext(int s, int t){
        if(IsEdge(s,t)){
            for(int i=0; i < contexts.size(); i++){
                pair<int,int> dIds = contexts[i]->GetDomainIds();
                if( (dIds.first == s && dIds.second == t) || ( dIds.first == t && dIds.second == s))
                    return contexts[i];
            }
        }else return NULL;
    }

    vector<NameMap*> * RelationGraph::GetNameMaps(){
        vector<NameMap*> * ret = new vector<NameMap*>;
        for(int i=0; i < contexts.size(); i++){
            pair<int,int> dIds = contexts[i]->GetDomainIds();
            cout<<"\ndids: "<<dIds.first<<" "<<dIds.second;
            cout<<"\nt";
            cout<<"\nchck "<<contexts[i]->GetNameMap(dIds.first)->GetNumEntries();
            cout<<"\nt";
            ret->push_back(contexts[i]->GetNameMap(dIds.first));
            ret->push_back(contexts[i]->GetNameMap(dIds.second));
        }
        return ret;
    }