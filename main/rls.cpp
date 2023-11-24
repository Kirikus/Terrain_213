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
