#include "network.h"

NetWork::NetWork(QObject *parent) : QObject{parent}
{
    setUAPool();
}

NetWork::~NetWork()
{

}

bool NetWork::online()
{
    QHostInfo info = QHostInfo::fromName("www.baidu.com");
    if(info.addresses().isEmpty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void NetWork::setUAPool()
{
    UAPOOL<<"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36 OPR/26.0.1656.60";
    UAPOOL<<"Opera/8.0 (Windows NT 5.1; U; en)";
    UAPOOL<<"Mozilla/5.0 (Windows NT 5.1; U; en; rv:1.8.1) Gecko/20061208 Firefox/2.0.0 Opera 9.50";
    UAPOOL<<"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; en) Opera 9.50";
    UAPOOL<<"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:34.0) Gecko/20100101 Firefox/34.0";
    UAPOOL<<"Mozilla/5.0 (X11; U; Linux x86_64; zh-CN; rv:1.9.2.10) Gecko/20100922 Ubuntu/10.10 (maverick) Firefox/3.6.10";
    UAPOOL<<"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/534.57.2 (KHTML, like Gecko) Version/5.1.7 Safari/534.57.2";
    UAPOOL<<"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.71 Safari/537.36";
    UAPOOL<<"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.64 Safari/537.11";
    UAPOOL<<"Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US) AppleWebKit/534.16 (KHTML, like Gecko) Chrome/10.0.648.133 Safari/534.16";
    UAPOOL<<"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.101 Safari/537.36";
    UAPOOL<<"Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko";
    UAPOOL<<"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/38.0.2125.122 UBrowser/4.0.3214.0 Safari/537.36";
    UAPOOL<<"Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)";
    UAPOOL<<"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0)";
    UAPOOL<<"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_0) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.56 Safari/535.11";
    UAPOOL<<"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.74 Safari/537.36 Edg/99.0.1150.55";
}

QString NetWork::getAgent()
{
    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_int_distribution<> distrib(0, UAPOOL.size()-1);
    int index = distrib(engine);
    QString UA =  UAPOOL.at(index);
    return UA;
}

QByteArray NetWork::Get(const QString &url)
{
    QEventLoop loop;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::UserAgentHeader, getAgent());
    QNetworkReply *reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if(reply->error())
    {
        emit netError(reply->errorString());
        reply->deleteLater();
        return "";
    }
    QByteArray data = reply->readAll();
    reply->deleteLater();
    return data;
}
