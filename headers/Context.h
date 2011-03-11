/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2008
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  Represent a context from Formal Concept Analysis
 *
 *
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/
#ifndef _CONTEXT_H
#define	_CONTEXT_H

#include "../Headers/IOSet.h"
#include "../Headers/Ops.h"

class Context{
    
public:
    Context(int,int);            //default constructor takes in number of rows and columns
    Context( Context &a);  //copy constructor
    Context(vector<IOSet*> &, vector<IOSet*> &); //constructor that sets the rows and columns
    ~Context();
    IOSet * GetRow(int);
    IOSet * GetCol(int);
    IOSet * GetSet(int domain,int setNum);    //return setNum from domain
    IOSet *GetAllRowLabels();
    IOSet *GetAllColLabels();
    int GetId();
    void SetId(int);
    void SetRowsId(int);
    void SetColsId(int);
    int GetRowsId();
    int GetColsId();
    string GetName();
    void SetName(string &);
    void PrintAsMatrix();
    void PrintAsMatrix(ofstream&);

    Context * GetSubContext(IOSet *a, IOSet *b,int aId, int bId); //return a sub context that contains the rows in a and columns in b

    void PrintAsFIMI();
    void PrintAsFIMI(ofstream &);
    int GetNumRows();
    int GetNumCols();
    double ExpectedConn(IOSet *a, IOSet *b); //get the expected connectivity of subspace (a,b)
    double ExpectedIdxConn(IOSet *a, IOSet *b); //get the expected index of connectivity of subspace (a,b)


    int GetNumOnes();
    double GetDensity();

    friend class RelationNode;
    friend class RelationEdge;
    friend class RelationGraph;
private:
    vector<IOSet*> rows;
    vector<IOSet*> cols;
    int id;
    int rowsId;
    int colsId;
    int numRows;
    int numCols;
    string name;
};


#endif	/* _CONTEXT_H */

