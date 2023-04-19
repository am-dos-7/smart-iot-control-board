#ifndef DISPLAYGUI_H
#define DISPLAYGUI_H

#include <QObject>
#include <QWidget>
#include <QTcpSocket>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <baseGui.h>

class DisplayGui: public BaseGui{
    Q_OBJECT
public:
    DisplayGui(QTcpSocket *, QWidget *p = NULL);
    ~DisplayGui();

public slots:
    void onReceived();      /* called for the packet received after the first */
    void onSendClicked();     // when switchButton is clicked

private:
    QLineEdit *textZone;      // will write a text which to be sent to the display
    QPushButton *sendButton;  // will send the text to the client when clicked
};

#endif // DISPLAYGUI_H
