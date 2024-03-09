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
    QVBoxLayout *vLayout = new QVBoxLayout();
    ui->centralwidget->setLayout(vLayout);


    QHBoxLayout *hConnectLayout = new QHBoxLayout();
    connectButton = new QPushButton("Connect");
    connectButton->setFixedSize(300, 50);
    hConnectLayout->addWidget(connectButton);


    QHBoxLayout *hDisconnectLayout = new QHBoxLayout();
    disconnectButton = new QPushButton("Disconnect");
    disconnectButton->setFixedSize(300, 50);
    disconnectButton->hide();
    hDisconnectLayout->addStretch();
    hDisconnectLayout->addWidget(disconnectButton);


    QHBoxLayout *hConnectStatusLayout = new QHBoxLayout();
    hConnectStatusLayout->setAlignment(Qt::AlignCenter);
    connectStatus = new QLabel();

    hConnectStatusLayout->addWidget(connectStatus);

    vLayout->addLayout(hDisconnectLayout);
    vLayout->addStretch();
    vLayout->addLayout(hConnectLayout);
    vLayout->addLayout(hConnectStatusLayout);
    vLayout->addStretch();

    connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);
    connect(disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectButtonClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onConnectButtonClicked() {
    qDebug() << "connectButton clicked";
    connectButton->hide();
    connectStatus->setText("Connected!");
    QTimer::singleShot(3000, this, &MainWindow::hideConnectionStatus);
}

void MainWindow::onDisconnectButtonClicked() {
    qDebug() << "disconnectButton clicked";
    disconnectButton->hide();
    connectStatus->show();
    connectStatus->setText("Disconnected!");
    QTimer::singleShot(3000, this, &MainWindow::showConnectionStatus);
}

void MainWindow::hideConnectionStatus() {
    qDebug() << "hideConnectionStatus call";
    connectStatus->hide();
    connectButton->hide();
    disconnectButton->show();
}

void MainWindow::showConnectionStatus() {
    qDebug() << "showConnectionStatus call";
    connectStatus->show();
    connectButton->show();
    connectStatus->clear();
}
