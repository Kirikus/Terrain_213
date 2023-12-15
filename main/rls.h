#ifndef RLS_H
#define RLS_H

#include <QWidget>
#include <map>

#include "../lib/point.h"
#include "../lib/frenel_multiplier.h"

namespace RC = ReflectionCoefficients;

namespace Ui {
class RLS;
}

class RLS : public QWidget
{
    Q_OBJECT

public:
    explicit RLS(QWidget *parent = nullptr);
    ~RLS();

    struct Data
    {
        PointCartesian position;

        double radius;
        double top_angle;
        double bottom_angle;
        double wave_length;

        RC::Polarization polarization = RC::HorizontalPolarization;
    };

    Data get_all_data();  // find position and parameters of RLS

private:
    Ui::RLS *ui;
};

#endif // RLS_H
