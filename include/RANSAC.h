#pragma once

#include <set>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits>


#include "ParameterEstimator.h"

/**
 * This class implements the RAndom SAmple Consensus (RANSAC) framework,
 * a framework for robust parameter estimation.
 * Given data containing outliers we estimate the model parameters using sub-sets of
 * the original data:
 * 1. Choose the minimal subset from the data for computing the exact model parameters.
 * 2. See how much of the input data agrees with the computed parameters.
 * 3. Goto step 1. This can be done up to (N choose m) times, where m is the number of
 *    data objects required for an exact estimate and N is the total number of data objects.
 * 4. Take the largest subset of objects which agreed on the parameters and compute a
 *    least squares fit using them.
 * 
 * This is based on:
 * Fischler M.A., Bolles R.C., 
 * ``Random Sample Consensus: A Paradigm for Model Fitting with Applications to Image Analysis and Automated Cartography'', 
 * Communications of the ACM, Vol. 24(6), 1981.
 *
 * Hartely R., Zisserman A., "Multiple View Geometry in Computer Vision", 2001.
 *
 * The class template parameters are T - objects used for the parameter estimation 
 *                                      (e.g. Point2D in line estimation, 
 *                                            std::pair<Point2D,Point2D> in homography estimation).
 *                                   S - type of parameter (e.g. double).                          
 *
 * @author: Ziv Yaniv (zivy@isis.georgetown.edu)
 *
 */
template<typename T , typename S>
class RANSAC{
public:
/**
 * @brief 
 * 
 * @param parameters the output model parameters
 * @param parameterEstimator 
 *                   Use n(least number of model) to estimate 
 *                  or Use least squares fit;
 * @param data      the input 
 * @param desiredProbabilityForNoOutliers the z of the paper
 * @return double return the percentage of data used in the least squares estimate
 */
    static double compute(std::vector<S> & parameters , 
                        ParameterEstimator<T , S>* parameterEstimator,
                        std::vector<T>& data , 
                        double desiredProbabilityForNoOutliers);


private:
/**
 * @brief pick m from n
 *      n!
 *  ------------
 *    m!*(n-m)!
 */ 
    unsigned int choose(unsigned int n , unsigned int m);

/**
 * @brief 
 * for second parameter in std::set<int* , SubSetIndexComparator>;
 */
   class SubSetIndexComparator{
   private:
      int length;
   public:
      SubSetIndexComparator(int arrayLength): length(arrayLength){}

      bool operator() (const int* arr1 , const int * arr2) const{
         for(int i = 0 ; i<this->length ; i++){
            if(arr1[i] < arr2[i]) return true;
            else if(arr1[i]>arr2[i]) return false;
         }
         return false;
      }
   }; //end of class SubSetIndexComparator

};