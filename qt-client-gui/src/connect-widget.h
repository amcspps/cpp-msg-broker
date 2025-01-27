#ifndef CONNECTLAYOUT_H
#define CONNECTLAYOUT_H


#include <QObject>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <glog/logging.h>


class ConnectWidget : public QWidget {
    Q_OBJECT

public:
    ConnectWidget(QWidget *parent = nullptr);
    ~ConnectWidget();
public slots:
    void slotOkButtonDone();
    void slotDisconnectButtonClicked();
private:
    QVBoxLayout *widgetVLayout;
    QPushButton *connectButton;
    QLabel *connectStatus;
    QHBoxLayout *hConnectLayout;
    QHBoxLayout *hConnectStatusLayout;
signals:
    void connectButtonClicked();
    void connectionSuccessful();
};


#endif // CONNECTLAYOUT_H

