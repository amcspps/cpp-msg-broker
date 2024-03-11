#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
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
    void switchToMainLayout();
    void switchToConnectLayout();
private:
    void createVCentralMainLayout();
    void createVCentralConnectLayout();
    Ui::MainWindow *ui;
    /*buttons*/
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    /*labels*/
    QLabel *connectStatus;
    QLabel *requestInputLabel;
    QLineEdit *requestInputLineEdit;
    /*vertical layouts*/
    QVBoxLayout *vCentralConnectLayout;
    QVBoxLayout *vCentralMainLayout;
    /*horizontal layouts*/
    QHBoxLayout *hDisconnectLayout;
    QHBoxLayout *hRequestInputLabelLayout;
    QHBoxLayout *hRequestInputLineEditLayout;
    QHBoxLayout *hConnectLayout;
    QHBoxLayout *hConnectStatusLayout;

};
#endif // MAINWINDOW_H
