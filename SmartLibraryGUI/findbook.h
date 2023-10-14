#ifndef FINDBOOK_H
#define FINDBOOK_H
#include <QDialog>
#include <QListWidgetItem>
#include "database.h"
#include "globalprocess.h"

namespace Ui {class findBook;}

class findBook : public QDialog
{
    Q_OBJECT
public:
    explicit findBook(QWidget *parent = nullptr);
    ~findBook();
private slots:
    void on_bookName_Edit_textChanged(const QString &str);
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_find_Btn_clicked();
    void on_quit_Btn_clicked();
private:
    Ui::findBook *ui;
    Database sql;
    QList<QSqlRecord> books;
    QList<QString> bookisbns;
    QPixmap tick;
    QPixmap cross;
    GlobalProcess *globalObj = GlobalProcess::getInstance();
};

#endif // FINDBOOK_H
