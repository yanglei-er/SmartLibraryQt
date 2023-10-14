#include "btdevcell.h"
#include "ui_btdevcell.h"

BtDevCell::BtDevCell(QWidget *parent) : QWidget(parent),ui(new Ui::BtDevCell)
{
    ui->setupUi(this);
    this->btName = ui->btName;
    this->btAddr = ui->btAddr;
    this->btRssi = ui->btRssi;
}

BtDevCell::~BtDevCell()
{
    delete ui;
}
