#include "borrowbook.h"
#include "ui_borrowbook.h"

borrowBook::borrowBook(QWidget *parent) : QDialog(parent), ui(new Ui::borrowBook)
{
    ui->setupUi(this);
}

borrowBook::~borrowBook()
{
    delete ui;
}

void borrowBook::on_quit_Btn_clicked()
{
    close();
}

