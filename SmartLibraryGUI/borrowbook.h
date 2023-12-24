#ifndef BORROWBOOK_H
#define BORROWBOOK_H
#include <QDialog>
#include "database.h"
#include "globalprocess.h"
#include "tools.h"

namespace Ui {class borrowBook;}

class borrowBook : public QDialog
{
    Q_OBJECT
public:
    explicit borrowBook(QWidget *parent = nullptr);
    ~borrowBook();
private:
    void bookInfo();
private slots:
    void bleRead(QString isbn);
    void on_scan_Btn_clicked();
    void on_borrow_Btn_clicked();
    void on_quit_Btn_clicked();    
private:
    Ui::borrowBook *ui;
    Database sql;
    QPixmap tick = TOOLS::loadImage(":/pic/tick.png", QSize(140,60));
    QPixmap cross = TOOLS::loadImage(":/pic/cross.png", QSize(140,60));
    GlobalProcess *globalObj = GlobalProcess::getInstance();
};

#endif // BORROWBOOK_H
