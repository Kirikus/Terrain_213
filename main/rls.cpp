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

RLS::Data RLS::get_all_data()
{
    Data data;
    data.latitude_degree = ui->latitude_degree->value();
    data.latitude_minutes = ui->latitude_minutes->value();
    data.latitude_seconds = ui->latitude_seconds->value();

    data.longitude_degree = ui->longitude_degree->value();
    data.longitude_degree = ui->longitude_minutes->value();
    data.longitude_degree = ui->longitude_seconds->value();

    data.high = ui->high->value();
    data.radius = ui->radius->value();
    data.top_angle = ui->top_angle->value();
    data.bottom_angle = ui->bottom_angle->value();
    return data;
}
