#include "viewbook.h"
#include "ui_viewbook.h"

viewBook::viewBook(QWidget *parent) : QDialog(parent), ui(new Ui::viewBook)
{
    ui->setupUi(this);
    ui->isbn_Edit->setFocus();
    if(globalObj->isBleConnect())
    {
        ui->isbn_Edit->setPlaceholderText("请扫描或输入13位ISBN码");
        connect(globalObj, &GlobalProcess::bleRead, this, &viewBook::bleRead);
    }
}

viewBook::~viewBook()
{
    delete ui;
    if(globalObj->isBleConnect())
    {
        disconnect(globalObj, &GlobalProcess::bleRead, this, &viewBook::bleRead);
    }
}

void viewBook::on_isbn_Edit_textChanged(const QString &str)
{
    qDebug()<<str;
    if(str == "----")
    {
        ui->isbn_Edit->setInputMask("");
        ui->attitude_label->clear();
    }
    else if(str.length() == 1)
    {
        ui->isbn_Edit->setInputMask("999-9-9999-9999-9");
        ui->isbn_Edit->setCursorPosition(1);
        ui->attitude_label->setPixmap(wrong);
    }
    else if(str.length() != 17)
    {
        ui->search_Btn->setEnabled(false);
        ui->attitude_label->setPixmap(wrong);
    }
    else if(str.length() == 17)
    {
        ui->isbn_Edit->setInputMask("999-9-9999-9999-9");
        ui->search_Btn->setEnabled(true);
        ui->attitude_label->setPixmap(TOOLS::loadImage(":/pic/right.png", QSize(40,40)));
    }
}


void viewBook::on_search_Btn_clicked()
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
    }
    else
    {
        ui->Tip->setText("此书不在数据库中");
    }
    ui->search_Btn->setEnabled(false);
}

void viewBook::on_isbn_Edit_returnPressed()
{
    if(ui->isbn_Edit->text().length() == 17)
    {
        on_search_Btn_clicked();
    }
}

void viewBook::bleRead(QString str)
{
    if(str.startsWith("978") && str.length() > 13)
    {
        QString isbn = str.mid(0, 13);
        if(isbn != ui->isbn_Edit->text().remove("-"))
        {
            ui->isbn_Edit->setText(isbn);
            ui->isbn_Edit->setInputMask("999-9-9999-9999-9");
            ui->isbn_Edit->setCursorPosition(17);
            on_search_Btn_clicked();
        }
    }
    else
    {
        ui->Tip->setText("条码错误，请重新扫描");
    }
}

void viewBook::on_quit_Btn_clicked()
{
    close();
}
