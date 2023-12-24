#include "borrowbook.h"
#include "ui_borrowbook.h"

borrowBook::borrowBook(QWidget *parent) : QDialog(parent), ui(new Ui::borrowBook)
{
    ui->setupUi(this);
    connect(globalObj, &GlobalProcess::bleRead, this, &borrowBook::bleRead);
}

borrowBook::~borrowBook()
{
    disconnect(globalObj, &GlobalProcess::bleRead, this, &borrowBook::bleRead);
    delete ui;
}

void borrowBook::bookInfo()
{

}

void borrowBook::bleRead(QString isbn)
{
    if(isbn.startsWith("978") && isbn.length() == 13)
    {
        if(isbn != ui->isbn_Edit->text().remove("-"))
        {
            ui->isbn_Edit->setText(isbn);
            ui->isbn_Edit->setInputMask("999-9-9999-9999-9");
            ui->borrow_Btn->setEnabled(true);
            bookInfo();
        }
    }
    else
    {
        ui->Tip->setText("条码错误，请重新扫描");
        ui->borrow_Btn->setEnabled(false);
    }
    ui->scan_Btn->setEnabled(true);
}

void borrowBook::on_scan_Btn_clicked()
{
    ui->scan_Btn->setEnabled(false);
    globalObj->SocketWrite("scan");
}

void borrowBook::on_borrow_Btn_clicked()
{

}

void borrowBook::on_quit_Btn_clicked()
{
    close();
}
