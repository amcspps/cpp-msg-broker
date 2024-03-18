#ifndef MainWidget_H
#define MainWidget_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>

class MainWidget : public QWidget {
    Q_OBJECT
public:
    MainWidget(QWidget *parent = nullptr);
    void setResponseLabel(QString label);
    void hideDisconnectButton();
    void hideSendButton();
    void hideRequestInputLabel();
    void hideRequestInputLineEdit();
    void showRequestInputLineEdit();
    void showRequestInputLabel();
    void showSendButton();
    void showDisconnectButton();
    void clearResponseLabel();
    void clearRequestInputLineEdit();
    QString getRequestInputLineEditText();
    ~MainWidget();
private:
    QVBoxLayout *widgetVLayout;
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

signals:
    void disconnectButtonClicked();
    void sendButtonClicked();
};




#endif // MainWidget_H
