#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QComboBox>
#include <QLabel>


class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();
signals:
    void okButtonClicked();
    void cancelButtonClicked();
private:
    QPushButton *okDialogButton;
    QPushButton *cancelDialogButton;
    QLabel *hostLabel;
    QLabel *portLabel;
    QLabel *logLabel;
    QComboBox *hostComboBox;
    QComboBox *portComboBox;
    QComboBox *logComboBox;
    QHBoxLayout *hHostLayout;
    QHBoxLayout *hPortLayout;
    QHBoxLayout *hLogLayout;
    QHBoxLayout *hButtonLayout;
    QVBoxLayout *verticalDialogLayout;
};

#endif // SETTINGSDIALOG_H
