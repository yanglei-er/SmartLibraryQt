#include "bluetooth.h"
#include "ui_bluetooth.h"
#include "btdevcell.h"

Bluetooth::Bluetooth(QWidget *parent) : QDialog(parent), ui(new Ui::Bluetooth)
{
    ui->setupUi(this);

    // 检测是否支持蓝牙
    if(QBluetoothLocalDevice::allDevices().length() == 0)
    {
        ui->label->setText("该设备不支持蓝牙！");
        ui->label->setStyleSheet("color:red");
        ui->search_Btn->setEnabled(false);
    }
    else
    {
        loading = new QMovie(":/pic/flush.gif");
        loading->setScaledSize(QSize(36, 36));
        ui->attitude_label->setMovie(loading);
        // ListWidget表头
        QListWidgetItem *item = new QListWidgetItem();
        BtDevCell *bdc = new BtDevCell();
        item->setFlags(item->flags() &~ Qt::ItemIsSelectable);
        item->setSizeHint(QSize(ui->listWidget->width(),bdc->height()));
        bdc->btName->setText("名称");
        bdc->btAddr->setText("地址");
        bdc->btRssi->setText("设备类型");
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, bdc);

        localDevice = new QBluetoothLocalDevice();
        discoveryAgent = new QBluetoothDeviceDiscoveryAgent(localDevice->address());

        // 检测蓝牙是否打开

        if(localDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff)
        {
            localDevice->powerOn();
        }

        // 搜索有关信号
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &Bluetooth::deviceDiscovered);
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &Bluetooth::deviceSearchFinished);
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred, this, &Bluetooth::deviceSearchErrorOccurred);
        // 连接有关信号
        connect(globalObj->getSocket(), &QBluetoothSocket::connected, this, &Bluetooth::socketConnected);
        connect(globalObj->getSocket(), &QBluetoothSocket::disconnected, this, &Bluetooth::socketDisconnect);
        connect(globalObj->getSocket(), &QBluetoothSocket::errorOccurred, this, &Bluetooth::socketErrorOccurred);
    }
}

Bluetooth::~Bluetooth()
{
    delete ui;
}

void Bluetooth::on_search_Btn_clicked()
{
    if(ui->listWidget->count() > 1)
    {
        ui->listWidget->clear();
        QListWidgetItem *item = new QListWidgetItem();
        BtDevCell *bdc = new BtDevCell();
        item->setFlags(item->flags() &~ Qt::ItemIsSelectable);
        item->setSizeHint(QSize(ui->listWidget->width(),bdc->height()));
        bdc->btName->setText("名称");
        bdc->btAddr->setText("地址");
        bdc->btRssi->setText("设备类型");
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, bdc);
    }
    ui->attitude_label->show();
    loading->start();
    ui->search_Btn->setEnabled(false);
    ui->connect_Btn->setEnabled(false);
    ui->label->setText("正在搜索");
    // 开始扫描
    devices.clear();
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::ClassicMethod);
}
// 发现设备
void Bluetooth::deviceDiscovered(QBluetoothDeviceInfo device)
{
    QListWidgetItem *item = new QListWidgetItem();
    BtDevCell *bdc = new BtDevCell();
    item->setSizeHint(QSize(ui->listWidget->width(),bdc->height()));
    bdc->btName->setText(device.name());
    bdc->btAddr->setText(device.address().toString());
    bdc->btRssi->setText(majorDeviceClassString(device.majorDeviceClass()));
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item,bdc);
    devices.append(device);
}
// 扫描完成
void Bluetooth::deviceSearchFinished()
{
    ui->attitude_label->hide();
    loading->stop();
    ui->label->setText(QString("共找到 %1 个设备").arg(devices.length()));
    ui->search_Btn->setEnabled(true);
    ui->search_Btn->setText("重新扫描");
}
// 扫描错误
void Bluetooth::deviceSearchErrorOccurred()
{
    ui->attitude_label->hide();
    loading->stop();
    ui->label->setText(discoveryAgent->errorString());
    ui->search_Btn->setEnabled(true);
    ui->search_Btn->setText("重新扫描");
}

void Bluetooth::on_connect_Btn_clicked()
{
    ui->listWidget->setEnabled(false);
    if(discoveryAgent->isActive())
    {
        discoveryAgent->stop();
        deviceSearchFinished();
    }
    if(!globalObj->isBleConnect())
    {
        ui->attitude_label->show();
        loading->start();
        ui->connect_Btn->setText("正在连接");
        ui->connect_Btn->setEnabled(false);
        globalObj->getSocket()->connectToService(devices.at(ui->listWidget->currentRow()-1).address(), QBluetoothUuid::ServiceClassUuid::SerialPort, QIODevice::ReadWrite);
    }
    else
    {
        globalObj->getSocket()->disconnectFromService();
    }
}
// 连接成功
void Bluetooth::socketConnected()
{
    ui->attitude_label->hide();
    loading->stop();
    globalObj->setBleStatus(true);
    ui->label->setText(QString("%1 连接成功").arg((devices.at(ui->listWidget->currentRow()-1).name())));
    ui->connect_Btn->setText("断开连接");
    ui->connect_Btn->setEnabled(true);
    ui->listWidget->setEnabled(true);
    connect(globalObj->getSocket(), &QBluetoothSocket::readyRead, globalObj->getInstance(), &GlobalProcess::socketReadyRead);
    emit bleConnected();
}
// 断开连接
void Bluetooth::socketDisconnect()
{
    globalObj->setBleStatus(false);
    ui->label->setText("当前未连接设备");
    ui->connect_Btn->setText("连接设备");
    ui->listWidget->setEnabled(true);
    emit bleDisconnected();
}
// 连接失败
void Bluetooth::socketErrorOccurred()
{
    ui->attitude_label->hide();
    loading->stop();
    ui->connect_Btn->setText("连接设备");
    ui->label->setText(globalObj->getSocket()->errorString());
    ui->listWidget->setEnabled(true);
}

void Bluetooth::on_listWidget_currentRowChanged(int currentRow)
{
    if(currentRow != 0 || globalObj->isBleConnect())
    {
        ui->connect_Btn->setEnabled(true);
    }
    else
    {
        ui->connect_Btn->setEnabled(false);
    }
}

void Bluetooth::on_ok_Btn_clicked()
{
    this->close();
    if(discoveryAgent)
    {
        if(discoveryAgent->isActive())
        {
            discoveryAgent->stop();
            deviceSearchFinished();
        }
    }
}

QString Bluetooth::majorDeviceClassString(QBluetoothDeviceInfo::MajorDeviceClass deviceClass)
{
    switch (deviceClass)
    {
        case QBluetoothDeviceInfo::MiscellaneousDevice:
            return "杂项设备";
        case QBluetoothDeviceInfo::ComputerDevice:
            return "电脑";
        case QBluetoothDeviceInfo::PhoneDevice:
            return "手机";
        case QBluetoothDeviceInfo::AudioVideoDevice:
            return "音视频设备";
        case QBluetoothDeviceInfo::NetworkDevice:
            return "网络设备";
        case QBluetoothDeviceInfo::PeripheralDevice:
            return "外围设备";
        case QBluetoothDeviceInfo::ImagingDevice:
            return "成像设备";
        case QBluetoothDeviceInfo::WearableDevice:
            return "可穿戴设备";
        case QBluetoothDeviceInfo::ToyDevice:
            return "玩具设备";
        case QBluetoothDeviceInfo::HealthDevice:
            return "健康设备";
        case QBluetoothDeviceInfo::UncategorizedDevice:
            return "未分类";
        default:
            return "未知设备";
    }
}
