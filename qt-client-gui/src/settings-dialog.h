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
#include <QValidator>


class NoIntSpcValidator : public QValidator {
public:
    State validate(QString &input, int &pos) const override {
        Q_UNUSED(pos);
        for (const QChar &ch : input) {
            if (ch.isDigit() || ch == ' ') {
                return Invalid;
            }
        }
        return Acceptable;
    }
};

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(QWidget *parent = nullptr);
    void dumpCfgIni(std::string cfg_path);
    ~SettingsDialog();
signals:
    void okButtonDone();
    void cancelButtonClicked();
public slots:
    void slotOkButtonDone();
    void slotCancelButtonClicked();
private:
    QPushButton *okDialogButton;
    QPushButton *cancelDialogButton;
    QLabel *hostLabel;
    QLabel *portLabel;
    QLabel *logdirLabel;
    QLabel *logLabel;
    QLabel *settingsStatus;
    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QLineEdit *logdirLineEdit;
    QComboBox *logComboBox;
    QHBoxLayout *hHostLayout;
    QHBoxLayout *hPortLayout;
    QHBoxLayout *hLogDirLayout;
    QHBoxLayout *hLogLayout;
    QHBoxLayout *hButtonLayout;
    QHBoxLayout *hSettingsStatusLayout;
    QVBoxLayout *verticalDialogLayout;
    QIntValidator *portValidator;
    NoIntSpcValidator *nisValidator;
};


#endif // SETTINGSDIALOG_H
