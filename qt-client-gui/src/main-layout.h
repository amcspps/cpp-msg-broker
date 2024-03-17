#ifndef MAINLAYOUT_H
#define MAINLAYOUT_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>

class MainLayout : public QVBoxLayout {
    Q_OBJECT
public:
    MainLayout(QWidget *parent = nullptr);
    void setResponseLabel(QString label);
    void hideDisconnectButton();
    void hideSendButton();
    ~MainLayout();
private:
    QPushButton *disconnectButton;
    QPushButton *sendButton;
    QLabel *requestInputLabel;
    QLabel *responseLabel;
    QLineEdit *requestInputLineEdit;

    QHBoxLayout *hDisconnectLayout;
    QHBoxLayout *hRequestInputLabelLayout;
    QIntValidator *requestValidator;
    QHBoxLayout *hRequestInputLineEditLayout;
    QHBoxLayout *hResponseLabelLayout;
    QHBoxLayout *hSendButtonLayout;

private slots:
    void onDisconnectButtonClicked();
    void onSendButtonClicked();
signals:
    void disconnectButtonClicked();
    void sendButtonClicked();
};




#endif // MAINLAYOUT_H
