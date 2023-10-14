#include "bookcell.h"
#include "ui_bookcell.h"

bookCell::bookCell(QWidget *parent) : QWidget(parent), ui(new Ui::bookCell)
{
    ui->setupUi(this);
    this->attitude = ui->attitude;
    this->bookName = ui->bookName;
    this->bookDesc = ui->bookDesc;
    this->press = ui->press;
    this->author = ui->author;
}

bookCell::~bookCell()
{
    delete ui;
}
