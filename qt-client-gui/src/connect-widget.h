#ifndef CONNECTLAYOUT_H
#define CONNECTLAYOUT_H

#include <QObject>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>

class ConnectWidget : public QWidget {
    Q_OBJECT

public:
    ConnectWidget(QWidget *parent = nullptr);
    void setConnectStatus(QString status);
    void hideConnectButton();
    void showConnectButton();
    void clearConnectStatus();
    ~ConnectWidget();

private:
    QVBoxLayout *widgetVLayout;
    QPushButton *connectButton;
    QLabel *connectStatus;
    QHBoxLayout *hConnectLayout;
    QHBoxLayout *hConnectStatusLayout;
signals:
    void connectButtonClicked();
};


#endif // CONNECTLAYOUT_H

