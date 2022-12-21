/**
 * @file lineParameterEstimator.cpp
 * @author TJL
 * @brief  
 * @version 0.1
 * @date 2022-12-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "lineParameterEstimator.h"
#include "Point2D.h"
#include "math.h"
LineParameterEstimator::LineParameterEstimator(double delta)
            :ParameterEstimator(2) , 
            _deltaSquared(delta*delta) {}


void LineParameterEstimator::estimate(std::vector<Point2D* > &data , std::vector<double> & parameters){
    parameters.clear();

    if(data.size()<this->_minForEstimate) return ;

    double nx = data[1]->_y - data[0]->_y;
    double ny = data[0]->_x - data[1]->_x;
    double norm = sqrt(nx * nx + ny * ny);

    parameters.push_back(nx/norm);
    parameters.push_back(ny/norm);
    parameters.push_back(data[0]->_x);
    parameters.push_back(data[1]->_y);
    
    return ;
}

void LineParameterEstimator::leastSquaresEstimate(std::vector<Point2D*>& data , 
                                                std::vector<double> & parameters){
double meanX, meanY, nx, ny, norm;
	double covMat11, covMat12, covMat21, covMat22; // The entries of the symmetric covarinace matrix
	int i, dataSize = data.size();

	parameters.clear();
	if(data.size()<this->_minForEstimate)
		return;

	meanX = meanY = 0.0;
	covMat11 = covMat12 = covMat21 = covMat22 = 0;
	for(i=0; i<dataSize; i++) {
		meanX +=data[i]->_x;
		meanY +=data[i]->_y;

		covMat11	+=data[i]->_x * data[i]->_x;
		covMat12	+=data[i]->_x * data[i]->_y;
		covMat22	+=data[i]->_y * data[i]->_y;
	}

	meanX/=dataSize;
	meanY/=dataSize;

	covMat11 -= dataSize*meanX*meanX;
  covMat12 -= dataSize*meanX*meanY;
	covMat22 -= dataSize*meanY*meanY;
	covMat21 = covMat12;

	if(covMat11<1e-12) {
		nx = 1.0;
	  ny = 0.0;
	}
	else {	    //lamda1 is the largest eigen-value of the covariance matrix 
	           //and is used to compute the eigne-vector corresponding to the smallest
	           //eigenvalue, which isn't computed explicitly.
		double lamda1 = (covMat11 + covMat22 + sqrt((covMat11-covMat22)*(covMat11-covMat22) + 4*covMat12*covMat12)) / 2.0;
		nx = -covMat12;
		ny = lamda1 - covMat22;
		norm = sqrt(nx*nx + ny*ny);
		nx/=norm;
		ny/=norm;
	}
	parameters.push_back(nx);
	parameters.push_back(ny);
	parameters.push_back(meanX);
	parameters.push_back(meanY);
}



bool LineParameterEstimator::agree(std::vector<double>& parameters , Point2D &data){
    double distance = parameters[0]*(data._x - parameters[2]) + parameters[1]*(data._y - parameters[3]);
    return distance * distance < this->_deltaSquared;
}