#include "rls.h"
#include "ui_rls.h"

RLS::RLS(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RLS)
{
    ui->setupUi(this);
}

RLS::~RLS()
{
    delete ui;
}

void RLS::reset_data()
{
    ui->latitude_degree->setValue(0);
    ui->latitude_minutes->setValue(0);
    ui->latitude_seconds->setValue(0);

    ui->longitude_degree->setValue(0);
    ui->longitude_minutes->setValue(0);
    ui->longitude_seconds->setValue(0);

    ui->bottom_angle->setValue(0);
    ui->top_angle->setValue(0);
    ui->high->setValue(0);
    ui->radius->setValue(0);
}
