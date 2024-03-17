#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"
#include "settings-dialog.h"
#include "connect-layout.h"
#include "main-layout.h"
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QIntValidator>
#include <QDialog>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onConnectButtonClicked();
    void onDisconnectButtonClicked();
    void onSendButtonClicked();
    //void switchToMainLayout();
    //void switchToConnectLayout();
private:
    Ui::MainWindow *ui;
    Client& client = Client::get_instance();
    QStackedWidget *stackedCentralWidget;
    ConnectLayout *connectLayout;
    MainLayout *mainLayout;
    //void createVCentralMainLayout();
    //void createVCentralConnectLayout();

    /*buttons*/
    //QPushButton *connectButton;
    //QPushButton *disconnectButton;
    //QPushButton *sendButton;
    /*labels*/
    //QLabel *connectStatus;
    //QLabel *requestInputLabel;
    //QLabel *responseLabel;
    /*LineEdits*/
    //QLineEdit *requestInputLineEdit;
    /*vertical layouts*/
    //QVBoxLayout *vCentralConnectLayout;
    //QVBoxLayout *vCentralMainLayout;
    /*horizontal layouts*/
    /*settings dialog*/
    //QDialog *settingsDialog;
};
#endif // MAINWINDOW_H
