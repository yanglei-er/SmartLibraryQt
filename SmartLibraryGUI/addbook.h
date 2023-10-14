#ifndef ADDBOOK_H
#define ADDBOOK_H
#include <QDialog>
#include <QMovie>
#include "database.h"
#include "globalprocess.h"

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
    void on_search_Btn_clicked();
    void on_quit_Btn_clicked();
    void on_addBtn_clicked();
    void on_shelfNum_Edit_textEdited(const QString &str);
    void on_shelfNum_Edit_returnPressed();
    void bleRead(QString str);
private:
    Ui::addBook *ui;
    Database sql;
    QPixmap wrong;
    QMovie *searching;
    GlobalProcess *globalObj = GlobalProcess::getInstance();
};

#endif // ADDBOOK_H
