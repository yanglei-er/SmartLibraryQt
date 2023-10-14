#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QFile>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    static bool sqlConnected();
    static void close();
    bool exists(const QString &isbn);
    QString addBook(const QString &isbn, const QString &name, const QString &author, const QString &press, const QString &pressDate, const QString &pressPlace, const qreal &price, const QString &clcName, const QString &bookDesc, const QString &pages, const QString &words, const int &shelfNumber, const int isBorrowed = 0);
    qint32 getBookNum();
    QSqlRecord getOneBookInfo(const QString &type, const QString &str);
    QList<QSqlRecord> getBooksInfo(const QString &type, const QString &str);
    QSqlRecord getOneBookInfoByIndex(const int &index);
    QList<QSqlRecord> getBooksInfoByName(const QString &name);
    QList<QString> getAllisbn();
    void editBook(const QString &isbn, const QString &name, const QString &author, const QString &press, const QString &pressDate, const QString &pressPlace, const qreal &price, const QString &clcName, const QString &bookDesc, const QString &pages, const QString &words, const int &shelfNumber, const int isBorrowed = 0);
    void delOneBook(const QString &isbn);
private:
    QSqlDatabase database;
};

#endif // DATABASE_H
