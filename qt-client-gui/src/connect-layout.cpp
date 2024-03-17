#include "connect-layout.h"
#include <QDebug>

ConnectLayout::ConnectLayout(QWidget *parent) {
    connectButton = new QPushButton("Connect");
    connectButton->setFixedSize(300, 50);

    hConnectLayout = new QHBoxLayout();
    hConnectLayout->addWidget(connectButton);

    hConnectStatusLayout = new QHBoxLayout();
    hConnectStatusLayout->setAlignment(Qt::AlignCenter);
    connectStatus = new QLabel();
    hConnectStatusLayout->addWidget(connectStatus);

    addStretch();
    addLayout(hConnectLayout);
    addLayout(hConnectStatusLayout);
    addStretch();

    connect(connectButton, &QPushButton::clicked, this, &ConnectLayout::onConnectButtonClicked);
}


ConnectLayout::~ConnectLayout() {
    delete connectButton;
    delete connectStatus;
    delete hConnectLayout;
    delete hConnectStatusLayout;

}

void ConnectLayout::onConnectButtonClicked() {
    qDebug() << "Slot connect button clicked";
    emit connectButtonClicked();
}

void ConnectLayout::setConnectStatus(QString status) {
    connectStatus->setText(status);

}

void ConnectLayout::hideConnectButton() {
    connectButton->hide();
}




