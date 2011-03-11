
#ifndef _CHASH_
#define _CHASH_


#define HASHNS __gnu_cxx

using namespace std;
#include <ext/hash_map>
#include "../Headers/HashNode.h"

typedef pair <int, IOSet*> Key_IOSet;
typedef HASHNS::hash_multimap<int, HashNode*,   
   			       HASHNS::hash<int>, equal_to<int> > HTable;

typedef pair<HTable::iterator, HTable::iterator> HTableFind;
typedef HTable::value_type HTPair;

bool Contains1(IOSet*,IOSet*);

class Hashtable{
public:
    HTable hashTbl;
    Hashtable();
   ~Hashtable();
    void Add(HashNode*);
    void Add(NCluster*);
    void Remove(NCluster*);
    void Remove(HashNode*);
   
    bool InTable(HashNode *);
    bool InTable(NCluster *);
    NCluster * Get( NCluster*);
    void Output(); 	
    int Size();		
};




#endif
