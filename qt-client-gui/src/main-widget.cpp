#include "main-widget.h"
#include <QDebug>


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
    connect(disconnectButton, &QPushButton::clicked, this, &MainWidget::onDisconnectButtonClicked);
    connect(sendButton, &QPushButton::clicked, this, &MainWidget::onSendButtonClicked);
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

void MainWidget::onDisconnectButtonClicked() {
    qDebug() << "on disconnect button clicked";
    emit disconnectButtonClicked();
}

void MainWidget::onSendButtonClicked() {
    qDebug() << "on send button clicked";
    emit sendButtonClicked();
}

void MainWidget::setResponseLabel(QString label) {
    responseLabel->setText(label);
}


void MainWidget::hideDisconnectButton() {
    disconnectButton->hide();
}

void MainWidget::hideSendButton() {
    sendButton->hide();
}

void MainWidget::hideRequestInputLabel() {
    requestInputLabel->hide();
}

void MainWidget::hideRequestInputLineEdit() {
    requestInputLineEdit->hide();
}

void MainWidget::showRequestInputLabel() {
    requestInputLabel->show();
}

void MainWidget::showRequestInputLineEdit() {
    requestInputLineEdit->show();
}

void MainWidget::clearResponseLabel() {
    responseLabel->clear();
}

void MainWidget::showSendButton() {
    sendButton->show();
}

void MainWidget::showDisconnectButton() {
    disconnectButton->show();
}

QString MainWidget::getRequestInputLineEditText() {
    return requestInputLineEdit->text();
}

void MainWidget::clearRequestInputLineEdit() {
    requestInputLineEdit->clear();
}

