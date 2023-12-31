#ifndef ADDBOOK_H
#define ADDBOOK_H
#include <QDialog>
#include <QMovie>
#include "database.h"
#include "globalprocess.h"
#include "tools.h"

namespace Ui {class addBook;}

class addBook : public QDialog
{
    Q_OBJECT
public:
    explicit addBook(QWidget *parent = nullptr);
    ~addBook();
    QList<QString> books;
private slots:
    void on_isbn_Edit_returnPressed();
    void on_isbn_Edit_textChanged(const QString &str);
    void on_scan_Btn_clicked();
    void on_search_Btn_clicked();
    void on_quit_Btn_clicked();
    void on_addBtn_clicked();
    void on_shelfNum_Edit_returnPressed();
    void bleRead(QString isbn);
private:
    Ui::addBook *ui;
    Database sql;
    QPixmap wrong = TOOLS::loadImage(":/pic/wrong.png", QSize(35,35));;
    QMovie *searching;
    GlobalProcess *globalObj = GlobalProcess::getInstance();
};

#endif // ADDBOOK_H
