#ifndef FINDBOOKCELL_H
#define FINDBOOKCELL_H
#include <QLabel>
#include <QWidget>

namespace Ui {class findbookcell;}

class findbookcell : public QWidget
{
    Q_OBJECT
public:
    explicit findbookcell(QWidget *parent = nullptr);
    ~findbookcell();
    QLabel *bookName;
    QLabel *author;
    QLabel *press;
private:
    Ui::findbookcell *ui;
};

#endif // FINDBOOKCELL_H
