

#include "../Headers/Ops.h"
#include "../Headers/NCluster.h"
class HashNode{
public:
	NCluster *a;
	int sup;
	int hashValue;
	HashNode(NCluster*, int, int);
	HashNode();
	void Output();
	void DeepCopy( HashNode* );
	~HashNode();
};
