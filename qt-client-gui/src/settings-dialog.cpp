#include "settings-dialog.h"
#include <iostream>
#include "client.h"


Client& client = Client::get_instance();


SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint);
    portValidator = new QIntValidator();
    nisValidator = new NoIntSpcValidator();
    okDialogButton = new QPushButton("Ok");
    cancelDialogButton = new QPushButton("Cancel");

    hostLabel = new QLabel("host: ");
    portLabel = new QLabel("port: ");
    logdirLabel = new QLabel("logdir: ");
    logLabel = new QLabel("log level: ");
    settingsStatus = new QLabel();
    settingsStatus->setAlignment(Qt::AlignCenter);

    hostLineEdit = new QLineEdit();
    portLineEdit = new QLineEdit();
    logdirLineEdit = new QLineEdit();
    logdirLineEdit->setText("../log");

    hostLineEdit->setValidator(nisValidator);
    portLineEdit->setValidator(portValidator);

    logComboBox = new QComboBox();
    logComboBox->addItem("INFO");
    logComboBox->addItem("ERROR");

    hHostLayout = new QHBoxLayout();
    hHostLayout->addWidget(hostLabel);
    hHostLayout->addWidget(hostLineEdit);

    hPortLayout = new QHBoxLayout();
    hPortLayout->addWidget(portLabel);
    hPortLayout->addWidget(portLineEdit);

    hLogDirLayout = new QHBoxLayout();
    hLogDirLayout->addWidget(logdirLabel);
    hLogDirLayout->addWidget(logdirLineEdit);

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
    verticalDialogLayout->addLayout(hLogDirLayout);
    verticalDialogLayout->addLayout(hLogLayout);
    verticalDialogLayout->addLayout(hSettingsStatusLayout);
    verticalDialogLayout->addLayout(hButtonLayout);

    setLayout(verticalDialogLayout);

    connect(okDialogButton, &QPushButton::clicked,
            this, &SettingsDialog::slotOkButtonDone);
    connect(cancelDialogButton, &QPushButton::clicked,
            this, &SettingsDialog::slotCancelButtonClicked);

}

SettingsDialog::~SettingsDialog() {
    delete okDialogButton;
    delete cancelDialogButton;
    delete hostLabel;
    delete portLabel;
    delete logdirLabel;
    delete logLabel;
    delete hostLineEdit;
    delete portLineEdit;
    delete logdirLineEdit;
    delete logComboBox;
    delete settingsStatus;
    delete hHostLayout;
    delete hPortLayout;
    delete hLogDirLayout;
    delete hLogLayout;
    delete hSettingsStatusLayout;
    delete hButtonLayout;
    delete verticalDialogLayout;
    delete portValidator;
    delete nisValidator;
}

void SettingsDialog::slotOkButtonDone() {
    if(!hostLineEdit->text().isEmpty() &&
       !portLineEdit->text().isEmpty() && !logdirLineEdit->text().isEmpty()) {
        dumpCfgIni(client.get_cfg_path());
        client.load_cfg();
        client.start_logging();
        hostLineEdit->clear();
        portLineEdit->clear();
        logdirLineEdit->setText("../log");
        settingsStatus->clear();
        hide();
        LOG(INFO) << "Qt: SettingsDialog ok button done";
        emit okButtonDone();
    }
    else {
        settingsStatus->setText("set valid parameters");
    }
}

void SettingsDialog::slotCancelButtonClicked() {
    hostLineEdit->clear();
    portLineEdit->clear();
    settingsStatus->clear();
    emit cancelButtonClicked();
    hide();
}

void SettingsDialog::dumpCfgIni(std::string cfg_path) {
    QSettings *settingsIni = new QSettings(QString::fromStdString(cfg_path), QSettings::IniFormat);
    settingsIni->beginGroup("client");
    settingsIni->setValue("host", hostLineEdit->text());
    settingsIni->setValue("port", portLineEdit->text());
    settingsIni->setValue("log_dir", logdirLineEdit->text());
    settingsIni->setValue("log_lvl", logComboBox->currentText());
    settingsIni->endGroup();
    delete settingsIni;
    LOG(INFO) << "Qt: SettingsDialog dumping cfg";
}
