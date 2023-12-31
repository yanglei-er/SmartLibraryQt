#include "bookmanage.h"
#include "ui_bookmanage.h"
#include "addbook.h"
#include "editbook.h"
#include "network.h"
#include "bookcell.h"

BookManage::BookManage(QWidget *parent) : QDialog(parent), ui(new Ui::BookManage)
{
    ui->setupUi(this);
    if(NetWork::online() && Database::sqlConnected())
    {
        ui->add_Btn->setEnabled(true);
        ui->net_attitude->setPixmap(TOOLS::loadImage(":/pic/green.png", QSize(30,30)));
    }
    if(Database::sqlConnected())
    {
        ui->export_Btn->setEnabled(true);
        ui->find_btn->setEnabled(true);
        ui->localdata_attitude->setPixmap(TOOLS::loadImage(":/pic/green.png", QSize(30,30)));
        indexMax = sql.getBookNum()-1;
        setWindowTitle(QString("图书管理 - 共 %1 本书").arg(indexMax+1));
        connect(ui->listWidget->verticalScrollBar(), &QScrollBar::valueChanged, this, &BookManage::scroll);

        if(indexMax >= 0)
        {
            addBookToList(6);
            connect(ui->listWidget->verticalScrollBar(), &QScrollBar::valueChanged, this, &BookManage::scroll);
        }
        else
        {
            ui->find_btn->setEnabled(false);
        }
    }
}

BookManage::~BookManage()
{
    delete ui;
}

void BookManage::addBookToList(const int &times)
{
    for(int i = 0; i < times; i++)
    {
        if(index > indexMax)
        {
            return;
        }
        QSqlRecord record = sql.getOneBookInfoByIndex(index);
        QListWidgetItem *item = new QListWidgetItem();
        bookCell *book = new bookCell();
        book->bookName->setText(QString("《%1》").arg(record.value("bookName").toString()));
        book->press->setText(QString("%1  %2").arg(record.value("press").toString(), record.value("clcName").toString()));
        book->author->setText(record.value("author").toString());
        book->bookDesc->setText(record.value("bookDesc").toString());
        book->attitude->setPixmap(record.value("isBorrowed").toBool()?cross:tick);
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, book);
        index++;
    }
}

void BookManage::scroll(int value)
{
    if(ui->listWidget->verticalScrollBar()->maximum() == value)
    {
        addBookToList(6);
    }
}

void BookManage::on_import_Btn_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, "导入数据库", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "智慧图书库(*.smartlibrary)");
    if(!filepath.isEmpty())
    {
        QMessageBox::warning(nullptr, "报告", "导入部分暂未完善，以后再来体验吧！");
    }
}

void BookManage::on_export_Btn_clicked()
{
    QString filepath = QFileDialog::getSaveFileName(this, "导出数据库", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/books.smartlibrary"), "智慧图书库(*.smartlibrary)");
    if(!filepath.isEmpty())
    {
        QFile::copy("./database/books.smartlibrary", filepath);
    }
}

void BookManage::on_add_Btn_clicked()
{
    addBook w;
    w.exec();
    if(!w.books.isEmpty())
    {
        for(int i = 0; i < w.books.length(); i++)
        {
            QSqlRecord record = sql.getOneBookInfo("isbn", w.books.at(i));
            QListWidgetItem *item = new QListWidgetItem();
            bookCell *book = new bookCell();
            book->bookName->setText(QString("《%1》").arg(record.value("bookName").toString()));
            book->press->setText(record.value("press").toString());
            book->author->setText(record.value("author").toString());
            book->bookDesc->setText(record.value("bookDesc").toString());
            book->attitude->setPixmap((record.value("isBorrowed").toBool())?cross:tick);
            ui->listWidget->insertItem(0 ,item);
            ui->listWidget->setItemWidget(item, book);
            addNumber++;
        }
    }
}

void BookManage::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->edit_Btn->setEnabled(true);
    ui->del_Btn->setEnabled(true);
}

void BookManage::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    on_edit_Btn_clicked();
}

void BookManage::on_edit_Btn_clicked()
{
    int currentIndex = ui->listWidget->currentRow();
    int editIndex = currentIndex;
    if(addNumber)
    {
        if(currentIndex < addNumber)
        {
            editIndex = indexMax - currentIndex;
        }
        else
        {
            editIndex = currentIndex - addNumber;
        }
    }
    QString isbn = sql.getOneBookInfoByIndex(editIndex).value("isbn").toString();
    editBook w(isbn);
    w.exec();
    if(w.needUpdate)
    {
        // 添加新的
        QSqlRecord record = sql.getOneBookInfo("isbn", isbn);
        QListWidgetItem *item = new QListWidgetItem();
        bookCell *book = new bookCell();
        book->bookName->setText(QString("《%1》").arg(record.value("bookName").toString()));
        book->press->setText(record.value("press").toString());
        book->author->setText(record.value("author").toString());
        book->bookDesc->setText(record.value("bookDesc").toString());
        book->attitude->setPixmap((record.value("isBorrowed").toBool())?cross:tick);
        ui->listWidget->insertItem(currentIndex+1 ,item);
        ui->listWidget->setItemWidget(item, book);
        // 删掉旧的
        delete ui->listWidget->takeItem(currentIndex);
    }
}

void BookManage::on_del_Btn_clicked()
{
    int currentIndex = ui->listWidget->currentRow();
    QString isbn = sql.getOneBookInfoByIndex(currentIndex).value("isbn").toString();
    QString bookName = sql.getOneBookInfo("isbn", isbn).value("bookName").toString();
    int ret = QMessageBox::warning(nullptr, "删除书籍", QString("您确定要删除《%1》吗？此操作不可撤销！").arg(bookName), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(ret == QMessageBox::Yes)
    {
        sql.delOneBook(isbn);
        indexMax--;
        index--;
        delete ui->listWidget->takeItem(currentIndex);
        ui->listWidget->currentItem()->setSelected(false);
        ui->edit_Btn->setEnabled(false);
        ui->del_Btn->setEnabled(false);
        setWindowTitle(QString("图书管理 - 共 %1 本书").arg(indexMax+1));
    }
}

void BookManage::on_find_btn_clicked()
{

}

void BookManage::on_quit_Btn_clicked()
{
    close();
}
