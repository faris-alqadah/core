//! Author: Faris Alqadah
/*!
 
 This class serves as the base function for Information Network Clustering algorithms that
 are based on Formal Concept Analysis and Concept lattices.

 All FCA-based algorithms should be formed as children classes of this class, as this
 class provides several generic class members that are utilized to indicate different modes
 of operation for each algorithm. For example, different modes of operation could be mining the clusters
 and outputing to a file, or maintaing in memory etc. Additionaly the class member variables indicate what
 quality functions should be used etc. Each derived class' documentation speficies what variables must be set
 in order to execute the underlying algorihtm correclty.

 */

#ifndef LATTICEALGOS_H
#define	LATTICEALGOS_H


#include "LatticeOps.h"
#include "./algos_helpers/basic.h"
#include "./algos_helpers/topk.h"
#include "QualityMeasures.h"

#include<fstream>
using namespace std;


class LatticeAlgos{


public:
//! Default constructor
   LatticeAlgos(){
    srchLvl =0; //default
    numConcepts=0; //default
    dispProgress=false; //default



///////////////////ENUMERATION MODES////////////////////////////////////////////
enumerationMode=1; //seting default

///////////////////QUALITY MODES////////////////////////////////////////////
qualityMode=1; //default
///////////////////Overlap MODES////////////////////////////////////////////
ovlpMode=1;
ovlpThresh=0.25;  //this is the overlap threshold...setting default
///////////////////TOP K MODES////////////////////////////////////////////
 topKK = 100;      //this is "K" for top k enumerations...setting default

///////////////////PRUNE MODES////////////////////////////////////////////
pruneMode=1; //defaults
///////////////////Algorithm Seletions////////////////////////////////////////////
neighborAlgo=1;            //defaults
conceptEnumerationAlgo=1;  //defaults
}









int srchLvl;  //! keeps track of the search level in an enumeration algorithm
int numConcepts; //! keeps track of the total number of concepts or clusters enumerated
bool dispProgress; //! flag to indicate if progress of the algorithm should output to the user (stdout)

///////////////////Data structs and files////////////////////////////////////////////
vector<NCluster*> CONCEPTS; //! data structure to hold the enumerated clusters in memory during algorithm exectuion
vector<NameMap*> NAME_MAPS; //! vector of name maps to be used to output clusters
string OUTFILE; //! if ENUM_FILE or ENUM_TOPK_FILE is selected then this file is used to output the concepts

ofstream OUT1;  //!ofstream used to output to OUTFILE.concepts
ofstream OUT2;  //!ofsteram used to output to OUTFILE.concept.names


///////////////////ENUMERATION MODES////////////////////////////////////////////
static const int ENUM_MEM=1; //! Enumeration mode that specifies to algorithms to mine and store clusters in memory
static const int ENUM_FILE=2;//! Enumeration mode that specifies to algorithms to mine and output clusters to a file, this file is specified by OUTFILE
static const int ENUM_TOPK_FILE=3;//! Enumeration mode that specifies to algorithms to mine only the top K clusters and output to a file, this file is specified by OUTFILE
static const int ENUM_TOPK_MEM=4;//! Enumeration mode that specifies to algorithms to mine only the top K clusters and store in memory

int enumerationMode; //! user will set this variable to indicate the desired enumeration mode

///////////////////QUALITY MODES////////////////////////////////////////////
static const int AREA=1; //! quality mode that indicates to use the area of a concept as its quality measure
static const int BETA=2;//! quality mode that indicates to use the beta area of a concept as its quality measure (see "An effective algorithm for mining 3-clusters" by Alqadah et al.)

int qualityMode; //! user will set this variable to indicate the desired qualityMode

double(*qualityFunction)(NCluster*, vector<double> &); //! function pointer to a quality measure, interface functions will set this acrroding to qualityMode
 vector<double> params; //! store the parameters for a quality function here, see the QualityMeasures.h documentation for specification of these parameters

///////////////////Overlap MODES////////////////////////////////////////////
static const int AVG_JACCARD=1; //! overlap mode that indicates to use the average jaccard coefficient across all sets of an n-cluster to compute overlapping
int ovlpMode; //! user will set this variable to inidicate the desired qualityMode

double(*ovlpFunction)(NCluster*,NCluster*); //! function pointer to an overlap function computer, interface function will set this according to ovlpMode
double ovlpThresh;  //! a threshold value that indicates how much overlap two clusters may have before an algorithm keeps the higher quality cluster


///////////////////TOP K MODES////////////////////////////////////////////
int topKK;  //! the number of clusters an algorithm should enumerate if user only wants the top k clusters

///////////////////PRUNE MODES////////////////////////////////////////////
static const int PRUNE_SIZE=1; //! prune mode that indicates pruning will be based on size (support pruning)
int pruneMode; //! user will set this variable to indicate the the desired pruning mode


vector<int> PRUNE_SIZE_VECTOR; //! if PRUNE_SIZE mode is selected this vector should be initialized to the min support of each domain
/*!
 PRUNE_SIZE_VECTOR[domainId-1] should contain the minumum number of objects a domain with domainId should contian in order to
 be considered for enumeration. Users must set the values for this vector
 */


///////////////////Algorithm Seletions////////////////////////////////////////////
static const int BERRY=1;  //! an algorithm selection for n-cluster enumeration and neighbor enumeration

int neighborAlgo;   //! user will set this variable to indicate what algorithm to use for lattice neighbor enumeration
int conceptEnumerationAlgo; //! user will set this variable to inidcate what algorithm to use for n-cluster enumeration



};
#endif	/* LATTICEALGOS_H */


