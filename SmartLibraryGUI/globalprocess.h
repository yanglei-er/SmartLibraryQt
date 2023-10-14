#ifndef GLOBALPROCESS_H
#define GLOBALPROCESS_H
#include <QObject>
#include <QBluetoothSocket>
#include <QTimer>

class GlobalProcess : public QObject
{
    Q_OBJECT
public:
    static GlobalProcess* getInstance();
    static void deleteInstance();
    QBluetoothSocket *getSocket();
    void setBleStatus(bool status);
    bool isBleConnect();
    void setBleDisconnect();
    void SocketWrite(const QString &str);
signals:
    void bleRead(const QString);
private:
    GlobalProcess(QObject *parent = nullptr);
    ~GlobalProcess(){};
    GlobalProcess(const GlobalProcess &){};
    const GlobalProcess &operator=(const GlobalProcess&);
private slots:
    void socketReadyRead();
private:
    static GlobalProcess *obj;
    QBluetoothSocket *socket;
    bool isBleConnected = false;
};
#endif // GLOBALPROCESS_H
