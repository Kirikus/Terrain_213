#ifndef RLS_H
#define RLS_H

#include <QWidget>

namespace Ui {
class RLS;
}

class RLS : public QWidget
{
    Q_OBJECT

public:
    explicit RLS(QWidget *parent = nullptr);
    ~RLS();
    void reset_data(); // reset all input data and configuration to default

private:
    Ui::RLS *ui;
};

#endif // RLS_H
