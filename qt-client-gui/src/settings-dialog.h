#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>

class NoSpaceValidator : public QValidator {
public:
    State validate(QString &input, int &pos) const override {
        if (input.contains(' ')) {
            return Invalid;
        }
        return Acceptable;
    }
};

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(QWidget *parent = nullptr);
    void clearPortLineEdit();
    void clearHostLineEdit();
    void clearSettingsStatusLabel();
    QString getHostLineEditText();
    int getPortLineEditText();
    QString getLogText();
    void dumpCfgIni(std::string cfg_path);
    ~SettingsDialog();
signals:
    void okButtonClicked();
    void cancelButtonClicked();
public slots:
    void slotOkButtonClicked();
private:
    QPushButton *okDialogButton;
    QPushButton *cancelDialogButton;
    QLabel *hostLabel;
    QLabel *portLabel;
    QLabel *logLabel;
    QLabel *settingsStatus;
    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QComboBox *logComboBox;
    QHBoxLayout *hHostLayout;
    QHBoxLayout *hPortLayout;
    QHBoxLayout *hLogLayout;
    QHBoxLayout *hButtonLayout;
    QHBoxLayout *hSettingsStatusLayout;
    QVBoxLayout *verticalDialogLayout;
    QIntValidator *portValidator;
    NoSpaceValidator *noSpaceValidator;
};

#endif // SETTINGSDIALOG_H
