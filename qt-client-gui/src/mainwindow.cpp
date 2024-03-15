#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QIntValidator>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createVCentralConnectLayout();
    ui->centralwidget->setLayout(vCentralConnectLayout);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void removeLayoutWidgets(QLayout *layout) {
    if (!layout)
        return;

    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *widget = item->widget()) {
            delete widget;
        } else if (QLayout *childLayout = item->layout()) {
            removeLayoutWidgets(childLayout);
        }
        delete item;
    }
}

void MainWindow::createVCentralMainLayout() {
    vCentralMainLayout = new QVBoxLayout();

    disconnectButton = new QPushButton("Disconnect");
    disconnectButton->setFixedSize(300, 50);

    sendButton = new QPushButton("Send");
    sendButton->setFixedSize(50, 30);

    hDisconnectLayout = new QHBoxLayout();
    hDisconnectLayout->addStretch();
    hDisconnectLayout->addWidget(disconnectButton);

    requestInputLabel = new QLabel("Введите число для отправки:");
    requestInputLabel->setFixedSize(300, 20);
    hRequestInputLabelLayout = new QHBoxLayout();
    hRequestInputLabelLayout->addWidget(requestInputLabel);


    hRequestInputLineEditLayout = new QHBoxLayout();
    requestInputLineEdit = new QLineEdit();
    requestValidator = new QIntValidator();
    requestInputLineEdit->setValidator(requestValidator);
    requestInputLineEdit->setFixedSize(300, 20);
    hRequestInputLineEditLayout->addWidget(requestInputLineEdit);


    hSendButtonLayout = new QHBoxLayout();
    hSendButtonLayout->addWidget(sendButton);

    responseLabel = new QLabel("debug");
    responseLabel->setFixedSize(300, 20);
    responseLabel->setAlignment(Qt::AlignCenter);
    hResponseLabelLayout = new QHBoxLayout();
    hResponseLabelLayout->addWidget(responseLabel);

    vCentralMainLayout->addLayout(hDisconnectLayout);
    vCentralMainLayout->addStretch();
    vCentralMainLayout->addLayout(hRequestInputLabelLayout);
    vCentralMainLayout->addLayout(hRequestInputLineEditLayout);
    vCentralMainLayout->addLayout(hResponseLabelLayout);
    vCentralMainLayout->addLayout(hSendButtonLayout);
    vCentralMainLayout->addStretch();

    connect(disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectButtonClicked);
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
}


void MainWindow::createVCentralConnectLayout() {
    vCentralConnectLayout = new QVBoxLayout();

    hConnectLayout = new QHBoxLayout();
    connectButton = new QPushButton("Connect");
    connectButton->setFixedSize(300, 50);
    hConnectLayout->addWidget(connectButton);


    hConnectStatusLayout = new QHBoxLayout();
    hConnectStatusLayout->setAlignment(Qt::AlignCenter);
    connectStatus = new QLabel();
    hConnectStatusLayout->addWidget(connectStatus);

    vCentralConnectLayout->addStretch();
    vCentralConnectLayout->addLayout(hConnectLayout);
    vCentralConnectLayout->addLayout(hConnectStatusLayout);
    vCentralConnectLayout->addStretch();

    connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);
}

void MainWindow::onConnectButtonClicked() {
    client.connect();
    client.create_tcp_socket();
    client.open_tcp_socket();
    client.login();
    client.open_channel();
    qDebug() << "connectButton clicked";
    connectButton->hide();
    connectStatus->setText("Connected!");
    QTimer::singleShot(500, this, &MainWindow::switchToMainLayout);
}

void MainWindow::onDisconnectButtonClicked() {
    client.close_channel();
    client.close_connection();
    client.disconnect();
    qDebug() << "disconnectButton clicked";
    disconnectButton->hide();
    createVCentralConnectLayout();
    removeLayoutWidgets(vCentralMainLayout);
    delete vCentralMainLayout;
    ui->centralwidget->setLayout(vCentralConnectLayout);
    connectButton->hide();
    connectStatus->setText("Disconnected!");
    QTimer::singleShot(500, this, &MainWindow::switchToConnectLayout);
}

void MainWindow::onSendButtonClicked() {
    qDebug() << "sendButton clicked";
    if(requestInputLineEdit->text().isEmpty()) {
        responseLabel->setText("input something");
    }
    else {
        client.create_reply_queue();
        client.publish_request(requestInputLineEdit->text().toInt());
        client.set_consumer();
        client.process_response();
    }
};

void MainWindow::switchToMainLayout() {
    qDebug() << "switchToMainLayout call";
    removeLayoutWidgets(vCentralConnectLayout);
    delete vCentralConnectLayout;
    createVCentralMainLayout();
    ui->centralwidget->setLayout(vCentralMainLayout);
}

void MainWindow::switchToConnectLayout() {
    qDebug() << "switchToConnectLayout call";
    connectButton->show();
    connectStatus->clear();
}
