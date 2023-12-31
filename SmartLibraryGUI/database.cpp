#include "database.h"

Database::Database(QObject *parent) : QObject{parent}
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("./database/books.smartlibrary");
    }
    if(!database.isOpen())
    {
        if(QFile::exists("./database/books.smartlibrary"))
        {
            database.open();
        }
    }
}

Database::~Database()
{

}

bool Database::sqlConnected()
{
    if(QFile::exists("./database/books.smartlibrary"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Database::close()
{
    QSqlDatabase database = QSqlDatabase::database("qt_sql_default_connection");
    database.close();
}

bool Database::exists(const QString &isbn)
{
    QSqlQuery sql(database);
    sql.exec(QString("SELECT * FROM main WHERE isbn = '%1'").arg(isbn));
    if(sql.next())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Database::addBook(const QString &isbn, const QString &name, const QString &author, const QString &press, const QString &pressDate, const QString &pressPlace, const qreal &price, const QString &clcName, const QString &bookDesc, const QString &pages, const QString &words, const int &shelfNumber, const int isBorrowed)
{
    QSqlQuery sql(database);
    sql.prepare("insert into main values (?,?,?,?,?,?,?,?,?,?,?,?,?)");
    sql.addBindValue(isbn);
    sql.addBindValue(name);
    sql.addBindValue(author);
    sql.addBindValue(press);
    sql.addBindValue(pressDate);
    sql.addBindValue(pressPlace);
    sql.addBindValue(price);
    sql.addBindValue(clcName);
    sql.addBindValue(bookDesc);
    sql.addBindValue(pages);
    sql.addBindValue(words);
    sql.addBindValue(shelfNumber);
    sql.addBindValue(isBorrowed);
    sql.exec();
}

void Database::editBook(const QString &isbn, const QString &name, const QString &author, const QString &press, const QString &pressDate, const QString &pressPlace, const qreal &price, const QString &clcName, const QString &bookDesc, const QString &pages, const QString &words, const int &shelfNumber, const int isBorrowed)
{
    QSqlQuery sql(database);
    sql.prepare("UPDATE main SET bookName = ?, author = ?, press = ?, pressDate = ?, pressPlace = ?, price = ?, clcName = ?, bookDesc = ?, pages = ?, words = ?, shelfNumber = ?, isBorrowed = ? WHERE isbn = ?");
    sql.addBindValue(name);
    sql.addBindValue(author);
    sql.addBindValue(press);
    sql.addBindValue(pressDate);
    sql.addBindValue(pressPlace);
    sql.addBindValue(price);
    sql.addBindValue(clcName);
    sql.addBindValue(bookDesc);
    sql.addBindValue(pages);
    sql.addBindValue(words);
    sql.addBindValue(shelfNumber);
    sql.addBindValue(isBorrowed);
    sql.addBindValue(isbn);
    sql.exec();
}

qint32 Database::getBookNum()
{
    QSqlQuery sql(database);
    sql.exec("SELECT count(shelfNumber) FROM main");
    sql.next();
    return sql.value(0).toInt();
}

QSqlRecord Database::getOneBookInfo(const QString &type, const QString &str)
{
    QSqlQuery sql(database);
    QSqlRecord record;
    if(!sql.exec(QString("SELECT * FROM main WHERE %1 = '%2'").arg(type, str)))
    {
        return record;
    }
    sql.next();
    record = sql.record();
    return record;
}

QList<QSqlRecord> Database::getBooksInfo(const QString &type, const QString &str)
{
    QSqlQuery sql(database);
    QList<QSqlRecord> records;
    if(!sql.exec(QString("SELECT * FROM main WHERE %1 = '%2'").arg(type, str)))
    {
        return records;
    }
    while(sql.next())
    {
        records << sql.record();
    }
    return records;
}

QSqlRecord Database::getOneBookInfoByIndex(const int &index)
{
    QSqlQuery sql(database);
    QSqlRecord record;
    if(!sql.exec(QString("SELECT * FROM main LIMIT %1, 1").arg(index)))
    {
        return record;
    }
    sql.next();
    record = sql.record();
    return record;
}

QList<QSqlRecord> Database::getBooksInfoByName(const QString &name)
{
    QSqlQuery sql(database);
    QList<QSqlRecord> records;
    if(!sql.exec(QString("SELECT * FROM main WHERE bookName like '%name%'").replace("name", name)))
    {
        return records;
    }
    while(sql.next())
    {
        records << sql.record();
    }
    return records;
}

void Database::delOneBook(const QString &isbn)
{
    QSqlQuery sql(database);
    sql.exec(QString("DELETE FROM main WHERE isbn = '%1'").arg(isbn));
}

void Database::borrowBook(const QString &isbn)
{
    QSqlQuery sql(database);
    sql.prepare(QString("UPDATE main SET isBorrowed = 1 WHERE isbn = ?"));
    sql.addBindValue(isbn);
    sql.exec();
}

void Database::returnBook(const QString &isbn)
{
    QSqlQuery sql(database);
    sql.prepare(QString("UPDATE main SET isBorrowed = 0 WHERE isbn = ?"));
    sql.addBindValue(isbn);
    sql.exec();
}

QList<QString> Database::getAllisbn()
{
    QSqlQuery sql(database);
    QList<QString> isbnList;
    sql.exec("SELECT isbn from main");
    while(sql.next())
    {
        isbnList<<sql.value(0).toString();
    }
    return isbnList;
}

void Database::mergeDatabase(const QString &filepath)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "second");
    db.setDatabaseName(filepath);
    if(db.open())
    {

    }
}
