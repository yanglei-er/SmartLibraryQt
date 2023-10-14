#ifndef NETWORK_H
#define NETWORK_H
#include <QHostInfo>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QEventLoop>
#include <random>

class NetWork : public QObject
{
    Q_OBJECT
public:
    explicit NetWork(QObject *parent = nullptr);
    ~NetWork();
    static bool online();
    QByteArray Get(const QString &url);
private:
    void setUAPool();
    QString getAgent();
signals:
    void netError(const QString); // 错误信号
private:
    QNetworkAccessManager manager;
    QList<QString> UAPOOL;
};

#endif // NETWORK_H
