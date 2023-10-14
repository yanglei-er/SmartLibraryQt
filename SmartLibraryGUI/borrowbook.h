#ifndef BORROWBOOK_H
#define BORROWBOOK_H
#include <QDialog>

namespace Ui {class borrowBook;}

class borrowBook : public QDialog
{
    Q_OBJECT
public:
    explicit borrowBook(QWidget *parent = nullptr);
    ~borrowBook();
private:
    Ui::borrowBook *ui;
};

#endif // BORROWBOOK_H
