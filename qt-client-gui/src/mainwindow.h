#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"
#include "settings-dialog.h"
#include "connect-widget.h"
#include "main-widget.h"
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QIntValidator>
#include <QDialog>
#include <QStackedWidget>

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
    void onOkButtonClicked();
    void onCancelButtonClicked();
private:
    QStackedWidget *centralStackedWidget;
    ConnectWidget *connectWidget;
    MainWidget *mainWidget;
    SettingsDialog *settingsDialog;
};
#endif // MAINWINDOW_H
