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

std::map<std::string, double> RLS::get_all_data()
{
    std::map<std::string, double> rls_info;
    rls_info["latitutude_degree"] = ui->latitude_degree->value();
    rls_info["latitutude_minutes"] = ui->latitude_minutes->value();
    rls_info["latitutude_seconds"] = ui->latitude_seconds->value();

    rls_info["longitude_degree"] = ui->longitude_degree->value();
    rls_info["longitude_degree"] = ui->longitude_minutes->value();
    rls_info["longitude_degree"] = ui->longitude_seconds->value();

    rls_info["high"] = ui->high->value();
    rls_info["radius"] = ui->radius->value();
    rls_info["top_angle"] = ui->top_angle->value();
    rls_info["bottom_angle"] = ui->bottom_angle->value();

    return rls_info;
}
