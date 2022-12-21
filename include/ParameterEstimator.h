/**
 * @file ParameterEstimator.h
 * @author TJL
 * @brief Parameter estimation interface:
 *      1.estimate() - estimate the model using n data;
 *      2.leastSquaresEstimate() - using concensus_set to update model
 *      3.agree() - if data[i] is a inlier 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include <vector>
/**
 * @brief 
 * 
 * @tparam T data type
 * @tparam S model parameter type
 */
template<typename T , typename S>
class ParameterEstimator{
    public:

    ParameterEstimator(unsigned int minElements) :_minForEstimate(minElements){}
    /**
     * @brief init estimate using the least points
     * 
     * @param data The data used for the estimate;
     * @param parameters output parameters of estimation
     */
    virtual void estimate(std::vector<T*> &data , std::vector<S>& parameters) = 0;

    /**
     * @brief using the guess_concensus_set to estimate the model
     * 
     * @param data guess_concensus_set data 
     * @param parameter output
     */
    virtual void leastSquaresEstimate(std::vector<T*> & data , std::vector<S> & parameter) = 0;

    /**
     * @brief 
     * This method tests if the given data agrees with the given model parameters
     */
    virtual bool agree(std::vector<S> & parameters , T&data) = 0;
    
    unsigned int numForEstimate(){return _minForEstimate;}

protected:

    unsigned int _minForEstimate; //least number of elements to determinate a model;
};