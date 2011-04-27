//! Author: Faris Alqadah
/*!Class for representing an real-valued object-sets such as tf-idf word sets
 along with their tf-idf value. Since these sets can be sparse (or more accuratley they
 are derived form sparse matrices) an index is maintained. Essentially is an IOSet
 with an added real-valued vector.
 */
#ifndef RSET_H
#define	RSET_H

#include "../headers/IOSet.h"
class RSet {
public:

   //! Default constructor
    RSet();
       //! Constructor that pre-allocates the size of the RSet
    RSet(int sz);
    //! Copy constructor
    RSet(RSet *a);
       //! Destructor
    ~RSet();
    //! Returns the size or number of elements in the RSet
    int Size();
    //! Returns the id of the RSet
    int Id();
     //! Set the id of the IOSet
    void SetId(int id);
    //! Prints contents of the RSet as space sperated pairs of real,intergers to stdout
    void Output();
    //! Prints contents of the RSet as space sperated pairs to ofstream
    void Output(ofstream& f);
    //! Prints contents of the RSet as space sperated pairs of names and values to ofstream using namemap to map the integers to names
    void Output(ofstream &f, NameMap *n);
   //! Adds pair to the end of the RSet, increasing the size of the RSet
    void Add(pair<int,double>);
    //! Assigns the private size variable of the RSet without actually re-allocationg memory
    /*!
     This operation should mainly be used by set operation algorithms where size of the
     RSet may not be known a-priori.
     */
    void SetSize(int x);

    //! Resize the RSet to x, this will physically re-allocate and de-allocate memory unlike SetSize()
    void Resize(int x);
    //! Returns true if both RSets have the same size and the exact contenets in the exact order and false otherwise
    bool Equal(RSet&);
    //! Returns true if the RSet contains the value,index pair specified
    bool Contains(pair<int,double>);
    //! Make a deep copy of the input RSet and assign it to self
    void DeepCopy(RSet*);
    //! Remove the element at the specified index
    void Remove(int);
    //! Find and remove the first element which has value=val if it exists
    void FindRemove(pair<int,double>);
    //! Find an element with the given id, return NaN if not found
    double FindReturn(int);
    //! Sort the elements of the RSet in asscending order
    void Sort();
    //! Return the ith element
    pair<int,double> At(int i);
    //! Return an IOSet of what indexes have values associated with them
    IOSet *GetIdxs();
    //! Remove all elements from the RSet
    void Clear();
    //! Set the marked flag
    void SetMarked(bool);
    //! Returns the value of the marked flag
    bool GetMarked();
    //! Returns the largest element in the RSet (largest in terms of value)
    pair<int,double> GetMaxElement();
    //! Returns the smalles element in the RSet (smallets in terms of value)
    pair<int,double> GetMinElement();
    //! Returns the largest index in self
    int GetMaxIdx();
    //! Returns the variance of this RSet
    double Variance();
    //! Returns the standard deviation of self
    double Std_Dev();
    //! Returns the mean of self
    double Mean();
    //! Set the quality
    void SetQuality(double);
    //!Returns the quality
    double GetQuality();
private:
    //! size of the RSet
    int size;
    //! id of the RSet
    int id;
    //! vector to hold the values
    vector< pair<int,double> > vals;
    //! has this RSet been marked for whatever reason??
    bool marked;
    //! the quality of this RSet
    double quality;

};

//! Compartor function used for IOSets, returns true if the a->GetSize() > b->GetSize()
bool RSet_Compare_Sup(IOSet *a, IOSet *b);

//! Compartor function used for IOSets, returns true if the a->GetId() > b->GetId()
bool RSet_Compare_Id(IOSet *a, IOSet *b);


#endif	/* RSET_H */

