#include "settings-dialog.h"
#include <iostream>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint);
    portValidator = new QIntValidator();
    noSpaceValidator = new NoSpaceValidator();
    okDialogButton = new QPushButton("Ok");
    cancelDialogButton = new QPushButton("Cancel");

    hostLabel = new QLabel("host: ");
    portLabel = new QLabel("port: ");
    logLabel = new QLabel("log level: ");
    settingsStatus = new QLabel("kek-lol");
    settingsStatus->setAlignment(Qt::AlignCenter);

    hostLineEdit = new QLineEdit();
    portLineEdit = new QLineEdit();

    hostLineEdit->setValidator(noSpaceValidator);
    portLineEdit->setValidator(portValidator);
    portLineEdit->setValidator(noSpaceValidator);

    logComboBox = new QComboBox();
    logComboBox->addItem("All");
    logComboBox->addItem("Exceptions only");

    hHostLayout = new QHBoxLayout();
    hHostLayout->addWidget(hostLabel);
    hHostLayout->addWidget(hostLineEdit);

    hPortLayout = new QHBoxLayout();
    hPortLayout->addWidget(portLabel);
    hPortLayout->addWidget(portLineEdit);

    hLogLayout = new QHBoxLayout();
    hLogLayout->addWidget(logLabel);
    hLogLayout->addWidget(logComboBox);

    hButtonLayout = new QHBoxLayout();
    hButtonLayout->addWidget(okDialogButton);
    hButtonLayout->addWidget(cancelDialogButton);


    hSettingsStatusLayout = new QHBoxLayout();

    hSettingsStatusLayout->addWidget(settingsStatus);

    verticalDialogLayout = new QVBoxLayout();

    verticalDialogLayout->addLayout(hHostLayout);
    verticalDialogLayout->addLayout(hPortLayout);
    verticalDialogLayout->addLayout(hLogLayout);
    verticalDialogLayout->addLayout(hSettingsStatusLayout);
    verticalDialogLayout->addLayout(hButtonLayout);


    setLayout(verticalDialogLayout);

    connect(okDialogButton, &QPushButton::clicked,
            this, &SettingsDialog::slotOkButtonClicked);
    connect(cancelDialogButton, &QPushButton::clicked,
            this, &SettingsDialog::cancelButtonClicked);

};


SettingsDialog::~SettingsDialog() {
    delete okDialogButton;
    delete cancelDialogButton;
    delete hostLabel;
    delete portLabel;
    delete logLabel;
    delete hostLineEdit;
    delete portLineEdit;
    delete logComboBox;
    delete hHostLayout;
    delete hPortLayout;
    delete hLogLayout;
    delete hSettingsStatusLayout;
    delete hButtonLayout;
    delete verticalDialogLayout;
    delete portValidator;
};

void SettingsDialog::slotOkButtonClicked() {
    qDebug() << "slot Settings Dialog ok button clicked";
    emit okButtonClicked();
}



void SettingsDialog::clearHostLineEdit() {
    hostLineEdit->clear();
}

void SettingsDialog::clearPortLineEdit() {
    portLineEdit->clear();
}

void SettingsDialog::clearSettingsStatusLabel() {
    settingsStatus->clear();
}

QString SettingsDialog::getHostLineEditText() {
    return hostLineEdit->text();
}

int SettingsDialog::getPortLineEditText() {
    return portLineEdit->text().toInt();
}

QString SettingsDialog::getLogText() {
    return logComboBox->currentText();
}

void SettingsDialog::dumpCfgIni(std::string cfg_path) {
    QSettings *settingsIni = new QSettings(QString::fromStdString(cfg_path), QSettings::IniFormat);
    settingsIni->beginGroup("client");
    settingsIni->setValue("host", getHostLineEditText());
    settingsIni->setValue("port", getPortLineEditText());
    settingsIni->endGroup();
    delete settingsIni;
    qDebug() << "dumping cfg";
};



