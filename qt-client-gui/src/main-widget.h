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
    ~MainWidget();
private:
    QVBoxLayout * widgetVLayout;
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




#endif // MainWidget_H
