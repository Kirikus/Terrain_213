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

private:
    Ui::RLS *ui;
};

#endif // RLS_H
