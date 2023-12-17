#ifndef RETURNBOOK_H
#define RETURNBOOK_H
#include <QDialog>
#include "database.h"
#include "globalprocess.h"
#include "tools.h"

namespace Ui {class returnBook;}

class returnBook : public QDialog
{
    Q_OBJECT
public:
    explicit returnBook(QWidget *parent = nullptr);
    ~returnBook();
private:
    void bookInfo();
private slots:
    void bleRead(QString isbn);
    void on_scan_Btn_clicked();
    void on_return_Btn_clicked();
    void on_quit_Btn_clicked();
private:
    Ui::returnBook *ui;
    Database sql;
    QPixmap tick = TOOLS::loadImage(":/pic/tick.png", QSize(140,60));
    QPixmap cross = TOOLS::loadImage(":/pic/cross.png", QSize(140,60));
    GlobalProcess *globalObj = GlobalProcess::getInstance();
};

#endif // RETURNBOOK_H
