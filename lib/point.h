#ifndef POINT_H
#define POINT_H
#include <cmath>

class Point2d
{
public:
    Point2d(double x, double y)
        :x{x}, y{y}
    {}

    double get_x() {return x;}
    double get_y() {return y;}
private:
    double x;
    double y;
};

class Point
{
protected:
    virtual double get_x() = 0;
    virtual double get_y() = 0;
    virtual double get_h() = 0;

public:
    Point() = default;
};

class PointCartesian : Point
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

    virtual double get_x() override {return x;};
    virtual double get_y() override {return y;};
    virtual double get_h() override {return h;};
};

class PointSpheric : Point
{
public:
    double get_R() {return R;};
    double get_phi() {return phi;};
    double get_azimuth() {return azimuth;};
    double get_d() {return R*cos(phi);}
    PointCartesian get_center() {return center;}
    PointCartesian get_target() {return target;}

    virtual double get_x() override {return target.get_x() - center.get_x();};
    virtual double get_y() override {return target.get_y() - center.get_y();};
    virtual double get_h() override {return target.get_h() - center.get_h();};

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

struct PointScreenAngle
{
public:
    Point2d point2d;
    double screening_angle;

    PointScreenAngle(Point2d point, double angle) : point2d{point}, screening_angle(angle) {};
    PointScreenAngle() : point2d(0, 0), screening_angle(-1) {};

    bool empty() { return screening_angle == -1 ? true : false; }
};

#endif // POINT_H
