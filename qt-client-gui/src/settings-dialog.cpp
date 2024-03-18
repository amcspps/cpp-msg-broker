#include "settings-dialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint);
    okDialogButton = new QPushButton("Ok");
    cancelDialogButton = new QPushButton("Cancel");

    hostLabel = new QLabel("host: ");
    portLabel = new QLabel("port: ");
    logLabel = new QLabel("log level: ");

    hostComboBox = new QComboBox();
    portComboBox = new QComboBox();
    logComboBox = new QComboBox();

    hHostLayout = new QHBoxLayout();
    hHostLayout->addWidget(hostLabel);
    hHostLayout->addWidget(hostComboBox);

    hPortLayout = new QHBoxLayout();
    hPortLayout->addWidget(portLabel);
    hPortLayout->addWidget(portComboBox);

    hLogLayout = new QHBoxLayout();
    hLogLayout->addWidget(logLabel);
    hLogLayout->addWidget(logComboBox);

    hButtonLayout = new QHBoxLayout();
    hButtonLayout->addWidget(okDialogButton);
    hButtonLayout->addWidget(cancelDialogButton);

    verticalDialogLayout = new QVBoxLayout();

    verticalDialogLayout->addLayout(hHostLayout);
    verticalDialogLayout->addLayout(hPortLayout);
    verticalDialogLayout->addLayout(hLogLayout);
    verticalDialogLayout->addLayout(hButtonLayout);
    setLayout(verticalDialogLayout);

    connect(okDialogButton, &QPushButton::clicked,
            this, &SettingsDialog::okButtonClicked);
    connect(cancelDialogButton, &QPushButton::clicked,
            this, &SettingsDialog::cancelButtonClicked);

};


SettingsDialog::~SettingsDialog() {
    delete okDialogButton;
    delete cancelDialogButton;
    delete hostLabel;
    delete portLabel;
    delete logLabel;
    delete hostComboBox;
    delete portComboBox;
    delete logComboBox;
    delete hHostLayout;
    delete hPortLayout;
    delete hLogLayout;
    delete hButtonLayout;
    delete verticalDialogLayout;
};

