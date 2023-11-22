#include "globalprocess.h"

GlobalProcess *GlobalProcess::obj = nullptr;

GlobalProcess *GlobalProcess::getInstance()
{
    if(!obj)
    {
        obj = new GlobalProcess();
    }
    return obj;
}

void GlobalProcess::deleteInstance()
{
    if(obj)
    {
        delete obj;
        obj = nullptr;
    }
}

QBluetoothSocket *GlobalProcess::getSocket()
{
    return socket;
}

void GlobalProcess::setBleStatus(bool status)
{
    isBleConnected = status;
}

bool GlobalProcess::isBleConnect()
{
    return isBleConnected;
}

void GlobalProcess::setBleDisconnect()
{
    socket->disconnectFromService();
}

void GlobalProcess::SocketWrite(const QString &str)
{
    qDebug() << str;
    socket->write(str.toUtf8());
}

GlobalProcess::GlobalProcess(QObject *parent)
{
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
}

void GlobalProcess::socketReadyRead()
{
    qDebug()<<126515665651651;
    QString str;
    qDebug()<< socket->readAll();
    QTimer::singleShot(200, this, [&](){str=QString::fromUtf8(socket->readAll());if(!str.isEmpty()){emit bleRead(str);}});
}
