#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bookmanage.h"
#include "network.h"
#include "tools.h"
#include "globalprocess.h"
#include "findbook.h"
#include "borrowbook.h"
#include "returnbook.h"
#include "viewbook.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(NetWork::online()){ui->net_attitude->setPixmap(TOOLS::loadImage(":/pic/green.png", QSize(30,30)));}
    if(Database::sqlConnected())
    {
        ui->sql_attitude->setPixmap(TOOLS::loadImage(":/pic/green.png", QSize(30,30)));
        ui->findBook_Btn->setEnabled(true);
        ui->viewBook_Btn->setEnabled(true);
    }
}

MainWindow::~MainWindow()
{
    Database::close();
    delete ui;
}

void MainWindow::on_bluetooth_Btn_clicked()
{
    connect(&ble, &Bluetooth::bleConnected, this, &MainWindow::bleConnect);
    connect(&ble, &Bluetooth::bleDisconnected, this, &MainWindow::bleDisconnect);
    ble.exec();
}

void MainWindow::on_bookMange_Btn_clicked()
{
    BookManage bookManage;
    bookManage.exec();
}

void MainWindow::on_settings_Btn_clicked()
{
    QMessageBox::warning(nullptr, "报告", "设置功能暂未完善，以后再来体验吧！");
}

void MainWindow::on_reboot_Btn_clicked()
{
    qApp->closeAllWindows();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
}

void MainWindow::on_quit_Btn_clicked()
{
    this->close();
}

void MainWindow::bleConnect()
{
    ui->ble_attitude->setPixmap(TOOLS::loadImage(":/pic/green.png", QSize(30,30)));
    ui->borrowBook_Btn->setEnabled(true);
    ui->returnBook_Btn->setEnabled(true);
    ui->viewBook_Btn->setEnabled(true);
    ui->stop_Btn->setEnabled(true);
}

void MainWindow::bleDisconnect()
{
    ui->ble_attitude->setPixmap(TOOLS::loadImage(":/pic/red.png", QSize(30,30)));
    ui->borrowBook_Btn->setEnabled(false);
    ui->returnBook_Btn->setEnabled(false);
    ui->viewBook_Btn->setEnabled(false);
    ui->stop_Btn->setEnabled(false);
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    GlobalProcess *globalObj = GlobalProcess::getInstance();
    if(globalObj->isBleConnect())
    {
        connect(globalObj->getSocket(), &QBluetoothSocket::disconnected, this, [&](){event->accept();});
        globalObj->getSocket()->disconnectFromService();
    }
    else
    {
        event->accept();
    }
}

void MainWindow::on_findBook_Btn_clicked()
{
    findBook w;
    w.exec();
}

void MainWindow::on_borrowBook_Btn_clicked()
{
    borrowBook w;
    w.exec();
}

void MainWindow::on_returnBook_Btn_clicked()
{
    returnBook w;
    w.exec();
}

void MainWindow::on_viewBook_Btn_clicked()
{
    viewBook w;
    w.exec();
}

void MainWindow::on_stop_Btn_clicked()
{
    GlobalProcess::getInstance()->SocketWrite("stop");
}
