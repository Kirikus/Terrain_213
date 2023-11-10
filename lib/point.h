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

class SphericPoint : Point
{
public:
    double get_R() {return R;};
    double get_phi() {return phi;};
    double get_azimuth() {return azimuth;};

    virtual double get_x() override {return center.get_x();};
    virtual double get_y() override {return center.get_y();};
    virtual double get_h() override {return center.get_h();};

    SphericPoint() = default;
    SphericPoint(CartesianPoint center, CartesianPoint target);
    SphericPoint(const SphericPoint& sp);

    void change_r(double new_R) {this->R = new_R;}
    void change_azimuth(double new_azimuth) {this->azimuth = new_azimuth;}
    void change_phi(double new_phi) {this->phi = new_phi;}

private:
    CartesianPoint center;
    double azimuth;
    double R;
    double phi;
};

#endif // POINT_H
