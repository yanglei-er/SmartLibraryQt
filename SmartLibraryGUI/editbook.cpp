#include "editbook.h"
#include "ui_editbook.h"

editBook::editBook(const QString &isbn, QWidget *parent) : QDialog(parent),ui(new Ui::editBook)
{
    ui->setupUi(this);
    ui->isbn_Edit->setInputMask("999-9-9999-9999-9");
    QSqlRecord record = sql.getOneBookInfo("isbn", isbn);
    ui->isbn_Edit->setText(isbn);
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
    isBorrowed = record.value("isBorrowed").toBool();
    ui->isBorrowed_Btn->setText(isBorrowed?"已借":"可借");
    ui->edit_Btn->setEnabled(false);
    ui->quit_Btn->setFocus();
    setWindowTitle(QString("编辑图书 - 《%1》").arg(record.value("bookName").toString()));
    needUpdate = false;
}

editBook::~editBook()
{
    delete ui;
}

void editBook::on_edit_Btn_clicked()
{
    QString isbn = ui->isbn_Edit->text().remove("-");
    QString bookName = ui->name_Edit->text();
    QString author = ui->author_Edit->text();
    QString press = ui->press_Edit->text();
    QString pressDate = ui->pressDate_Edit->text();
    QString pressPlace = ui->pressPlace_Edit->text();
    qreal price = ui->price_Edit->text().toDouble();
    QString clcName = ui->clc_Edit->text();
    QString bookDesc = ui->bookDesc_Edit->toPlainText();
    QString pages = ui->pages_Edit->text();
    QString words = ui->words_Edit->text();
    int shelfNum = ui->shelfNum_Edit->text().toInt();
    sql.editBook(isbn, bookName, author, press, pressDate, pressPlace, price, clcName, bookDesc, pages, words, shelfNum, isBorrowed);
    close();
}

void editBook::on_quit_Btn_clicked()
{
    needUpdate = false;
    close();
}

void editBook::on_name_Edit_textEdited(const QString &str)
{
    if(str.isEmpty())
    {
        ui->Tip->setText("书名不能为空");
        ui->edit_Btn->setEnabled(false);
    }
    else
    {
        needUpdate = true;
        ui->edit_Btn->setEnabled(true);
    }
}

void editBook::on_author_Edit_textEdited(const QString &str)
{
    if(str.isEmpty())
    {
        ui->Tip->setText("作者不能为空");
        ui->edit_Btn->setEnabled(false);
    }
    else
    {
        needUpdate = true;
        ui->edit_Btn->setEnabled(true);
    }
}

void editBook::on_press_Edit_textEdited(const QString &str)
{
    needUpdate = true;
    ui->edit_Btn->setEnabled(true);
}

void editBook::on_pressDate_Edit_textEdited(const QString &str)
{
    ui->edit_Btn->setEnabled(true);
}

void editBook::on_pressPlace_Edit_textEdited(const QString &str)
{
    ui->edit_Btn->setEnabled(true);
}

void editBook::on_price_Edit_textEdited(const QString &str)
{
    ui->edit_Btn->setEnabled(true);
}

void editBook::on_pages_Edit_textEdited(const QString &str)
{
    ui->edit_Btn->setEnabled(true);
}

void editBook::on_words_Edit_textEdited(const QString &str)
{
    ui->edit_Btn->setEnabled(true);
}

void editBook::on_clc_Edit_textEdited(const QString &str)
{
    ui->edit_Btn->setEnabled(true);
}

void editBook::on_bookDesc_Edit_textChanged()
{
    needUpdate = true;
    ui->edit_Btn->setEnabled(true);
}

void editBook::on_shelfNum_Edit_textEdited(const QString &str)
{
    if(str.isEmpty())
    {
        ui->Tip->setText("书架号不能为空");
        ui->edit_Btn->setEnabled(false);
    }
    else
    {
        ui->edit_Btn->setEnabled(true);
    }
}

void editBook::on_isBorrowed_Btn_clicked()
{
    ui->edit_Btn->setEnabled(true);
    needUpdate = true;
    isBorrowed = !isBorrowed;
    ui->isBorrowed_Btn->setText(isBorrowed?"已借":"可借");
}

