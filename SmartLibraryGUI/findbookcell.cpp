#include "findbookcell.h"
#include "ui_findbookcell.h"

findbookcell::findbookcell(QWidget *parent) : QWidget(parent), ui(new Ui::findbookcell)
{
    ui->setupUi(this);
    this->bookName = ui->bookName;
    this->press = ui->press;
    this->author = ui->author;
}

findbookcell::~findbookcell()
{
    delete ui;
}
