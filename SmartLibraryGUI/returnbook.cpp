#include "returnbook.h"
#include "ui_returnbook.h"

returnBook::returnBook(QWidget *parent) : QDialog(parent), ui(new Ui::returnBook)
{
    ui->setupUi(this);
    connect(globalObj, &GlobalProcess::bleRead, this, &returnBook::bleRead);
}

returnBook::~returnBook()
{
    disconnect(globalObj, &GlobalProcess::bleRead, this, &returnBook::bleRead);
    delete ui;
}

void returnBook::bleRead(QString isbn)
{
    if(isbn.startsWith("978") && isbn.length() == 13)
    {
        if(isbn != ui->isbn_Edit->text().remove("-"))
        {
            ui->isbn_Edit->setText(isbn);
            ui->isbn_Edit->setInputMask("999-9-9999-9999-9");
            bookInfo();
        }
    }
    else
    {
        ui->Tip->setText("条码错误，请重新扫描");
    }
    ui->scan_Btn->setEnabled(true);
}

void returnBook::bookInfo()
{
    ui->name_Edit->clear();
    ui->author_Edit->clear();
    ui->press_Edit->clear();
    ui->pressDate_Edit->clear();
    ui->pressPlace_Edit->clear();
    ui->price_Edit->clear();
    ui->pages_Edit->clear();
    ui->words_Edit->clear();
    ui->clc_Edit->clear();
    ui->bookDesc_Edit->clear();
    ui->shelfNum_Edit->clear();
    ui->isBorrowed_Label->clear();
    ui->Tip->clear();

    QString isbn = ui->isbn_Edit->text().remove("-");
    if(sql.exists(isbn))
    {
        QSqlRecord record = sql.getOneBookInfo("isbn", isbn);
        ui->name_Edit->setText(record.value("bookName").toString());
        ui->author_Edit->setText(record.value("author").toString());
        ui->press_Edit->setText(record.value("press").toString());
        ui->pressDate_Edit->setText(record.value("pressDate").toString());
        ui->pressPlace_Edit->setText(record.value("pressPlace").toString());
        ui->price_Edit->setText(record.value("price").toString());
        ui->pages_Edit->setText(record.value("pages").toString());
        ui->words_Edit->setText(record.value("words").toString());
        ui->clc_Edit->setText(record.value("clcName").toString());
        ui->bookDesc_Edit->setText(record.value("bookDesc").toString());
        ui->shelfNum_Edit->setText(record.value("shelfNumber").toString());
        ui->isBorrowed_Label->setPixmap(record.value("isBorrowed").toBool()?cross:tick);
        ui->return_Btn->setEnabled(true);
    }
    else
    {
        ui->Tip->setText("此书不在数据库中");
        ui->return_Btn->setEnabled(false);
    }
}

void returnBook::on_scan_Btn_clicked()
{
    ui->scan_Btn->setEnabled(false);
    globalObj->SocketWrite("scan");
}

void returnBook::on_return_Btn_clicked()
{
    QString isbn = ui->isbn_Edit->text().remove("-");
    QSqlRecord record = sql.getOneBookInfo("isbn", isbn);
    globalObj->SocketWrite(QString("带我去,%1").arg(record.value("shelfNumber").toString()));
    ui->Tip->setText(QString("小车正在启动，前往%1号书架").arg(record.value("shelfNumber").toString()));
    ui->return_Btn->setEnabled(false);
}

void returnBook::on_quit_Btn_clicked()
{
    close();
}
