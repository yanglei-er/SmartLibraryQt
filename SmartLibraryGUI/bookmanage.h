#ifndef BOOKMANAGE_H
#define BOOKMANAGE_H
#include <QDialog>
#include <QListWidgetItem>
#include <QScrollBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include "database.h"
#include "tools.h"

namespace Ui {class BookManage;}

class BookManage : public QDialog
{
    Q_OBJECT
public:
    explicit BookManage(QWidget *parent = nullptr);
    ~BookManage();
private:
    void addBookToList(const int &times);
private slots:
    void on_import_Btn_clicked();
    void on_export_Btn_clicked();
    void on_add_Btn_clicked();
    void on_edit_Btn_clicked();
    void on_del_Btn_clicked();
    void on_find_btn_clicked();
    void on_quit_Btn_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void scroll(int value);
private:
    Ui::BookManage *ui;
    int index = 0;
    int indexMax = 0;
    int addNumber = 0;
    QPixmap tick = TOOLS::loadImage(":/pic/tick.png", QSize(112,48));
    QPixmap cross = TOOLS::loadImage(":/pic/cross.png", QSize(112,48));
    Database sql;
};

#endif // BOOKMANAGE_H
