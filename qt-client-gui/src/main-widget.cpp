#include "main-widget.h"
#include <QDebug>
#include "client.h"

extern Client& client;

MainWidget::MainWidget(QWidget *parent) {
    widgetVLayout = new QVBoxLayout();

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

    requestInputLineEdit = new QLineEdit();
    requestValidator = new QIntValidator();
    requestInputLineEdit->setValidator(requestValidator);
    requestInputLineEdit->setFixedSize(300, 20);

    hRequestInputLineEditLayout = new QHBoxLayout();
    hRequestInputLineEditLayout->addWidget(requestInputLineEdit);

    responseLabel = new QLabel("response status will be here");
    responseLabel->setFixedSize(300, 20);
    responseLabel->setAlignment(Qt::AlignCenter);


    hResponseLabelLayout = new QHBoxLayout();
    hResponseLabelLayout->addWidget(responseLabel);

    hSendButtonLayout = new QHBoxLayout();

    hSendButtonLayout->addWidget(sendButton);


    widgetVLayout->addLayout(hDisconnectLayout);
    widgetVLayout->addStretch();
    widgetVLayout->addLayout(hRequestInputLabelLayout);
    widgetVLayout->addLayout(hRequestInputLineEditLayout);
    widgetVLayout->addLayout(hResponseLabelLayout);
    widgetVLayout->addLayout(hSendButtonLayout);
    widgetVLayout->addStretch();

    setLayout(widgetVLayout);
    connect(disconnectButton, &QPushButton::clicked, this, &MainWidget::slotDisconnectButtonClicked);
    connect(sendButton, &QPushButton::clicked, this, &MainWidget::slotSendButtonClicked);
}

MainWidget::~MainWidget() {
    delete disconnectButton;
    delete sendButton;
    delete requestInputLabel;
    delete responseLabel;
    delete requestInputLineEdit;

    delete hDisconnectLayout;
    delete hRequestInputLabelLayout;
    delete requestValidator;
    delete hRequestInputLineEditLayout;
    delete hResponseLabelLayout;
    delete hSendButtonLayout;
    delete widgetVLayout;
}

void MainWidget::slotDisconnectButtonClicked() {
    LOG(INFO) << "Qt: MainWidget slot disconnect button clicked";
    //qDebug() << "main widget slot disco butt clicked";
    client.close_channel();
    client.close_connection();
    client.disconnect();
    disconnectButton->hide();
    sendButton->hide();
    requestInputLabel->hide();
    requestInputLineEdit->clear();
    requestInputLineEdit->hide();
    responseLabel->setText("Disconnected!");
    emit disconnectButtonClicked();
}


void MainWidget::slotOkButtonDone() {
    LOG(INFO) << "Qt: MainWidget slot connect button clicked";
    //qDebug() << "main widget slot connect button clicked";
    disconnectButton->show();
    sendButton->show();
    requestInputLabel->show();
    requestInputLineEdit->show();
    responseLabel->clear();
}


void MainWidget::slotSendButtonClicked() {
    LOG(INFO) << "Qt: MainWidget slot send button clicked";
    //qDebug() << "main widget slot send button clicked";
    if(requestInputLineEdit->text().isEmpty()) {
        LOG(ERROR) << "Qt: No number input provided";
        responseLabel->setText("input something");
    }
    else {
       client.create_reply_queue();
       client.publish_request(requestInputLineEdit->text().toInt());
       client.set_consumer();
       auto response = client.process_response();
       if(!std::get<0>(response)) {
            LOG(ERROR) << "Qt: no response from server";
            responseLabel->setText("request timed out, server is down");
       }
       else {
            responseLabel->setText(
                       QString::fromStdString("received from server: " + std::get<1>(response)));
            LOG(INFO) << "Qt: got response from server";
       }
    }
}

