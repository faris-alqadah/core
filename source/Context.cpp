#include "../Headers/Context.h"


Context::Context(int r ,int c){
    numRows = r;
    numCols=c;
    rows.resize(numRows);
    cols.resize(numCols);
    id =-1;
    rowsId=-1;
    colsId=-1;
    name="~";
}
Context::Context( Context &a){
    id=a.id;
    rowsId=a.rowsId;
    colsId=a.colsId;
    numRows=a.numRows;
    numCols=a.numCols;
    name=a.name;
    rows.resize(numRows);
    cols.resize(numCols);
    for(int i=0; i < numRows; i++){
        rows[i] = new IOSet;
        rows[i]->DeepCopy(a.rows[i]);
    }
    for(int i=0; i < numCols; i++){
        cols[i] = new IOSet;
        cols[i]->DeepCopy(a.cols[i]);
    }
}
Context::Context(vector<IOSet*> &rr, vector<IOSet*> &cc){
    numRows = rr.size();
    numCols = cc.size();
    name="~";
    id=-1;
    rowsId=-1;
    colsId=-1;
    rows.resize(numRows);
    cols.resize(numCols);
    for(int i=0; i < numRows; i++){
        rows[i] = new IOSet;
        rows[i]->DeepCopy(rr[i]);
    }
    for(int i=0; i < numCols; i++){
        cols[i] = new IOSet;
        cols[i]->DeepCopy(cc[i]);
    }

}
IOSet * Context::GetSet(int domain,int setNum){
    if(domain == rowsId) return GetRow(setNum);
    else return GetCol(setNum);
}
IOSet * Context::GetRow(int rowNum){
    //IOSet *ret = new IOSet(*rows[rowNum]);
    return rows[rowNum];
}
IOSet * Context::GetCol(int colNum){
   // IOSet *ret = new IOSet(*cols[colNum]);
    return cols[colNum];
}
IOSet * Context::GetAllRowLabels(){
    IOSet *ret = new IOSet;
    for(int i=0; i < numRows; i++) ret->Add(i);
    return ret;
}
IOSet * Context::GetAllColLabels(){
    IOSet *ret = new IOSet;
    for(int i=0; i < numCols; i++) ret->Add(i);
    return ret;
}
int Context::GetId(){return id;}
void Context::SetRowsId(int rid){ rowsId = rid;}
void Context::SetColsId(int cid){ colsId = cid;}
int Context::GetRowsId(){ return rowsId;}
int Context::GetColsId(){ return colsId;}
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
    cout<<"\nis a Cols: "<<(aId == colsId);
    cout<<"\n\n";
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

double Context::ExpectedConn(IOSet *a, IOSet *b){

    double e1,e2;
    double sum=0;
    //iterate over rows
    for(int i=0; i < a->Size(); i++) sum += rows[a->At(i)]->Size();
    e1 = ( (double) b->Size()/ (double) numCols ) *  sum;
    sum=0;
    for(int i=0; i < b->Size(); i++) sum += cols[b->At(i)]->Size();
    e2 = ( (double) a->Size()/ (double) numRows ) * sum;
    return (e1+e2)/2;
    //if (e1 > e2) return e1;
    //else return e2;
}
double Context::ExpectedIdxConn(IOSet *a, IOSet *b){
    cout<<"\nexpected conn: "<<ExpectedConn(a,b);
     return (double) ExpectedConn(a,b) / ((double) a->Size()*b->Size());
 }

int Context::GetNumOnes(){
    int cnt=0;
    for(int i=0; i < rows.size(); i++) cnt += rows[i]->Size();
    return cnt;
}

double Context::GetDensity(){
    return (double)GetNumOnes() / ( (double)rows.size()*(double)cols.size() );
}
