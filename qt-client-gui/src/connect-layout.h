#ifndef CONNECTLAYOUT_H
#define CONNECTLAYOUT_H

#include <QObject>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>

class ConnectLayout : public QVBoxLayout {
    Q_OBJECT

public:
    ConnectLayout(QWidget *parent = nullptr);
    void setConnectStatus(QString status);
    void hideConnectButton();
    ~ConnectLayout();

private slots:
    void onConnectButtonClicked();
private:
    QPushButton *connectButton;
    QLabel *connectStatus;
    QHBoxLayout *hConnectLayout;
    QHBoxLayout *hConnectStatusLayout;
signals:
    void connectButtonClicked();
};


#endif // CONNECTLAYOUT_H

