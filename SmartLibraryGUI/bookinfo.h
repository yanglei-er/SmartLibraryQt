#ifndef BOOKINFO_H
#define BOOKINFO_H
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "network.h"

class BookInfo
{
public:
    BookInfo();
    QString getBookFromNet(const QString &isbn);
private:
    QString JsonParse(const QByteArray &json);
public:
    QString isbn;
    QString bookName;
    QString author;
    QString press;
    QString pressDate;
    QString pressPlace;
    qreal price;
    QString clcName;
    QString bookDesc;
    QString pages;
    QString words;
private:
    NetWork net;
};

#endif // BOOKINFO_H
