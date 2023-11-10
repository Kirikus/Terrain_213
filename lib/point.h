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

class CartesianPoint : Point
{
private:
    double x;
    double y;
    double h;

public:

    CartesianPoint() = default;
    CartesianPoint(double x, double y, double h)
        :x{x}, y{y}, h{h}
    {}

    virtual double get_x() override {return x;};
    virtual double get_y() override {return y;};
    virtual double get_h() override {return h;};
};

