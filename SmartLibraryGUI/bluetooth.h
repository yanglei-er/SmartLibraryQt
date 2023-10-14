#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#include <QDialog>
#include <QListWidgetItem>
#include <QScrollBar>
#include <QBluetoothLocalDevice>
#include <QBluetoothSocket>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QMovie>
#include "globalprocess.h"

namespace Ui {class Bluetooth;}

class Bluetooth : public QDialog
{
    Q_OBJECT
public:
    explicit Bluetooth(QWidget *parent = nullptr);
    ~Bluetooth();
signals:
    void bleConnected();
    void bleDisconnected();
private slots:
    void on_ok_Btn_clicked();
    void on_listWidget_currentRowChanged(int currentRow);
    void on_search_Btn_clicked();
    void on_connect_Btn_clicked();
    // 有关搜索信号
    void deviceDiscovered(QBluetoothDeviceInfo device); // 发现设备
    void deviceSearchFinished(); // 搜索完毕
    void deviceSearchErrorOccurred(); // 搜索错误
    // 有关连接信号
    void socketConnected(); // 连接成功
    void socketDisconnect(); // 断开连接
    void socketErrorOccurred(); // 连接错误
private:
    Ui::Bluetooth *ui;
    QMovie *loading;
    QBluetoothLocalDevice *localDevice = nullptr;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = nullptr;
    QList<QBluetoothDeviceInfo> devices;
    GlobalProcess *globalObj = GlobalProcess::getInstance();
    // 转码设备类型
    QString majorDeviceClassString(QBluetoothDeviceInfo::MajorDeviceClass deviceClass);
};

#endif // BLUETOOTH_H
