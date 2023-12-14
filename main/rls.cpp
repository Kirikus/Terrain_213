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
    data.position.change_x(ui->x->value());
    data.position.change_y(ui->y->value());
    data.position.change_z(ui->z->value());

    data.radius = ui->radius->value();
    data.top_angle = ui->top_angle->value();
    data.bottom_angle = ui->bottom_angle->value();
    data.wave_length = ui->wave_length->value();

    int polarization_id = ui->polarization_type->checkedId();
    switch (polarization_id) {
    case 0:
        data.polarization = RC::VerticalPolarization;
        break;
    case 1:
        data.polarization = RC::HorizontalPolarization;
        break;
    case 2:
        data.polarization = RC::CircularPolarization;
        break;
    case 3:
        data.polarization = RC::CrossPolarization;
        break;
    }

    return data;
}
