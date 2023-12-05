#ifndef RLS_H
#define RLS_H

#include <QWidget>
#include <map>

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
        double latitude_degree;
        double latitude_minutes;
        double latitude_seconds;

        double longitude_degree;
        double longitudee_minutes;
        double longitude_seconds;

        double high;
        double radius;
        double top_angle;
        double bottom_angle;
    };

    Data get_all_data();  // find position and parameters of RLS

private:
    Ui::RLS *ui;
};

#endif // RLS_H
