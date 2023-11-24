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

    std::map<std::string, double> get_all_data();  // return position and parameters of RLS

private:
    Ui::RLS *ui;
};

#endif // RLS_H
