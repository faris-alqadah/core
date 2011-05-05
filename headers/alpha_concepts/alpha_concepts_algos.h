//! Author: Faris Alqadah
/*!

 This class serves as the base class for Information Network Clustering algorithms involve
 real-valued contexts.
 All algorithms for real-valued contexts should be formed as children classes of this class, as this
 class provides several generic class members that are utilized to indicate different modes
 of operation for each algorithm. For example, different modes of operation could be mining the clusters
 and outputing to a file, or maintaing in memory etc. Additionaly the class member variables indicate what
 quality functions should be used etc. Each derived class' documentation speficies what variables must be set
 in order to execute the underlying algorihtm correclty.

 */


#ifndef _ALPHA_CONCEPTS_ALGOS_H
#define	_ALPHA_CONCEPTS_ALGOS_H

#include "../core/LatticeAlgos.h"
#include "consistency.h"
#include "dispersion.h"

class AlphaConceptsAlgos : public LatticeAlgos{
public:
AlphaConceptsAlgos():LatticeAlgos() {
    consistencyMode=1; //default
    dispersionMode=1; //default
    alpha=1; //default
};

/*! Pointer to a function  that creates the parameters for a
 consistency function
 */
void(*paramFunction)(RContext *,IOSet *, int, int,int, vector<double> &);

//! the alpha parameter for alpha concepts or semi-concepts
double alpha;

/////////////////////////////////Dispersion Modes///////////////////////////////
 //! dispersion function mode that indicate to use range as the dispersion function
 static const int RANGE=1;
 //! user will set this variable to indicate the desired disperion function
 int dispersionMode;
 //! function pointer to a dispersion fuinction, interface functions will set this according to dispersionMode
 double(*dispersionFunction)(RSet*,vector<double> &);

/////////////////////////////////Consistency Modes///////////////////////////////
//! consistency function mode that uses the alpha-sigma rule, with assumed normal distributions
 static const int ALPHA_SIGMA=1;
//! consistency function mode that uses the maximum spacing estimator with assumed uniform distributions
 static const int MAX_SPACE_UNIFORM=2;
//! user will set this variable to indicate the desired consistency function
 int consistencyMode;
 //! function pointer to a consistency function, interface functions will set this according to consistencyMode
 double(*consistencyFunction)(RSet*,vector<double>&);


 /////////////////////////////////Data structures for easy reference///////////////////////////////
 //! The real valued context for which the algorithm is being applied
 RContext *K;
 //! Id of the source or query domain
 int s;
 //!Id of the target or non-query domain
 int t;
};
#endif	/* _ALPHA_CONCEPTS_ALGOS_H */

