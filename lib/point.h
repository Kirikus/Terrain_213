#ifndef POINT_H
#define POINT_H
#include <cmath>

class Point
{
public:
    Point() = default;

    virtual double get_x() const = 0;
    virtual double get_y() const = 0;
    virtual double get_h() const = 0;
};

class Point2d : public Point
{
public:
    Point2d(double x, double y)
        :x{x}, y{y}
    {}

    virtual double get_x() const override {return x;}
    virtual double get_y() const override {return y;}
    virtual double get_h() const override {return 0;}

private:
    double x; // in meters
    double y; // in meters
};

class PointCartesian : public Point
{
private:
    double x; // in meters
    double y; // in meters
    double h; // in meters

public:

    PointCartesian() = default;
    PointCartesian(double x, double y, double h)
        :x{x}, y{y}, h{h}
    {}
    PointCartesian(Point2d p, double h)
        :x{p.get_x()}, y{p.get_y()}, h{h}
    {}

    virtual double get_x() const override {return x;};
    virtual double get_y() const override {return y;};
    virtual double get_h() const override {return h;};
};

class PointSpheric : public Point
{
public:
    double get_R() const {return R;};
    double get_phi() const {return phi;};
    double get_azimuth() const {return azimuth;};
    double get_d() const {return R*cos(phi);}
    PointCartesian get_center() const {return center;}
    PointCartesian get_target() const {return target;}

    virtual double get_x() const override {return target.get_x() - center.get_x();};
    virtual double get_y() const override {return target.get_y() - center.get_y();};
    virtual double get_h() const override {return target.get_h() - center.get_h();};

    PointSpheric() = default;
    PointSpheric(PointCartesian center, PointCartesian target);
    PointSpheric(const PointSpheric& sp);

    void change_r(double new_R);
    void change_azimuth(double new_azimuth);
    void change_phi(double new_phi);
    void change_d(double new_d);
    void move_target(double new_x, double new_y, double new_h);

private:
    PointCartesian center;
    PointCartesian target;

    double azimuth; //in radians
    double R; //in meters
    double phi; //in radians
};

#endif // POINT_H
