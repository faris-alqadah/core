/*!
 @author Faris Alqadah
 
 This class serves as the base function for Information Network Clustering algorithms that
 are based on Formal Concept Analysis and Concept lattices.

 All FCA-based algorithms should be formed as children classes of this class, as this
 class provides several generic class members that are utilized to indicate different modes
 of operation for each algorithm. For example, different modes of operation could be mining the clusters
 and outputing to a file, or maintaing in memory etc. Additionaly the class member variables indicate what
 quality functions should be used etc. Each derived class' documentation speficies what variables must be set
 in order to execute the underlying algorihtm correclty.
 
 Additionally contains data structures for representing a lattice

 */

#ifndef LATTICEALGOS_H
#define	LATTICEALGOS_H


#include "LatticeOps.h"
#include "RLatticeOps.h"
#include "../algos_helpers/basic.h"
#include "../algos_helpers/topk.h"
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
    enumerationMode=1; //seting default
    qualityMode=1; //default
    ovlpMode=1;
    ovlpThresh=0.25;  //this is the overlap threshold...setting default
    topKK = 100;      //this is "K" for top k enumerations...setting default
    pruneMode=1; //defaults
    outputEdges=false; //defaults (do not output the edges)
}








//! keeps track of the search level in an enumeration algorithm
int srchLvl;
//! keeps track of the total number of concepts or clusters enumerated
int numConcepts;
//! flag to indicate if progress of the algorithm should output to the user (stdout)
bool dispProgress; 

//Data structures and files
//! data structure to hold the enumerated clusters in memory during algorithm exectuion
vector<NCluster*> CONCEPTS;
//! map to hold edges (pairs) if pair enumeration mode is selected
map<int, pair<int,int> > EDGES;
 //! vector of name maps to be used to output clusters
vector<NameMap*> NAME_MAPS;
//! if ENUM_FILE or ENUM_TOPK_FILE is selected then this file is used to output the concepts
string OUTFILE;
//! if output edges flag enabled use this file
string EDGES_FILE;
//!ofstream used to output to OUTFILE.concepts
ofstream OUT1;
 //!ofsteram used to output to OUTFILE.concept.names
ofstream OUT2;
//! ofstream used to output edges
ofstream EDGES_OUT;


/////////////////////////////////Enumeation Modes///////////////////////////////
//! Enumeration mode that specifies to algorithms to mine and store clusters in memory
static const int ENUM_MEM=1;
//! Enumeration mode that specifies to algorithms to mine and output clusters to a file, this file is specified by OUTFILE
static const int ENUM_FILE=2;
//! Enumeration mode that specifies to algorithms to mine only the top K clusters and output to a file, this file is specified by OUTFILE
static const int ENUM_TOPK_FILE=3;
//! Enumeration mode that specifies to algorithms to mine only the top K clusters and store in memory
static const int ENUM_TOPK_MEM=4;
//! Users will set this variable to indicate the enumeration mode
int enumerationMode;
//! set this flag to true of enumerated edges should be output to the edges file
bool outputEdges;

/////////////////////////////////Quality Modes///////////////////////////////
//! quality mode that indicates to use the area of a concept as its quality measure
static const int AREA=1;
//! quality mode that indicates to use the beta area of a concept as its quality measure (see "An effective algorithm for mining 3-clusters" by Alqadah et al.)
static const int BETA=2;
//! user will set this variable to indicate the desired qualityMode
int qualityMode; 
//! function pointer to a quality measure, interface functions will set this acrroding to qualityMode
double(*qualityFunction)(NCluster*, vector<double> &);
 //! store the parameters for a quality function here, see the QualityMeasures.h documentation for specification of these parameters
 vector<double> params;





/////////////////////////////////Overlap Modes///////////////////////////////
 //! overlap mode that indicates to use the average jaccard coefficient across all sets of an n-cluster to compute overlapping
static const int AVG_JACCARD=1;
 //! user will set this variable to inidicate the desired qualityMode
int ovlpMode;
//! function pointer to an overlap function computer, interface function will set this according to ovlpMode
double(*ovlpFunction)(NCluster*,NCluster*);
//! a threshold value that indicates how much overlap two clusters may have before an algorithm keeps the higher quality cluster
double ovlpThresh;  

/////////////////////////////////Top K Modes///////////////////////////////
 //! the number of clusters an algorithm should enumerate if user only wants the top k clusters
int topKK; 

/////////////////////////////////Prune Modes///////////////////////////////
 //! prune mode that indicates pruning will be based on size (support pruning)
static const int PRUNE_SIZE=1;
 //! user will set this variable to indicate the the desired pruning mode
int pruneMode;

//! if PRUNE_SIZE mode is selected this vector should be initialized to the min support of each domain
/*!
 PRUNE_SIZE_VECTOR[domainId-1] should contain the minumum number of objects a domain with domainId should contian in order to
 be considered for enumeration. Users must set the values for this vector
 */
vector<int> PRUNE_SIZE_VECTOR; 


};



class LatticeHashNode{
  public:
    LatticeHashNode(){ } 
    vector<string> uppers;
    vector<string> lowers;
    NCluster *concept; 
    
};

/*!

Hold a concet lattice in memory. Hash table where key is a string of domain_1 identities concated by underscores
Each key maps to a lattice node which contains pointer to an Ncluster and vectors of hash keys to the lower and upper neighbors

*/
class LatticeHash{	
  public:
    LatticeHash(){}
    //! add a node to the hash
    void AddNode(NCluster *a, vector<NCluster*> *upperNeighbors, vector<NCluster*> *lowerNeighbors);
    //! add a node to the hash
    void AddNode(NCluster *a);
    
    void OutputNode(NCluster *a,int t, ofstream& out,vector<NameMap*>&nm);

    //!update upper neighbors of a to include (the upper), both concepts are assumed to exist in the hash
    void UpdateUppers(NCluster *a, NCluster *theUpper);
    
     //!update lower neighbors of a to include (the lower), both concepts are assumed to exist in the hash
    void UpdateLowers(NCluster *a, NCluster *theLower);
    
    //! look for upper neighbors for a, if none exist or a is not in table returns null
    vector<NCluster*> * GetUpperNeighbors(NCluster *a,int t);
    //! look for lower neighbors for a, if none exist or a is not in table returns null
    vector<NCluster*> * GetLowerNeighbors(NCluster *a,int t);
    //! Output the map to a file in format key | set_1 ids | set_2 ids | upper neighbor_1_key .... upper neighbor_n_key | lower neighbor_1_key ... lower neighbor_n_key
    void OutputToFile(string &fileName);
    //! Read in the entire datastructure from file format descibed above
    void ReadFromFile(string &fileName);
    //! create hash key by appending ids of objects in a with dashes in between
      void MakeHashKey(NCluster*a, int t, string &theKey);
private:
  map<string,LatticeHashNode*> latticeMap;

};

  




#endif	/* LATTICEALGOS_H */


