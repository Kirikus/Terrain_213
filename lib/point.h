#ifndef POINT_H
#define POINT_H
#include <cmath>

class Point
{
protected:
    virtual double get_x();
    virtual double get_y();
    virtual double get_h();

public:
    Point() = default;
};
