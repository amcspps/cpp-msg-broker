#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QIntValidator>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setGeometry(100,100,800,600);
    connectWidget = new ConnectWidget();
    mainWidget = new MainWidget();

    centralStackedWidget = new QStackedWidget();
    setCentralWidget(centralStackedWidget);
    centralStackedWidget->addWidget(connectWidget);
    centralStackedWidget->addWidget(mainWidget);

    connect(connectWidget, &ConnectWidget::connectButtonClicked, this, &MainWindow::onConnectButtonClicked);
    connect(mainWidget, &MainWidget::disconnectButtonClicked, this, &MainWindow::onDisconnectButtonClicked);
    connect(mainWidget, &MainWidget::sendButtonClicked, this, &MainWindow::onSendButtonClicked);
}

MainWindow::~MainWindow()
{
    delete connectWidget;
    delete mainWidget;
    delete centralStackedWidget;
}

void MainWindow::onConnectButtonClicked() {
    qDebug() << "mainwindow on connect button clicked";
    client.connect();
    client.create_tcp_socket();
    client.open_tcp_socket();
    client.login();
    client.open_channel();
    connectWidget->hideConnectButton();
    connectWidget->setConnectStatus("Connected!");

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, timer]() {
        centralStackedWidget->setCurrentWidget(mainWidget);
        timer->deleteLater();
    });
    timer->start(500);
}
void MainWindow::onDisconnectButtonClicked() {
    qDebug() << "mainwindow on disconnect button clicked";
    client.close_channel();
    client.close_connection();
    client.disconnect();
    mainWidget->hideDisconnectButton();
    mainWidget->hideSendButton();
    mainWidget->hideRequestInputLabel();
    mainWidget->hideRequestInputLineEdit();
    mainWidget->setResponseLabel("Disconnected!");

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, timer]() {
        centralStackedWidget->setCurrentWidget(connectWidget);
        timer->deleteLater();
    });
    timer->start(500);

}
void MainWindow::onSendButtonClicked() {
    qDebug() << "mainwindow on send button clicked";
}

// void MainWindow::onDisconnectButtonClicked() {
//     client.close_channel();
//     client.close_connection();
//     client.disconnect();
//     qDebug() << "disconnectButton clicked";
//     disconnectButton->hide();
//     //createVCentralConnectLayout();
//     //removeLayoutWidgets(vCentralMainLayout);
//     //delete vCentralMainLayout;
//     ui->centralwidget->setLayout(vCentralConnectLayout);
//     connectButton->hide();
//     connectStatus->setText("Disconnected!");
//     QTimer::singleShot(500, this, &MainWindow::switchToConnectLayout);
// }

// void MainWindow::onSendButtonClicked() {
//     qDebug() << "sendButton clicked";
//     if(requestInputLineEdit->text().isEmpty()) {
//         responseLabel->setText("input something");
//     }
//     else {
//         client.create_reply_queue();
//         client.publish_request(requestInputLineEdit->text().toInt());
//         client.set_consumer();
//         auto response = client.process_response();
//         if(!std::get<0>(response)) {
//             responseLabel->setText("request timed out, server is down");
//         }
//         else {
//             responseLabel->setText(QString::fromStdString("received from server: " + std::get<1>(response)));
//             std::cout << "response: " << std::get<1>(response) << std::endl;
//         }
//     }
// };

// void MainWindow::switchToMainLayout() {
//     qDebug() << "switchToMainLayout call";
//     //removeLayoutWidgets(vCentralConnectLayout);
//     //delete vCentralConnectLayout;
//     //createVCentralMainLayout();
//     ui->centralwidget->setLayout(vCentralMainLayout);
// }

//void MainWindow::switchToConnectLayout() {
    //qDebug() << "switchToConnectLayout call";
    //connectButton->show();
    //connectStatus->clear();
//}


// void MainWindow::createSettingsDialog() {

// }

// void MainWindow::onBtnClicked() {
//     qDebug() << "signal caught" ;
// }
