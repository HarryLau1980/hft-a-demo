#include "ledctrl.h"
#include "ui_ledctrl.h"

LedCtrl::LedCtrl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedCtrl)
{
    ui->setupUi(this);
}

LedCtrl::~LedCtrl()
{
    delete ui;
}
