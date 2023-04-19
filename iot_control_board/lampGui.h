#ifndef LAMPGUI_H
#define LAMPGUI_H
#include <QObject>
#include <QWidget>
#include <QTcpSocket>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QPalette>
#include <baseGui.h>

#define SWITCH_CMD "a"

class LampGui: public BaseGui{
    Q_OBJECT
public:
    LampGui(QTcpSocket *, QWidget *p = NULL);
    ~LampGui();

public slots:
    void onReceived();      /* called for the packet received after the first */
    void onSwitchClicked();     // when switchButton is clicked

private:
    QPushButton *switchButton;  // will send a command to the client when clicked
    QIcon *onStateIcon;              // icons depending on the state of the switch
    QIcon *offStateIcon;
    bool isSwitchOn = false;    // boolean holding switch status
};

#endif // LAMPGUI_H
