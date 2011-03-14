#include "../Headers/Context.h"


Context::Context(int n1 ,int n2){
    domain1 = new NCluster(n1);
    domain2 = new NCluster(n2);
    domain1.SetId(0);
    domain2.SetId(1);
    id =0;
    name="~";
}
Context::Context( Context &a){
    domain1->DeepCopy(*a.domain1);
    domain2->DeepCopy(*a.domain2);
    id = a.id;
    name = a.name;
}
Context::Context(NCluster *d1, NCluster *d2){
    assert(d1 != NULL && d2 != NULL);
    domain1 = new NCluster(*d1);
    domain2 = new NCluster(*d2);
    id = 0;
    name = "~";
}
Context::~Context(){
    if (domain1 != NULL) delete domain1;
    if (domain2 != NULL) delete domain2;
    domain1 = NULL;
    domain2 = NULL;
}
IOSet * Context::GetSet(int domain,int setNum){
    assert(setNum > 0);
    assert(domain == domain1->GetId() || domain == domain2->GetId());
    if (domain == domain1->GetId()){
        assert(setNum < domain1->GetN());
        return domain1->GetSet(setNum);
    }else{
        assert(setNum < domain2->GetN());
        return domain2->GetSet(setNum);
    }
}
IOSet * Context::GetLabels(int domain){
    assert(domain == domain1->GetId() || domain == domain2->GetId());
    IOSet *ret = new IOSet;
    if(domain == domain1->GetId() ){
        for(int i=0; i < domain1->GetN(); i++) ret->Add(domain1->GetSet(i)->Id());
    }else{
        for(int i=0; i < domain2->GetN(); i++) ret->Add(domain2->GetSet(i)->Id());
    }
    return ret;
}

int Context::GetId(){return id;}


void Context::SetId(int iid){id = iid;}
string Context::GetName(){return name;}
void Context::SetName(string &n){ name = n;}

int Context::GetNumRows(){return numRows;}
int Context::GetNumCols(){return numCols;}

void Context::PrintAsMatrix(){
    for(int i=0; i < numRows; i++){
        for(int j=0; j < numCols; j++){
            if (j > 0)
                cout<<"\t";

            if( rows[i]->Contains(j)) cout<<"1";
            else cout<<"0";
        }
        cout<<"\n";
    }
}

void Context::PrintAsMatrix(ofstream &out){
       for(int i=0; i < numRows; i++){
        for(int j=0; j < numCols; j++){
            if (j > 0)
                out<<"\t";
            if( rows[i]->Contains(j)) out<<"1";
            else out<<"0";
        }
        out<<"\n";
    }
}


Context * Context::GetSubContext(IOSet *a, IOSet *b,int aId, int bId){
    vector<IOSet*> aa;
    vector<IOSet*>bb;

    aa.resize(numRows);
    bb.resize(numCols);
    for(int i=0; i < numRows; i++)  {
       IOSet *t = this->GetSet(aId,i);
       aa[i] = Intersect(t,b);

    }
    for(int i=0; i < numCols; i++)  {
       IOSet *t = this->GetSet(bId,i);
       bb[i] = Intersect(t,a);
    }
    Context *tt = new Context(aa,bb);
    return tt;

   }

void Context::PrintAsFIMI(){
    for(int i=0; i < numRows; i++){
        rows[i]->Output();
        cout<<"\n";
    }

}
void Context::PrintAsFIMI(ofstream &out){
        for(int i=0; i < numRows; i++){
        rows[i]->Output(out);
        out<<"\n";
    }
  }

int Context::GetNumOnes(){
    int cnt=0;
    for(int i=0; i < rows.size(); i++) cnt += rows[i]->Size();
    return cnt;
}

double Context::GetDensity(){
    return (double)GetNumOnes() / ( (double)rows.size()*(double)cols.size() );
}
