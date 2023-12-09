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

void GlobalProcess::SocketWrite(const QString &str)
{
    socket->write(str.toUtf8());
}

GlobalProcess::GlobalProcess(QObject *parent)
{
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
}

void GlobalProcess::socketReadyRead()
{
    QTimer::singleShot(100, this, [&](){
        QString str=QString::fromUtf8(socket->readAll());
        if(!str.isEmpty()){qDebug()<<str;emit bleRead(str);}});
}
