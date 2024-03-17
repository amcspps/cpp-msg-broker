#include "main-layout.h"
#include <QDebug>


MainLayout::MainLayout(QWidget *parent) {
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

    responseLabel = new QLabel("debug");
    responseLabel->setFixedSize(300, 20);
    responseLabel->setAlignment(Qt::AlignCenter);


    hResponseLabelLayout = new QHBoxLayout();
    hResponseLabelLayout->addWidget(responseLabel);

    hSendButtonLayout = new QHBoxLayout();

    hSendButtonLayout->addWidget(sendButton);

    addLayout(hDisconnectLayout);
    addStretch();
    addLayout(hRequestInputLabelLayout);
    addLayout(hRequestInputLineEditLayout);
    addLayout(hResponseLabelLayout);
    addLayout(hSendButtonLayout);
    addStretch();

    connect(disconnectButton, &QPushButton::clicked, this, &MainLayout::onDisconnectButtonClicked);
    connect(sendButton, &QPushButton::clicked, this, &MainLayout::onSendButtonClicked);
}

MainLayout::~MainLayout() {
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
}

void MainLayout::onDisconnectButtonClicked() {
    qDebug() << "on disconnect button clicked";
    emit disconnectButtonClicked();
}



void MainLayout::onSendButtonClicked() {
    qDebug() << "on send button clicked";
    emit sendButtonClicked();
}



void MainLayout::setResponseLabel(QString label) {
    responseLabel->setText(label);
}


void MainLayout::hideDisconnectButton() {
    disconnectButton->hide();
}

void MainLayout::hideSendButton() {
    sendButton->hide();
}












