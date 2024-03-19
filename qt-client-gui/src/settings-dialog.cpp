#include "settings-dialog.h"
#include <iostream>
#include "client.h"

Client& client = Client::get_instance();

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint);
    portValidator = new QIntValidator();
    QRegExp re("^[^\\d\\s]*$");
    reValidator = new QRegExpValidator(re);
    okDialogButton = new QPushButton("Ok");
    cancelDialogButton = new QPushButton("Cancel");

    hostLabel = new QLabel("host: ");
    portLabel = new QLabel("port: ");
    logLabel = new QLabel("log level: ");
    settingsStatus = new QLabel();
    settingsStatus->setAlignment(Qt::AlignCenter);

    hostLineEdit = new QLineEdit();
    portLineEdit = new QLineEdit();

    hostLineEdit->setValidator(reValidator);
    portLineEdit->setValidator(portValidator);

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
            this, &SettingsDialog::slotOkButtonDone);
    connect(cancelDialogButton, &QPushButton::clicked,
            this, &SettingsDialog::slotCancelButtonClicked);

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
    delete reValidator;
};

void SettingsDialog::slotOkButtonDone() {
    qDebug() << "slot Settings Dialog ok button clicked";
    if(!hostLineEdit->text().isEmpty() && !portLineEdit->text().isEmpty()) {
        dumpCfgIni(client.get_cfg_path());
        client.load_cfg();
        hostLineEdit->clear();
        portLineEdit->clear();
        settingsStatus->clear();
        settingsStatus->clear();
        hide();
        emit okButtonDone();
    }
    else if (hostLineEdit->text().isEmpty() && portLineEdit->text().isEmpty()) {
        settingsStatus->setText("set parameters");
    }
    else if (hostLineEdit->text().isEmpty()) {
        settingsStatus->setText("set host");
    }
    else if (portLineEdit->text().isEmpty()) {
        settingsStatus->setText("set port");
    }
}

void SettingsDialog::slotCancelButtonClicked() {
    hostLineEdit->clear();
    portLineEdit->clear();
    settingsStatus->clear();
    emit cancelButtonClicked();
    hide();
}


// void SettingsDialog::clearHostLineEdit() {
//     hostLineEdit->clear();
// }

// void SettingsDialog::clearPortLineEdit() {
//     portLineEdit->clear();
// }

// void SettingsDialog::clearSettingsStatusLabel() {
//     settingsStatus->clear();
// }

// QString SettingsDialog::getHostLineEditText() {
//     return hostLineEdit->text();
// }

// int SettingsDialog::getPortLineEditText() {
//     return portLineEdit->text().toInt();
// }

// QString SettingsDialog::getLogText() {
//     return logComboBox->currentText();
// }

// void SettingsDialog::setSettingsStatus(QString status) {
//     settingsStatus->setText(status);
// }


void SettingsDialog::dumpCfgIni(std::string cfg_path) {
    QSettings *settingsIni = new QSettings(QString::fromStdString(cfg_path), QSettings::IniFormat);
    settingsIni->beginGroup("client");
    settingsIni->setValue("host", hostLineEdit->text());
    settingsIni->setValue("port", portLineEdit->text());
    settingsIni->endGroup();
    delete settingsIni;
    qDebug() << "dumping cfg";
};



