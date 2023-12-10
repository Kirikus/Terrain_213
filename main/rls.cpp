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

void RLS::get_all_data()
{
    this->latitude_degree = ui->latitude_degree->value();
    this->latitude_minutes = ui->latitude_minutes->value();
    this->latitude_seconds = ui->latitude_seconds->value();

    this->longitude_degree = ui->longitude_degree->value();
    this->longitude_degree = ui->longitude_minutes->value();
    this->longitude_degree = ui->longitude_seconds->value();

    this->high = ui->high->value();
    this->radius = ui->radius->value();
    this->top_angle = ui->top_angle->value();
    this->bottom_angle = ui->bottom_angle->value();
}
