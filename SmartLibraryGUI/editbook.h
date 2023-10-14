#ifndef EDITBOOK_H
#define EDITBOOK_H
#include <QDialog>
#include <database.h>

namespace Ui {class editBook;}

class editBook : public QDialog
{
    Q_OBJECT
public:
    explicit editBook(const QString &isbn, QWidget *parent = nullptr);
    ~editBook();
private slots:
    void on_edit_Btn_clicked();
    void on_quit_Btn_clicked();
    void on_name_Edit_textEdited(const QString &str);
    void on_author_Edit_textEdited(const QString &str);
    void on_press_Edit_textEdited(const QString &str);
    void on_pressDate_Edit_textEdited(const QString &str);
    void on_pressPlace_Edit_textEdited(const QString &str);
    void on_price_Edit_textEdited(const QString &str);
    void on_pages_Edit_textEdited(const QString &str);
    void on_words_Edit_textEdited(const QString &str);
    void on_clc_Edit_textEdited(const QString &str);
    void on_bookDesc_Edit_textChanged();
    void on_shelfNum_Edit_textEdited(const QString &str);
    void on_isBorrowed_Btn_clicked();
public:
    bool needUpdate = false;
private:
    Ui::editBook *ui;
    Database sql;
    bool isBorrowed = false;
};

#endif // EDITBOOK_H
