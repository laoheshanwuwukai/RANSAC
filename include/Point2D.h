/**
 * @file Point2D.h
 * @author TJL
 * @brief Point Data type
 * @version 0.1
 * @date 2022-12-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include <iostream>
class Point2D
{
public:
    Point2D(double px , double py) : _x(px) , _y(py){}

    //~Point2D();

    double _x , _y;
};

inline std::ostream& operator<<(std::ostream &output , const Point2D& pnt){
    output<<pnt._x<<" "<<pnt._y<<std::endl;
    return output;
}