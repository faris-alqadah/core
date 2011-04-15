# include "../../headers/alpha_concepts/consistency.h"


double AlphaSigma(RSet *a, vector<double> &params){
    //!params[0] - alpha
    //!params[1] - std(K[g,M])
    //!see paper
    assert(params.size() >= 2);
    return (2*params[1])/params[0];
}


double MaxSpaceUniform(RSet *a, vector<double> &params){
    //!params[0] - alpha
    //!params[1] - |M|
    //!params[2] - range(K[g,M])
    //!see paper
    assert(params.size() >= 3);
    return params[0]*(params[1]-1)*(params[2]-params[1]*params[2]);
}