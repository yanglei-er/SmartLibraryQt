#include "bookinfo.h"

BookInfo::BookInfo()
{

}

QString BookInfo::getBookFromNet(const QString &isbn)
{
    QByteArray metadata = net.Get(QString("http://47.99.80.202:6066/openApi/getInfoByIsbn?isbn=%1&appKey=ae1718d4587744b0b79f940fbef69e77").arg(isbn));
    if(metadata.isEmpty())
    {
        return "无法访问网络数据库";
    }
    QString result = JsonParse(metadata);
    if(result.isEmpty())
    {
        return "";
    }
    else
    {
        return result;
    }
}

QString BookInfo::JsonParse(const QByteArray &json)
{
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject jsonObj = doc.object();
    if(jsonObj.value("success").toBool())
    {
        QJsonValue dataArry = jsonObj.value("data");
        QJsonObject dataObj = dataArry.toObject();
        bookName = dataObj.value("bookName").toString();
        author = dataObj.value("author").toString();
        press = dataObj.value("press").toString();
        pressDate = dataObj.value("pressDate").toString();
        pressPlace = dataObj.value("pressPlace").toString();
        price = dataObj.value("price").toDouble()/100;
        clcName = dataObj.value("clcName").toString();
        bookDesc = dataObj.value("bookDesc").toString();
        pages = dataObj.value("pages").toString();
        words = dataObj.value("words").toString();
        return "";
    }
    else
    {
        return jsonObj.value("msg").toString();
    }
}
