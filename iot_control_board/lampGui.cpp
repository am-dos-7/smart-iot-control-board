#include "lampGui.h"

LampGui::LampGui(QTcpSocket *s, QWidget *p): BaseGui(s, p)
{
    offStateIcon = new QIcon("off.png");
    onStateIcon = new QIcon("on.png");

    switchButton = new QPushButton("Switch");
    switchButton->setIcon(*offStateIcon);           // off by default
    connect(switchButton, SIGNAL(clicked()), this, SLOT(onSwitchClicked()));

    layout->addWidget(switchButton);
}

void LampGui::onReceived()
{
    /* Data received */
    char *tmp = new char[sock->bytesAvailable() + 1];   // memory allocation for received message
    if(tmp == NULL)
    {
        perror("Error on tmp alloc");
        exit(EXIT_FAILURE);
    }

    memset(tmp, '\0', sock->bytesAvailable() + 1);  // buffer initialization

    sock->read(tmp, sock->bytesAvailable());        // read message into buffer

    qDebug() << tmp;

    delete[] tmp;           // free buffer memory
}

void LampGui::onSwitchClicked()
{
    if(sock->write(SWITCH_CMD)){    // send SWITCH_CMD to the client
        // and on succesfull transmission,
        isSwitchOn = !isSwitchOn;               // update state,
        if(isSwitchOn)                         // and update icon
            switchButton->setIcon(*onStateIcon);  //
        else
            switchButton->setIcon(*offStateIcon);
    }
}

LampGui::~LampGui(){

}
