#ifndef MainWidget_H
#define MainWidget_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>
#include <glog/logging.h>
class MainWidget : public QWidget {
    Q_OBJECT
public:
    MainWidget(QWidget *parent = nullptr);
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
public slots:
    void slotOkButtonDone();
private slots:
    void slotDisconnectButtonClicked();
    void slotSendButtonClicked();
signals:
    void disconnectButtonClicked();
};




#endif // MainWidget_H
