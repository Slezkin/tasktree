#include "desk.h"
#include "ui_desk.h"

desk::desk(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::desk)
{
    ui->setupUi(this);
}

desk::~desk()
{
    delete ui;
}
