/********************************************************************/
//      topk helper files
//
//   Functionality is provided to keep track of "top k" clusters during
//  an enumeration process
/********************************************************************/


#include "../RelationGraph.h"
#include "../Ops.h"
#include "basic.h"
/********************************************************************/
// RetainTopK(vector<NCluster*> &v, NCluster *c, double (*Quality)(NCluster*, NCluster*), ovlpThresh)
//
//   Attempts to add the cluster c to v, this can happen in three cases
//      v.size() < k
//          OR
//      there does not exist any cluster x in v s.t Ovlp(c,x) > ovlpThresh && Quality(c) > min(Quality(x)) for any x in v
//          OR
//     there exists at least once cluster c s.t. ovlp(c,x) > ovlpThresh && Quality(c) > Quality(x) then c replaces x
//
//  Pre-Condition: c is not NULL, Quality is a valid function pointer to a quality function,
//                                0vlp is a valid function pointer to to an overlpa function
//                                  <= ovlpThresh  <= 1
//                                  k > 0
//                                  The quality field of c has been set appropriatley
//  Post-Condition:  c is added to v if it qualifies as stated above, if c is added its quality field is set to the correct quality
//  returns:none
//  output: none
/********************************************************************/
void RetainTopK_Overlap(vector<NCluster*> &v, NCluster *c,double (*Ovlp)(NCluster*,NCluster*), double ovlpThresh, int k);


