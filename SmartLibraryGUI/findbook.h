#ifndef FINDBOOK_H
#define FINDBOOK_H
#include <QDialog>

namespace Ui {class findBook;}

class findBook : public QDialog
{
    Q_OBJECT
public:
    explicit findBook(QWidget *parent = nullptr);
    ~findBook();
private:
    Ui::findBook *ui;
};

#endif // FINDBOOK_H
