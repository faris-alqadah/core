/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2009
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  An n-cluster is essentially a vector of IOSets with some additonal properties
 *  such as
 *  
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/

#ifndef _NCLUSTER_H
#define	_NCLUSTER_H

#include "../Headers/IOSet.h"

using namespace std;

class NCluster {
public:
    NCluster(unsigned int); //constructor that only specifies n
    NCluster(unsigned int, bool);
    NCluster(unsigned int, vector<IOSet*> &); //constructor that assigns n and all data to ncluster
    NCluster(NCluster &); //copy constructor for deep copy
    ~NCluster(); //destructor

    void DeepCopy(NCluster&); //make a deep copy of an ncluster
    void Output(); //output the nCluster (all the sets)
    void Output(ofstream&); //output the ncluster to a file
    void Output(ofstream&, vector<NameMap *>&nm); //outptut ncluster with names to file
    bool IsHeight(int); //returns if the set specified by i contributes to height of the ncluster
    void SetAsHeight(int); //make the set specified by the parameter a "height" set
    double GetHeight(); // get the height of an ncluster
    double GetWidth(); //get the width of the ncluster
    void SetHeight(double); //Set the height
    void SetWidth(double); //set the width
    int GetN(); //get n
    vector<NCluster*>* GetUppers(); //return pointer to the upper neighbors
    vector<NCluster*>* GetLowers(); //return pointer to the lower neighbors
    void SetUpperNeighbors(vector<NCluster*> &); //set the upper neighbors
    void SetLowerNeighbors(vector<NCluster*> &); //set the lower neighbors
    IOSet * GetSet(int); //get pointer to the specified by the argument
    void AssignSet(int, IOSet*); //assign the set specified by the first argument to the set specified in the 2nd argument
    double GetQuality(); //return the quality of a cluster
    double SetQuality(double); //set the quality of a cluster
    void InitalizeSet(int,IOSet*);  //set the ith set to its intial value of the pointer
        void ComputeHeight();   //compute the height of the ncluster
    void ComputeWidth();    //compute the width of the ncluster

    ////////////////////////////////////////////////////////////////////////////////
    // Reads concepts from a pre-computed lattice
    //Assumes global variable latticeInputFile is defined
    ////////////////////////////////////////////////////////////////////////////////
   friend void ReadConcepts(ifstream&,int,int,int);

   ////////////////////////////////////////////////////////////////////////////////
    //Function used by ReadLattice to process each concept
    ////////////////////////////////////////////////////////////////////////////////
    friend NCluster * ProcessNode(string&,bool);

    ////////////////////////////////////////////////////////////////////////////////
    // Reads in a lattice from a file of pre-computed concept lattice
    //Global variable latticeInputFile should be declared. This function is called by'
    // ReadPreLattice()
    ////////////////////////////////////////////////////////////////////////////////
    friend void ReadLattice(ifstream&,int,int,int);

private:
    unsigned int n; //degree of cluster
    vector<IOSet*> sets; //the sets
    double quality;  //quality of the ncluster



};





#endif

