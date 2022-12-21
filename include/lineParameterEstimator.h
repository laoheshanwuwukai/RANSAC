#pragma once
/**
 * @file lineParameterEstimator.h
 * @author TJL
 * @brief LineParameterEstimator

 * @copyright Copyright (c) 2022
 * 
 */


#include "ParameterEstimator.h"
#include "Point2D.h"


/**
 * estimates the parameters of 2D lines
 * n is line normal 
 * p is the point int line
 * a is a point in line
 *  dot(n , p-a) = 0;
 */


class LineParameterEstimator
        :public ParameterEstimator<Point2D , double>{

public:
    LineParameterEstimator(double delta);
/**
 * @brief Use the mininum number of datas to create a model
 * 
 * @param data At least two Points
 * @param parameters Out put parameters of model
 *                  [n_x , n_y , pnt_x , pnt_y] 
 * if the number of data is less than 2 , parameters return null
 */
    virtual void estimate(std::vector<Point2D* > &data , std::vector<double> &parameters);
    

/**
 * @brief Use all data to compute the model parameters
 * @param data input data 
 * @param parameter output parameters;
 */
    virtual void leastSquaresEstimate(std::vector<Point2D*> & data , std::vector<double> & parameter) ;

/**
 * @brief Return true if a point2D is inliner,else return false;
 * 
 * @param parameters model parameters
 * @param data 
 * @return true 
 * @return false 
 */
    virtual bool agree(std::vector<double> & parameters , Point2D& data) ;

    static void debugTest(std::ostream & out);
private:

    double _deltaSquared ; //if dis(P , L)^2<delta^2 the point is inliner
};