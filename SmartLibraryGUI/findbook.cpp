#include "findbook.h"
#include "ui_findbook.h"

findBook::findBook(QWidget *parent) : QDialog(parent), ui(new Ui::findBook)
{
    ui->setupUi(this);
}

findBook::~findBook()
{
    delete ui;
}
