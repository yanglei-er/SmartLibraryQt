#include "addbook.h"
#include "ui_addbook.h"
#include "tools.h"
#include "bookinfo.h"

addBook::addBook(QWidget *parent) : QDialog(parent), ui(new Ui::addBook)
{
    ui->setupUi(this);
    wrong = TOOLS::loadImage(":/pic/wrong.png", QSize(35,35));
    searching = new QMovie(":/pic/flush.gif");
    searching->setScaledSize(QSize(40, 40));
    if(globalObj->isBleConnect())
    {
        ui->isbn_Edit->setPlaceholderText("请扫描或输入13位ISBN码");
        ui->scan_Btn->setEnabled(true);
        connect(globalObj, &GlobalProcess::bleRead, this, &addBook::bleRead);
    }
}

addBook::~addBook()
{
    delete ui;
    if(globalObj->isBleConnect())
    {
        disconnect(globalObj, &GlobalProcess::bleRead, this, &addBook::bleRead);
    }
}

void addBook::bleRead(QString isbn)
{
    if(isbn.startsWith("978") && isbn.length() == 13)
    {
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
    ui->scan_Btn->setEnabled(true);
}

void addBook::on_isbn_Edit_textChanged(const QString &str)
{
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
        ui->search_Btn->setEnabled(true);
        ui->attitude_label->setPixmap(TOOLS::loadImage(":/pic/right.png", QSize(40,40)));
    }
}

void addBook::on_isbn_Edit_returnPressed()
{
    if(ui->isbn_Edit->text().length() == 17)
    {
        on_search_Btn_clicked();
    }
}

void addBook::on_scan_Btn_clicked()
{
    ui->scan_Btn->setEnabled(false);
    globalObj->SocketWrite("scan");
}

void addBook::on_search_Btn_clicked()
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

    ui->isbn_Edit->setEnabled(false);
    ui->search_Btn->setEnabled(false);
    ui->attitude_label->setMovie(searching);
    searching->start();

    if(sql.exists(ui->isbn_Edit->text().remove("-")))
    {
        ui->Tip->setText("本书已在数据库中");
        QSqlRecord record = sql.getOneBookInfo("isbn", ui->isbn_Edit->text().remove("-"));
        if(!record.isEmpty())
        {
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
            ui->bookDesc_Edit->setEnabled(true);
            ui->bookDesc_Edit->setReadOnly(true);
            ui->bookDesc_Edit->setFocusPolicy(Qt::NoFocus);
        }
        else
        {
            ui->Tip->setText("无法查询数据库");
        }
    }
    else
    {
        BookInfo bookInfo;
        QString result;
        result = bookInfo.getBookFromNet(ui->isbn_Edit->text().remove("-"));
        if(result.isEmpty())
        {
            ui->name_Edit->setText(bookInfo.bookName);
            ui->author_Edit->setText(bookInfo.author);
            ui->press_Edit->setText(bookInfo.press);
            ui->pressDate_Edit->setText(bookInfo.pressDate);
            ui->pressPlace_Edit->setText(bookInfo.pressPlace);
            ui->price_Edit->setText(QString::number(bookInfo.price));
            ui->pages_Edit->setText(bookInfo.pages);
            ui->words_Edit->setText(bookInfo.words);
            ui->clc_Edit->setText(bookInfo.clcName);
            ui->bookDesc_Edit->setText(bookInfo.bookDesc);

            ui->bookDesc_Edit->setFocusPolicy(Qt::StrongFocus);
            ui->name_Edit->setEnabled(true);
            ui->author_Edit->setEnabled(true);
            ui->press_Edit->setEnabled(true);
            ui->pressDate_Edit->setEnabled(true);
            ui->pressPlace_Edit->setEnabled(true);
            ui->price_Edit->setEnabled(true);
            ui->pages_Edit->setEnabled(true);
            ui->words_Edit->setEnabled(true);
            ui->clc_Edit->setEnabled(true);
            ui->bookDesc_Edit->setEnabled(true);
            ui->shelfNum_Edit->setEnabled(true);
            ui->shelfNum_Edit->setFocus();
        }
        else
        {
            ui->Tip->setText(result);
        }
    }
    ui->isbn_Edit->setEnabled(true);
    searching->stop();
    ui->attitude_label->clear();
}

void addBook::on_shelfNum_Edit_textEdited(const QString &str)
{
    if(str.length() == 0)
    {
        ui->addBtn->setEnabled(false);
    }
    else
    {
        ui->addBtn->setEnabled(true);
    }
}

void addBook::on_addBtn_clicked()
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
    if(!(bookName.isEmpty() && author.isEmpty()))
    {
        ui->Tip->setText(QString("正在添加%1").arg(bookName));
        QString result = sql.addBook(isbn,bookName,author,press,pressDate,pressPlace,price,clcName,bookDesc,pages,words,shelfNum);
        if(result.isEmpty())
        {
            ui->Tip->setText(QString("《%1》添加成功").arg(bookName));
            ui->isbn_Edit->clear();
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

            ui->name_Edit->setEnabled(false);
            ui->author_Edit->setEnabled(false);
            ui->press_Edit->setEnabled(false);
            ui->pressDate_Edit->setEnabled(false);
            ui->pressPlace_Edit->setEnabled(false);
            ui->price_Edit->setEnabled(false);
            ui->pages_Edit->setEnabled(false);
            ui->words_Edit->setEnabled(false);
            ui->clc_Edit->setEnabled(false);
            ui->bookDesc_Edit->setEnabled(false);
            ui->shelfNum_Edit->setEnabled(false);
            ui->addBtn->setEnabled(false);

            ui->isbn_Edit->setFocus();
            books << isbn;
        }
        else
        {
            ui->Tip->setText(result);
        }
    }
    else
    {
        ui->Tip->setText("书名和作者名不能为空！");
    }
}

void addBook::on_shelfNum_Edit_returnPressed()
{
    if(ui->shelfNum_Edit->text().length() != 0)
    {
        on_addBtn_clicked();
    }
}

void addBook::on_quit_Btn_clicked()
{
    close();
}
