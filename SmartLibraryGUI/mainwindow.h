#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QCloseEvent>
#include <QProcess>
#include "bluetooth.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_bluetooth_Btn_clicked();
    void on_bookMange_Btn_clicked();
    void on_settings_Btn_clicked();
    void on_reboot_Btn_clicked();
    void on_quit_Btn_clicked();
    void on_findBook_Btn_clicked();
    void on_borrowBook_Btn_clicked();
    void on_returnBook_Btn_clicked();
    void on_viewBook_Btn_clicked();
    // 蓝牙信号
    void bleConnect();
    void bleDisconnect();
    void on_stop_Btn_clicked();
private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent * event);
    Bluetooth ble;
};
#endif // MAINWINDOW_H
