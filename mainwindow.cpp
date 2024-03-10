#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

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

void MainWindow::createVCentralMainLayout() {
    vCentralMainLayout = new QVBoxLayout();

    disconnectButton = new QPushButton("Disconnect");
    disconnectButton->setFixedSize(300, 50);


    QHBoxLayout *hDisconnectLayout = new QHBoxLayout();
    hDisconnectLayout->addStretch();
    hDisconnectLayout->addWidget(disconnectButton);

    vCentralMainLayout->addLayout(hDisconnectLayout);
    vCentralMainLayout->addStretch();

    connect(disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectButtonClicked);
}


void MainWindow::createVCentralConnectLayout() {
    vCentralConnectLayout = new QVBoxLayout();

    QHBoxLayout *hConnectLayout = new QHBoxLayout();
    connectButton = new QPushButton("Connect");
    connectButton->setFixedSize(300, 50);
    hConnectLayout->addWidget(connectButton);


    QHBoxLayout *hConnectStatusLayout = new QHBoxLayout();
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
    qDebug() << "connectButton clicked";
    connectButton->hide();
    /*TODO: update hideConnectionStatus*/
    connectStatus->setText("Connected!");
    QTimer::singleShot(500, this, &MainWindow::switchToMainLayout);
}

void MainWindow::onDisconnectButtonClicked() {
    qDebug() << "disconnectButton clicked";
    disconnectButton->hide();
    createVCentralConnectLayout();
    delete vCentralMainLayout;
    ui->centralwidget->setLayout(vCentralConnectLayout);
    connectButton->hide();
    connectStatus->setText("Disconnected!");
    QTimer::singleShot(500, this, &MainWindow::switchToConnectLayout);
}

void MainWindow::switchToMainLayout() {
    qDebug() << "switchToMainLayout call";
    connectStatus->hide();
    delete vCentralConnectLayout;
    createVCentralMainLayout();
    ui->centralwidget->setLayout(vCentralMainLayout);
}

void MainWindow::switchToConnectLayout() {
    qDebug() << "switchToConnectLayout call";
    connectButton->show();
    connectStatus->clear();
}
