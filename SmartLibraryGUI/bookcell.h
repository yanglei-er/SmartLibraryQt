#ifndef BOOKCELL_H
#define BOOKCELL_H
#include <QWidget>
#include <QLabel>
#include <QTextEdit>

namespace Ui {class bookCell;}

class bookCell : public QWidget
{
    Q_OBJECT
public:
    explicit bookCell(QWidget *parent = nullptr);
    ~bookCell();
    QLabel *attitude;
    QLabel *bookName;
    QLabel *author;
    QLabel *press;
    QTextEdit *bookDesc;
private:
    Ui::bookCell *ui;
};

#endif // BOOKCELL_H
