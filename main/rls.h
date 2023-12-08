#ifndef RLS_H
#define RLS_H

#include <QWidget>
#include <map>

#include "../lib/point.h"

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
    };

    Data get_all_data();  // find position and parameters of RLS

private:
    Ui::RLS *ui;
};

#endif // RLS_H
