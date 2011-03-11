/* 
 * File:   Tail.h
 * Author: alqadaf
 *
 * Created on November 6, 2009, 3:38 PM
 */


#ifndef _TAIL_H
#define	_TAIL_H

#include "../Headers/IOSet.h"
#include<list>
using namespace std;

typedef list<IOSet*> TailList;

class Tail {
public:
    Tail();
    ~Tail();
    list<IOSet*> tailList;
    void AddNode(IOSet*);
    list<IOSet*>::iterator RemoveNode(int);
    list<IOSet*>::iterator RemoveNode2(list<IOSet*>::iterator);
    list<IOSet*>::iterator RemoveNode1(int,int&);
    void Clear();
    void Output();
    void Output1();
    void Sort();
    void Sort_Id();
    int Size();
    void DeepCopy(Tail *);
    IOSet* Get(int);
    IOSet* Get_Id(int);
    IOSet *GetIds();
    bool Contains(int);
    void RemoveNotInList(IOSet*);
private:

};

#endif


#endif	/* _TAIL_H */

