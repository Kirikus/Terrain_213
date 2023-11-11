#ifndef POINT_H
#define POINT_H
#include <cmath>

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
    double x;
    double y;
    double h;

public:

    PointCartesian() = default;
    PointCartesian(double x, double y, double h)
        :x{x}, y{y}, h{h}
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

    virtual double get_x() override {return target.get_x();};
    virtual double get_y() override {return target.get_y();};
    virtual double get_h() override {return target.get_h();};

    PointSpheric() = default;
    PointSpheric(PointCartesian center, PointCartesian target);
    PointSpheric(const PointSpheric& sp);

    void change_r(double new_R) {this->R = new_R;}
    void change_azimuth(double new_azimuth) {this->azimuth = new_azimuth;}
    void change_phi(double new_phi) {this->phi = new_phi;}

private:
    PointCartesian center;
    PointCartesian target;

    double azimuth;
    double R;
    double phi;
};

#endif // POINT_H
