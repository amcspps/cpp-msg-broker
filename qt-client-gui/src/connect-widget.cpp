#include "connect-widget.h"
#include <QDebug>
#include <QTimer>
#include "client.h"
extern Client& client;
ConnectWidget::ConnectWidget(QWidget *parent) {
    widgetVLayout = new QVBoxLayout();
    connectButton = new QPushButton("Connect");
    connectButton->setFixedSize(300, 50);

    hConnectLayout = new QHBoxLayout();
    hConnectLayout->addWidget(connectButton);

    hConnectStatusLayout = new QHBoxLayout();
    hConnectStatusLayout->setAlignment(Qt::AlignCenter);
    connectStatus = new QLabel();
    hConnectStatusLayout->addWidget(connectStatus);

    widgetVLayout->addStretch();
    widgetVLayout->addLayout(hConnectLayout);
    widgetVLayout->addLayout(hConnectStatusLayout);
    widgetVLayout->addStretch();

    setLayout(widgetVLayout);

    connect(connectButton, &QPushButton::clicked, this, &ConnectWidget::connectButtonClicked);
}


ConnectWidget::~ConnectWidget() {
    delete connectButton;
    delete connectStatus;
    delete hConnectLayout;
    delete hConnectStatusLayout;
    delete widgetVLayout;

}

void ConnectWidget::slotOkButtonDone() {
    qDebug() << "connect widget slot ok button clicked (conn success)";
    connectButton->hide();
    try {
        client.connect();
        client.create_tcp_socket();
        client.open_tcp_socket();
        client.login();
        client.open_channel();
        connectStatus->setText("Connected!");

        emit connectionSuccessful();
    }
    catch(...) {
        connectStatus->setText("Connection Failed!");
        QTimer *timer = new QTimer();
        connect(timer, &QTimer::timeout, [this, timer]() {
            connectStatus->setText("");
            connectButton->show();
            timer->deleteLater();
        });
        timer->start(500);
    }


}

void ConnectWidget::slotDisconnectButtonClicked() {
    qDebug() << "connect widget slot disconnect button clicked";
    connectStatus->clear();
    connectButton->show();
}

