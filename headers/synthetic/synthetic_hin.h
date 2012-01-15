//! Author: Faris Alqadah
/*!
 Generate synthetic HINs using mostly power laws

 */
#ifndef _SYNTHETIC_HIN_H
#define	_SYNTHETIC_HIN_H

#include "../core/LatticeAlgos.h"
#include "../../headers/core/Ops.h"

//! Generates a synthetic context using power law distributions and writes to a file
/*!
    \param cardA number of objects in first domain
    \param cardB number of objects in second domain
    \param fileName name of file to write FIMI file and subsequently name files
 *
 */
void MakeSynContext(int cardA,int cardB, string &fileName);


//! Generates a scale free node degree distribution, randomly selects value of gamma between 2 and 3
/*!
    \param maxK the largest possible node degree
    \param dist empty vector that will hold the distribution after execution
 */
void ScaleFreeDist(int maxK,vector<double> &dist);

//! Generates a scale free node degree distribution, randomly selects value of gamma between 2 and 3
/*!
    \param maxK the largest possible node degree
    \param c normalizing constant
    \param gamma gamma parameter (typically between 2 and 3)
    \param dist empty vector that will hold the distribution after execution
 */
void ScaleFreeDist(int maxK,double c,double gamma, vector<double> &dist);


//! Generates a scale free node degree distribution, returns the normailzing factor
/*!
    \param maxK the largest possible node degree
    \param gamma gamma parameter (typically between 2 and 3)
    \param dist empty vector that will hold the distribution after execution
 */
double ScaleFreeDist(int maxK,double gamma, vector<double> &dist);

//! Assign degree sizes according the provided scale free node degree distribution
/*!
    \param numNodes the number of nodes
    \param distb the scale free node degree distribution
    \param dedgress empty vector that will hold the degrees after execution
 */
void AssignDegrees(int numNodes, vector<double> &distb, vector<int> &degrees);

#endif	/* _SYNTHETIC_HIN_H */

