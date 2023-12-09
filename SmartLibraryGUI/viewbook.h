#ifndef VIEWBOOK_H
#define VIEWBOOK_H
#include <QDialog>
#include "database.h"
#include "tools.h"
#include "globalprocess.h"

namespace Ui {class viewBook;}

class viewBook : public QDialog
{
    Q_OBJECT
public:
    explicit viewBook(QWidget *parent = nullptr);
    ~viewBook();
private slots:
    void bleRead(QString isbn);
    void on_isbn_Edit_textChanged(const QString &str);
    void on_scan_Btn_clicked();
    void on_search_Btn_clicked();
    void on_quit_Btn_clicked();
    void on_isbn_Edit_returnPressed();
    void on_find_Btn_clicked();

private:
    Ui::viewBook *ui;
    GlobalProcess *globalObj = GlobalProcess::getInstance();
    Database sql;
    QPixmap tick = TOOLS::loadImage(":/pic/tick.png", QSize(140,60));
    QPixmap cross = TOOLS::loadImage(":/pic/cross.png", QSize(140,60));
    QPixmap wrong = TOOLS::loadImage(":/pic/wrong.png", QSize(35, 35));
};

#endif // VIEWBOOK_H
