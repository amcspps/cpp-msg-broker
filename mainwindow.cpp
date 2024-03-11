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

    hDisconnectLayout = new QHBoxLayout();
    hDisconnectLayout->addStretch();
    hDisconnectLayout->addWidget(disconnectButton);

    requestInputLabel = new QLabel("Введите число для отправки:");
    hRequestInputLabelLayout = new QHBoxLayout();
    hRequestInputLabelLayout->addWidget(requestInputLabel);


    hRequestInputLineEditLayout = new QHBoxLayout();
    requestInputLineEdit = new QLineEdit();
    hRequestInputLineEditLayout->addWidget(requestInputLineEdit);


    vCentralMainLayout->addLayout(hDisconnectLayout);
    vCentralMainLayout->addStretch();
    vCentralMainLayout->addLayout(hRequestInputLabelLayout);
    vCentralMainLayout->addLayout(hRequestInputLineEditLayout);
    vCentralMainLayout->addStretch();

    connect(disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectButtonClicked);
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
    qDebug() << "connectButton clicked";
    connectButton->hide();
    connectStatus->setText("Connected!");
    QTimer::singleShot(500, this, &MainWindow::switchToMainLayout);
}

void MainWindow::onDisconnectButtonClicked() {
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
