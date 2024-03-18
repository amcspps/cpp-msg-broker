#include "connect-widget.h"
#include <QDebug>

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

void ConnectWidget::setConnectStatus(QString status) {
    connectStatus->setText(status);
}

void ConnectWidget::hideConnectButton() {
    connectButton->hide();
}

void ConnectWidget::showConnectButton() {
    connectButton->show();
}

void ConnectWidget::clearConnectStatus() {
    connectStatus->clear();
}

