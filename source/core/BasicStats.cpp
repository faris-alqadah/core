
#include "../../headers/core/BasicStats.h"




double Hypgeo_Mean(int n, int m, int NN){
    double nn=n;
    double mm=m;
    double NNN=NN;
    return ( nn*mm )/NNN;
}
double Hypgeo_Dev(int n, int m, int NN){
    double nn=n;
    double mm=m;
    double NNN=NN;
    double top = nn*mm*(NNN-nn)*(NNN-m);
    double bottom = pow(NNN,2)*(NNN-1);
    return sqrt(top/bottom);
}
double Z_Score(double x, double mean, double std){
    return (x-mean)/std;
}

double BetaBinomial_Mean(int n, int alpha, int beta){
    return (double)(n*alpha)  / (double)(alpha+beta);
}

double BetaBinomial_Dev(int n, int alpha, int beta){
    return sqrt(   (double)( n*alpha*beta*(alpha+beta+n)   )     / (double)(  (pow(alpha+beta,2))*(alpha+beta+1) )       );
}
