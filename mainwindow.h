#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>

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

private slots:
    void onConnectButtonClicked();
    void onDisconnectButtonClicked();
    void hideConnectionStatus();
    void showConnectionStatus();
private:
    Ui::MainWindow *ui;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QLabel *connectStatus;
};
#endif // MAINWINDOW_H
